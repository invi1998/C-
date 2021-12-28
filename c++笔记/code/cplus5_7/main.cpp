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

using namespace std;

// unique_ptr<string> funcqu()
// {
//     // unique_ptr<string>pr(new string("返回局部对象"));
//     // return pr; // 从函数返回一个局部的unique_ptr对象
//                 // 返回这种局部对象，导致系统给我们生产一个临时的unique_ptr，调用unique——ptr的移动构造函数

//     // 当然，我们也可以这样写
//     return unique_ptr<string>(new string("返回局部对象"));
// }

void mydelete(string *pd)
{
    cout << "删除器执行" << endl;
    delete pd;
    pd = nullptr;
}

int main()
{
    // // 返回unique_ptr
    // unique_ptr<string>ps;
    // // 虽然unique_ptr不能拷贝，但是当这个unique_ptr将要被销毁，是可以拷贝的，最常见的就是从函数中返回
    // ps = funcqu();
    // // 可以用ps来接，则临时对象直接构造在ps里，如果不接，则临时对象会被释放，同时释放掉所指的对象的智能指针
    // cout << *ps << endl;
    
    // 指定删除器
    // 缺省使用delete删除对象指针
    // 格式：unique_ptr<指向对象类型，删除器>智能指针变量名；
    // 删除器，可调用对象，比如函数，类重载圆括号（）
    // unique_ptr删除器相对复杂，多了一步，先要在类型模板参数中传递进去类型名，然后，在参数中再给具体的删除器函数
    
    // 删除器的写法
    
    typedef void(*fp)(string *); // 定义一个函数指针类型，类型名为fp
    
    unique_ptr<string, fp>ps1(new string("rrrrrrrrrr"), mydelete);
    cout << sizeof(ps1) << endl; // 16
    
    // 使用using定义类型名
    using fp2 = void(*) (string*);
    unique_ptr<string, fp2>ps2(new string("rrrrrrrrrr"), mydelete);
    cout << sizeof(ps2) << endl; // 16
    
    typedef decltype(mydelete) *fp3; // 这里使用decltype来猜这个类型，这里多了一个*,因为
    // decltype的函数返回类型是void (string *)，加一个*表示这个是一个函数指针类型  即： void*(string*)
    unique_ptr<string, fp3>ps3(new string("rrrrrrrrrr"), mydelete);
    cout << sizeof(ps3) << endl; // 16
    
    // 也可以直接这样写
    unique_ptr<string, decltype(mydelete)*>ps4(new string("rrrrrrrrrr"), mydelete);
    cout << sizeof(ps4) << endl; // 16
    
    // 使用lambda表达式  
    // lambda表达式可以理解为带有operator（）类类型的对象
    // 所以，使用decltype推断出来它是个类类型，把lambda表达式看为类
    auto mydeleteLambda = [](string *pda) {
        delete pda;
        pda = nullptr;
        cout << "lambda表达式执行" << endl;
    };
    unique_ptr<string, decltype(mydeleteLambda)> ps5(new string("rrrrrrrrrr"), mydeleteLambda);
    cout << sizeof(ps5) << endl; // 8
    
    
    // 指定删除器额外说明
    // shared_ptr就算连个shared_ptr指定删除器不同，只要他们所指向的对象类型相同，那么这两个shared_ptr也属于同一类型
    // 但是 unique_ptr不一样，指定unique_ptr删除器，会影响unique_ptr的类型，所以从灵活性来讲，shared_ptr设计更灵活
    // 之前说shared_ptr的时候，删除器不同，只要指向类型一样，那么都可以放到同一个容器中
    // unique_ptr如果删除器不同，那么就等于整个unique_ptr类型不同，类型不同的智能指针是没法放到同一容器中
    
    
    // 尺寸问题
    // unique_ptr和裸指针大小引用
    string *p;
    cout << sizeof(p) << endl; // 8
    unique_ptr<string>ps(new string());
    cout << sizeof(ps) << endl; // 8
    // 如果你增加了自己的删除器，则整个大小可能会改变（比如使用lambda表达式则尺寸不会改变 ）
    
    // 增加字节对效率会有影响，所以自定义删除器需要慎用
    // shared_ptr，不管你是使用何种删除器，他的大小都是裸指针的两倍
    
    return 0;
}

