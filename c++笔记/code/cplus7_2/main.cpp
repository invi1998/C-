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
    
    // new 内存分配细节探秘
    // 一块内存的回收，影响范围很广，远远不止你申请的时候的内存大小
    // 分配内存这个事，绝对不是简简单单的分配出去一个需要的内存大小，他还会在这个内存块周围做很多处理，比如记录分配出去的内存大小等
    void func()
    {
       char *ppoint = new char[10];
       memset(ppoint, 0, 10);
       delete[] ppoint;
    }
}

namespace _nmsp2
{
    // 重载类中的operator new和 operator delete操作符
    // void *temp = operator new(sizeof(A));
    // A *pa = static_cast<A*>(temp);
    // pa->A::A();
    // pa->A::~A();
    // operator delete(pa);
    
    class A
    {
        public:
            int m_i = 100;
            static void *operator new(size_t size);
            static void operator delete(void *pheade);
            
            A()
            {
                std::cout << "A()" << std::endl;
            }
            ~A()
            {
                std::cout << "~A()" << std::endl;
            }
    };
    
    void *A::operator new(size_t size)
    {
        std::cout << "static void *operator new(size_t size);" << std::endl;
        A *ptmp = (A*)malloc(size);
        return ptmp;
    }
    
    void A::operator delete(void *pheade)
    {
        std::cout << "static void *operator delete(void *pheade);" << std::endl;
        free(pheade);
    }
    
    void func()
    {
        A *pa = new A();
        std::cout << pa->m_i << std::endl;
        delete pa;
        // static void *operator new(size_t size);
        // A()
        // 100
        // ~A()
        // static void *operator delete(void *pheade);
        
        // 如果不想调用我们自己写的operator new和operator delete，就想调用系统的，可以使用如下写法(操作符前加两::)
        A *pa2 = ::new A();
        pa2->m_i;
        ::delete pa2;

    }
}

namespace _nmsp3
{
    // 重载类中的operator new[]和 operator delete[]操作符
    
    class A
    {
        public:
            static void *operator new(size_t size);
            static void operator delete(void *pheade);
            static void *operator new[](size_t size);
            static void operator delete[](void *pheade);
            
            A()
            {
                std::cout << "A()" << std::endl;
            }
            ~A()
            {
                std::cout << "~A()" << std::endl;
            }
    };
    
    void *A::operator new(size_t size)
    {
        std::cout << "static void *operator new(size_t size);" << std::endl;
        A *ptmp = (A*)malloc(size);
        return ptmp;
    }
    
    void A::operator delete(void *pheade)
    {
        std::cout << "static void *operator delete(void *pheade);" << std::endl;
        free(pheade);
    }
    
    void *A::operator new[](size_t size)
    {
        std::cout << "size大小" << size << std::endl;
        std::cout << "static void *operator new[](size_t size);" << std::endl;
        A *ptmp = (A*)malloc(size);
        return ptmp;
    }
    
    void A::operator delete[](void *pheade)
    {
        std::cout << "static void *operator delete[](void *pheade);" << std::endl;
        free(pheade);
    }
    
    void func()
    {
        A *pa = new A[10]();
        delete[] pa;
        
        // 发现构造函数和析构函数都被调用了10次，但是operator new[] 和 operator delete[] 都各自只被调用一次
        
        // size大小18  // 这里多出来的8个字节，就是用来记录这里的数组大小
        // static void *operator new[](size_t size);
        // A()
        // A()
        // A()
        // A()
        // A()
        // A()
        // A()
        // A()
        // A()
        // A()
        // ~A()
        // ~A()
        // ~A()
        // ~A()
        // ~A()
        // ~A()
        // ~A()
        // ~A()
        // ~A()
        // ~A()
        // static void *operator delete[](void *pheade);
        
    }
}

int main()
{
    
    // _nmsp1::func();
    // _nmsp2::func();
    _nmsp3::func();
    
    return 0;
}


