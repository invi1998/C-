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

using namespace std;

void func1(int v)
{
    std::cout << "void func1(int v); v = " << v << std::endl;
}

class TC
{
    public:
        void operator()(int tv)
        {
            std::cout << "void operator()(int tv)， TV = " << tv << std::endl;
        }
        void putong(int tv)
        {
            std::cout << "void putong(int tv)； tv = " << tv << std::endl;
        }
        int m_i = 100;
        
    public:
        static int stcFunc(int tv)
        {
            std::cout << "static int stcFunc(int tv)" << tv << std::endl;
            return tv;
        }
};

class TC2
{
    public:
        using fpoint = void(*)(int); // 定义一个函数指针fpoint
        static void mysfunc(int tv)
        {
            std::cout << "static void mysfunc(int tv), tv = " << tv << std::endl;
        }
        operator fpoint()
        {
            return mysfunc;
        }
};

class CB
{
    std::function<void()>funcCallback;
    
    public:
        CB(const std::function<void()> &fv):funcCallback(fv)
        {
            std::cout << "CB(const std::funcCallback<void()> &fv):funcCallback(fv)" << std::endl;
        }
        
        void runCallback(void)
        {
            funcCallback();   
        }
};

class CT
{
    public:
        void operator()(void)
        {
            std::cout << "void operator()(void)" << std::endl;
        }
};

void myCallBack(int cs, const std::function<void(int)> &f)
{
    f(cs);
}

void run(int x)
{
    std::cout << x << std::endl;
}

void myfuncBind(int x, int y, int z)
{
    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
    std::cout << "z = " << z << std::endl;
}

void func4(int &a, int &b)
{
    a++;
    b++;
}

class CTC
{
    public:
        CTC()
        {
            std::cout << "CTC()" << std::endl;
        }
        
        ~CTC()
        {
            std::cout << "~CTC()" << std::endl;
        }
        
        CTC(const CTC&temv):m_i(temv.m_i)
        {
            std::cout << "CTC(const CTC&temv):m_i(temv.m_i)" << std::endl;
        }
        
        void operator()()
        {
            std::cout << "void operator()()" << this << std::endl;
        }
        
        void func(int v1, int v2) // 普通成员函数
        {
            std::cout << "v1 = " << v1 << ", v2 = " << v2 << std::endl;
            m_i = v1;
        }
        int m_i = 0;
};

int main()
{
    // 可调用对象
    // 函数，重载()运算符类的对象
    // 1）函数指针
    void(*pmf1)(int) = func1; // 定义一个函数指针pmf1，并给初值func1 
    // void(*pmf1)(int) = &func1; // 这两种写法一样,加不加&(地址符)效果一样
    pmf1(535);  // 调用函数，这就是一个可调用对象
    // void func1(int v); v = 535
    // 2）具有operator()成员函数的类对象（仿函数）
    // 仿函数 （functor） 行为类似于一个函数的行为（something that performs a function）
    // c++11中，仿函数是通过在类中重载()运算符实现，所以仿函数有称为函数对象（function object）
    // 能行使函数功能的类
    
    TC tc1;
    tc1(788); // 调用（）操作符，这也是一个可调用对象
    // void operator()(int tv)， TV = 788
    // 等价于 
    tc1.operator()(788);
    // void operator()(int tv)， TV = 788
    
    //  3)可被转换为函数指针的类对象（仿函数/函数对象）
    TC2 tc2;
    tc2(588);   // 先调用fpoint,再调用mysfunc()，这个也是一个可调用对象
    // static void mysfunc(int tv), tv = 588
    // 等价于
    tc2.operator TC2::fpoint()(346);
    // static void mysfunc(int tv), tv = 346
    
    // 4）类成员函数指针
    TC tc3;
    void(TC::*myfuncPointTC)(int) = &TC::putong;
    // 定义一个 类成员函数指针变量 myfuncPointTC 并给初始值
    (tc3.*myfuncPointTC)(8888);
    // void putong(int tv)； tv = 8888
    
    // 总结：
    // 1）看成对象
    // 2）我们可以对其使用()调用运算符，如果a是可调用对象，那么我们就可以编写a(parm...)代码
    // 
    
    // 如何把各种不同形式的可调用对象的调用形式统一一下？方便调用
    // std::function(可调用对象包装器) c++11
    // std::function是一个类模板，用来装各种可调用对象
    // 不能装类成员函数指针
    // std::function类模板的特点，就是能够通过给他指定模板参数，他就能够用统一的方式来处理函数
    // 1）绑定普通函数
    std::function<void(int)> f1 = func1;
    f1(222);
    // void func1(int v); v = 222
    // 2) 绑定类的静态成员函数
    std::function<int(int)> f2 = TC::stcFunc;
    std::cout << f2(1123) << std::endl;
    // static int stcFunc(int tv)1123
    // 1123
    
    // 3）绑定仿函数
    TC tc4;
    std::function<void(int)> f3 = tc4;
    f3(999);
    // void operator()(int tv)， TV = 999
    
    TC2 tc5;
    std::function<void(int)> f4 = tc5;
    f4(10101010);
    // static void mysfunc(int tv), tv = 10101010
    
    // 范例演示
    CT ct;  // 可调用对象
    CB cb(ct);  // cb需要可调用对象来做参数进行构造，ct因为有operator()，所以可以转为可调用对象
    cb.runCallback();
    // CB(const std::funcCallback<void()> &fv):funcCallback(fv)
    // void operator()(void)
    
    for(auto i = 0; i < 3; ++i)
    {
        myCallBack(i, run); 
        // 0
        // 1
        // 2
    }
    
    
    // std::bind
    // std::bind绑定器，他也是一个类模板， c++11引入的
    // 他能将对象及其相关的参数绑定到一起，绑定完之后可以进行直接调用，也可以用std::function进行保存，在需要的时候进行调用
    // 格式
    // std::bind(待绑定的函数对象/函数指针/成员函数指针, 参数绑定值...)
    // 1）将可调用对象和参数绑定到一起，构成一个仿函数，所以可以直接调用
    // 2）如果函数有多个参数，可以只绑定一部分参数，其他参数在调用的时候载补充完整（指定）
    auto bf1 = std::bind(myfuncBind, 12, 23, 34);
    // auto表示不关心bind返回类型，其实这里bind返回的是一个仿函数对象，可以直接调用，也可以赋给std::function
    bf1(); // 因为我们在上文中已经把所有形参进行了绑定，这里就可以不用传任何参数直接调用
    
    auto bf2 = std::bind(myfuncBind, placeholders::_1, placeholders::_2, 34);
    bf2(99, 88);
    // placeholders 占位符
    // 表示绑定函数myfuncBind的第三个参数绑定为34， 但是第一个和第二参数由调用 bf()的时候的第一个和第二参数来指定
    // _1, _2这些是标准库里定义的，占位符的含义
    // 这里placeholders::_1 表示这个位置的参数（当前placeholders::_1所在位置）将由函数调用时传入的第一个参数代替
    // placeholders::_2同理
    std::bind(myfuncBind, placeholders::_1, placeholders::_2, 34)(2222,33333); // 直接bind调用
    std::bind(myfuncBind, placeholders::_2, placeholders::_1, 34)(2222,33333); // 调用顺序 33333， 2222， 34
    
    
    int q = 100;
    int p = 200;
    
    auto bf4 = std::bind(func4, q, placeholders::_1);
    bf4(p);
    std::cout << "q = " << q << " p = " << p << std::endl;
    // q = 100 p = 201
    // 这说明bind对于预先绑定的函数参数是通过值传递的，所以这个q（值传递）的值不会改变
    // bind对于不事先绑定的参数，通过placeholders::n来进行绑定的参数，是通过运用传递的，所以这里p是引用传递
    
    
    // bind绑定成员函数
    CTC ctc;
    auto bf5 = std::bind(&CTC::func, ctc, placeholders::_1, placeholders::_2);
    // 成员函数是有绝对地址的，跟着类走，跟成员对象无关，但是如果你要调用这个成员函数，是需要使用类对象进行调用的
    bf5(444, 555);
    std::cout << ctc.m_i << std::endl; // 0
    // v1 = 444, v2 = 555
    // 上文中的参数 ctc ， 会导致调用CTC的拷贝构造函数来生成一个CTC类的临时对象，作为std::bind的返回值（bind返回的是仿函数类型对象）
    // 后续的func调用，修改的是临时的CTC对象的成员变量m_i值,并不影响真实的ctc对象的m_i的值，所以这就是为啥上文中 ctc.m_i == 0
    
    std::bind(&CTC::func, &ctc, placeholders::_1, placeholders::_2)(444, 555);
    std::cout << ctc.m_i << std::endl; // 444
    // 当把ctc改为引用，那么就会真实改变到对象的成员变量值
    
    // 结论：bind绑定成员函数，第二个参数如果前面添加 &，那么就不会生成临时对象，后续成员函数func修改的也是实际ctc对象的成员值
    // 这说明此时bind返回的这个对象其实是ctc对象本身（仿函数类型对象）
    
    // bind和function 配合使用
    std::function<void(int, int)> bfc = std::bind(&CTC::func, ctc, placeholders::_1, placeholders::_2);
    bfc(10, 20);
    
    // 把成员变量地址当函数一样进行绑定，绑定的结果放在std::function<int &(void)>里面进行保存、
    // 说白了就是使用可调用对象的方式保存一个变量
    std::function<int&()> bf7 = std::bind(&CTC::m_i, &ctc);
    // 如果bind第二个参数不是引用，那么这个bind会导致系统会调用两次拷贝构造函数
    // 第一次执行拷贝构造函数是系统要利用ctc产生临时CTC对象
    // 第二次拷贝构造函数执行是因为std::bind要返回一个CTC对象（仿函数，拷贝自临时CTC对象）
    // 执行完std::bind之后，临时的CTC对象会被释放，返回测这个临时CTC对象（仿函数）会被bf7接着
    bf7() = 900;
    std::cout << ctc.m_i << std::endl; // 900
    
    
    auto rt = std::bind(CTC());
    // CTC()构造临时对象，然后又调用拷贝构造函数生成了一个可调用对象，作为std::bind的返回内容，
    rt(); // 调用的是operator()
    
    auto bf8 = std::bind(run, std::placeholders::_1);
    for(int i = 0; i < 4; i++)
    {
        myCallBack(i, bf8);
        // 0
        // 1
        // 2
        // 3
    }
    
    // 总结：
    // 1）bind思想，所谓的延迟调用，将可调用对象同意格式，保存起来，需要的时候再进行调用
    // 2）我们有用std::function绑定一个可调用对象，但是类成员不能绑，std::bind成员函数，成员变量都能够绑定
    
    
    return 0;
}
