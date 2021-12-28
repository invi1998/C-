/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;


// 希望定义一个类型，键的类型不变（string），但是值得类型可变（自己指定）
template <typename ST>
class map_s
{
    public:
        typedef map<string, ST>typeM;
};

// C++11的新特新（第一一个类型别名的新方法）
template <typename T>
using s_map_T = map<string, T>;
// 这里s_map_T就是我们自己取得类型别名
// using用来给一个“类型模板”取名字（别名）用

template <typename T>
using myfunc_M = int(*)(T, T); // 定义一个叫myfunc_M的函数指针模板

int RegTest (int a, int b)
{
    return a * b;
}

template <typename T1, typename T2, typename T3>
T1 sum(T2 i, T3 j)
{
    T1 result = i + j;
    return result;
}

int main()
{
    // using定义模板别名
    // typedef ：用于定义类型别名
    typedef unsigned int unsigned_unit; // 定义一个无符号整形并取别名 unsigned_unit
    unsigned_unit abc;
    // map<string, int> ; // 我们想给这个数据类型起一个别名，应该如何书写？
    typedef map<string, int> map_s_i; // 使用typedef将这个了类型定义为map_s_i的类型别名
    map_s_i mymap;
    mymap.insert({"first", 1});
    mymap.insert({"seccond", 2});
    
    typedef map<string, string> map_s_s;
    map_s_s mysmap;
    mysmap.insert({"ttt", "tds"});
    
    
    map_s<int>::typeM int_type_map;
    
    int_type_map.insert({"gggg", 324});
    // 我们未来实现这种比较通用的《string, 任意类型》的map容器，不得不写一个类来实现（C++98的局限性）
    
    s_map_T<int> s_m_Int;
    s_m_Int.insert({"test", 32});
    
    cout << s_m_Int["test"] << endl;
    
    // using在用于定义类型（定义类型模板）的时候，是包含了typedef的所有功能的
    typedef unsigned int unnitTypedef;
    using unnitUsing = unsigned int;
    // 分别使用两种方法定义一个无符号整形别名
    
    typedef map<string, int> s_map_t_typedef;
    using s_map_t_using = map<string, int>;
    
    // 两种方法定义一个指针别名
    typedef int(*FuncTypedef)(int, int);
    using FuncUsing = int(*)(int, int);
    
    myfunc_M<int> point_func_int; //  myfunc_M<int>是一个函数指针类型，point_func_int是一个函数指针（变量）
    point_func_int = RegTest; // 把函数地址赋值给函数指针
    cout << point_func_int(3, 5) << endl;
    
    // using 中使用这种模板，既不是类模板也不是函数模板，我们可以看成是一种新的模板类型，类型模板（模板别名）
    
    // 显示指定模板参数
    
    auto result = sum<double, double, double>(2100000000, 2100000000);
    cout << result << endl;
}

