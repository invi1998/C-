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
#include <algorithm>

using namespace std;

namespace _nmsp1
{
    
    // 算法（函数模板）概述
    // 查找，排序等
    // 这种函数模板是属于全局函数（或者全局的函数模板）
    // 算法的前两个参数的类型，一般都是迭代器类型，用来表示某个容器的元素的一个区间
    
    void myfunc(int i) // 这里这个i指代的是容器中的迭代出来的元素，所以需要和容器的元素类型保持一致
    {
        std::cout << i << std::endl;
    }
    
    bool mysort(int i, int j)
    {
        // return i < j; // 从小到大排序
        return i > j; // 从大到小排序
    }
    
    class S
    {
        public:
            bool operator()(int i, int j)
            {
                return i > j;
            }
    };
    
    void func()
    {
        // list<int>mylist = {1,23,44,2,33,43,2,12,0,231};
        // list<int>::iterator iterList_begin = mylist.begin();
        // list<int>::iterator iterList_end = mylist.end(); // 最后一个元素的下一个位置
        
        // 算法名（iterList_begin， iterList_end）传递进去的应该是一个前闭后开的区间
        // 前闭后开的区间的好处一般认为有两条
        // 1）算法只要判断迭代器只要等于后面这个开区间，那就表示迭代结束
        // 2）如果第一个形参等于第二个形参，也就是iterbegin == iterend；就表示是一个空区间
        
        // 我们可以理解算法是一种搭配迭代器使用的全局函数，算法和具体容器没有关联，只和迭代器有关联
        // 算法这种泛型编程方式，增加了灵活性，但是缺失了直观性；某些数据结构和算法之间的兼容性也不是特别好
        
        
        // 使用stl算法需要事先include《algorithm》头文件
        // for_each()
        // 内部的实现机制可能是：
        // template<class InputIterator, class Function>
        // Function for_each(InputIterator first, InputIterator last, Function t)
        // {
        //     for (; first != last; ++first)
        //     {
        //         f(*first);
        //     }
        //     return f;
        // }
        // 这段代码表明2个问题
        // 1）算法区分迭代器的种类，这个种类决定看某些算法的效率
        // 2)f(*first); 这是一个可调用对象
        
        // 当有些成员函数和全局函数（算法）同名同时存在时，优先考虑使用同名的成员函数，因为同名成员函数都是根据容器特化的
        // find() 查找返回的是一个迭代器
        // 使用find（）时需要先提供一个迭代器
        // 但是使用容器里的成员函数find就不需要
        
        
        std::vector<int> myvec = {12, 34, 2, 12, 1, 5, 341, 42} ;
        for_each(myvec.begin(), myvec.end(), myfunc); // myfunc 是一个可调用对象
        std::vector<int>::iterator finditer = find(myvec.begin(), myvec.end(), 321);
        if (finditer!=myvec.end())
        {
            std::cout << "找到元素321" << std::endl;
        }
        else
        {
            std::cout << "没有找到元素321" << std::endl;
        }
        
        // 比如map
        map<int, string> mymap;
        mymap.insert(std::pair<int, string>(123, "tttt"));
        mymap.insert(std::pair<int, string>(13, "ttdfgdft"));
        mymap.insert(std::pair<int, string>(1, "tsdfsttt"));
        auto ite = mymap.find(13);
        if (ite != mymap.end())
        {
            std::cout << ite->second << std::endl;
        }
        
        // find_if 需要第三个参数（可调用对象），根据可调用对象的计算结果，返回true或者false
        auto result = find_if(myvec.begin(), myvec.end(), [](int val){
            // lambda表达式也是可调用对象
            if (val > 15)
            {
                return true;
            }
            return false;
        });
        
        if(result != myvec.end())
        {
            std::cout << "找到元素大于15的，值为" << *result << std::endl;
        }
        else
        {
            std::cout << "未找到匹配项" << std::endl;
        }
        
        // sort()
        // sort(myvec.begin(), myvec.end()); // 缺省（按照从小到大的顺序排列）
        sort(myvec.begin(), myvec.begin() + 3);
        for(auto iter = myvec.begin(); iter != myvec.end(); ++iter)
        {
            std::cout << *iter << std::endl;
        }
        // 自定义比较函数，返回bool值
        // sort(myvec.begin(), myvec.end(), mysort);
        S sortS;
        sort(myvec.begin(), myvec.end(), sortS);
        for(auto iter = myvec.begin(); iter != myvec.end(); ++iter)
        {
            std::cout << *iter << std::endl;
        }
        
    }
}

int main()
{
    _nmsp1::func();

    return 0;
}

