```c++
/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

using namespace std;

shared_ptr<int> myfunc (shared_ptr<int> &ptr)
// void myfunc (shared_ptr<int> &ptr) // 如果是使用引用作为参数，则智能指针的引用计数不会增加
{
    cout << *ptr << endl;
    return ptr;
}

void mm(shared_ptr<int> s)
{
    cout << s.use_count() << endl;
    return;
}

void myshareddelte(int *p) // 我们自定义的shared_ptr删除器，用于删除int型的对象指针，
// 当对象引用计数为0，就会自动调用该删除器函数来删除该对象
{
    cout << "删除器打印日志" << endl;
    delete p;
}

class A
{
    public:
        A()
        {
            
        }
        ~A(){}
};

// 实现一个函数模板来封装shared_ptr数组
template<typename T>
shared_ptr<T> make_shared_array(size_t size)
{
    return shared_ptr<T>(new T[size], std::default_delete<T[]>());
}

int main()
{
    // shared_ptr引用计数的增加和减少
    // auto p0 = make_shared<int>(1000); // 目前p0所指向的对象只有一个引用着
    // auto p1(p0); // 智能指针定义时初始化，p0和p1都指向了相同的对象，此对象目前有两个引用者
    
    // // 在如下情况下，所有指向这个对象的shared_ptr都会加1
    // // 1、像上面这样，使用一个shared_ptr来初始化另一个shared_ptr
    // // 2、把智能指针单做实参往函数里传递
    // myfunc(p1); // 此时 p0 ,p1的智能指针计数都是3
    
    // // 3、作为函数的返回值
    // auto p2 = myfunc(p1);
    // cout << p0.use_count() << endl; // 3
    // cout << p1.use_count() << endl; // 3
    // cout << p2.use_count() << endl; // 3
    
    // // 引用计数减少
    // 1、给shared_ptr赋予新值
    // p0 = make_shared<int>(200);
    // cout << p0.use_count() << endl; // 1
    // p1 = make_shared<int>(400); 
    // cout << p1.use_count() << endl; // 1
    // p2 = make_shared<int>(900);
    // cout << p2.use_count() << endl; // 1
    // mm(p2);
    // cout << p2.use_count() << endl; // 1
    
    // 2、局部的shared_ptr离开其作用域
    // auto p3 = make_shared<int>(9000);
    // auto p4(p3); // 引用计数2
    // cout << p4.use_count() << endl;
    // mm(p4);
    // // 2-> 3 -> 2 在外面，2个计数，通过值传递，引用计数加1，离开局部作用域范围，引用计数减一

    // // 3、当一个shared_ptr引用计数从1变为0，则他会自动释放自己所管理的对象内存
    // auto p5 = make_shared<int>(1000);
    // auto p6 = make_shared<int>(1000);
    // p5 = p6; // 给p5赋值会让P5指向p6所指的对象， 该对象引用计数变为2，则原来的p5所指向的对象的引用计数会变为从1变为0
    // cout << p5.use_count() << *p5 << endl;
    // cout << p6.use_count() << *p6 << endl;
    
    // // unique()；判断一个智能指针是否独占某一个对象，是返回true
    // cout << p5.unique() << endl;
    
    // cout << (0 == false) << endl;
    // reset() :恢复，复位重置
    // a)reset不带参数
    // 若pi是唯一指向该对象的指针，那么释放pi所指向的对象， 并将pi置空
    // 若pi不是唯一指向该对象的指针，那么不释放pi指向的对象，但是指向该独享的引用计数会减1，同时pi置空
    // shared_ptr<int> pi(new int(800));
    // // pi.reset();
    // // cout << pi.use_count() << endl; // 0
    // // cout << pi.unique() << endl; // false 0
    // auto pii(pi);
    // pi.reset();
    // cout << pi.use_count() << endl; // 0
    // cout << pi.unique() << endl; // false 0
    // cout << pii.use_count() << endl; // 1
    // cout << pii.unique() << endl; // true 1
    
    // b)reset带参数 一般这个参数是new出来的指针
    // 若pi是唯一指向该对象的指针，那么释放pi所指向的对象， 并将pi指向新对象
    // 若pi不是唯一指向该对象的指针，那么不释放pi指向的对象，但是指向该独享的引用计数会减1，同时pi指向新对象
    // shared_ptr<int>pi (new int(300));
    // // pi.reset(new int(7090));
    // // cout << pi.use_count() << endl; // 1
    // // cout << pi.unique() << endl; // true 1
    // // cout << *pi << endl; // 7090
    // auto pii(pi);
    // pi.reset(new int(2020));
    // cout << pi.use_count() << endl; // 1
    // cout << pi.unique() << endl; // true 1
    // cout << *pi << endl; // 7090
    // cout << pii.use_count() << endl; // 1
    // cout << pii.unique() << endl; // true 1
    // cout << *pii << endl; // 300
    
    // // 空指针也可以通过reset来重新初始化
    // shared_ptr<int> np;
    // np.reset(new int(9000));
    
    // 解引用 * 获取p指向的对象
    // shared_ptr<int> p(new int(2000));
    // cout << *p << endl; // 2000
    
    // get()  考虑到有些函数的参数需要的是一个内置的裸指针，而不能是智能指针
    // 返回p中保存的裸指针，小心使用，如果智能指针释放了所指向的对象，那么返回的裸指针也变得无效了
    // shared_ptr<int> pluo(new int(3400));
    // int *io = pluo.get();
    // cout << *io << endl; // 3400
    // *io = 45;
    // cout << *pluo << endl; // 45
    
    // swap() 交换两个智能指针所指向的对象
    // shared_ptr<string> p1(new string{"ptr"});
    // shared_ptr<string> p2(new string{"shared_"});
    
    // cout << *p1 << *p2 << endl; // ptrshared_
    // std::swap(p1, p2);
    // // 或者
    // // p1.swap(p2);
    // cout << *p1 << *p2 << endl; // shared_ptr
    
    // =nullptr
    // 将所指向的对象引用计数减1，若引用计数为0了，则将智能指针指向的对象释放。然后将智能指针设置为空
    // shared_ptr<string> p1(new string{"ptr"});
    // p1 = nullptr;
    
    // 智能指针名字作为判断条件
    
    // 指定删除器及其数组问题
    // 指定删除器
    // 1.定时机制帮助我们删除所指向的对象 delete 默认使用delete运算符作为默认的资源西沟方式
    // 我们可以指定自己的删除器取代系统的默认删除器
    // shared_ptr指定删除器的方式比较简单，一般只需要在测试中添加具体的删除器函数名即可
    // shared_ptr<int> pp(new int (122321), myshareddelte);
    // 删除器还可以是一个lambda表达式
    // shared_ptr<int> pp(new int (122321), [](int *p) {
    //     cout <<"调用lambda表达式删除器" << endl;
    //     delete p;
    // });
    // shared_ptr<int> pp2(pp); // 引用计数 2
    
    // pp2.reset(); // 剩下一个引用计数，pp2为nullptr
    // pp.reset(); // 调用完这行就会调用我们自定义的删除器
    
    // 有些情况下使用默认删除器处理不了（比如使用shared_ptr管理动态数组），这时需要我们提供自定义的删除器
    // shared_ptr<int> pparray(new int[10], [](int *p) {
    //     cout << "调用delete[]自定义删除器" << endl;
    //     delete[] p;
    // })
    
    // shared_ptr<A> pa(new A[10]);
    // *** Error in `./a.out': munmap_chunk(): invalid pointer: 0x0000000000ce9c28 *** 报异常
    // shared_ptr<A> pa(new A[10], [](A *p) {
    //     cout << "调用delete[]自定义删除器" << endl;
    //     delete[] p;
    // }); 
    // 调用delete[]自定义删除器
    
    // 使用default_delete来做删除器，default_delete是标准模板库里的模板类，她内部也是使用delete实现的，也可以实现数组删除
    // shared_ptr<A> pa(new A[10], std::default_delete<A[]>());
    
    // 定义数组的的时候，在尖括号中加中括号<[]>，就可以避免自己写删除器来进行数组对象的删除
    // shared_ptr<A[]> Pa(new A[10]);
    
    // 使用自定义的make_shared_array函数模板来生层一个智能指针对象数组
    shared_ptr<int> pintArray = make_shared_array<int>(5);
    shared_ptr<A> pintA = make_shared_array<A>(15);
    
    // 指定删除器额外说明， 就算是两个shared_ptr指定了不同的删除器，只要他们所指向的对象类型相同，那么这两个shared_ptr也属于同一个类型
    auto lambda1 = [](int *p){
        cout << "拉姆达表达式1执行了" << endl;
        delete p;
    };
    auto lambda2 = [](int *p){
        cout << "拉姆达表示2执行了" << endl;
        delete p;
    };
    
    shared_ptr<int> p1(new int[2], lambda1);
    shared_ptr<int> p2(new int[3], lambda2);
    // p2 = p1; // 这行代码会导致p2先调用lambda,把自己所指向的对象进行释放，然后指向p1所指向的对象，p1所指向的对象，引用计数加1 = 2
    // // 整个main执行王弼后，还会调用lambda1来释放p1, p2共同指向的对象
    // // 拉姆达表示2执行了
    // // 拉姆达表达式1执行了
    
    // 类型相同，就代表他们可以放到元素类型相同的容器里来
    vector<shared_ptr<int>> pvce{p1, p2};
    
    // make_shared是提倡的生成shared——ptr的方式，但是make_shared不支持我们自定义删除器



    return 0;
}


```

share_ptr的引用和减少与常见操作
```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
#include <memory>

using namespace std;

class A
{
public:
	A()
	{}
	~A()
	{
		
	}
};

//void myfunction(shared_ptr<int>&myp)//使用引用，引用计数不会增加
void myfunction(shared_ptr<int>myp)
{
	cout << "智能指针做函数参数，引用计数增加" << endl;
}

shared_ptr<int> myfunction02(shared_ptr<int>&myp)
{
	cout << "智能指针做函数返回值，引用计数增加" << endl;
	return  myp;
}


void mydelete(int*p)//自己指定的删除器
{
	cout << "这是我们自己定义的删除器" << endl;
	//......
	delete p;
}

//3.函数模板封装
template<typename T>
shared_ptr<T>make_shared_array(size_t size)
{
	return shared_ptr<T>(new T[size], default_delete<T[]>());
}


int main(void)
{
	//1引用用计数的增加
	auto p1 = make_shared<int>(100);//int =100对象只有一个引用者，p1
	auto p2(p1);//两个引用者p1,p2		2 strong refs
	myfunction(p2);//把智能指针当做函数实参传递给函数 --引用计数增加1
	//作为函数实参传递，会增加引用
	auto p3 = myfunction02(p2);


	//2引用计数的减少
	auto p4 = make_shared<int>(400);//p4指向新对象，p4引用计数为1
	p1 = make_shared<int>(500);//p1引用计数减少为1

		//3.当一个shared_ptr类型的指针从1变为0，他会自动释放自己所管理（指向）的对象
	auto p5 = make_shared<int>(1000);
	auto p6 = make_shared<int>(1000);
	p6 = p5;//这是是赋值，p6的引用计数变为2， p5引用计数变为2，


	//shared_ptr返回有多少个智能指针指向某个对象
	int myCount = p6.use_count();
	cout << myCount << endl;	//2
	int myCount02 = p5.use_count();
	cout << myCount02 << endl;	//2

	//2.unique(*)是否该智能指针独占指向某个对象，返回true或者false
	shared_ptr<int> p7(new int(7));
	if (p7.unique())
	{
		cout << "unique成立，只有p7指向一块内存对象" << endl;
	}

	//reset不带参数，复位，置空
	shared_ptr<int> p8(new int(8));
	auto p9(p8);
	p8.reset();
	if (p8==nullptr)
	{
		cout << "p8指针为空" << endl;
	}
	//reset带参数new一个新对象
	shared_ptr<int> p10(new int(10));
	p10.reset(new int(11));//释放原来内存，指向新的内存

	//空指针也可以使用reset重新初始化
	shared_ptr<int>p11;
	p1.reset(new int(11));
	
	//4解引用
	shared_ptr<int>p12(new int(12));
	cout << *p12 << endl;
	
	//get()返回指针
	shared_ptr<int>p13(new int(13));
	int*p14 = p13.get();
	cout << *p14 << endl;
	//这里不要手工释放p14，它指向的内存空间归智能指针p13管理

	//swap()
	shared_ptr<string> p15(new string("aaaa"));
	shared_ptr<string> p16(new string("bbbb"));
	swap(p15, p16);
	cout << *p15 << endl;//bbbb
	cout << *p16 << endl;//aaaa

	//=nullptr
	p15 = nullptr;

	//智能指针名字作为判断条件
	if (p16)
	{
		cout << "p16指向一个对象" << endl;
		p16 = nullptr;
		
	}

	//指定自定定义的删除器
	shared_ptr<int>p17(new int(17), mydelete);
	p17 = nullptr;//调用自己写的删除器,自己需要在函数里面进行内存的释放
	//删除器可以是一个lambda表达式
	shared_ptr<int>p18(new int(18), [](int*p) {delete p; });


	//有些情况下，默认删除器处理不了---动态数组
	shared_ptr<int>p19(new int[10], [](int*p) {delete[]p; });

	//可以使用default_delete来做删除器，default_delete是标准库里面的模板类
	shared_ptr<A>pA(new A[10], std::default_delete<A[]>());//加上[]表示是删除的一个数组

	//c++17  定义数组的时候在<>中添加[]

	//shared_ptr<A[]>pA2(new A[10]);//这样c++17可以正常处理数组的释放问题


	//shared_ptr模板封装
	shared_ptr<int>p20 = make_shared_array<int>(5);
	
	system("pause");
	return 0;
}

/*
*(1)shared_ptr引用计数的增加和减少
*共享式指针--引用计数--每一个shared_ptr都指向相同的内存，最后一个指针负责释放。
*	1.引用计数的增加
*	每个shared_ptr都会记录有多少其他的shared_ptr指针指向相同的对象。
*	在如下情况下：所有指向这个兑现的shared_ptr引用计数都会增加1：
*		使用一个指针初始化另一个指针
*		把智能指针当做函数实参传递给函数（使用引用，引用计数不会增加）
*		作为函数返回值
*	2.引用计数的减少
*		1.给shared_ptr赋予新的值，让shared_ptr指向一个新的对象
*		2.局部的shared_ptr离开引用
*		3.当一个shared_ptr类型的指针从1变为0，他会自动释放自己所管理（指向）的对象
*		
*(2)shared_ptr类型指针的常用操作
*	1.shared_ptr返回有多少个智能指针指向某个对象
*	2.unique(*)是否该智能指针独占指向某个对象，返回true或者false
*	3.reset()复位，恢复，重置
*		reset()不带参数，那么指针是唯一指向的指针，那么释放指向的内存空间，并将指针置空
*				若不是指向该对象的唯一指针，那么引用该对象的引用计数会减少，同时将指针置空。
*		reset()带参数，参数一般是new出来的指针，
*			若指针是唯一指针，则释放指针指向的对象，让指针指向新对象
*			若指针不是唯一指针，则不释放指针，引用计数减少1，指向新对象
*	4.解引用，使用*或者指针指向的内容
*	5.get返回p中保存的指针，如果智能指针释放了所指向的对象，返回的裸指针也就没有意义。
*		因为有些第三方库的函数参数，有的需要一个裸指针，而不是智能指针
*	6.swap()交换两个智能指针所指向的对象
*	7.=nullptr指向空
*		1.将指针的引用计数减少1，若引用计数减少为0，则释放指针指向的对象
*		2.将智能指针置空
*	8.智能指针名字作为判断条件
*	9.指定删除器和数组问题
*		一定时机，智能指针会帮助我们删除所指向的对象，将delete运算符作为默认的资源析构方式。
*		如果我们有自己的需求，可以指定自己的删除器取代系统提供的默认删除器。
*		shared_ptr在参数中添加具体的删除器函数名字即可。
*
*	指定删除器额外说明：
*		******就算两个share_ptr指定了不同的删除器，但是只要他们执行的对象类型相同，这连个share_ptr类型指针就是同一个类型。
*			类型相同，就代表可以放到元素类型为该对象类型的容器里来。
*			
*		make_shared是提倡的方法，但是不能生成自己的删除器。
*(3)函数模板来封装shared_ptr数组
*
*
*其他：引用计数算在内的指针叫强引用。
*
*/
```