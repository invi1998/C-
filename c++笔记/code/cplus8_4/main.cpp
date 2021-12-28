/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <queue>
#include <forward_list>
#include <map>
#include <unordered_set>
#include <list>

using namespace std;

namespace _nmsp1
{
    // 分配器概述：和容器紧密关联，一起使用
    // 确切的说，是内存分配器，所以分配器引入的主要目的，就是扮演内存池的角色，通过大量减少
    // malloc的调用，来节省内存，甚至还有一定效率的提高
    
    // 分配器的使用：
    // allocator分配器，是一个类模板，我们写代码时极少会直接使用到这个allocator这种容器的分配器
    void func()
    {
        list<int> mylist; // 双向链表，不要求内存挨着
        mylist.push_back(10);
        mylist.push_back(323);
        mylist.push_back(90);
        
        for(auto iter = mylist.begin(); iter!=mylist.end(); iter++)
        {
            std::cout << *iter << std::endl; // 打印值
            std::cout << &(*iter) << std::endl; // 打印值所在的内存地址
            // 10
            // 0x908c30
            // 323
            // 0x908c50
            // 90
            // 0x908c70
            // 经过测试，发现，缺省的（默认的）分配器并没有采用内存池技术
        }
        
        allocator<int> aalloc; // 定义一个allocator对象，为类型为int的兑现分配内存
        int *p = aalloc.allocate(3); // allocate是分配器中的重要函数，用来分配一段原始的未构造的内存
        // 里面的参数3表示这段内存能保存3个类型为int的对象（12字节）
        int *q = p;
        *q = 1; q++;
        *q = 2; q++;
        *q = 3;
        
        aalloc.deallocate(p, 3); // deallocate()也是分配器中的重要函数，用于释放内存，需要记住分配了几个对象
        // 释放时需要释放正确数量的数量
    }
}

int main()
{
    // printf("Hello World");
    
    _nmsp1::func();

    return 0;
}
