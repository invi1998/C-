/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

std::vector<std::function<bool(int)>> gv;   // 全局变量，每个元素都是一个function，参数为int，返回类型bool

void func1()
{
    srand((unsigned)time(NULL));
    // int tmpvalue = rand() % 6; // 产生一个0-5之间的随机数
    
    // static int tmpvalue = rand() % 6; // 产生一个0-5之间的随机数
    static int tmpvalue = 4;
    
    // gv.push_back([&](int tv){
    gv.push_back(
        // [&](auto tv){ // 引用悬空
        // [=](auto tv){
        [](auto tv) { // 静态变量是不需要捕获的
        
            std::cout << tmpvalue << std::endl;
            
            if(tv%tmpvalue == 0) // 如果tv是tmpvalue的倍数
            {
                return true;
            }
            return false;
        }
    );
    tmpvalue++;
}

class AT
{
    public:
        int m_tempvalue =  7;
        // void addItem()
        // {
        //     gv.push_back(
        //         //  = 按值捕获
        //         // 我们会认为这个[=]是按值捕获，使得我们可以访问这个成员变量m_tmpvalue，
        //         // 所以我们就会顺理成章的认为这个lambda表达式所使用的m_tempvalue是按值捕获得到的
        //         // 但是实际上成员变量是不存在捕获的（lambda无法捕获成员变量），
        //         // 这里使用[=]捕获的是this指针值，
        //         // 所以这里lambda表达式实际上是这样子的
        //         [this](auto tv){
        //         // [=](auto tv){ // 有 = 就相当于有this
                    
        //             std::cout << m_tempvalue << std::endl; // this->m_tempvalue
                    
        //             if(tv % m_tempvalue == 0) // 如果tv是tmpvalue的倍数
        //             {
        //                 return true;
        //             }
        //             return false;
        //         }
        //     );
        // }
        
        
        // void addItem()
        // {
        //     auto tmpvalueCopy = m_tempvalue;
        //     gv.push_back(
        //         //  = 按值捕获
        //         // 我们会认为这个[=]是按值捕获，使得我们可以访问这个成员变量m_tmpvalue，
        //         // 所以我们就会顺理成章的认为这个lambda表达式所使用的m_tempvalue是按值捕获得到的
        //         // 但是实际上成员变量是不存在捕获的（lambda无法捕获成员变量），
        //         // 这里使用[=]捕获的是this指针值，
        //         // 所以这里lambda表达式实际上是这样子的
        //         [tmpvalueCopy](auto tv){
        //         // [=](auto tv){ // 有 = 就相当于有this
                    
        //             std::cout << tmpvalueCopy << std::endl; // this->m_tempvalue
                    
        //             if(tv % tmpvalueCopy == 0) // 如果tv是tmpvalue的倍数
        //             {
        //                 return true;
        //             }
        //             return false;
        //         }
        //     );
        // }
        
        
        // 广义lambda捕获
        void addItem()
        {
            gv.push_back(
                [abc = m_tempvalue](auto tv){ // 将m_tempvalue复制到闭包里来，lambda里使用的都是副本
                    std::cout << abc << std::endl; // this->m_tempvalue
                    
                    if(tv % abc == 0) // 如果tv是tmpvalue的倍数
                    {
                        return true;
                    }
                    return false;
                }
            );
        }
};

int main()
{
    // 捕获列表中的&
    // 捕获外部作用域中的所有变量，并作为引用在lambda表达式中使用
    // 按照引用方式捕获，会导致lambda表达式包含绑定到局部变量的引用
    // 引用捕获超出作用范围的情形也叫做 “引用悬空”
    // func1(); // 调用结束，tempvalue内存被回收
    // std::cout << gv[0](80) << std::endl; // 非法调用，会出现不可预料的后果
    // 因为此时此刻，你在进行调用的时候（调用容器里的lambda），会发现，lambda表达式里的tmpvalue值已经不在作用范围类了（tempvalue是定义在函数func1的作用域里的），
    // 一旦调用func1函数结束，那么内部的局部变量tempvalue就会被回收内存，
    // 此时此刻在调用lambda表达式就会导致调用了一个作用域范围外的变量，这就是使用&的一个坑的例子
    
    
    // 形参列表可以使用auto
    // c++14允许在lambda表达式中使用auto
    
    // 成员变量的捕获问题
    // AT *at = new AT();
    // at->addItem();
    // delete at;
    // std::cout << gv[0](10) << std::endl;    // 7 0
    // 结论：这里lambda表达式运算正确与否，取决去at对象是否存在，只有at对象存在，这个lambda表达式才执行正确
    // delete at;
    
    // 要明确一个概念，lambda的捕获的概念，只针对于在创建lambda表达式的作用域内可见的 非静态局部变量（包括形参）
    // 这里的m_tempvalue并不是非静态局部变量，它是AT类的成员变量，成员变量是不能被捕获到的
    // this: this指针用来指向对象本身，所以这里使用[=]捕获的是this指针值，
    
    // 那么我们如何改造，即使这个对象被销毁，lambda也能正常执行呢？（lambda表达式不依赖对象本身）
    // 解决办法就是将成员变量赋值给lambda内部的一个局部变量
    
    
    // 广义lambda捕获（generalized lambda capture）
    // 它也能解决lambda表达式执行依赖对象的问题
    // c++14
     
    
    // 静态局部变量（捕获这种事，事不包括静态局部变量的，也就是说，静态局部变量是不能被捕获的）
    // 虽然不能被捕获，但是是可以直接在lambda中直接使用的，静态局部变量是存储在静态存储区，他的有效期是一直到程序运行结束
    // 这种对static变量的使用，有点类似于按引用捕获这种效果
    func1();
    gv[0](10);  // 5
    func1();
    gv[0](10);  // 6
    gv[1](10);  // 6

    return 0;
}

