```c++
/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
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

void myprint()
{
    for(int i = 0; i < 100; ++i)
    {
        cout << "线程开始执行" << i << endl;
    }
}

class TA
{
  public:
  int &m_i;
    TA(int &i):m_i(i) // 普通构造函数 注意这里使用引用传值，当调用线程的detach（）时，会存在内存泄漏的问题
    // 因为使用了detach，主线程结束，里面的内存被销毁，而这里使用的时引用，那么就相当于你在这里使用了一个被销毁的内存
    // 所以，如果是detach，这里要使用值传递（复制的方法）
    {
        cout << "ta ()构造函数执行" << endl;
    }
    TA(const TA &ta):m_i(ta.m_i) // 拷贝构造函数
    {
        cout << "ta()拷贝构造函数执行" << endl;
    }
    ~TA()
    {
        cout << "析构函数执行" << endl;
    }
    void operator() ()
    {
        cout << "类类型的可调用对象 的 线程执行了" << endl;
        cout << "m_i的值为：" << m_i << endl;
    }
    
};

int main()
{
    // 程序运行起来生成一个进程，同时这个进程的运行也代表所属的主线程开始自动运行
    
    cout << "thread" << endl;
    
    // 主线程从main函数开始运行，那么我们自己创建的线程也需要一个入口函数（初始函数）开始运行
    // 一旦这个函数运行完毕，就代表我们的线程执行完毕
    
    // 整个进程是否执行完毕的标志，是主线程是否执行完毕，如果主线程执行完毕了，就代表整个进程执行完毕
    // 此时如若其他子线程没有执行完毕，那么，一般情况下这些子线程也会被操作系统强行终止
    
    // 所以，一般情况下，如果想保持子线程的生命周期（运行状态），那么就该让主线程一直保持运行
    
    // thread 是标准库里的一个类
    // join()； 加入、回合。说白了就是阻塞，阻塞主线程，让主线程等待子线程执行完毕，然后子线程和主线程汇合
    std::thread mytobj(myprint); // 这个实参是一个可调用对象
    // 创建了线程，线程执行入口是myprint()， myprint()开始执行
    // mytobj.join(); // 主线程阻塞到这里，等待子线程mytobj执行完毕
    if (mytobj.joinable())
    {
        mytobj.detach();
        if (mytobj.joinable())
        {
            mytobj.join();
        }
    }
    
    cout << "test" << endl;
    
    // detach(); 
    // 传统规律，主线程需要等待子线程执行完毕，然后自己再最后退出
    // detach：分离，也就是主线程不和子线程汇合，子线程和主线程各自执行各自的，不必等待
    // 我们创建了很多子线程，让主线程挨个等待子线程执行完毕，当不需要这种等待的时候就可以使用detach
    // 一旦detach之后，与这个主线程关联的thread对象就会失去与这个主线程的关联关系，此时这个子线程就会驻留在后台运行
    // 这个子线程就相当于被C++运行时库接管，当这个子线程执行完毕之后，由运行时库负责清理该线程相关的资源（守护线程）
    // 一旦调用了detach()，我们之后就不能再使用join()，否者会爆异常
    
    // joinable(); 判断是否可以成功使用join或者detach();(返回true，可以join或者detach)
    
    // 其他创建线程的方法
    // 用类对象 的可调用对象重载
    // 一旦调用了这个detach，那主线程执行完毕，那这里用的这个ta这个对象还在吗？
    // 对象肯定是不在了的，但是为什么会依旧可以执行呢，因为实际上这里这个对象时被复制到线程中去的，ta在主线程执行完会被销毁
    // 但是所复制的ta对象依旧存在
    // 所以，只要你这个ta对象里没有引用，没有指针，那么就不会产生问题
    int mi = 66;
    TA ta(mi);
    thread mytobj2(ta); // ta可调用对象
    mytobj2.join();
    // mytobj2.detach();
    
    // 使用lambda表达式
    auto mylamdbathread = []{
        cout << "拉姆达创建的线程执行" << endl;
    };
    thread mytobj3(mylamdbathread);
    mytobj3.join();
 
    return 0;
}


```

# 一、范例演示线程运行的开始和结束

程序运行起来，生成一个进程，该进程所属的主线程开始自动运行；
cout<<“I Love China!” << endl; //实际上这个是主线程在执行，主线程从main()函数返回，则整个进程执行完毕。

主线程从main()开始执行，那么我们自己创建的线程，也需要从一个函数开始运行（初始函数），一旦这个函数运行完毕，就代表着我们这个线程运行结束。

整个进程是否执行完毕的标志是主线程是否执行完，如果主线程执行完毕了，就代表整个进程执行完毕了；
此时，一般情况下：如果其他子线程还没有执行完毕，那么这些子线程也会被操作系统强行终止。
所以，一般情况下，我们得到一个结论：如果大家想保持子线程的运行状态的话，那么大家要让主线程一直保持运行，不要让主线程停止。
（这条规则有例外）
a、包含一个头文件thread要包含进来


```c++
void myprint(){
	cout<<"我的线程开始执行了"<<endl;
	//...
	cout<<"我的线程执行完毕了1"<<endl;
	cout<<"我的线程执行完毕了2"<<endl;
	cout<<"我的线程执行完毕了3"<<endl;
	cout<<"我的线程执行完毕了4"<<endl;
	cout<<"我的线程执行完毕了5"<<endl;
	cout<<"我的线程执行完毕了6"<<endl;
	cout<<"我的线程执行完毕了7"<<endl;
	cout<<"我的线程执行完毕了8"<<endl;
	cout<<"我的线程执行完毕了9"<<endl;
}

class TA
{
public:
	int &m_i;
	TA(int &i):m_i(i)	//转换构造函数
	{
		cout<<"TA（）构造函数被执行"<<endl;
	}
	TA(const TA &ta):m_i(ta.m_i)
	{
		cout<<"TA（）的拷贝构造函数被执行"<<endl;
	}
	~TA()
	{
		cout<<"TA()析构函数被执行"<<endl;
	}
	void operator()()	//不能带参数
	{
		//cout<<"我的线程operator开始执行了"<<endl;
		cout<<"m_i1的值为："<<m_i<<endl;
		cout<<"m_i2的值为："<<m_i<<endl;
		cout<<"m_i3的值为："<<m_i<<endl;
		cout<<"m_i4的值为："<<m_i<<endl;
		cout<<"m_i5的值为："<<m_i<<endl;
		cout<<"m_i6的值为："<<m_i<<endl;
	}
};


```

b、初始函数要写。
c、main中开始写代码
大家必须明确一点：有两个线程在跑，相当整个程序的执行有两条线在同时走，所以可以同时干两件事，即使一条线被堵住了，另外一条线还是可以通行的。这就是多线程。

# 1.1、thread：是个标准库里的类

# 1.2、join()：加入/汇合，说白了就是阻塞，阻塞主线程，让主线程等待子线程执行完毕，然后子线程和主线程汇合，然后主线程再往下走

如果主线程执行完毕了，但子线程没执行完毕，这种程序员是不合格的，写出来的程序也是不稳定的；
一个书写良好的程序，应该是主线程等待子线程执行完毕后，自己才能最终退出；

# 1.3、detach()：传统多线程程序主线程要等待子线程执行完毕，然后自己再最后退出；
detach：分离，也就是主线程不和子线程汇合了，你主线程执行你的，我子线程执行我的，你主线程也不必等我子线程运行结束，你可以先执行结束，这并不影响主线程执行。
为什么引入detach()：我们创建了很多子线程，让主线程逐个等待子线程结束，这种编程方法不太好，所以引入了detach()；
一旦detach()之后，与这个主线程关联的thread对象就会失去与这个主线程的关联，此时这个子线程就会驻留在后台运行（主线程跟该子线程失去联系）。
这个子线程就相当于被C++运行时刻接管，当这个子线程执行完成后，由运行时库负责清理该线程相关的资源（守护线程）。
detach()使用线程myprint失去我们自己的控制。

# 1.4、joinable()：判断是否可以成功使用join()或者detach()的；返回true(可以join或者detach)或者false(不能join或者detach)。

myprint可调用对象。


```c++
thread mytobj(myprint);	//1、创建了线程，线程执行起点（入口）myprint();
//2、myprint线程开始执行。
if (mytobj.joinable())
{
	mytobj.join();
}

//阻塞主线程并等待myprint子线程执行完
mytobj.join();	//主线程阻塞到这里等待myprint()执行完，当子线程执行完毕，这个join()就执行完毕，主线程就继续往下走。
mytobj.detach();//一旦调用了detach()，就不能再用join()，否则系统会报告异常。
if (myobj.joinable())
{
	cout<<"joinable == true"<<endl;
}
else
{
	cout<<"joinable == false"<<endl;
}


cout<<"I Love China!1"<<endl;
cout<<"I Love China!2"<<endl;
cout<<"I Love China!3"<<endl;
cout<<"I Love China!4"<<endl;
cout<<"I Love China!5"<<endl;
return 0;

```

# 二：其他创建线程的手法
# 2.1、用类对象（可调用对象），以及一个问题范例
大家可能还有一个疑问：一旦调用了detach()，那我主线程执行结束了，我这里用的这个ta这个对象还在吗？
这个对象实际是被复制到线程中去；

```c++
int myi = 6;
TA ta(myi);
thread  mytobj3(ta);	//ta：可调用对象。
mytobj3.join();	//等待子线程执行结束
mytobj3.detach();	//使用时普通变量之类的传引用要注意参数在主线程的生命周期，类对象thread也会自己拷贝临时变量
cout<<"I Love China"<<endl;
return 0;
```


# 2.2、用lambda表达式

```c++
auto mylamthread = []
{
	cout<<"我的线程3开始执行了"<<endl;
	//...
	cout<<"我的线程3执行结束了"<<endl;
};
thread mytobj4(mylamthread);
mytobj4.join();

```
