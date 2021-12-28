/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

// void *operator new(size_t size)
// {
//     std::cout << "operator new()" << std::endl;
//     return malloc(size);
// }

// void *operator new[](size_t size)
// {
//     std::cout << "operator new[]()" << std::endl;
//     return malloc(size);
// }

// void operator delete(void *phead)
// {
//     std::cout << "operator delete()" << std::endl;
//     free(phead);
// }
// void operator delete[](void *phead)
// {
//     std::cout << "operator delete[]()" << std::endl;
//     free(phead);
// }

namespace _nmsp1
{
    
    // 重载全局operator new和operator delete
    // 重载全局operator new[]和operator delete[]
    
    // 类内的重载operator new和operator delete 和 operator new[]和operator delete[]
    // 会覆盖全局的重载
    
    class A
    {
        public:
            A()
            {
                std::cout << "A::A()" << std::endl;
            }
            ~A()
            {
                std::cout << "A::~A()" << std::endl;
            }
            void *operator new(size_t size)
            {
                std::cout << "(A*) operator new()" << std::endl;
                A *ppoint = (A*)malloc(size);
                return ppoint;
            }
            
            void *operator new[](size_t size)
            {
                std::cout << "(A*) operator new[]()" << std::endl;
                A* ppoint = (A*)malloc(size);
                return ppoint;
            }
            
            void operator delete(void *phead)
            {
                std::cout << "(A*) operator delete()" << std::endl;
                free(phead);
            }
            void operator delete[](void *phead)
            {
                std::cout << "(A*) operator delete[]()" << std::endl;
                free(phead);
            }
        
    };
    
    void func()
    {
        int *pi = new int(100);
        delete pi;
        char *pcarr = new char[10];
        delete[] pcarr;
        A *pa = new A();
        delete pa;
        A *pa_arr = new A[3];
        delete[] pa_arr;
    }
}

namespace _nmsp2
{
    // 定位new  (placement new)
    // 有placement new 但是没有对应的placement delete
    // 功能： 在已经分配的原始内存中初始化一个对象
    // 1）已经分配，定位new并不分配内存，你需要事先将这个定位new要使用的内存分配出来
    // 2）初始化一个对象的内存：我们可以理解为调用这个对象的构造函数
    // 所以，定位new就是能够在一个预先分配好的内存块上构造一个对象
    // 格式：
    // new (地址) 类类型（参数）;
    
    class A
    {
        public:
            int m_i;
            
            A():m_i(0)
            {
                std::cout << "A::A():m_i(0)" << std::endl;
            }
            A(int tempValue):m_i(tempValue)
            {
                std::cout << "A::A(int tempValue):m_i(tempValue)" << std::endl;
            }
            
            ~A()
            {
                std::cout << "A::~A()" << std::endl;
            }
            
            // 定位new重载
            void *operator new(size_t size, void *phead)
            {
                std::cout << "*operator new(size_t size, void *phead)" << std::endl;
                return phead; // 收到内存开始地址，只需要原路返回
            }
    };
    
    void func()
    {
        // 定位new必须要先有内存，所以先申请出一个内存
        void *myplace = (void *)new char[sizeof(A)]; // 为了通用性，这里都是用void *进行分配
        A* pmyAOBJ = new (myplace) A(); // 调用无参构造函数，足以这里并不会额外分配内存
        delete pmyAOBJ;
        
        
        void *myplace2 = (void *)new char[sizeof(A)];
        A* pmyAOBJ2 = new (myplace2) A(1234); // 调用一个参数的构造函数，这里并不会额外分配内存
        pmyAOBJ2->~A(); // 调用类A的析构函数.手动调用类析构函数是可以的，但是手动调用构造函数一般不行
        delete[] (void *)myplace2;
    }
}

namespace _nmsp3
{
    // 多种版本的operator new重载
    // 可以重载多个版本的operator new,只要每个版本参数不同就行，但是第一个参数必须为size_t,表示你要new 对象的sizeof值
    
    
    class A
    {
        public:
            A()
            {
                std::cout << "A::A()" << std::endl;
            }
            
            void *operator new(size_t size, int v1, int v2)
            {
                std::cout << v1 + v2 << std::endl;
                std::cout << "*operator new(size_t size, int v1, int v2)" << std::endl;
                A *ppoint = (A*)malloc(size);
                return ppoint;
            }
    };
    
    void func()
    {
        A *pa = new (123, 123) A(); // 自定义new
    }
}

int main()
{
    // _nmsp1::func();
    // _nmsp2::func();
    _nmsp3::func();

    return 0;
}
