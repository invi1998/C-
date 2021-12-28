/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
#include <memory>
#include <array>

using namespace std;

namespace _nmsp1
{
    // stl: 容器、迭代器，算法（函数），分配器（内存分配）、其他（适配器，仿函数、函数对象等）
    // 容器分类
    // vector list map
    // (1)顺序容器（sequence container）放进去在哪，这个元素就排在哪
    // Array, vector, deque, list（双向链）, Forward-list（单链）
    // (2)关联性容器（associative container） 数: 元素是键值对，适合做查找;你能控制插入的内容，但是不能控制插入的位置
    // set/multiset  map/multimap
    // (3)无序容器 （unodered container）哈希表  C++11里退出的，元素的位置不重要，重要的是元素是否在这个容器里面
    // 无序容器也属于一直给你关联容器
    // unodered_set,unodered_multiset,unodered_map,unodered_multimap
    
    // Array(数组，内存空间连续，大小空间固定，申请的时候是多大，就是多大，不可以动态改变)
    
    
    void func()
    {
        array<string, 5> myarray = {"aaaa", "bbbb", "cccc", "ddddd", "eeeee"};
        
        std::cout << "myarray.size()" << myarray.size() << std::endl;
        myarray[0] = "-----------";
        myarray[4] = "+++++++++++++++++";
        std::cout << "sizeof(string)" << sizeof(string) << std::endl;
        for(size_t i = 0; i < myarray.size(); ++i)
        {
            const char* p = myarray[i].c_str();
            std::cout << "-----------begin-----------" << std::endl;
            std::cout << "数组元素值：" << p << std::endl;
            printf("对象地址 = %p\n", &myarray[i]);
            printf("指向的字符串地址 = %p\n", p);
            std::cout << "-----------end-----------" << std::endl;
        }
        
        const char *p1 = "------------------";
        const char *p2 = "++++++++++++++++++";
        printf("指向字符串p1的地址 = %p\n", p1);
        printf("指向字符串p2的地址 = %p\n", p2);
    }
}

namespace _nmsp2
{
    // vector
    // 往尾部插入和删除元素都很快
    // 但是往中间和开头插入删除元素，可能会导致很多后续元素重新析构 + 构造，效率很低
    // 查找数据速度不会太快
    
    // vector 容器他的内存是挨着的，vector有一个空间的概念，每一个空间可以装一个元素
    
    class A
    {
        public:
            int m_i;
            A(int tmp):m_i(tmp)
            {
                std::cout << "A::A()" << std::endl;
            }
            A(const A& tempA):m_i(tempA.m_i)
            {
                std::cout << "A::A(const &A tempA)" << std::endl;
            }
            ~A()
            {
                std::cout << "A::~A()" << std::endl;
            }
    };
    
    void func()
    {
        vector<A> myvecA;
        for(int i; i<5; ++i)
        {
            std::cout << "-----------begin-----------" << std::endl;
            myvecA.push_back(A(i));
            std::cout << "------------end------------" << std::endl;
        }
    }
}

int main()
{
    // _nmsp1::func();
    _nmsp2::func();

    return 0;
}
