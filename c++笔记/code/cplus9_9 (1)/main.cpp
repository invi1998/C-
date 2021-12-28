/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <initializer_list>
#include "stdarg.h"
#include "stdlib.h"

using namespace std;

void printValue(initializer_list<string> args, int vv)
{
    for(auto iter = args.begin(); iter!= args.end(); ++iter)
    {
        std::cout << (*iter).c_str() << std::endl;
        // 返回一个迭代器，（*iter）指向该元素，使用.c_str()将string的值打印出来
    }
    for(auto &iv:args)
    {
        std::cout << iv.c_str() << std::endl;
    }
    std::cout << args.size() << std::endl;
}

class CT
{
    public:
        explicit CT(const initializer_list<int> &tmpvalue)
        {
            
        }
};

// 带...的可变参数函数，需用用到几个宏
double averge(int num, ...) // num里面传递的一般是...里的可变参数的数量
{
    // 类型定义（宏）
    va_list valist; // 创建一个va_list类型的变量
    double sum = 0;
    va_start(valist, num); // 使得valist指向起始的参数
    for(int i = 0; i < num; ++i)
    {
        // 遍历参数
        sum = sum + va_arg(valist, int); // 参数二说明返回类型为int
    }
    va_end(valist); // 释放va_list
    return sum;
}

void funcPrint(const char* msg, ...)
{
    va_list valist; // 创建一个va_list类型的变量
    
    int csg = atoi(msg); // 这个就是拿到可变参数数量，这个数量后续用于遍历和获取可变参数
    
    va_start(valist, msg);
    
    int paramscount = 0;
    while(paramscount < csg)
    {
        char *p; // 这里我们假定给的可变参数都是字符串，（如果是要实现printf，那么就要根据%d,%s等来解析字符串获取可变参数类型）
        p = va_arg(valist, char*);
        printf("第%d个参数是：%s\n", paramscount, p);
        paramscount++;
    }
    va_end(valist);
    
    
}

int main()
{
    //   可变参数函数
    // 能够接收非固定参数个数的函数
    // initializer_list标准库类型，该类型能够使用的先决条件是，所有的实参类型相同
    
    // initializer_list（初始化列表）
    // 如果一个函数，他的实参数量不可预知，但是所有实参的类型相同，那么我么就可以使用这个initializer_list类型来接
    // initializer_list是C++11中提供的新类型，他也是一个类模板
    // 我么可以把initializer_list理解为某种类型值得数组，这个类模板里指定的类型就是这个数组里保存的数据的类型
    // 
    
    initializer_list<int> myarray; // 空列表
    initializer_list<int> myarray2 = {12, 23,64,6,32}; // 也可以初始化时进行值初始化
    // 注意，initializer_list里的元素永远都是常量值。不能被改变
    
    // begin(),end()遍历，size()获取元素个数
    printValue({"qqqq", "dddddddd", "dssssssse", "sdwqqq"}, 23);
    // 若要往initializer_list形参里传递值得一个序列，就必须把这个序列放到花括号里{}作为一个整体
    // c++11将使用花括号{}的初始化方式作为一种比较通用的值初始化方式，可用于很多的类型初始化
    // 含有initializer_list形参的函数也可以包含其他形参
    
    // 范围for语句工作原理
    for(auto &templave : myarray2)
    {
        std::cout << templave << std::endl;
    }
    std::cout << myarray2.size() << std::endl;
    
    // 拷贝和赋值
    // 拷贝或者赋值一个initializer_list对象，不会拷贝列表中的元素，原来的对象和拷贝或者赋值出来的独享共享表中的元素
    initializer_list<string> str1 = {"qq","jjj","ggg","lll"};
    initializer_list<string> str2(str1);
    initializer_list<string> st3;
    st3 = str2;
    
    
    // 初始化列表做构造函数参数
    // CT ct1 = {10, 2, 32, 24}; // 隐式类型转换，将{。。。}转换为initializer_list类型
    // 如果要停止使用隐式类型转换，在构造函数前面加explicit
    // 初始化的时候就该这么写
    CT ct1{10, 2, 32, 24};
    // 或者
    // CT ct1 = CT({10, 2, 32, 24});
    
    
    // 省略号形参(...)    需要引入"stdarg.h"
    // 省略号形参一般无法正确处理类类型形参，所以在C++应该尽量少用（能正确处理的 int, char , *）
    // 他也是属于可变参数函数的，对人参数数量不固定， 但是函数的所有参数是存储在线性连续的栈空间里的
    // 而且这种带...的可变参数函数必须只至少有一个普通参数。这样我们就能通过这个普通参数来进行寻址后续的所有可变参数的类型及其值
    
    std::cout << "求和：" << averge(4, 123, 232, 42, 1231) << std::endl;
    
    // printf也是可以使用...可变参数
    printf("value = %d, value2 = %d\n", 123, 232);
    // printf的实现是
    // int printf(char const *const _Format, ...){};
    
    funcPrint("3", "aa", "dsf", "sdfsa");
    // 第0个参数是：aa
    // 第1个参数是：dsf
    // 第2个参数是：sdfsa
    
    // 注意
    // 1）至少要有一个有效的形参，形参不能完全是...
    // 2）...只能出现在形参列表的最后一位
    // 3）...之前的逗号可以省略
    // 4）如果有多个普通参数，那么va_start(valist, msg)第二个参数必须绑定...前面那个参数
    // 5）一般这些可变参数类型是数值型或者字符串型可以正常处理，其他类类型一般都不能正常处理，所以...能用的场合并不多
    
    return 0;
}



