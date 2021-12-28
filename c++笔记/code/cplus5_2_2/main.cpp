/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

shared_ptr<int> makes(int value)
{
    // return new int(value); // 不可以，无法将new得到的int *转换为shared_ptr
    return shared_ptr<int>(new int(value)); // 可以，显式的用int *创建shared_ptr<int>对象
}

int main()
{
    int *p = new int();
    int *q = p;
    int *r = q;
    // 只有pqr都不在使用了，才能进行内存释放
    // cout << p << endl;
    // cout << q << endl;
    // cout << r << endl;
    // 0x638c20
    // 0x638c20
    // 0x638c20
    
    // auto_ptr (C++98, 目前auto_ptr已经在新标准中被弃用，尽量避免使用)
    // unique_ptr (c++11)
    // shared_ptr (c++11)
    // weak_ptr (c++11)
    // 这三中智能指针都是类模板，我们可以将new获得的指针赋值给他们
    // shared_ptr 是共享式指针，多个指针指向同一个对象，最后一个指向被销毁时，这个对象才会被释放
    // unique_ptr 独占式指针，同一个时间内，只有一个指针指向对象。当然该独享的所有权是可以被移交的
    // weak_ptr 是辅助shared_ptr的
    
    shared_ptr<int> pi(new int(100)); // 指向一个值为100的int
    // shared_ptr<int> pi = new int(100); // 错误。因为智能指针时explicit，不可以进型隐式类型转换，必须使用直接初始化形式（一般带=的都是隐式类型转换）
    cout << pi << endl;
    
    
    shared_ptr<int> ptr = makes(400);
    cout << *ptr << endl;

    // 裸指针可以初始化shared_ptr,但是不推荐，智能指针和裸指针不要穿插使用
    // int *ppi = new int;
    // shared_ptr<int> spi(ppi); // 可以，但是不推荐，会有使用陷阱
    
    // 非要用，也得是使用new来进行
    shared_ptr<int> spi(new int);
    
    // make_shared 函数
    // 他是标准库里的一个函数模板。安全，高效
    // 他能在动态内存（堆）中，创建和初始化一个对象，并返回此对象的shared_ptr
    shared_ptr<int> p2 = make_shared<int>(100); // 这个shared_ptr 指向一个值为100的内存， 有点类似int *p1 = new int (100);
    shared_ptr<string> p3 = make_shared<string>(5, 'a');
    cout << *p3 << endl; // aaaaa
    
    shared_ptr<int> p4 = make_shared<int>(); // p4指向一个int，int里保存的值是0
    p4 = make_shared<int>(500); // p4指向一个新的int，int里保存的是500， P4首先先释放刚才指向0的内存，然后再指向500的int内存
    
    return 0;
}
