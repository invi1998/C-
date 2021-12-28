/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
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
#include <functional>

using namespace std;

namespace _nmsp1
{
    // 算法适配器（函数适配器）
    // 最典型的就是绑定器（bind）
    
    // 绑定器
    // 老版本里有bind1st, bind2nd;
    // C++11里，名字被改为bind
    
    class A
    {
        public:
        bool operator()(int val)
        {
            // return val > 40;
            return 40 < val;
        }
    };
    
    void func()
    {
        vector<int> myvec = {12, 34, 55, 34, 34, 23, 12, 67, 34, 56};
        // 统计某个值在容器里出现的次数
        int icout = count(myvec.begin(), myvec.end(), 34);
        std::cout << icout << std::endl;
        
        // cout_if
        A acout;
        int icouta = count_if(myvec.begin(), myvec.end(), acout);
        std::cout << icouta << std::endl;
        
        // bind(less<int>(), 40, placeholders::_1);
        // 表示less<int>的operator（）的第一个参数绑定为40，
        // 这样当调用less<int>这个可调用对象时，就用这里的placeholders::_1来表示，在调用这个函数时，会被传入的第一个参数所取代
        // placeholders::_1是一个占位符
        
        // auto bf = bind(less<int>(), 40, placeholders::_1);
        // bf(19); // 就会被编译为less<int>(40, 19)
        
        icouta = count_if(myvec.begin(), myvec.end(), bind(less<int>(), 40, placeholders::_1));
        std::cout << icouta << std::endl;
        
        // 1)bind() 函数适配器中的绑定器
        // 2）less<int>() 是一个函数对象（仿函数），在这里是一个临时对象
        // 3）count_if  是一个算法（函数模板）
        
        // 迭代器适配器
        // reverse_iterator  反向迭代器
        vector<int> iv = {100, 200, 300, 400};
        for (vector<int>::reverse_iterator iter = iv.rbegin(); iter != iv.rend(); ++iter)
        {
            std::cout << *iter << std::endl;
        }
    }
}

int main()
{
    // printf("Hello World");
    _nmsp1::func();

    return 0;
}
