```c++
/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>

#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

void func()
{
    // 我们在一个函数内定义一个对象，一旦离开了这个函数，那么这个对象的生命周期就结束了
    int a; // 栈上分配
    
    // 如果是一个局部静态对象，不同的编译器分配的时候会有不同，常见的就是在调用到这个对象（这个函数的时候）会给局部静态对象分配内存
    // 他的生命周期会持续到这个程序执行完毕才被释放
    static int k = 100; // 在静态变量存储区分配
    
    // 这些区域的分配和销毁不需要程序员区控制，编译器会自动进行分配和销毁
}

class A
{
    public:
        A()
        {
            cout << "A" << endl;
        }
};

int main()
{
    // 直接内存管理（new/delete）
    
    // A a; // 创建对象，编译器自动完成
    
    // new 分配方式，我们称为动态分配（分配在堆上）：直接内存管理（new/delete）
    // 如何初始化
    // int *pint = new int;
    // string *pointstring = new string; // 空字符串，说明调用了string的默认构造函数
    
    int *pint = new int(23); // 用圆括号给一个int的动态对象初值
    string *mystring2 = new string(5, 'a'); // 生成一个字符串，字符串内容是5个a
    
    vector<int> *pvec = new vector<int>{1,2,3,4,5}; // new 一个容器对象，里面有5个元素
    
    // 概念“值初始化”：用空括号（）来进行初始化
    string *mystring3 = new string(); // 是否是值初始化都是空字符串
    int *intp = new int(); // 值初始化的方式，intp的值被初始化为了0,而不用圆括号是一个随机值。
    
    A *pa = new A;
    A *paa = new A();
    // 自己定义的类对象是否是值初始化，效果都一样，都是调用的构造函数和
    // 不过new对象的时候，能够进行值初始化都进行值初始化，防止他的值没有被初始化
    
    
    
    // C++11中， auto可以和new配合使用
    string *mystring4 = new string(5, 'd');
    auto mystring5 = new auto(mystring4); // mystring5 = string **她是一个指针的指针
    // string **mystring5 = new string *(mystring4);
    
    cout << mystring4 << endl; // 0x1765d50
    cout << mystring4->c_str() << endl; // ddddd
    cout << *mystring5 << endl; // 0x1765d50
    cout << (*mystring5)->c_str() << endl; // ddddd
    delete mystring5;
    // 发现他两指向的都是同一块内存，所以，使用delete进行内存销毁的时候，只能删除一个，不然，就会出错，
    // 因为你删除一个的时候，另一个也被删除了，再去删除就会删除一块不知名的内存块，造成错误
    
    // const 对象也可以进行动态分配
    const int *pintint = new const int(200);
    cout << *pintint << endl;
    cout << *mystring5 << endl;
    
    
    int i = 12;
    int *op = &i;
    // delete op; // 千万不要这样做。因为 i 是在栈中分配的，没有使用new分配内存，不能执行delete，否者执行会爆异常
    
    int *p = new int();
    int *p2 = p;
    delete p2; // 没问题
    // delete p; // 异常，因为p和p2都指向同一个内存，不能重复删除同一块内存
    
    p2 = nullptr; // delete之后，建议将这个指针设置为空，表明该指针不指向任何对象，避免指针悬空
    
    
    return 0;
}

```

new和delete常见用法

```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>

using namespace std;



int main(void)
{
	static  int j = 23;//局部静态对象，在静态区分配内存

	int i = 10;//由系统进行分配和释放
	int *p1 = new int;//动态内存分配，初值未定义
	string *s1 = new string;//空字符串，系统调用string默认的构造函数
	string *s2 = new string(5, 'a');//aaaaa


	vector<int>* pointVector = new vector<int>{ 1,2,3,4,5 };//注意这种类型是存放int的vector*类型
	

	//值初始化
	string *s3 = new string();//用空括号来初始化就叫值初始化
	int *p2 = new int();//值为0，不是未定
	//new创建对象的时候，能够进行“值初始化”就进行初始化，防止没有初始化出现问题
	//
	//c++11中，auto可以和new配合使用
	string *s4 = new string("jisuanjizu");
	auto s5 = new auto(s4);//s4,s5指向同一段内存 s5是string**类型 指针的指针


	//const对象也可以动态分配
	const int*p4 = new const int(200);


	
	system("pause");
	return 0;
}

/*
*(1)直接内存管理 new delete
*	在一个函数内定义一个对象，但是一旦离开这个函数，这个对象就失效了，这是局部对象。
*	静态局部对象
*	1.new分配内存方式我们称为动态分配，分配在堆上，直接由程序员进行内存管理。
*	delete作用是回收new分配的内存--释放内存。不是new分配的内存，不能使用delete使用。
*	2.delete释放一块内存，只能delete一块，不能delete多次。
*	3.空指针可以多次删除，但是没什么意义。
*	4.new出来的内存一定不要忘记delete,否则内存泄漏。时间一长导致资源紧张，运行崩溃。
*	5.delet后的内存不要再使用。
*	6.指针不指向任何内存空间，要指向null.
*	7.同一块内存释放两次，也会报异常。
*	8使用new、delete进行内存管理要十分小心。
*		c++11中引入了“智能指针”，当你忘记delete，智能指针会自动帮你delete
*
*(2)创建新工程，观察内存泄漏
*MFC应用程序在一定程序上（程序退出的时候），能帮助我们发现内存泄漏。
*
*(3)
*
*
*/
```