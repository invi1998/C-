/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

template<typename T, typename U>
struct TU
{
    TU()
    {
        cout << "泛化版本构造函数" << endl;
    }
    void functest()
    {
        cout << "泛化版本" << endl;
    }
};

// 当T和U这两个模板参数类型都是int型时， 我们希望给出一个特化版本
// 全特化，所有模板类型参数都有具体类型代表
// 特化版本代码编译器会优先选择（覆盖泛化版本）

template<> // 全特化
struct TU<int, int>
{
    TU()
    {
        cout << "int_int全特化构造函数" << endl;
    }
    void functest()
    {
        cout << "int_int全特化版本" << endl;
    }
};

template<> // 全特化(可以有任意多个合理的全特化版本)
struct TU<int, double>
{
    TU()
    {
        cout << "int_double全特化构造函数" << endl;
    }
    void functest()
    {
        cout << "int_double全特化版本" << endl;
    }
};

// 为string——string的特化版本特化了成员函数
template<>
void TU<string, string>::functest()
{
    cout << "string_string的functest成员函数" << endl;
}

// 带三个模板参数的
template <typename T, typename H, typename U>
struct THU
{
    void functhu()
    {
        cout << "THU泛化版本" << endl;
    }
};

// 从参数数量上进行偏特化
template <typename H>
struct THU<int, H, string>
{
    void functhu()
    {
        cout << "THU<int, H, string>偏特化版本" << endl;
    }
};

// 从模板参数范围上 
// int -> const int（整形缩小为常量整形）
// T -> T*(从任意类型缩小为任意类型的指针类型，范围缩小), 
// T -> T&,或者T->T&&；
template <typename T>
struct TR
{
    void func()
    {
        cout << "TR泛化版本" << endl;
    }
};

template <typename T>
struct TR<const T> // const T特化
{
    void func()
    {
        cout << "TR（const T特化）特化版本" << endl;
    }
};

template <typename T>
struct TR<T *> // T * 指针 特化
{
    void func()
    {
        cout << "TR（T* 指针）特化版本" << endl;
    }
};

template <typename T>
struct TR<T&> // T& 左值引用 特化
{
    void func()
    {
        cout << "TR（T& 左值引用 特化）特化版本" << endl;
    }
};

template <typename T>
struct TR<T&&> // T&& 右值值引用 特化
{
    void func()
    {
        cout << "TR（ T&& 右值值引用）特化版本" << endl;
    }
};


// 函数模板全特化
// 泛化版本
template<typename T, typename F>
void TFfunc(T i, F j)
{
    cout << "TFfunc泛化版本" << endl;
    cout << i << endl;
    cout << j << endl;
}

// template<typename T>
// void TFfunc<int, T>(int &i, T &j)
// {
//     cout << "函数模板不能偏特化" << endl;
//     cout << i << endl;
//     cout << j << endl;
// }

// 全特化
template<>
void TFfunc(int &i, double &j)
{
    cout << "TFfunc全特化版本" << endl;
    cout << i << endl;
    cout << j << endl;
}

// 重载版本
void TFfunc(int &i, double &j)
{
    cout << "TFfunc函数重载版本" << endl;
    cout << i << endl;
    cout << j << endl;
}

int main()
{
    // 一、类模板特化
    // 一_1、类模板全特化
    //      a)、常规全特化
    
    TU<char, int> tu_c_i;
    tu_c_i.functest(); // 泛化版本
    
    TU<int, int> tu_i_i;
    tu_i_i.functest(); // int_int全特化版本
    
    TU<int, double> tu_i_d;
    tu_i_d.functest(); // int_double全特化版本
    
    // 特化成员函数而不是模板
    TU<string, string> tu_s_s; // 执行的是泛化版本的构造函数
    tu_s_s.functest(); // 执行的是 string_string的functest成员函数
    
    THU<double, int, double> did_thu; // THU泛化版本
    did_thu.functhu();
    
    THU<int, int, string> iis_thu; // THU<int, H, string>偏特化版本
    iis_thu.functhu();
    
    TR<double>trd; // TR泛化版本
    trd.func();
    
    TR<double *>trd_P; // TR（T* 指针）特化版本
    trd_P.func();
    
    TR<double &>trd_Y; // TR（T& 左值引用 特化）特化版本
    trd_Y.func();
    
    TR<const double &>trd_YC; // TR（T& 左值引用 特化）特化版本
    trd_YC.func();          // 既满足const 也满足左值引用，调用左值引用特化版本
    
    TR<double &&>trd_YY; // TR（ T&& 右值值引用）特化版本
    trd_YY.func();
    
    TR<const double>trd_C; // TR（const T特化）特化版本
    trd_C.func();
    
    TR<const double *>trd_CP; // TR（T* 指针）特化版本 
                             // 也就是既满足const也满足指针时，调用指针的特化版本
    trd_CP.func();
    
    
    // 
    const char *p = "I love china";
    int i = 12;
    TFfunc(p, i);
    // TFfunc泛化版本
    // I love china
    // 12
    
    double b = 45.65f;
    TFfunc(i, b);
    // 全特化函数模板实际上等价于实例化一个函数模板，但是并不等价于一个函数重载
    // void TFfunc<int, double>(int &, double &){}; 全特化，等价于实例化一个函数模板
    // void TFfunc(int &i, double &j) // 函数重载应该长这样
    
    // 编译器会选择最合适的版本进行调用：普通版本函数优先，然后是特化版本，最后是泛化版本
    
    // 函数模板不能偏特化
    // 比如如下代码会报错 非法使用显示模板参数
    // template<typename T>
    // void TFfunc<int, T>(int &i, T &j)
    // {
    //     cout << "函数模板不能偏特化" << endl;
    //     cout << i << endl;
    //     cout << j << endl;
    // }
    
    // 模板特化版本，放置位置建议
    // 模板定义。实现都放在一个.h文件中
    // 模板的特化版本和模板泛化版本都应该放置在同一个.h文件中
    // .h文件中前面放泛化版本，后面放特化版本
}

