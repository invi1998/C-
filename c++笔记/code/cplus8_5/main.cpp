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
#include <set>

using namespace std;

namespace _nsmp1
{
    // 迭代器
    // 迭代器是一个“遍历stl容器全部或者部分元素”对象（行为类似指针）
    // 迭代器用来表现容器里的元素位置；
    // 迭代器紧密依赖容器，一般来说，是容器里边定义这迭代器，是由容器提供的
    
    void func()
    {
        vector<int>iv = {100, 200, 300};
        
        for(vector<int>::iterator iter = iv.begin(); iter!=iv.end(); iter++)
        {
            std::cout << *iter << std::endl;
        }
    }
}

namespace _nsmp2
{
    // 迭代器的分类
    // 分类的依据是迭代器的移动特性以及在这个迭代器上能够做的操作
    // 迭代器，行为指针，表示一个位置，我们一般分类是依据他的跳跃能力，每个分类是一个对应的struct定义
    
    // 1）输出型迭代器（output iterator）
    // struct output_iterator_tag;
    // 2）输入型迭代器(intput iterator)
    // struct input_iterator_tag;
    // 3)前向迭代器（forward iterator）
    // struct forward_iterator_tag;
    // 4)双向迭代器（bidirectional iterator）
    // struct bidirectional_iterator_tag;
    // 5)随机访问迭代器（random-access iterator）
    // struct random_access_iterator_tag;
    
    // 这些解构都是有继承关系的
    // random_access_iterator_tag --继承自--> bidirectional_iterator_tag ---继承自--> forward_iterator_tag ---继承自--> input_iterator_tag
    
    
    // 大多数容器里都有一个::iterator迭代器类型，并不是所有的容器都有迭代器
    // 比如：stack queue 这种容器stl就没有为其提供迭代器，因为他们都需要遵照后进先出的原则或者先进先出的规则进行数据的入和出，不需要你进行遍历也不允许你进行遍历
    
    // 1）输出型迭代器 output_iterator_tag
    // 能力：一步一步往前走，并且能够通过这个类型的迭代器改写容器中的数据
    // *iter = val  将val鞋子迭代器所指向的位置
    // ++iter       向前步进（step forward），返回新位置
    // iter++       向前步进（step forward）, 返回旧位置
    // TYPE(iter)   复制迭代器（copy构造函数）（其实和下面演示的写法，产生一个临时变量的写法一致）
    
    // 2）输入型迭代器 input_iterator_tag
    // 能力：一次一个以向前的方向读取元素，并按照这个顺序一个一个返回元素值
    // *iter            读取实际元素
    // iter->member     读取实际元素的成员（如果有的话）
    // ++iter           向前步进（step forward）返回新位置
    // iter++           向前步进（step forward）, 返回旧位置
    // iter1 == iter2   判断两个迭代器是否相等
    // iter1 != iter2   判断两个迭代器是否不等
    // TYPE(iter)       复制迭代器（copy构造函数）（其实和下面演示的写法，产生一个临时变量的写法一致）
    
    // 3）前向迭代器 forward_iterator_tag
    // 能力：因为继承自 输入型迭代器（input_iterator_tag），因此它能以向前步进的方式读取元素，并且在读取元素时能有额外保障
    // *iter            读取实际元素
    // iter->member     读取实际元素的成员（如果有的话）
    // ++iter           向前步进（step forward）返回新位置
    // iter++           向前步进（step forward）, 返回旧位置
    // iter1 == iter2   判断两个迭代器是否相等
    // iter1 != iter2   判断两个迭代器是否不等
    // TYPE(iter)       复制迭代器（copy构造函数）（其实和下面演示的写法，产生一个临时变量的写法一致）
    // TYPE()           创建迭代器（defaut 构造函数）
    // iter1 = iter2    迭代器赋值（assign）
    
    // 4)双向迭代器 bidirectional_iterator_tag
    // 能力：在前向迭代器的基础上，增加了往回（反向迭代）的功能，也就是迭代位置可以回退，新增如下操作
    // --iter           步退（返回新位置）
    // iter--           步退（返回旧位置）
    
    // 5）随机访问迭代器 random_access_iterator_tag
    // 能力：在双向迭代器的基础上，新增了随机访问的能力（也就是增减某个偏移量能够计算计算距离，并且支持关系运算）
    // 新增如下操作
    // iter[n]          访问索引位置为n的元素
    // iter+=n          前进N个元素（如果n是负数，则改为回退）
    // iter-=n          回退n个元素（如果n是负数，则改为前进）
    // iter+n           返回iter之后的第n个元素
    // n+iter           返回iter之后的第n个元素
    // iert-n           返回iter之前的第n个元素
    // iter1-iter2      返回iter1和iter2之间的距离
    // iter1<iter2      判断iter1是否在iter2之前
    // iter1>iter2      判断iter1是否在iter2之后
    // iter1<=iter2     判断iter1是否不再iter2之后
    // iter1>=iter2     判断iter1是否不再iter2之前
    
    
    void _display_category(random_access_iterator_tag mytag)
    {
        std::cout << "random_access_iterator_tag" << std::endl;
    }
    
    void _display_category(bidirectional_iterator_tag mytag)
    {
        std::cout << "bidirectional_iterator_tag" << std::endl;
    }
    
    void _display_category(forward_iterator_tag mytag)
    {
        std::cout << "forward_iterator_tag" << std::endl;
    }
    
    void _display_category(input_iterator_tag mytag)
    {
        std::cout << "input_iterator_tag" << std::endl;
    }
    
    void _display_category(output_iterator_tag mytag)
    {
        std::cout << "output_iterator_tag" << std::endl;
    }
    
    template <typename T>
    void display_category(T ite)
    {
        // T的类型编译器能推导出来
        std::cout << "--------------begin---------------" << std::endl;
        
        // 整个这个类型 iterator_traits<T>::iterator_category 叫 过滤器（萃取机），用来获取T迭代器类型的种类
        typename iterator_traits<T>::iterator_category cagy; // 相当于获取迭代器种类
        
        _display_category(cagy); //  编译器会挑选一个最合适的参数的重载_display_category
        std::cout << "typeid(ite).name() = " << typeid(ite).name() << std::endl;
        std::cout << "---------------end----------------" << std::endl;
    }
    
    void func()
    {
        display_category(array<int, 100>::iterator()); // 这种 类型() ： 代表会产生一个临时对象
        // 比如类A（）代表产生一个临时A类对象
        display_category(vector<int>::iterator());
        display_category(list<int>::iterator());
        display_category(map<int, int>::iterator());
        display_category(set<int>::iterator());
        // --------------begin---------------
        
        // random_access_iterator_tag
        
        // typeid(ite).name() = Pi
        
        // ---------------end----------------
        
        // --------------begin---------------
        
        // random_access_iterator_tag
        
        // typeid(ite).name() = N9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEE
        
        // ---------------end----------------
        
        // --------------begin---------------
        
        // bidirectional_iterator_tag
        
        // typeid(ite).name() = St14_List_iteratorIiE
        
        // ---------------end----------------
        
        // --------------begin---------------
        
        // bidirectional_iterator_tag
        
        // typeid(ite).name() = St17_Rb_tree_iteratorISt4pairIKiiEE
        
        // ---------------end----------------
        
        // --------------begin---------------
        
        // bidirectional_iterator_tag
        
        // typeid(ite).name() = St23_Rb_tree_const_iteratorIiE
        
        // ---------------end----------------
        
    }
}

int main()
{
    // printf("Hello World");
    // _nsmp1::func();
    _nsmp2::func();

    return 0;
}

