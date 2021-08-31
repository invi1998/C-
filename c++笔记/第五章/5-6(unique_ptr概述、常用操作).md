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

using namespace std;

auto myfunc()
{
    return unique_ptr<string>(new string("ooooo")); // 返回一个临时对象（临时对象都是右值）
}

int main()
{
    // unique_ptr
    // 独占式的智能指针，同一时刻只能有一个unique_ptr指向这个对象（这块内存）
    // 当这个unique_ptr被销毁时，它所指向的对象也被销毁
    // 格式
    // unique_ptr<T> name；
    // 常规初始化
    // unique_ptr<int> pi; // 可以指向int对象的一个空指针
    // if (pi == nullptr)
    // {
    //     cout << "nullptr" << endl;
    // }
    
    // unique_ptr<int>pi1(new int(12221)); // 此时pi1指向值为12221的int对象
    // cout << *pi1 << endl;
    
    // // make_unique()
    // // C++11中没有，C++14中才有，不支持指定删除器，如果不需要删除器，优先使用make_unique，更高的性能
    // unique_ptr<int> pi2 = make_unique<int>(123);
    // auto pi3 = make_unique<int>(234);
    // unique_ptr<int>pi4(new int(99));
    
    // unique_ptr常规操作
    // unique_ptr不支持的操作
    // unique_ptr<string> ps1(new string("rrrrr"));
    // unique_ptr<string> ps2(ps1); // 该智能指针不支持拷贝动作（定义时初始化）
    // unique_ptr<string> ps3 = ps1; // 该智能指针不支持拷贝动作（定义时初始化）
    // unique_ptr<string> ps4;
    // ps4 = ps1; // 独占式的智能指针不支持复制操作
    
    // 移动语义
    // unique_ptr<string> ps1(new string("rrrrr"));
    // unique_ptr<string> ps2 = std::move(ps1); // 移动完之后，ps1为空，ps2指向原来ps1的指向
    
    // release（）；放弃对指针的控制权，切断了智能指针和其指向的对象之间的联系，并返回裸指针，同时将智能指针置空
    // 、返回的裸指针可以手动delete，也可以用来初始化另一个智能指针，或者
    // unique_ptr<string> ps1(new string("sdfsdg"));
    // unique_ptr<string> ps2(ps1.release());
    // cout << *ps2 << endl;
    
    // // ps2.release(); //返回了裸指针，但是却没有对其进行删除处理，导致内存泄漏
    // string *pn = ps2.release();
    // delete pn;
    
    // reset()
    // reset()不带参数，释放智能指针所指向的对象，并将智能指针置空
    // reset()带参数的情况，释放智能指针所指向的对象，并指向参数里的新对象
    // unique_ptr<string>ps1(new string("sgsdfsd"));
    // unique_ptr<string>ps2(new string("ssssss"));
    // ps1.reset(ps2.release()); // reset释放ps1指向的内存，让ps1指向ps2指向的内存，同时ps2被置空
    // cout << *ps1 << endl; // ssssss

    // ps1.reset(new string("aaaaaaaaaaaa"));
    // cout << *ps1 << endl; // aaaaaaaaaaaa
    
    //  = nullptr;
    // 释放智能指针所指向的对象、并将智能指针置空
    // unique_ptr<string>ps1(new string("sgsdfsd"));
    // ps1 = nullptr;
    
    // 指向一个数组(这里不要忘记[]，其实原理和shared_ptr是一样的)
    // unique_ptr<int[]> ptarray(new int[10]); // 注意需要跟上【】
    // ptarray[1] = 23;
    // ptarray[6] = 90;
    
    // cout << ptarray[6] << endl;
    // cout << ptarray[5] << endl;
    
    // get（）
    // 返回智能指针中的裸指针
    // unique_ptr<string> ps1(new string("sdfs"));
    // auto *p = ps1.get();
    // *p = "pppp";
    // delete p; // 千万不要delete裸指针
    
    // 解引用 * 获取该智能指针指向的对象,可以直接操作
    // unique_ptr<int[]>pt2(new int[10]);
    // *pt[0]; // 对于定义内容是数组，是没有*解引用运算符的，直接pt[index]就可以范文数组元素
    
    // swap（）交换两个智能指针指向的对象
    // unique_ptr<int>ps1 = make_unique<int>(4400);
    // unique_ptr<int>ps2 = make_unique<int>(5500);
    
    // std::swap(ps1, ps2);
    // ps1.swap(ps2);
    
    // 智能指针名字作为判断条件
    // unique_ptr<string>ps1(new string("sssss"));
    // if (!ps1)
    // {
    //     cout << "null" << endl;
    // } else {
    //     cout << "not null" << endl;
    // }
    
    // 转换为shared_ptr
    // 如果unique_ptr为右值时，就可以将它赋值给shared_ptr，
    // 因为shared_ptr他包含一个显式的构造函数，可用于将右值unique_ptr转换为shared_ptr,
    // 然后这个shared_ptr将接管这个unique_ptr的所有权
    
    
    shared_ptr<string> pss1 = myfunc(); // 这里会创建控制块
    
    unique_ptr<string>ps(new string("ggggggg"));
    shared_ptr<string>sps(std::move(ps));
    cout << *sps << endl; // ggggggg

    return 0;
}


```

unique_ptr概述和常见操作
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
	A(){}
	~A()
	{
		;
	}
};

auto myFunction()
{
	return unique_ptr<string>(new string("jisuanjiz"));//这是个右值
	//临时对象都是右值
}

int main(void)
{
	unique_ptr<int>p1;//指向一个空指针
	if (p1==nullptr)
	{
		cout << "p1是空指针" << endl;

	}
	unique_ptr<int>p2(new int(34354));//p2指向一个值为34354的int对象
	//make_unique c++14中引入，不需要指定删除器的时候选择这个
	unique_ptr<int>p3 = make_unique<int>(234);
	auto p4 = make_unique<int>(200);//p4是一个智能指针

	//2.1
	unique_ptr<string>p5(new string("jisuanjizuchengyaunli"));
	//智能指针不支持拷贝动作,也不支持赋值操作
	//unique_ptr<string> p6(p5);//error
	//unique_ptr<string>p7 = p5;//error

	//2.2移动语义
	unique_ptr<string>p8 = std::move(p5);//p5为空，p8指向原来的内存空间

	//2.3 release()释放
	unique_ptr<string>p9(p5.release());//使用p5的裸指针来初始化p9
	string *p10 = p9.release();
	delete p10;//一旦release()就需要手工释放
	
	//2.4
	unique_ptr<string>p11(new string("dsfsdf"));
	//p11.reset();//释放内存空间，
	p11.reset(p5.release());//release释放p5指向的内存空间的联系，同时p11指向这个空间
	
	//2.5nullptr
	unique_ptr<int>p12(new int(234));
	p12 = nullptr;

	//*2.6指向一个数组
	unique_ptr<int[]>pArray(new int[123]);
	pArray[0] = 0;
	pArray[1] = 0;

	unique_ptr<A[]>pAA(new A[10]);//当有自己的析构函数<>里面必须加上[]，否则会报异常

	//2.7get() 返回智能指针中返回的裸指针
	//注意得到裸指针不要释放内存空间，让智能指针自己释放更好，否则会释放2次出错
	
	

	//2.11 转换成shared_ptr类型，转换后赋值
	shared_ptr<string>p13 = myFunction();

	
	system("pause");
	return 0;
}

/*
*(1)unique_ptr独占式指针-对象的专属所有权
*同一时刻智能有一个unique_ptr指向这个对象。当unique_ptr被释放的时候，它所指向的对象也被销毁。
*	1.常规初始化
*	2.
*(2)unique_ptr常用操作
*	2.1unique_ptr不支持的操作
*	2.2 移动语义
*	2.3release(),放弃对指针指向对象的控制权，返回裸指针，同时将智能指针置空。
*		返回的裸指针可以正常使用。
*	2.4reset()
*	不带参数，释放智能指针对象所占的内存空间，并经智能指针置空
*	带参数，释放原来的内存空间，并且指向新对象
*
*	2.5 =nullptr释放智能指针所指向的内存空间，并将智能指针置空
*
*	2.6指向一个数组
	2.7get() 返回智能指针中返回的裸指针
	
*	2.8解引用 *获取智能指针的对象，直接进行操作
*	2.9swap()交换两个智能指针所指向的对象
*	2.10智能指针名字作为if判断条件
*
*	2.11 unique_ptr转换成shared_ptr类型，转换后赋值
*	shared_ptr包含一个显式的构造函数，
*	转换的过程中会创建控制块
*(3)
*
*	unique_ptr是没有控制块的
*
*/
```