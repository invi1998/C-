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

using namespace std;

class CT
{
    public:
        int i;
        int j;
};

int testf()
{
    return 10;
}

const int &&myfunctest(void)
{
    // return 0;
}

template<typename T>
class CCT
{
    public:
        // typename T::iterator iter; // 定义一个T类型的迭代器类型
        decltype(T().begin()) iter;      
        // 如果 T = const std::vector<int> 那么，T().begin()返回的就是 std::vector<int>::const_iterator类型
        // 如果 T = std::vector<int> 那么，T().begin()返回的就是 std::vector<int>::iterator类型
        // 然后这里T后面加一个括号，表示生成该类型的一个临时对象，后面就可以直接用点来进行成员函数调用，所以就直接调用了该对象的.begin()
        void getBegin(T &t)
        {
            iter = t.begin();
        }
};

class AO
{
    public:
        AO()
        {
            std::cout << "AO()" << std::endl;
        }
        ~AO()
        {
            std::cout << "~AO()" << std::endl;
        }
        
        int func() const
        {
            std::cout << "A::func()" << std::endl;
        }
        int m_i = 99;
};

// c++98 使用偏特化解决
// template<typename T>
// class CCT<const T>
// {
//     public:
//         typename T::const_iterator iter; // 定义一个T类型的常量迭代器类型
//         void getBegin(const T &t)
//         {
//             iter = t.begin();
//         }
// };

auto funcBack_add(int a, int b) -> decltype(a + b)
{
    return a + b;
}

int &tf(int & i)
{
    return i;
}

double tf(double & d)
{
    return d;
}

template<typename T>
auto funcback(T &tv) -> decltype(tf(tv))
// 这里还就必须嘚使用功能返回类型后置语法，因为如果你改为
// decltype(tf(tv)) funcback(T &tv) 
// 编译会报错，因为你这行代码就表明，在使用功能tv的时候，还没有进行定义，他是在函数形参里面才完成的定义
{
    return tf(tv);
}

template<typename T>
// T& mydouble(T & v)
// 我们此时希望通过auto来推断这个函数返回类型
// auto mydouble(T &v) // 在调用的时候，编译报错（见下文）
decltype(auto) mydouble(T &v)
// 这里把auto理解为要推导的类型，推导的过程我们使用decltype
{
    v *= 2;
    return v;
}

decltype(auto) tff1()
{
    int i = 1;
    return i;
}

decltype(auto) tff2()
{
    int i = 1;
    return (i); // 加了括号， 导致返回的是一个int & 类型
    // 这样的代码会导致一个内存泄漏的问题：
    // 因为你返回的是一个引用，你在局部变量里返回一个引用（就会导致执行玩这个函数的时候，这个引用就被系统回收）
}

int main()
{
    // decltype含义和举例
    // decltype用于推导类型，对于一个给定的变量名或者表达式，decltype能够告诉你该名字或者表达式的类型
    // auto a = 10; // 我们并不想用表达式的值类初始化这个变量
    // c++11中引入了decltype（说明符）；他的主要作用就是返回操作数的数据类型
    // decltype特点
    // 1）decltype的自动类型推断会发生在编译期(和auto一样)
    // 2）decltype不会真正计算表达式的值
    // a) decltype后的圆括号是一个变量
    
    // const int i = 0;
    // const int &iv = i;
    // auto j1 = i;    // j1 = int;  传值方式推断（引用属性和const属性都会被丢弃）
    // decltype(i) j2 = 15;    // j2 = const int。如果decltype中是一个变量，则变量的const属性会返回
    // decltype(iv) j3 = j2;   // j3 = const int &; 如果decltype中是一个变量,则变量的const属性以及引用属性都会返回，
    //                         // decltype很循规蹈矩，有啥就返回啥
                            
    decltype(CT::i) a;  //a = int
    CT tmpct;
    decltype(tmpct) tmpct2; // tmpct2 = CT
    decltype(tmpct2.i) mv = 34; // mv = int
    int x = 1, y = 2;
    auto &&z = x; // x是一个左值，auto = int &; z = int &;万能引用
    decltype(z) &&h = y; // z = int &; h = int & (这里用到了引用折叠规则)
    
    
    // b) decltype后的圆括号中是非变量（是表达式）
    // decltype会返回表达式的结果对应的类型
    decltype(9) kk = 5; // kk = int
    int i = 0;
    int *pi = &i;
    int &iy = i;
    decltype(iy + 1) j; // j = int (因为iy + 1得到的是一个整形表达式)
    decltype(pi) k; // k = int * (因为pi是一个变量)
    *pi = 4;
    decltype(*pi) k2 = i;   // k2 = int &
    // *pi是指针指向的对象，而且能给对象赋值，所以*pi是一个左值
    // *pi是一个表达式不是一个变量，又因为他有*号,
    // 如果表达式的结果能够作为赋值语句左边的值，那decltype返回的就是一个引用
    // 所以这种情况需要专门记：decltype后面是一个非变量的表达式，并且表达式能够作为等号左边的内容，那么他的返回就类似一个左值引用 &
    
    decltype(i) k3; // k3 = int; 因为i只是一个变量
    
    decltype((i)) iy2 = i; // 如果在变量名外额外扩了一层或者多层括号，那么编译器就会把这个变量视为一个表达式
    // 又因为变量（表达式）可作为等号左边的内容，所以当前范例：iy2 = int &；
    
    // 结论decltype((变量))的结果永远都是一个引用
    
    
    // c) decltype后是函数
    decltype(testf()) tempv = 14; // tempv = int 
    // tempv 的类型就是函数testf()的返回类型，（注意：这里不会去真实的调用函数）
    // 这里编译器没有去调用函数testf（）,只是使用函数的返回值类型作为tempv的类型
    decltype(testf) tempv2; // tempv2 = int (void), 这个有返回类型，有参数类型，代表一种可调用对象
    // 标准库 function用法，类模板
    function<decltype(testf)> ftemp = testf; // 声明了一个function函数类型，用来代表一个可调用对象
    // 他所代表的可调用对象是 int(void)
    std::cout << ftemp() << std::endl;
    
    decltype(myfunctest()) myy = 0; // myy = const int &&
    
    
    // decltype的主要用途
    // 1）应付可变类型，一般decltype主要应用在模板编程中
    // using consttype = std::vector<int>; // 定义类型别名
    using consttype = const std::vector<int>; // 如果定义的类型别名设置为const
    consttype myvec = {12, 34, 53, 23, 55};
    CCT<consttype> cct;
    cct.getBegin(myvec);
    
    // 常量容器
    // const std::vector<int> myvec = {12, 34, 53, 23, 55};
    // 需要在定义时初始化，且后面不能修改（数目，值）
    // 如果想遍历这种常量容器，是需要使用常量迭代器来进行遍历
    // std::vector<int>::const_iterator iter;
    // 所以上文中我们类模板里定义的迭代器（typename T::iterator iter; // 定义一个T类型的迭代器类型），
    // 就不能用于遍历这种常量容器，故编译器报错
    // C++98的时代，如果需要解决这种问题，就需要实现对于该类型的类模板偏特化，这里是属于参数范围的偏特化
    
    // 测试类型后面加圆括号生成临时对象
    AO().func();    // 生成一个临时AO对象，并调用func成员函数
    // AO()
    // A::func()
    // ~AO()
    
    // (const AO()).func(); // 和上面打印一样
    decltype(AO().func()) newao = 999;  // newao = int
    // 注意，这里decltype虽然传入AO类并且使用其临时对象的func()函数的返回值类型作为类型推断，但是实际上根本没有构造过该AO类，也没有进一步去调用器func函数
    
    
    
    // 2）通过变量表达式抽取变量类型
    vector<int> ac;
    ac.push_back(12);
    ac.push_back(23);
    vector<int>::size_type acsize = ac.size();
    std::cout << acsize << std::endl; // 2
    decltype(ac)::size_type acsize2 = acsize; // 抽取ac的类型，也就是vector<int>,
    // 所以这行等价于 vector<int>::size_type acsize2 = acsize; 
    std::cout << acsize2 << std::endl;
    
    typedef decltype(sizeof(0)) size_t_myt; // size_t_myt = unsigned int  因为 sizeof(0) 返回的是一个无符号整形
    // 等价于 typedef decltype(sizeof(int)) size_t_myt;
    // 等价于 typedef unsigned int size_t_myt; // 也就是定义一个无符号整形的类型别名，叫size_t_myt
    size_t_myt abc;
    abc = 1;
    
    // 3) auto结合decltype构成返回类型后置语法(这里auto不是自动类型推断，而是返回类型后置语法的组成部分)
    // auto funcBack(int a, int b)->int {} 
    // 语法格式： auto + 函数名 + 形参列表 + -> + 函数返回类型 + 函数体
    
    int ii = 19;
    std::cout << funcback(ii) << std::endl; // 19
    double id = 123.34f;
    std::cout << funcback(id) << std::endl; // 123.34
    
    
    // 4) decltype(auto) 用法 c++14版本开始支持
    // a) 用于函数返回类型
    
    int aaa = 100;
    mydouble(aaa) = 122; // 因为模板函数mydouble返回的是一个引用，所以可以放在左边，用来接收左值
    // 函数模板改为 auto mydouble(T &v) 后，编译报错，
    // 因为此时mydouble返回类型变为了int（右值），不能给右值赋值（编译器会报错 = 左边必须为左值）
    
    std::cout << aaa << std::endl;  // 122
    
    decltype(mydouble(aaa)) ioio = aaa; // ioio = int &, 
    
    // b)用于变量声明中
    int xx = 1;
    const int & yy = 1;
    auto zz = yy;    // zz = int (注意：值传递的时候，const和&都会被auto丢弃)
    decltype(auto) zz2 = yy;  // zz2 = const int &;
    // auto丢弃掉的const &能够通过decltype捡回来
    
    // c) 再谈(x)
    int im = 10;
    decltype((im)) im2 = im; // im = int &
    
    decltype(tff1()) tesaa = 234; // 没问题 ，因为推断出返回类型就是 int
    int ag = 1;
    decltype(tff2()) testbb = ag;  // testbb = int &;
    // 编译没问题，
    tff2() = 1243; // 语法上没问题，因为这里返回的引用是局部变量的引用，在执行完tff2()函数的时候，这个内存就已经别回收了
    // 此时你在往这块内存写东西，就相当于往系统内存中写，这是非法的
    
    return 0;
}
