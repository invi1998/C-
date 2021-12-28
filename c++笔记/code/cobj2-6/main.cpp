/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

using namespace std;

class X
{
    public:
        int m_i;
        X(const X& x)
        {
            m_i = x.m_i;
            std::cout << "X::X(const X& x)" << std::endl;
        }
        X()
        {
            m_i = 0;
            std::cout << "X::X()" << std::endl;
        }
        ~X()
        {
            std::cout << "~X()" << std::endl;
        }
};

void func(X temx)
{
    return;
}

// 老编译器视角
// void func(X &temx)
// {
//     return;
// }

X funcx()
{
    X x;
    return x;   // 系统产生临时对象并把x的内容拷贝构造给了临时对象
}

int main()
{
    // 程序转换语义
    // 我们写的代码，编译器会对代码进行拆分，拆分成编译器更容易理解和实现的代码
    // 编译器是如何解析这些代码的。
    // 程序员角度/编译器角度
    
    // 1）定义时初始化对象
    X x0;
    x0.m_i = 100;
    // 定义对象并用x0来进行初始化（一下写法均调用了拷贝构造函数）
    X x1 = x0;
    X x2(x0);
    X x3 = (x0);
    
    // 切换到编译器视角，编译器会干两个事
    // X x100;         // 1）定义对象，并为对象分配内存，从编译器视角来看，这句并没有调用类的构造函数
    // x100.X::X(x0);  // 2）直接调用对象的拷贝构造函数
    
    // 2）参数的初始化（现代编译器视角）
    func(x0);
    
    // 老编译器器视角
    X tempobj;          // 编译器产生一个临时对象
    tempobj.X::X(x0);   // 调用拷贝构造函数
    func(tempobj);      // 用临时对象调用func
    tempobj.X::~X();    // func调用完之后，本析构被调用
    
    // 3）返回值初始化
    X xm = funcx();
    
    
    
    
    
    
    return 0;
}
