/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

class CT
{
    public:
        int m_i = 100;
        void mfunc(int x, int v)
        {
            // auto lam = [=, &x, v](){ // 报错，开头按值捕获，后续的显示捕获方式都得设置为&引用捕获
            // auto lam = [=, &x](){
            // 同样的，如果开头是默认引用捕获，后续的显示捕获方式都得写为按值捕获
            // auto lam = [&, x](){
            auto lam = [this, &x, v](){
                // 无论是使用this还是&还是=捕获，都可以达到访问类成员的目的
                std::cout << "x = " << x << std::endl;
                std::cout << "v = " << v << std::endl;
                x++;
                return m_i; // 因为有this，所以这个访问是合法的
            };
            
            std::cout << lam() << std::endl;
            std::cout << "x = " << x << std::endl;
            std::cout << "v = " << v << std::endl;
        }
};

void funccc(int i)
{
    std::cout << i << std::endl;
}


int main()
{
    // lambda表达式 用法简介
    // lambda是c++11引入的一个很重要的特性，他也是一种可调用对象
    // lambda表达式他定义了一个匿名函数，并且可以捕获一定范围内的变量
    // 格式 
    // [中括号 捕获列表](形参)->返回类型后置{函数体}; 分号结尾
    
    auto f = [](int a)->int {
        return a + 1;
    };
    std::cout << f(100) << std::endl;   // 101
    // 特点
    // 1）是一个匿名函数，也可以理解为可调用的代码单元，或者理解为“未命名的内联函数”
    // 2）他也有一个返回类型，也有一个参数列表
    // 3）与函数不同的是，lambda可以在函数内部定义，这个是常规函数做不到的（函数内部套函数在C++中是不被允许的）
    
    // a)
    // 这是一个返回类型后置语法，（lambda表达式的返回类型后置是必须的）
    // 因为很多时候，lambda表达式返回类型特别明显，所以允许lambda表达式返回类型可以省略，编译器可以自动推导
    // lambda表达式的形参可以有默认值 [](int a = 100)->int {}
    // 注意：编译器并不是总能推断出返回类型，这时候编译会报错，这时候需要手动写明后置返回类型
    
    // b)
    // 没有参数的时候，参数列表也可以省略，甚至()空括号也可以省略，所以如下格式都合法
    auto f1 = [](){return 1;};
    auto f2 = []{return 2;};
    // c)
    // 捕获列表[]和函数体{}不能省略，必须时刻包含
    // d)
    // lambda表达式调用方法和普通函数相同，都是使用()这种函数调用运算符进行调用
    // e)
    // lambda表达式可以不返回任何类型，不返回任何类型的时候就是void
    auto f3 = []{};
    // f）
    // 函数体末尾的分号不能省略
    
    
    // 捕获列表 ： [捕获列表]
    // lambda表达式是通过捕获列表来捕获一定范围内的变量，范围是什么意思？
    // a)
    // [] 空中括号表示不捕获任何变量（但是不包括静态局部变量，lambda表达式可以直接使用静态局部变量,所以局部静态变量时不需要捕获的）
    int i = 9;
    static int is = 11;
    auto f4 = [](){
        // return i; // 报错，无法捕获外部变量，不认识i在哪定义
        return is;
        // 所以看来，lambda表达式毕竟还是匿名函数，不能按照常规函数去进行理解
    };
    // b)
    // [&] 表示捕获外部作用域中所有变量，并作为引用在lambda函数体内进行使用
    auto f5 = [&]{
        i++;
        return i;
    };
    std::cout << f5() << std::endl; // 10
    std::cout << i << std::endl; //10
    // c)
    // [=] 表示捕获外部作用域中所有变量，并作为副本（按值传递）在lambda函数体内部使用（也就是指明，可以使用它的值，但是不允许对其进行赋值）
    auto f6 = [=]{
        return i + 100;
    };
    std::cout << f6() << std::endl; // 110
    std::cout << i << std::endl; //10
    // d)
    // [this] 一般用于类中，捕获当前类中this指针，让lambda表达式和当前类成员函数有同样等级的访问权限
    // 如果中括号中已经使用了 & 或者 = ,那么就默认使用了this。说白了，捕获this的目的就是为了在lambda表达式中使用当前类的成员函数和成员变量
    CT ct;
    ct.mfunc(1, 2); // 100
    // e)
    // [变量名] 如果是多个变量，彼此之间用逗号分隔，[变量名]表示按值捕获变量名代表的变量，同时不捕获其他变量
    // [&变量名] 按引用捕获变量，同时不捕获其他变量
    // f）
    // [=, &变量名] 按值捕获所有外部变量，但是按引用捕获 &变量名 所代表的变量，这里 = 必须写在开头
    // 捕获列表 开头的捕获表示默认捕获方式（也叫做隐式捕获方式），其他的都是显示捕获方式
    
    
    // lambda表达式延迟调用和易出错细节分析
    int x = 5;
    // auto f7 = [=] // 当遇到auto这行的时候，也就是lambda在捕获的时候，x的值就已经被复制到lambda表达式中
    auto f7 = [&]
    {
        return x;  
    };
    x = 10;
    std::cout << f7() << std::endl;
    // 按值捕获的时候，我们认为这里应该是返回10，但是实际上返回的是5
    // 首先，[=]表示按值捕获，编译器在遇到lambda表达式这行的时候（lambda表达式定义的时候），就已经完成了值捕获，后续外部变量怎么改变也不会影响到拉姆达表达式里按值捕获的变量值了
    
    
    // lambda表达式中的mutable （易变得）
    // 在使用mutable的时候，参数列表的圆括号不管有没有参数，都必须显示写出来，不可省略
    // mutable 的作用就是使得你按值捕获的变量在lambda表达式内部可以进行修改(但是，同样是改不到外部变量)
    int xx = 5;
    auto f8 = [=]() mutable
    {
        xx = 6;
        return xx;
    };
    
    std::cout << f8() << std::endl; // 6
    std::cout << xx << std::endl; // 5
    
    // lambda表达式的类型及其存储
    // C++11中lambda表达式的类型被称为 闭包类型（closure Type） 
    // 闭包（函数内的函数）可调用对象，本质上就是lambda表达式创建的运行时期的对象；
    // lambda表达式是一种比较特殊的，匿名的 类类型【闭包类】的对象，然后又生成了一个匿名的该类的类对象
    // 我们可以认为他是一个带有operator()的类类型对象（仿函数，函数对象）
    // 所以，我们也就可以使用std::function和std::bind来保存和调用lambda表达式
    // 每个lambda表达式都会触发编译器为我们生成一个独一的类类型
    
    // lambda表达式可以使我们就地定义匿名函数，（就地封装短小的功能闭包）
    
    auto ff1 = [](){}; // ff1就是一个未命名的类类型对象
    
    std::function<int(int)> fc1 = [](int c1){return c1;};
    std::cout << fc1(233) << std::endl;
    
    std::function<int(int)> fc2 = std::bind([](int c3){return c3;}, std::placeholders::_1);
    std::cout << fc2(xx) << std::endl;
    
    
    //  不捕获任何变量的lambda表达式，也就是捕获列表为空，可以转换为一个普通的函数指针
    
    using functype = int (*)(int); // 定义一个函数指针类型
    functype fp = [](int v){return v * 10;};
    std::cout << fp(23) << std::endl;
    
    
    //  语法糖
    int ar[5];
    ar[0] = 35;  //  便捷写法
    *(ar + 1) = 45; // 等价于 ar[1] = 45;
    
    std::cout << ar[1] << std::endl;    // 45
    
    // lambda再演示和优点总结
    // for_each
    // 是一个函数模板，需要包含头文件《algorithm》
    vector<int> vec = {232, 234, 44, 32, 545, 23};
    for_each(vec.begin(), vec.end(), funccc);
    
    int sum = 1;
    for_each(vec.begin(), vec.end(), [&sum](int v){
        sum += v;
        std::cout << sum << std::endl;
    });
    std::cout << sum << std::endl;
    
    // find_if
    // 也是一个函数模板，也需要包含头文件<algorithm>，用于查找，他的第三个参数也是函数对象
    auto ress = find_if(vec.begin(), vec.end(), [](int v){
        std::cout << v << std::endl;
        if (v > 50)
            return true;
       return  false;
    });
    //  注意：find_if返回的是一个迭代器，指向的是第一个满足条件的元素，如果这样的元素不存在，那么这个迭代器就会指向vec.end()
    if (ress != vec.end())
    {
        std::cout << "找到了" << *ress << std::endl;
    }
    else
    {
        std::cout << "没找到匹配的元素" << std::endl;
    }

    
    
    return 0;
}


