（1）传递临时对象作为线程参数
（1.1）要避免的陷阱 （解释1）
（1.2）要避免的陷阱 （解释2）
（1.3）总结
（2）临时对象作为线程参数继续讲
（2.1）线程id概念
（2.2）临时对象构造时机抓捕
（3）传递类对象、智能指针作为线程参数
（4）用成员函数指针做线程函数

```c++
/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <memory>
#include <thread>

using namespace std;

// void myprint(const int &i, char *pmybuf)
void myprint(const int &i, const string &pmybuf)
{
    cout << i << endl; // 这里通过内存跟踪，发现。这里的i并不是mavar的引用，（虽然语法上看起来，这个执行函数就是是用来mavar的引用）
    // 实际上而是值传递，所以我们认为即使主线程detach了子线程，那么后续继续使用i的值仍然是安全的
    // 但是即便是安全的，也不推荐使用引用
    // cout << myprint << endl;
    // 同样通过内存跟踪，发现这里pmbuf和主线程中的mybuf的内存指向是一样的，那就代表
    // 指针在detach子线程时，是绝对会出现问题的
    cout << *myprint << endl;
    return;
}

class A
{
    public:
        // mutable int m_i; // mutable这个关键字表明，这个成员 变量在任何情况下都是可修改的
        int m_i; // 现在测试std::ref,就不需要使用mutable
        // 类型转换构造函数（因为他只有一个参数，可以将一个int型转为一个A类类型对象）
        A(int a):m_i(a){
            cout << "构造函数执行" << this << "  thread_id: " << std::this_thread::get_id() << endl;
        }
        A(const A &a):m_i(a.m_i){
            cout << "拷贝构造函数执行" << this << "  thread_id: " << std::this_thread::get_id() << endl;
        }
        ~A()
        {
            cout << "析构函数执行" << this << "   thread_id: " << std::this_thread::get_id() << endl;
        }
        
        void thread_loack(int num)
        {
            cout << "子线程thread_loack执行了" << this << "   thread_id: " << std::this_thread::get_id() << endl;
        }
        
        void operator()(int num)
        {
            cout << "[子线程]()执行了" << this << "   thread_id: " << std::this_thread::get_id() << endl;
        }
};

void myprintA(const int i, const A &tmpa)
{
    // tmpa.m_i = 9090; // 修改该值，不会影响到main函数
    cout << &tmpa << endl; // 这里打印的是tmpa对象的地址
}
// void myprintB(const A &tmpa)
void myprintB(A &tmpa) // 测试std::ref，修改对象值并影响到主线程 采用引用传值方式，且不加const
{
    tmpa.m_i = 9090; // 修改该值，不会影响到main函数
    cout << &tmpa << endl; // 这里打印的是tmpa对象的地址
}

void myprintC(unique_ptr<int> pq) // 测试std::ref，修改对象值并影响到主线程 采用引用传值方式，且不加const
{
    // cout << pq->m_i << endl;
    // cout << &tmpa << endl; // 这里打印的是tmpa对象的地址
}

int main()
{
    // 传递临时对象做线程参数
    // int mvar = 1;
    // int &mavar = mvar;
    // char mybuf[] = "this is a char[] test";
    // // thread mythreadOBJ1(myprint, mavar, mybuf); // 注意这里第一个参数是线程执行函数的函数名，后面的依次是该函数的实参
    // // 切记不要把实参写成括号包给线程执行函数
    // // 这里mybuf是在什么时候被转换为string对象的
    // // 所以这里就存在这种隐患,那就是他可能在我们主线程执行完毕才将mybuf转为string对象，那这种情况下
    // // 系统就存在严重BUG，因为主线程执行完毕，mybuf已经被回收内存，再转string就是将一个不属于mybuf的内存转为string
    // // 优化方案，直接在传参阶段就完成string的临时对象构造
    // thread mythreadOBJ1(myprint, mavar, string(mybuf));
    // // mythreadOBJ1.join();
    // mythreadOBJ1.detach(); 
    // 
    
    int mvar = 100;
    int mysecond = 123;
    
    // thread mytobj(myprintA, mvar, mysecond); // 这里我们希望这个mysecond这个整形参数转为A类型对象传给线程函数
    
    // mytobj.join();
    // 构造函数执行
    // 0x7f8161f92dbc
    // 析构函数执行
    // 主线程运行结束
    
    // 当我们使用detach的时候，再试一下
    // mytobj.detach();
    // 主线程运行结束
    
    // 我们发现，这里会存在这样一个问题，有时候，这个A类临时对象会被构造出来，有时候不会（如果A类比较复杂，或者主线程结束较快，大概率不会被构造出来）
    // 这种情况就说明，可能会存在主线程运行结束之后，子线程才会去构造这个临时的类A参数，那这样就会出现严重问题
    
    
    // thread mytobj(myprintA, mvar, A(mysecond));
    // // 通过这样在传递实参阶段就生成一个类A的临时对象传进去，发现，无论怎样，这个临时对象都被构造出来了
    // // 其实也就是在主线程结束之前，我就已经通过这个参数构造了一个临时的A对象，并传递到线程中去了
    // mytobj.detach();
    // 构造函数执行0x7ffc9351e420
    // 拷贝构造函数执行0x7ffc9351e3e0 // 这个拷贝构造函数其实说明了，线程在创建的时候，虽然我们写的是引用
                                      // 但是他还是很粗暴的使用了值传递（拷贝了一个参数对象进来）
    // 拷贝构造函数执行0xf1ec28
    // 析构函数执行0x7ffc9351e3e0
    // 析构函数执行0x7ffc9351e420
    // 主线程运行结束
    // 0xf1ec28
    // 0xf1ec28
    // 析构函数执行0xf1ec28
    
    // 结论：在创建线程的同时，构造临时对象的传递方法是可行的
    // 只要使用临时构造的对象作为参数传递给线程，那么就一定能在主线程结束之前就将所需的对象参数构造出来
    // 即便后续detach了，子线程还是能安全运行
    // 若传递int这种简单类型，建议都采用值传递（不要使用引用）
    // 如果传递类对象，要避免隐式类型转换，应该全部都在创建线程的时候就构建临时对象，然后在函数参数里使用引用来接，因为如果不用引用，他在线程函数里还会再构造一次
    // 建议不使用detach，防止局部变量失效导致线程对内存的非法引用
    
    // 线程ID：id是个数字，每个线程不管是主线程还是子线程，实际上都对应一个独一的线程ID
    // 使用 std::this_thread::get_id()获取线程ID
    
    // cout << "主线程ID： " << std::this_thread::get_id() << endl;
    // thread mytobj(myprintA, mvar, A(mysecond));
    // mytobj.detach();
    // 主线程ID： 140574650500992
    // 构造函数执行0x7fff1e4238b0  thread_id: 140574650500992
    // 拷贝构造函数执行0x7fff1e423870  thread_id: 140574650500992
    // 拷贝构造函数执行0x8adc28  thread_id: 140574650500992
    // 析构函数执行0x7fff1e423870   thread_id: 140574650500992
    // 析构函数执行0x7fff1e4238b0   thread_id: 140574650500992
    // �线程运行结束
    
    // 传递类对象，智能指针作为线程参数
    // A myobj(12);
    // thread mtobj(myprintB, myobj);
    // // 将类对象作为线程参数
    // // 发现我们使用了引用传进去的类的成员变量只在子线程中被修改，但是并没有影响到主线程中的类对象，
    // // 其实很好理解，因为线程不管你是什么类型的传递方式，她都是按照值传递的方法将参数传递到线程中去的
    // mtobj.join();
    
    // std::ref 如果我们确实需要在线程中修改传入的参数并影响到主线程中，那就需要使用std::ref来进行参数传递
    // A myobj(12); 
    // cout << myobj.m_i << endl; // 12
    // thread mtobj(myprintB, std::ref(myobj));
    // mtobj.join();
    // cout << myobj.m_i << endl; // 9090
    
    // unique_ptr<int> pq(new int(100));
    // thread mtobj2(myprintC, std::move(pq));
    // // 凡是使用到指针的，都应该使用join（）
    // mtobj2.join();
    
    // // 使用成员函数指针做线程函数参数
    // A myobjA(1000);
    // std::thread myobj4(&A::thread_loack, myobjA, 1234);
    // // std::thread myobj4(&A::thread_loack, std::ref(myobjA), 1234);
    // myobj4.join();
    
    // 构造函数执行0x7ffe64ded278  thread_id: 140293837657984
    
    // �贝构造函数执行0x7ffe64ded234  thread_id: 140293837657984
    
    // 拷贝构造函数执行0x117bd9c  thread_id: 140293837657984
    
    // 析构函数执行0x7ffe64ded234   thread_id: 140293837657984
    
    // 子线程thread_loack执行了0x117bd9c   thread_id: 140293820712704
    
    // 析构函数执行0x117bd9c   thread_id: 140293820712704
    
    // 主线程运行结束
    
    // 析构函数执行0x7ffe64ded278   thread_id: 140293837657984


    A myobj5(344);
    // std::thread tobj5(myobj5, 466);
    std::thread tobj5(std::ref(myobj5), 455);
    tobj5.join();
    // 构造函数执行0x7ffdbbc258a0  thread_id: 140437192005504
    
    // [子线程]()执行了0x7ffdbbc258a0   thread_id: 140437175060224
    
    // 主线程运行结束
    
    // 析构函数执行0x7ffdbbc258a0   thread_id: 140437192005504


    
    cout << "主线程运行结束" << endl;

    return 0;
}



```

# 一：传递临时对象作为线程参数

1.1、要避免的陷阱1（解释1）
1.2、要避免的陷阱2（解释2）

```c++
class A
{
	public:
	mutable int m_i;	//const下也能修改
	//类型转换构造函数
	A(int a):m_i(a){cout<<"构造函数执行"<<this<<"threadid="<<std::this_thread::get_id()<<endl;}
	A(const A &a):m_i(a.m_i){cout<<"拷贝构造函数执行"<<this<<"threadid="<<std::this_thread::get_id()<<endl}
	~A(){cout<<"析构函数执行"<<this<<"threadid="<<std::this_thread::get_id()<<endl;}
	void thread_work(int num)	//来个参数
	{
		cout<<"子线程thread_work执行"<<this<<"thradid="<<std::this_thrad::get_id()<<endl;
	}
	void operator()(int num)
	{
		cout<<"子线程()执行"<<this<<"thradid="<<std::this_thrad::get_id()<<endl;
	}
};
//void myprint(const int &i, char *pmybuf)
void myprint(const int &i, const string &pmybuf)	//感觉可行其实还是不行
{
	cout<<i<<endl;	//分析认为，i并不是mvar的引用，实际是值传递，那么我们认为，即使主线程detach了子线程，那么子线程中用i仍然是安全的。
	cout<<pmybuf.c_str()<<endl;	//指针在detach子线程时，绝对会有问题
	cout<<*pmybuf<<endl;
	return;
}
void myprint(const int i, const A&pmybuf)
{
	cout<<&pmybuf<<endl;	//打印地址
}
//void myprint2(const A &pmybuf)
void myprint2(unique_ptr<int> pzn)
{
	pmybuf.m_i = 199;//我们修改该值不会影响main函数
	cout<<"子线程myprint2的参数地址是"<<&pmybuf<<"threadid="<<std::this_thread::get_id()<<endl;
}
int main()
{
	int mvar = 1;
	int &mvary = mvar;
	char mybuf[] = "this is a test!";
	thread mytobj(myprint,mvar,mybuf);//但是mybuf到底是在什么时候转成string.
	//事实上存在，mybuf都被回收了（main函数执行完了），系统才用mybuf去转string
	thread mytobj(myprint,mvar,string(mybuf));//这里直接将mybuf转换成string对象，这是一个可以保证在线程中用肯定有效的对象
	//mytobj.join();

	//1.2
	int mvar = 1;
	int mysecondpar = 12;
	thread myobj(myprint, mvar, A(mysecondpar));//我们是希望mysecondpar转成A类型对象传递给myprint的第二个参数
	//在创建线程的同时构造临时对象的方法传递参数是可行的；
	//mytojb.join;

	cout<<"主线程id是"<<std::this_thread::get_id()<<endl;
	int myar = 1;
	std::thread myobj(myprint2, A(mvar));	//这样显示转换是安全的
	
	A myobj(10);	//生成一个类对象
	std::thread myobj(myprint2, myobj);//myobj将类对象作为线程参数
	unique_ptr<int> myp(new int(100));
	std::thread mytobj(myprint2, std::move(myp));
	
	//用成员函数指针做线程函数
	A myobj(10);	//生成一个类对象
	std::thread myobj(&A::thread_work, myobj, 15);//&myobj==std::ref(myobj)
	mytobj.join();	//用成员函数必须用join
	
	//用（）作为线程入口函数
	A myobj(10);	//生成一个类对象
	std::thread myobj( myobj, 15);//&myobj==std::ref(myobj)//用std::ref后就不调用拷贝构造函数了，那么后续如果调用mytobj.detach()就不安全了；
	mytobj.join();
	
	mytobj.detach();	//子线程和主线程分别执行。
	cout<<"I Love China!"<<endl;
	return 0;
}
```

# 1.3、总结

a、若传递int这种简单类型参数，建议都是值传递，不要用引用。防止节外生枝。
b、如果传递类对象，避免隐式类型转换。全部都在创建线程这一行就构建出临时对象来，然后在函数参数里，用引用来接，否则系统还会多构造一次临时对象
c、建议不使用detach(),，只使用join()；这样就不存在局部变量失效导致线程对内存的非法引用问题；

二：临时对象作为线程参数继续讲，常用测试大法；
2.1、线程id概念：ID是个数字，每个线程（不管是主线程还是子线程）实际上都对应着一个数字，而且每个线程对应的数字都不同。也就是说，不同的线程，它的线程id(数字）必然是不同；
线程id可以用C++标准库里的函数来获取。std::this_thread::get_id()来获取。
2.2、临时对象构造时机捕获

三：传递类对象作为线程参数
正常子线程下修改类成员变量不会影响主线程类成员变量，不管有没有引用在线程都以拷贝接收类对象
用std::ref可以使对象真正传引用

四：用成员函数指针做线程函数
```