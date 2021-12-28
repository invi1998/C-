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
#include <ctime>

#include<bits/stdc++.h>

using namespace std;

namespace _nmsp1
{
    // 内存池概念和实现原理
    // malloc内存分配，浪费内存，频繁分配小块内存浪费更加明显
    // “内存池”需要解决的问题
    // 1）减少malloc的次数，减少malloc的次数就意味着减少内存的浪费
    // 2）减少malloc的调用次数，是否能提高程序运行速度？能，但是效率提升不明显
    
    // “内存池”实现原理
    // 1）使用malloc申请一个大块内存，当有内存分配需求的时候，再从这块内存中一点点的进行分配
    // 当大块内存分配光了，然后会再次malloc，再一点点进行分配
    
    
    
    // 实现一个针对该类的内存池
    
    class A
    {
        public:
            static void *operator new(size_t size);
            static void operator delete(void *phead);
            static int m_count; // 记录分配次数
            static int m_iMallocCount; // 没malloc一下，就统计一次
            
        private:
            A *pnext;
            static A *n_FreePosi; // 总是指向一块可以分配出去的内存的首地址
            static int n_sTrunckCount; // 一次分配多少倍的该类内存
            
    };
    
    int A::m_count = 0;
    int A::m_iMallocCount = 0;
    
    A *A::n_FreePosi = nullptr;
    int A::n_sTrunckCount = 50; // 一次分配5倍的内存大小作为内存池
    
    void *A::operator new(size_t size)
    {
        // A *ppoint = (A*)malloc(size);
        // return ppoint;
        A *tempPoint;
        if (n_FreePosi == nullptr)
        {
            // 可用内存为空，申请分配大块内存
            size_t realsize = n_sTrunckCount * size;
            n_FreePosi = reinterpret_cast<A*>(new char[realsize]); // 全局传统new，调用的是系统层的malloc
            tempPoint = n_FreePosi; // 将这个tempPoint指向这个内存块的首地址
            
            // 把分配出来的这5块内存彼此之间连起来，供后续分配使用
            for(;tempPoint != &n_FreePosi[n_sTrunckCount - 1]; ++tempPoint)
            {
                // 这里将每个内存块的pnext都指向下一个内存块
                tempPoint->pnext = tempPoint + 1;
            }
            // 最后这个内存块的pnext指向空
            tempPoint->pnext = nullptr;
            ++m_iMallocCount;
            // 统计这里分配大块内存的次数
        }
        
        tempPoint = n_FreePosi;
        // 将这个首地址赋给临时变量返回
        n_FreePosi = n_FreePosi->pnext;
        // 然后将可用内存指向该内存的pnext（也就是下一个内存块）
        ++m_count;
        // 统计new的调用次数（operator new）
        
        return tempPoint;
    }
    
    void A::operator delete(void *phead)
    {
        // free(phead);
        // return;
        
        static_cast<A*>(phead)->pnext = n_FreePosi;
        n_FreePosi = static_cast<A*>(phead);
    }
    
    
    void func()
    {
       clock_t start, end;
       start = clock();
       for (int i = 0; i < 500000; i++)
       {
           A *pa = new A();
       }
       end = clock();
       cout << "申请分配内存的次数为：" << A::m_count << "实际malloc的次数为" << A::m_iMallocCount << "用时：" << end - start  << std::endl;
    
        // 申请分配内存的次数为：500000实际malloc的次数为100000用时：8098
        // 申请分配内存的次数为：500000实际malloc的次数为10000用时：8150
    }
}

namespace _nmsp2
{
    // 嵌入式指针（embedded pointer）
    // 一般应用在内存池技术中，要成功使用嵌入式指针需要有个前提条件（类A对象的sizeof不小于四字节）
    // 工作原理：借用类A对象所占用的内存空间中的前四个字节，这四个字节用来链住空闲的内存块
    // 但是，一旦某一块被分配出去，那么这个块的前4个字节就不需要了，此时这4个字节就可以被类A对像正常使用
    
    class TestEP
    {
        public:
            int n_i;
            int n_j;
        public:
        // 这个成员是一个结构体，然后这个结构体的成员是一个指针，指向的是另外一个解构的对象
            struct obj
            {
                struct obj *next; // 这个next就是嵌入式指针
                // 自己是一个obj解构对象，然后把自己这个next指针指向另一个obj解构对象，最终把多个obj类型通过next像链表一样窜起来
            };
    };
    
    void func()
    {
        TestEP myTest;
        std::cout << sizeof(myTest) << std::endl; // 8
        
        TestEP::obj *ptemp; // 定义一个指针
        ptemp = (TestEP::obj *)&myTest; // 把mytest对象的首地址给指针ptemp，这个ptemp就指向myTest对象的首地址
        std::cout << sizeof(ptemp->next) << std::endl; // 8
        std::cout << sizeof(TestEP::obj) << std::endl; // 8
    }
}

namespace _nmsp3
{
    // 内存池代码改进
    // 单独为内存池技术写一个类
    class myalloctor
    {
        public:
            void *allocate(size_t size)
            {
                obj *tplink;
                if (n_FreePosi == nullptr)
                {
                    // 为空，就要申请一块大内存
                    size_t realsize = n_sTrunckCount * size;
                    n_FreePosi = (obj*)malloc(realsize);
                    tplink = n_FreePosi;
                    
                    // 把分配出来的这5块内存彼此之间连起来
                    for (int i = 0; i < n_sTrunckCount - 1; ++i)
                    {
                        tplink->next = (obj *)((char *)tplink + size);
                        // 这里需要把tplink转为char*，因为char*是一个字节，只有使用char* + 内存块大小才能真正调到下一块内存
                        tplink = tplink->next;
                    }
                    tplink->next = nullptr;
                }
                tplink = n_FreePosi;
                n_FreePosi = n_FreePosi->next;
                return tplink;
            }
            
            // 释放内存接口
            void deallocate(void *pthead)
            {
                ((obj*)pthead)->next = n_FreePosi;
                n_FreePosi = (obj*)pthead;
            }
            
        private:
            // 写在类内的解构，这样让其只能在类内使用
            struct obj
            {
                struct obj *next; // 嵌入式指针
            };
            int n_sTrunckCount = 5;
            obj * n_FreePosi = nullptr;
    };
    
    class A
    {
        public:
            int m_i;
            int m_j;
            
        public:
            static myalloctor myalloc; // 静态成员变量，跟着类走，不占用类对象内存空间
            static void *operator new(size_t size)
            {
                return myalloc.allocate(size);
            }
            static void operator delete(void *phead)
            {
                return myalloc.deallocate(phead);
            }
            
    };
    
    myalloctor A::myalloc; // 定义静态成员变量
    
    void func()
    {
        A *mpya[100];
        for (int i=0; i<15; ++i) // ++i是右值，编译时可以扫产生一次中间变量
        {
            mpya[i] = new A();
            printf("%p\n", mpya[i]);
        }
        for (int i = 0; i<15; ++i)
        {
            delete mpya[i];
        }
    }
}

namespace _nmsp4
{
    // 内存池代码改进
    // 单独为内存池技术写一个类
    class myalloctor
    {
        public:
            void *allocate(size_t size)
            {
                obj *tplink;
                if (n_FreePosi == nullptr)
                {
                    // 为空，就要申请一块大内存
                    size_t realsize = n_sTrunckCount * size;
                    n_FreePosi = (obj*)malloc(realsize);
                    tplink = n_FreePosi;
                    
                    // 把分配出来的这5块内存彼此之间连起来
                    for (int i = 0; i < n_sTrunckCount - 1; ++i)
                    {
                        tplink->next = (obj *)((char *)tplink + size);
                        // 这里需要把tplink转为char*，因为char*是一个字节，只有使用char* + 内存块大小才能真正调到下一块内存
                        tplink = tplink->next;
                    }
                    tplink->next = nullptr;
                }
                tplink = n_FreePosi;
                n_FreePosi = n_FreePosi->next;
                return tplink;
            }
            
            // 释放内存接口
            void deallocate(void *pthead)
            {
                ((obj*)pthead)->next = n_FreePosi;
                n_FreePosi = (obj*)pthead;
            }
            
        private:
            // 写在类内的解构，这样让其只能在类内使用
            struct obj
            {
                struct obj *next; // 嵌入式指针
            };
            int n_sTrunckCount = 5;
            obj * n_FreePosi = nullptr;
    };
    
// 宏实现，简化类A的写法
#define DECLARE_POOL_ALLOC()\
public:\
    static myalloctor myalloc;\
    static void *operator new(size_t size)\
    {\
        return myalloc.allocate(size);\
    }\
    static void operator delete(void *phead)\
    {\
        return myalloc.deallocate(phead);\
    }\
    
// -------------------
#define IMPLEMENT_POOL_ALLOC(classname)\
myalloctor classname::myalloc;
// -------------------

    class A
    {
        DECLARE_POOL_ALLOC();
        
        public:
            int m_i;
            int m_j;
            
    };
    
    IMPLEMENT_POOL_ALLOC(A);
    
    void func()
    {
        A *mpya[100];
        for (int i=0; i<15; ++i) // ++i是右值，编译时可以扫产生一次中间变量
        {
            mpya[i] = new A();
            printf("%p\n", mpya[i]);
        }
        for (int i = 0; i<15; ++i)
        {
            delete mpya[i];
        }
    }
}

int main()
{
    
    // _nmsp1::func();
    // _nmsp2::func();
    // _nmsp3::func();
    _nmsp4::func();
    
    return 0;
}



