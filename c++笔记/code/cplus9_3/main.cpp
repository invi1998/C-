/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <vector>
// #include <boost/type_index.hpp>

using namespace std;

// namespace _nmsp1
// {
//     template<typename T> // T 是类型模板参数，所以T是有类型的
//     // void myfunc(const T &tmpv) // tmpv是一个形参，他也是有类型的，往往形参tmpv和T类型模板参数的类型不一样
//     // void myfunc(T &tmpv)
//     // void myfunc(T *tmpv)
//     // void myfunc(T&& tmpv)
//     void myfunc(T tmpv)
//     {
//         std::cout << "---------------begin-----------------" << std::endl;
//         using boost::typeindex::type_id_with_evr;
//         std::cout << "T = " << type_id_with_evr<T>().pretty_name() << std::endl;
//         // 显示T的类型
//         std::cout << "tmpv = " << type_id_with_evr<decltype(tmpv)>().pretty_name() << std::endl;
//         // 显示形参tmpv的类型
//         std::cout << "----------------end------------------" << std::endl;
//     }
    
//     void testFunc()
//     {
//         return;
//     }
    
//     void func()
//     {
//         // 如何查看类型推断结果（模板类型推导）
//         // 如何利用boost库将编译器推断的结果打印出来
        
//         // 理解模板类型推断
//         myfunc(100);
//         // T : int      tmpv: const int &
        
//         // 指针或者引用类型
//         // void myfunc(T &tmpv)
//         // 如果tmpv是一个指针或者引用，但是不是一个万能引用
//         int i = 18;         // int  i的类型是int型，i本身是一个左值
//         const int j = i;    // const int
//         const int &k = i;   // const int &
    
//         myfunc(i);          // T：int           tmpv: int &
//         myfunc(j);          // T: const int     tmpv: const int&
//         myfunc(k);          // T：const int     tmpv: const int&
//         // 1）如果实参是引用类型，则引用类型会被忽略，T不会被推导为引用类型
//         // 2）当我们往引用类型形参里传const类型实参时，形参就会变成const引用，
//         // 实参的const属性会成为类型模板参数T类型的推导的一部分，所以不用担心会在myfunc中修改原有const属性的实参
    
//         // 如果是常量引用
//         // void myfunc(const T &tmpv)
//         int i = 18;         // int 
//         const int j = i;    // const int
//         const int &k = i;   // const int &
    
//         myfunc(i);          // T：int           tmpv: const int&
//         myfunc(j);          // T: int           tmpv: const int&
//         myfunc(k);          // T：int           tmpv: const int&
//         // 1）如果实参是引用类型，则引用类型会被忽略，T不会被推导为引用类型
//         // 2）对于有const属性的实参，在T中推导后，T中的const属性没有了，因为模板函数myfunc的形参tmpv里面出现了 const，
//         // 实参的const属性会成为类型模板参数T类型的推导的一部分，所以不用担心会在myfunc中修改原有const属性的实参
    
    
//         // 如果是指针
//         // void myfunc(T *tmpv)
//         int i = 18;         // int 
//         const int j = i;    // const int
//         const int &k = i;   // const int &
//         const int *pi = &i;
    
//         myfunc(&i);          // T：int           tmpv: int *
//         myfunc(pi);          // T: const int     tmpv: const int*
//         // 如果tmpv中没有const,则实参中的const会被带到T类型中去
//         // 如果tmpv中有const,则T类型中不会带有const
        
//         // 万能引用：
//         // 如果tmpv是万能引用 T&& 能接收左值或者右值，会根据你传递进来的是左值还是右值，表现会有所不同
//         // void myfunc(T&& tmpv)
//         int i = 18;         // int 
//         const int j = i;    // const int
//         const int &k = i;   // const int &
    
//         myfunc(i);          // i是左值 T：int &         tmpv: int &     （出现了引用折叠）
//         myfunc(j);          // j是左值 T: const int &   tmpv: const int &
//         myfunc(k);          // k是左值 T：const int &   tmpv: const int &
//         myfunc(199);        // 100右值 T：int           tmpv：int &&
        
//         // 传值方式
//         // 如果tmpv不是指针也不是引用，而是常规的传值方式
//         // void myfunc(T tmpv)
//         int i = 18;         // int 
//         const int j = i;    // const int
//         const int &k = i;   // const int &
    
//         myfunc(i);          // i是左值 T：int           tmpv: int
//         myfunc(j);          // j是左值 T：int           tmpv: int。 const属性没有传递进去，因为形参会得到一个新的副本
//         myfunc(k);          // k是左值 T：int           tmpv: int
//         // 现在传指针进去
//         char mystr[] = "iiiiiiiiiiiiii";
//         const char* const point = mystr;
//         // 第一个const表示point指向的目标内存不能通过point来改变
//         // 第二个const表示point指向一个内容后不可以再指向其他内容
//         mystr(point);       // T：const char *          tmpv: const char * 
//         // 第二个const没有了，第一个const保留
//         // 总结：如果你传递的是（常量指针）const char *或者（常量数组）const char[]数组，那么这个const会被保留
    
    
//         // 数组做实参
//         // 数组名代表首地址
//         const char[] mystr = "ooooooooooooo";
//         // 值传递时：void myfunc(T tmpv)
//         myfunc(mystr);       // T：const char *          tmpv: const char * 
//         // 引用传递时：void myfunc(T& tmpv)
//         myfunc(mystr);      // T：const char [14]         tmpv: const char (&)[14] 
//         // T被推导为了数组类型，tmpv中的（&）代表该数组的一个引用
//         // 函数模板获取数组长度
//         // template<typename T, unsigned l>
//         // void myfunc(T (&tmpv)[l])
//         // {
//         //     std::cout << l << std::endl;
//         // }
        
//         // 函数名做实参
//         // 函数名相当于一个函数首地址，可以赋值给一个函数指针
//         // 值传递时 void myfunc(T tmpv)
//         myfunc(testFunc);   // T: void (*)(void)      tmpv: void (*)(void)  函数指针
//         // 引用传值时 void myfunc(T& tmpv)
//         myfunc(testFunc);   // T: void(void)      tmpv: void (&)(void)  函数引用
        
        
//         // 总结：
//         // 1）推断中，引用类型实参的引用类型等于不存在
//         // 2）万能引用实参为左值或者右值，推断结果不一样
//         // 3）按值传递实参，实参的const属性没有用
        
        
//         // 引用折叠规则
//         int i = 18; // i的类型是int型、  i本身是一个左值
//         myfunc(100); // 100右值 T = int ，tmpv = int &&;
//         myfunc(i);  // i是左值 T = int &, tmpv = int &
        
//         // void myfunc(int & &&tmpv) {...........}  // 我们将T带入模拟编译器实例化模板， 认为这应该是编译器实际实例化出来的myfunc的样子
//         // void myfunc(int & tmpv) {...........}  // 但是实际上编译最终的实例化结果是这样子
        
//         // 引用折叠 c++11新规则（reference-collapsing rules），也叫 引用坍塌
//         // C++中有明确含义的引用只有两种，一种是&左值引用（类型），一种是带&&的右值引用（类型）
        
//         // void myfunc(int & &&tmpv) // 两组，第一组（int &）是左值引用，第二组（&& tmpv）是右值引用
//         // 对于分成两组的，第一组： 左值/右值引用 第二组：左值/右值引用
//         // 左值引用 -- 左值引用     &       &       ======折叠=====>    &
//         // 左值引用 -- 右值引用     &       &&      ======折叠=====>    &
//         // 右值引用 -- 左值引用     &&      &       ======折叠=====>    &
//         // 右值引用 -- 右值引用     &&      &&      ======折叠=====>    &&
        
//         // 折叠规则：如果任意一个引用为左值，那么折叠结果都为左值（传染），否者就是右值引用
        
//         // 引用的引用
//         // void func(int & && i) // 系统编译报错 "i引用的引用非法"
//         int b = 500;
//         int &bi = b;        // bi是b的引用
//         int & &byy = bi;    // 引用的引用非法（编译不通过） byy是 bi的引用（这个就叫引用的引用，注意这里两个引用符必须空格分开，否者就成了右值引用）
//         int & &byy = b;     // 引用的引用非法（编译不通过）
        
//         // 引用的引用 有这个叫法，但是不可以在程序中这么写 也就是 & &非法
//         // 但是编译器内部在进行函数模板类型推断的时候，也可能会出现引用的引用，这个时候编译器会用引用折叠进行处理
//         // 但是编译器不允许程序开发者写出引用的引用这种代码
        
//         // 需要引用折叠的实际场景：函数模板实例化，auto
//     }
    
// }

namespace _nmsp2
{
    
    // 定义一个模板函数 ： 要把收到的参数以及这些参数相对应的类型不变（比如左值引用。右值引用，const）的转发给其他函数（转发给myfunc）
    // 这就叫转发
    template<typename F, typename T1, typename T2>
    // void myfuncTmp(F f, T1 t1, T2 t2) // F就是第三方我们要调用的函数（要转发到的目标函数）
    void myfuncTmp(F f, T1 &&t1, T2 &&t2)
    {
        
        // 万能引用后（针对 myfuncTmp(myfunc, 40, j); ）
        // T1 = int, t1 = int &&
        // T2 = int &, t2 = int &
        
        // 针对 myfuncTmp(myfunc2, 30, k); 
        // T1 = int, t1 = int &&。但是，因为函数里的形参总是左值，所以t1是左值（类型是右值引用）
        // 所以在调用myfunc2的时候，往里面的v1(右值引用，注定其只能绑定右值)，t1绑定到右值引用，就报错了
        // f(t1, t2);
        // 改造为完美转发
        f(std::forward<T1>(t1), std::forward<T2>(t2));
    }
    
    // void myfunc(int v1, int v2)
    void myfunc(int v1, int & v2)
    {
        ++v2; // 改变V2的值，让其自加1
        std::cout << v1 + v2 << std::endl;
    }
    
    void myfunc2(int &&v1, int & v2)
    {
        std::cout << v1 << std::endl;
        std::cout << v2 << std::endl;
    }
    
    void printInfo(int &i) // 类型是左值引用的形参
    {
        std::cout << "printInfo()的参数类型是左值引用" << std::endl;   
    }
    
    void printInfo(int &&i) // 类型是右值引用的形参
    {
        std::cout << "printInfo()的参数类型是右值引用" << std::endl;   
    }
    
    template<typename T>
    void printTmp(T && t)
    {
        printInfo(t);
        printInfo(std::forward<T>(t));
        printInfo(std::move(t));
    }
    
    void func()
    {
        // 转发 完美转发
        
        int i = 10;
        myfunc(20, i);  // 20 + 11 = 31
        std::cout << i << std::endl;    // 10 ==变成引用后==> 11
        
        int j = 70;
        myfuncTmp(myfunc, 40, j);   // 40 + 71 = 111
        std::cout << j << std::endl;    // 70 ==变成引用后==> 70（错误） ===变成万能引用后===> 71（正确）
        
        // 这里j不是71，说明我们的转发模板出了问题（当我们遇到引用的时候就不能正常工作）
        // 想象中myfuncTmp在被调用后实例化的结果是
        // void myfuncTmp(void (*myfunc)(int, int &), int t1, int t2)
        // 所以，这里就导致我们函数模板里传递给目标函数的是t2，而不是我们传递的 引用j(&j),
        // 那么我们如何通过函数模板给目标函数传递一个引用呢？（修改转发函数模板的参数，让其能够保持给定实参的左值性，当然，如果实参有const属性，我们也希望const属性能被传递进去）
        // 通过 万能引用 实现（实参的所有信息都能传递到万能引用中，从而让编译器推导出函数模板的最终形参类型）
        // 如果不用万能引用，只用普通的引用传值 ，则只有const属性能传递到函数模板中，而实参中的左值和右值无法传递
        
        int k = 100;
        myfunc2(200, k);
        
        // 定义一个右值引用类型(右值引用绑定右值)
        int &&youzhi = 90;
        // 虽然 &&youzhi是绑定到右值的，但是 youzhi 本身他是一个左值，因为 youzhi 是在等号左边待着的
        int &z = youzhi; // 证明 youzhi 是一个左值，因为youzhi可以绑定到左值引用
        // &&youzhi 叫右值引用，youzhi是一个左值（有地址）
        // youzhi 是一个左值，但是他的类型是右值引用（&&）
        // 也就是说，左值引用 还是 右值引用 这种概念说的是他的类型，而不是它本身
        // void fc(int &&v){}  // 注意：函数中的形参总是左值，即使他的类型是右值引用
        // myfunc2(youzhi, k); // cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’
        // 无法将参数1从int转为int &&
        // myfunc2(std::move(youzhi), k); // 可以使用move左值转右值
        
        // 同样的，通过转发模板，也会报错无法将左值转右值
        // myfuncTmp(myfunc2, youzhi, k); // 报错
        myfuncTmp(myfunc2, 30, k); // 报错
        
        
        // 完美转发  让我们可以写能够接收任意类型的实参的函数模板，并将其转发到目标函数，目标函数会接收到与转发函数完全相同的参数
        // std::forward
        // c++11 中专门为转发而存在的一个新函数，要么返回左值，要么返回右值
        // f(std::forward<T1>(t1), std::forward<T2>(t2));
        // 发挥作用的条件
        // 调用模板函数，模板函数参数是万能引用，模板函数负责转发
        // std::forward的能力就是按照实参本来的类型进行转发
        // 对std::forward的理解：
        // 1）实参如果是一个左值，forward是按照形参原来的类型进行处理，所以std::forward之后还是左值
        // 2）实参如果原来是一个右值，到了形参中变成了左值，forward是按照形参原来的类型进行处理，所以std::forward之后变成右值
        // 所以，这里看来std::forward有强制将左值转为右值的能力，forword只对原来是个右值的情况有用
        // forward的能力就是保持原始实参的类型
        
        printTmp(100);
        // 100 右值， T = int；  t = int &&  但是t本身是左值
        // printInfo()的参数类型是左值引用
        // printInfo()的参数类型是右值引用
        // printInfo()的参数类型是右值引用
        printTmp(k);
        // k 左值， T = int &；  t = int &  但是t本身是左值
        // printInfo()的参数类型是左值引用
        // printInfo()的参数类型是左值引用
        // printInfo()的参数类型是右值引用
        
        // 实参是左值还是右值，这个信息会被保存到转发函数模板里的万能引用里面的模板类型参数里去的
    
        // std::move()和std::forward的区别
        // forward 就是强制把一个左值转为右值，但是如果你实参就是左值，那forward啥也不干（有条件的类型转换）
        // move() 无条件的强制类型转换（强制将左值转右值）
        
        // 万能引用 并不是一种新的引用类型，他只是一种写代码的表象
        // 但是万能引用这个概念的存在是很有意义的
        
    }
}


int main()
{
    // _nmsp1::func();
    _nmsp2::func();
    
    return 0;
}

