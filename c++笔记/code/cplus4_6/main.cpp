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
#include <vector>
#include <typeinfo>
#include <queue>
#include <list>

using namespace std;

namespace _nmsp
{
    // 可变参数模板：（variadic template ）：允许模板值含有0个及其任意个模板参数
    // 在语法中也和传统模板不一样，多了一个...
    // 1、可变参数模板
    // 我们把这个args称为一包或者一堆参数，而且这些参数类型可以不同，我们理解T的时候不能理解为一个类型，而应该理解为0到多个不同的类型，
    // 那自然args也应该多个不同类型的参数包
    // 注意：
    // T后面带了... ，所以我们称呼为：可变参类型； 这个东西看起来是一个类型，实际上包含了0至多个不同的类型
    // args：可变形参：代表的是一包形参
    
    // 在具体的函数形参中，引用&的位置，出现在类型名的后面 void myfuncTU(const T& t, U&... argsU)
    
    // 参数包的展开
    // 一般都是使用递归方式展开
    // 这就要求我们在代码编写中有一个参数包的展开函数，和一个同名的递归终止函数
    
    template<typename... T>
    void myfunc(T... args)
    {
        cout << sizeof...(args) << endl; // 获取可变参模板参数列表的长度（参数个数）
        cout << sizeof...(T) << endl;
    }
    
    // 同名递归终止函数
    void myfuncTU()
    {
        cout << "参数为空，调用了递归终止函数" << endl;
    }
    // 对于需要进行参数包展开的，建议写成这种格式
    template<typename T, typename... U>
    void myfuncTU(const T& first_args, U&... other_args)
    {
        cout << typeid(first_args).name() << endl;
        cout << sizeof...(other_args) << endl;
        
        cout << "收到的参数值的数据类型为:" << typeid(first_args).name() << endl;
        
        myfuncTU(other_args...); // 递归调用
    }
    
    void func()
    {
        myfunc();  // 0 0
        myfunc("char", 12); // 2 2
        myfunc('s', 23.3f, 34); // 3 3
        
        // myfuncTU(); // 报错，因为myfuncTU里面只是需要包含一个参数
        string aa("sdfsdfs");
        myfuncTU(aa);
        vector<string> myVC{"ttttt", "dddddd"};
        int a1 = 12;
        double a2 = 23.3f;
        bool a3 = false;
        myfuncTU(myVC, a1, a2, a3);
        // 0
        // 0
        // 2
        // 2
        // 3
        // 3
        // Ss
        // 0
        // 收到的�数值的数据类型为:Ss
        // �数为空，调用了递归终��函数
        // St6vectorISsSaISsEE
        // 3
        // 收到的参数值的数据类型为:St6vectorISsSaISsEE
        // i
        // 2
        // 收到的参数值的数据类型为:i
        // d
        // 1
        // 收到的参数值的数据类型为:d
        // b
        // 0
        // 收到的参数值的数据类型��:b
        // 参数为空，调用了�归终止函数


    }
}

namespace _nmsp_class_template
{
    
    // 可变参类模板
    // 允许模板定义中含有0到任意多个模板参数
    
    // 主模板（泛化版本的类模板，需要先有泛化，才能有偏特化版本）
    template<typename... Args>
    class MyClassFO
    {
        public:
            MyClassFO()
            {
                cout << "泛化版本的类模板执行了" << endl;
            }
    };
    
    // 特化一个没有任何参数版本的类模板
    template<>
    class MyClassFO<>
    {
        public:
            MyClassFO()
            {
                cout << "无模板参数的特化版本构造函数执行了" << endl;
            }
    };
    
    
    template<typename First, typename... Other>
    class MyClassFO<First, Other...>:private MyClassFO<Other...> // 注意，这里使用继承的方法实现类似于可变参函数里的递归。这个是一个偏特化的类模板
    {
        public:
            MyClassFO():m_i(0)
            {
                cout << "MyClassFO::MyClassFO()构造函数执行了, this指针为：" << this << endl;
            } // 构造函数
            
            // 通过类继承方式展开参数包
            MyClassFO(First parf, Other... paro):m_i(parf),MyClassFO<Other...>(paro...)
            {
                cout << endl << "-------------------------begin----------------------" << endl;
                
                cout << "MyClassFO::MyClassFO(parf, paro)带参构造函数执行了" << this << endl;
                
                cout << "成员变量 m_i 的值为：" << m_i << endl;
                
                cout << "--------------------------end-----------------------" << endl;
            } // 带参的构造函数的写法（注意，这里给形参包paro使用初始化列表赋值的时候，需要代用父类（也就是给父类进行初始化列表赋值））
            
            
            First m_i; // 成员变量
    };
    
    void func()
    {
        // MyClassFO<int, float, double> myfo;
         
        // 无模板参数的特化版本构造函数执行了
        // MyClassFO::MyClassFO()构造函��执行了, this指针为：0x7fffcee255a0
        // MyClassFO::MyClassFO()构造函数执行了, this指针为：0x7fffcee255a0
        // MyClassFO::MyClassFO()构造函数执��了, this指针为：0x7fffcee255a0
        
        // 这三个类的继承关系就是
        // MyClassFO<int, float, double> === 父类（继承自）===> MyClassFO<float, double>
        // MyClassFO<float, double> === 父类（继承自）===> MyClassFO<double>
        // MyClassFO<double> === 父类（继承自）===> MyClassFO<>
        
        MyClassFO<int, float, double> myfo_with_args(130, 12.33f, 233.43);
        // 注意这里： 使用递归继承，对可变参类模板的实例化的类打印this，会发现他们的this指向都是相同的
        
        //无模板参数的特化版本构造函数执行了
        
        
        // -------------------------begin----------------------
        
        // MyClassFO::MyClassFO(parf, paro)带参构造函数��行了0x7fff7557f9b0
        
        // 成员变量 m_i 的值为：233.43
        
        // --------------------------end-----------------------
        
        
        
        // -------------------------begin----------------------
        
        // MyClassFO::MyClassFO(parf, paro)带参构造函数执行了0x7fff7557f9b0
        
        // 成员变量 m_i 的值为：12.33
        
        // --------------------------end-----------------------
        
        
        
        // -------------------------begin----------------------
        
        // MyClassFO::MyClassFO(parf, paro)带参构造函数执行了0x7fff7557f9b0
        
        // 成员变量 m_i 的值为：130
        
        // --------------------------end-----------------------


    }
    
    
}


namespace _nmsp_class_include_class
{
    // 主模板（泛化版本的类模板，需要先有泛化，才能有偏特化版本）
    template<typename... Args>
    class MyClassFO
    {
        public:
            MyClassFO()
            {
                cout << "泛化版本的类模板执行了" << endl;
            }
    };
    
    // 特化一个没有任何参数版本的类模板
    template<>
    class MyClassFO<>
    {
        public:
            MyClassFO()
            {
                cout << "无模板参数的特化版本构造函数执行了" << endl;
            }
    };
    
    template<typename First, typename... Other>
    class MyClassFO<First, Other...> //:private MyClassFO<Other...> // 注意，这里使用继承的方法实现类似于可变参函数里的递归。这个是一个偏特化的类模板
    {
        public:
            MyClassFO():m_i(0)
            {
                cout << "MyClassFO::MyClassFO()构造函数执行了, this指针为：" << this << endl;
            } // 构造函数
            
            
            // 通过递归组合方式展开参数包
            // 注意这里使用父类组合方式解构参数包与继承方式的区别
            MyClassFO(First parf, Other... paro):m_i(parf),m_o(paro...) //,MyClassFO<Other...>(paro...)
            {
                cout << endl << "-------------------------begin----------------------" << endl;
                
                cout << "MyClassFO::MyClassFO(parf, paro)带参构造函数执行了" << this << endl;
                
                cout << "成员变量 m_i 的值为：" << m_i << endl;
                
                cout << "--------------------------end-----------------------" << endl;
            } // 带参的构造函数的写法（注意，这里给形参包paro使用初始化列表赋值的时候，需要代用父类（也就是给父类进行初始化列表赋值））
            
            
            First m_i; // 成员变量
            MyClassFO<Other...> m_o; // 父类组合变量（组合关系，父类复合子类）
    };
    
    void func()
    {
        MyClassFO<string, int, float, double, long> fo_sifdl("test", 23, 45.54f, 333.333, 12222222222);
         // 注意这里： 使用组合关系，对可变参类模板的实例化的类打印this，会发现他们的this指向都是不相同的
         
        // 无模板参数的特化版本�造函数执行了
        
        
        
        // -------------------------begin----------------------
        
        // MyClassFO::MyClassFO(parf, paro)带参构造函数��行了0x7fff62fd3610
        
        // 成员变量 m_i 的值为：12222222222
        
        // --------------------------end-----------------------
        
        
        
        // -------------------------begin----------------------
        
        // MyClassFO::MyClassFO(parf, paro)带参构造函数执行了0x7fff62fd3608
        
        // 成员变量 m_i 的值为：333.333
        
        // --------------------------end-----------------------
        
        
        
        // -------------------------begin----------------------
        
        // MyClassFO::MyClassFO(parf, paro)带参构造函数执行了0x7fff62fd3600
        
        // 成员变量 m_i 的值为：45.54
        
        // --------------------------end-----------------------
        
        
        
        // -------------------------begin----------------------
        
        // MyClassFO::MyClassFO(parf, paro)带参构造函数执行了0x7fff62fd35f8
        
        // 成员变量 m_i 的值为：23
        
        // --------------------------end-----------------------
        
        
        
        // -------------------------begin----------------------
        
        // MyClassFO::MyClassFO(parf, paro)带参构造函数执行了0x7fff62fd35f0
        
        // 成员变量 m_i 的值为：test
        
        // --------------------------end-----------------------
        

    }
}

namespace _nmsp_4
{
    // 可变参数类模板
    // 通过tuple和递归调用方式展开参数包
    // 这种展开参数包需要写类的特化版本，有一定难度
    // 实现思路：
    // 计数器从0开始，每处理一个参数，计数器加1，一直到把所有参数完成，最后实现一个模板偏特化，作为递归调用的结束
    
    // 这里count是第几个参数，用于统计，从9开始，然后length标识参数个数，T就是参数包的参数类型包
    template<int count, int length, typename...T>
    class TubpleClass
    {
        public:
            static void mytupleFunc_static(const tuple<T...>&t)
            {
                cout << "value值为：" << get<count>(t) << endl;
                TubpleClass<count+1, length, T...>::mytupleFunc_static(t);
            }
    };
    
    // 需要一个特化版本用于结束递归调用
    template<int length, typename... T>
    class TubpleClass<length, length, T...>
    {
        public:
            static void mytupleFunc_static(const tuple<T...> &t)
            {
                cout << "递归调用结束" << endl;
            }
    };
    
    
    template<typename...T>
    void myFuncTuple(const tuple<T...> &t) // 可变参函数模板
    {
        TubpleClass<0, sizeof...(T), T...>::mytupleFunc_static(t);
        // 注意这里是调用类的静态成员函数。使用类名：：静态成员函数名（参数）调用
    }
    
    void func()
    {
        tuple<float, int, int> mytuple(12.4f, 22, 44); //这就是一个tuple（元组），一堆各种东西的组合
        // cout << get<0>(mytuple) << endl; // 12.4
        // cout << get<1>(mytuple) << endl; // 22
        // cout << get<2>(mytuple) << endl; // 44
        
        myFuncTuple(mytuple);
        // value值为：12.4

        // value值为：22
        
        // value值为：44
        
        // 递归�用结束

    }
}

namespace _nmsp_5
{
    // 模板模板参数， 首先，这是一个模板参数，然后，这个模板本身又是一个模板
    template<
            typename T, // 类型模板参数
            // template<class> class Container // 这就是一个模板模板参数
            // 模板模板参数还有一个写法
            template<typename V> typename Container // 这里这个V没有任何用途，可以省略，如上面的写法就是省略的，语法上没问题
            > // T,U叫模板参数，因为他们前面都有typename，所以又叫类型，模板参数
    class TUClass
    {
        public:
            T m_i;
            
            Container<T> myc; // container是作为一个类模板来使用的（因为他后面可以加尖括号<>）
                // 也就是说，如果你想在类模板中把模板参数作为一个类模板使用，就需要把这个模板参数写为模板模板参数
    
            
            TUClass()
            {
                for(int i=0; i < 10; ++i)
                {
                    myc.push_back(i*i);
                }
            }
            
            void getValue()
            {
                auto begin = myc.begin();
                auto end = myc.end();
                
                cout << endl;
                
                while(begin != end)
                {
                    cout << "-[" << *begin << "]-";
                    ++begin;
                }
                
                cout << endl;
            }
        
    };
    
    template<typename T>
    using MVect = vector<T, allocator<T>>; // 这种写写法很固定，定义一个容器别名
    
    template<typename T>
    using MList = list<T, allocator<T>>; // 这种写写法很固定，定义一个list列表别名
    
    void func()
    {
        TUClass<int, MVect> my_tuc_vec; // 往容器里塞入元素，元素类型为int
        TUClass<int, MList> my_tuc_list; // 往列表里塞入元素，元素类型为int
        
        my_tuc_vec.getValue();
        
        my_tuc_list.getValue();
        
        
        // -[0]--[1]--[4]--[9]--[16]--[25]--[36]--[49]--[64]--[81]-
        
        
        // -[0]--[1]--[4]--[9]--[16]--[25]--[36]--[49]--[64]--[81]-




        
        
    }
}


int main()
{
    // 演示1
    // _nmsp::func();
    
    // 演示2
    // _nmsp_class_template::func();
    
    // 演示3
    // _nmsp_class_include_class::func();
    
    // 演示4
    // _nmsp_4::func();
    
    // 演示5
    _nmsp_5::func();
    
    
    return 1;
}


