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

using namespace std;

// unique_ptr<string> funcqu()
// {
//     // unique_ptr<string>pr(new string("返回局部对象"));
//     // return pr; // 从函数返回一个局部的unique_ptr对象
//                 // 返回这种局部对象，导致系统给我们生产一个临时的unique_ptr，调用unique——ptr的移动构造函数

//     // 当然，我们也可以这样写
//     return unique_ptr<string>(new string("返回局部对象"));
// }

void mydelete(string *pd)
{
    cout << "删除器执行" << endl;
    delete pd;
    pd = nullptr;
}

int main()
{
    // // 返回unique_ptr
    // unique_ptr<string>ps;
    // // 虽然unique_ptr不能拷贝，但是当这个unique_ptr将要被销毁，是可以拷贝的，最常见的就是从函数中返回
    // ps = funcqu();
    // // 可以用ps来接，则临时对象直接构造在ps里，如果不接，则临时对象会被释放，同时释放掉所指的对象的智能指针
    // cout << *ps << endl;
    
    // 指定删除器
    // 缺省使用delete删除对象指针
    // 格式：unique_ptr<指向对象类型，删除器>智能指针变量名；
    // 删除器，可调用对象，比如函数，类重载圆括号（）
    // unique_ptr删除器相对复杂，多了一步，先要在类型模板参数中传递进去类型名，然后，在参数中再给具体的删除器函数
    
    // 删除器的写法
    
    typedef void(*fp)(string *); // 定义一个函数指针类型，类型名为fp
    
    unique_ptr<string, fp>ps1(new string("rrrrrrrrrr"), mydelete);
    cout << sizeof(ps1) << endl; // 16
    
    // 使用using定义类型名
    using fp2 = void(*) (string*);
    unique_ptr<string, fp2>ps2(new string("rrrrrrrrrr"), mydelete);
    cout << sizeof(ps2) << endl; // 16
    
    typedef decltype(mydelete) *fp3; // 这里使用decltype来猜这个类型，这里多了一个*,因为
    // decltype的函数返回类型是void (string *)，加一个*表示这个是一个函数指针类型  即： void*(string*)
    unique_ptr<string, fp3>ps3(new string("rrrrrrrrrr"), mydelete);
    cout << sizeof(ps3) << endl; // 16
    
    // 也可以直接这样写
    unique_ptr<string, decltype(mydelete)*>ps4(new string("rrrrrrrrrr"), mydelete);
    cout << sizeof(ps4) << endl; // 16
    
    // 使用lambda表达式  
    // lambda表达式可以理解为带有operator（）类类型的对象
    // 所以，使用decltype推断出来它是个类类型，把lambda表达式看为类
    auto mydeleteLambda = [](string *pda) {
        delete pda;
        pda = nullptr;
        cout << "lambda表达式执行" << endl;
    };
    unique_ptr<string, decltype(mydeleteLambda)> ps5(new string("rrrrrrrrrr"), mydeleteLambda);
    cout << sizeof(ps5) << endl; // 8
    
    
    // 指定删除器额外说明
    // shared_ptr就算连个shared_ptr指定删除器不同，只要他们所指向的对象类型相同，那么这两个shared_ptr也属于同一类型
    // 但是 unique_ptr不一样，指定unique_ptr删除器，会影响unique_ptr的类型，所以从灵活性来讲，shared_ptr设计更灵活
    // 之前说shared_ptr的时候，删除器不同，只要指向类型一样，那么都可以放到同一个容器中
    // unique_ptr如果删除器不同，那么就等于整个unique_ptr类型不同，类型不同的智能指针是没法放到同一容器中
    
    
    // 尺寸问题
    // unique_ptr和裸指针大小引用
    string *p;
    cout << sizeof(p) << endl; // 8
    unique_ptr<string>ps(new string());
    cout << sizeof(ps) << endl; // 8
    // 如果你增加了自己的删除器，则整个大小可能会改变（比如使用lambda表达式则尺寸不会改变 ）
    
    // 增加字节对效率会有影响，所以自定义删除器需要慎用
    // shared_ptr，不管你是使用何种删除器，他的大小都是裸指针的两倍
    
    return 0;
}


```


unique_ptr使用方法总结和智能指针概述

```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
#include <memory>

using namespace std;


unique_ptr<string>myFunction()
{
	unique_ptr<string>pr(new string("dsfsdf"));
	return  pr;
	/*
	 * 从函数返回一个局部对象，系统给我们生成一个临时unique_ptr对象，调用unique_ptr的移动构造函数
	 * 
	 */
}

void myDelete(string *pdel)
{
	delete pdel;
	pdel = nullptr;
}
typedef void(*myf)(string *);
//定义函数指针类型 using myf=void(*)(string*)
//typedef decltype(myDelete)*myf;

//lambda表达式
auto mydel=[](string*del)
{
	delete del;
	del = nullptr;
};


int main(void)
{
	unique_ptr<string> p1;
	p1 = myFunction();//临时对象直接构造在p1中。不接受会释放掉临时对象及其所指向的内存空间。


	//指定删除器--取代系统默认的删除器
	unique_ptr<string, myf> p2(new string("jisuanj"), myDelete);

	//另一种写法
	unique_ptr<string, decltype(myDelete)*>p3(new string("jisann"), myDelete);
	//用lambda表达式来写，可以理解成带有operator()类型的类对象
	unique_ptr<string, decltype(mydel)>p4(new string("sdfdsf"), mydel);
	
	//unque_ptr和裸指针相同
	string *p = nullptr;
	cout << sizeof(p) << endl;//4
	unique_ptr<string>p5(new string("sdf"));
	cout << sizeof(p5) << endl;//4
	
	system("pause");
	return 0;
}

/*
*(1)返回unique_ptr智能指针
*是不能拷贝，要被销毁的时候，还是可以拷贝的，最常见的是从一个函数返回一个unique_ptr
*
*(2)指定删除器
*	1.缺省情况下，会用delete释放内存空间
*	unique_ptr<指定的对象类型，删除器>智能指针变量名。
*	删除器本质上是一个可调用对象。
*	shared_ptr删除器直接在参数里面加上函数名字即可。但是unique_ptr的删除器相对负载一点。
*	需要向类模板中传递具体类型和删除函数类型。
	unique_ptr<string, myf> p2(new string("jisuanj"), myDelete);
*
*	2.指定删除器额外说明
*		shared_ptr指定的删除器不同，但是指向的对象类型相同，两个shared_ptr就是同一种类型。
*		但是unique_ptr的类型包含删除器，也就说，删除器不同，unique_ptr不是同一个类型。
*
*(3)尺寸问题
*	shared_ptr是裸指针的2倍，但是unque_ptr和裸指针相同.但是删除器不同可能占用内存空间不同
*	lambda --4字节,
*	函数指针类型--8字节
*	增加字节对效率有影响，自定义删除器哟啊谨慎使用。
*	shared_ptr不管指定什么删除器都是裸指针的2倍。
*
*(4)智能指针总结
*	4.1智能指针背后的设计思想
*		主要目的：帮助我们释放内存，防止内存泄漏。
*	4.2auto_ptr为什么被废弃？	--设计问题，会默认转移所有权
*		c++98时代的智能指针，具有unique_ptr智能指针的一部分功能。
*		且不能在容器中保存，并不能从函数中返回。
*		虽然unique_ptr和auto_ptr都是独占式的，但是auto_ptr进行赋值操作=会默认转移所有权。
*	建议：使用unique_ptr取代auto_ptr
*	
*	4.3智能指针的选择
*		shared_ptr
*		unique_ptr
*		如果程序有多个指针指向同一个对象，使用shared_ptr
*		如果不需要多个指针指向同一个指针，使用unique_ptr指针
*	
*
*
*/
```