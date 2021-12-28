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
#include <list>
#include <mutex>
#include <condition_variable>
#include <future>

#include<bits/stdc++.h>

namespace _nmsp1
{
    class A
    {
        public:
            int m_i;
            
            A()
            {
                
            }
            ~A()
            {
                
            }
            
            virtual void func()
            {
                
            }
    };
    
    void func()
    {
        // new
        // new 类对象的时候加与不加括号的区别
        // 1)如果是一个空类，那么如下两者没有区别；
        // 2）假设类中有成员变量,
        // 则带括号的初始化会把一些成员变量有关的内存清零，但是不是把整个对象的内存全部清零
        // 3）当类中有构造函数,不管你有没有虚函数，下面两种写法没有区别，至于初始化结果，则是根据你类中定义的构造函数怎么是实现决定
        // 你自己定义的构造函数会怎么进行初始化，那么这里的初始化结果就是什么
        // 4）当类中没有自己的构造函数，但是有虚函数，那么初始化结果和情况（2）一样
        
        // new 干了啥
        // new 关键字（操作符）
        // new 干了两个事，一个是调用了operator new()，一个是调用类A的构造函数
        A *pa1 = new A();
            // 调用operator new() 
                // 调用malloc函数进行内存分配
            // 调用类A的构造函数 A::A() 有构造函数就调用（或者系统自动生成一个构造函数也调用，否者就不调用）
            
        // delete干了啥
        delete pa1;
            // 调用类A的析构函数 A::~A() （同样也需要类中有析构函数）
            // 然后调用 operator delete()
                // operator delete()调用 free()
                
        
        
        A *pa2 = new A;
        
        int *p3 = new int; // 初值随机
        int *p4 = new int(); // 初值为0
        int *p5 = new int(100); // 初值100
        
        std::cout << *p3 << std::endl;
    }
}

int main()
{
    
    _nmsp1::func();
    
    return 0;
}


