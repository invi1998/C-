/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

template<typename T> // 改造为函数模板
// void func(const int &abc)  // abc这个变量时一个什么类型？ const int &
void func(const T &abc)
{
    
}


// void myfunc(int &&tmprv) // 参数tmprv 是一个右值引用类型
template<typename T>
void myfunc(T &&tmprv) // 注意，这里的&&是属于形参tmprv的一部分。&&和T类型没有关系
// void myfunc(const T &&tmprv) // const 剥夺万能引用资格
// 修改为函数模板之后
// 1）看到tmprv既能接收左值，又能接收右值
// 2）tmprv的类型是 T&&
// 从编译不报错，T被推断出来的，不仅仅是int型（因为如果是int ,就会导致 int && tmprv 不能接收左值）
{
    // T&&或者说tmprv才是万能引用，而单独的T不是万能引用
    // 如果传递进来的是整形左值，tmprv就该推断为int &类型；
    // 如果传递进来的是整形右值，tmprv就该推断为int&&类型；
    
    cout << tmprv << endl;
    return;
}
template<typename T> void funcVec(std::vector<T>&& t)
{
    
}

template<typename T>
class Mytest
{
    public:
        void testFunc(T&& x)
        // 这个x不是万能引用，只是一个右值引用
        // 因为这个testFunc是一个成员函数，成员函数并不涉及到模板类型推断
        // 我们在调用这个类模板的时候，需要先对这个类模板进行实例化，在这个时候，这个类就已经被实例化出来了
        // 那么他的成员函数就已经确定了参数类型了
        {
            std::cout << x << std::endl;
        }
        
        template<typename T2>
        void testFunc2(T2 && x)
        // 是万能引用，因为他需要在实际调用的时候才能进行类型推断
        {
            std::cout << x << std::endl;
        }
        
};

int main()
{
    // printf("Hello World");
    // 类型区别基本概念

    // func(10);
    // T 是什么类型？abc是什么类型？
    // T的类型和abc的类型往往不相同
    // T 的类型 int, abc 的类型 const int &
    // 调用函数模板时给的参数10会对T的类型产生影响，abc的类型会对T的类型产生影响
    
    // universal reference / 万能引用 / 未定义引用
    // &&表示右值引用。
    // int &&rv = 100;
    
    myfunc(100);
    int i = 100;
    // myfunc(i); // cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’
    // 错误，右值引用不能用来接收左值（引用）
    myfunc(i); // myfunc()为函数模板时，就可以正常调用
    
    
    // 万能引用语境
    // 1）必须是函数模板
    // 2）必须发生了模板类型推断，并且函数模板形参的样子是：T&&， auto也存在万能引用的概念
    // T&& 就是万能引用
    
    // 万能引用和右值引用解释起来的区别
    // 1）右值引用得传递右值，否者编译器报错
    // 2）万能引用作为函数形参时，可以传递左值，也可以传递右值
    // 如果我们传递的是左值，那么这个万能引用就变成了左值引用
    // 如果我们传递的是右值，那么这个万能引用就变成了右值引用
    
    // 1）void func(int &&t); 右值引用
    // 2）template<typename T> void func(T&& t); 万能引用
    // 3）template<typename T> void func(std::vector<T>&& t); 右值引用(因为 T 和 && 没有紧挨着)
    
    vector<int> av = {12,12,1234,4};
    // funcVec(av); // 无法将左值转为右值
    funcVec(std::move(av));
    
    // 万能引用资格剥夺与辨认
    // 剥夺
    // const 修饰符会剥夺一个引用成为万能引用的资格，会被打回原形，变成普通的右值引用
    // 所以，函数模板类型参数T前面不要加任何修饰词
    // 辨认
    
    Mytest<int> mt;
    int i0 = 100;
    // mt.testFunc(i0); // cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’
    mt.testFunc2(i0);
    mt.testFunc(std::move(i0));

    return 0;
}
