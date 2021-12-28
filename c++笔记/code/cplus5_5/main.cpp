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
#include <memory>
#include <vector>

using namespace std;

// shared_ptr<int> createO(int value)
// {
//     return make_shared<int>(value);
// }

// // void myfunc(int value)
// shared_ptr<int> myfunc(int value)
// {
//     shared_ptr<int> pmt = createO(value);
//     cout << *pmt << endl;
//     return pmt; // 离开作用域后，pmt会被释放，他所指向的内存也会被回收
// }

// void proc(shared_ptr<int> prc)
// {
//     return;
// }

// class CT
// {
//     public:
//         shared_ptr<CT>getShelf()
//         {
//             return shared_ptr<CT>(this);
//         }
//         CT()
//         {
            
//         }
// };

// class CT : public enable_shared_from_this<CT>
// {
//     public:
//         shared_ptr<CT>getShelf()
//         {
//             // return shared_ptr<CT>(this); // 这里也不能用裸指针this
//             // 改用
//             return shared_from_this(); // 这个是enable_shared_form_this这个类模板里的方法，要通过此方法返回智能指针
//         }
// };

// class CA;
// class CB
// {
//     public:
//         shared_ptr<CA>m_ca_p;
//         ~CB()
//         {
//             int test;
//             test = 1;
//             cout << "~CB" << endl;
//         }
// };

// class CA
// {
//     public:
//         // shared_ptr<CB>m_cb_p;
//         weak_ptr<CB>m_cb_p; // 把这里变为弱引用
//         ~CA()
//         {
//             int test;
//             test = 1;
//             cout << "~CA" << endl;
//         }
// };

int main()
{
    // std::shared_ptr使用场景
    // auto p1 = myfunc(122);
    
    // 慎用裸指针
    // int *p =new int(100);
    // // proc(p); // 语法错，int*不能隐式转为shared_ptr<int>
    // proc(shared_ptr<int>(p)); // 参数是一个临时的shared_ptr，用一个裸指针来进行显式的构造
    // *p = 45; // 陷阱，会出现潜在的不可预料的问题，因为此时p指向的内存已经被释放了
    // 因为我们在上面proc函数调用的时候，使用p临时构造了一个临时的shared_ptr指针对象（这之后，该裸指针的所有内存管理，都将交由这个shared_ptr进行管理
    // 智能指针销毁，裸指针也一并被销毁，所以使用裸指针绑定到shared_ptr之后，都不建议继续使用这个裸指针进行操作），然后作为实参以值传递的方式
    // 传给函数proc,proc执行完毕，就会销毁这个传进来的临时shared_ptr，这个操作就会导致p被释放
    // 此后再用p进行操作，都将是违法的高危操作
    
    // shared_ptr<int> myp(new int(100));
    // proc(myp);
    // *myp = 1220;
    // cout << *myp << endl;
    
    // 不要用裸指针初始化多个shared_ptr
    // int *p = new int(1223);
    // shared_ptr<int>p1(p);
    // shared_ptr<int>p2(p);
    // cout << p1.use_count() << endl; // 1
    // cout << p2.use_count() << endl; // 1
    // 高危代码
    // 因为上面我们看到，p1和p2的引用计数都是1，正常情况应该是2，但是为1说明了这两个引用并没有贯通，各指各的，一旦我们程序结束，释放内存，会导致
    // p1引用计数为1，释放一次p内存,然后p2引用计数因为两者没关领到，所以引用计数不会减一，会继续进行内存释放，p又会被释放一次内存
    // 这就造成不可预料的后果
    
    // shared_ptr<int>p1(new int);
    // shared_ptr<int>p2(p1); // 这种写法是OK的，p1和p2指向同一个内存地址并且两者是互通的（用的都是同一个控制块）
    // cout << p1.use_count() << endl; // 2
    // cout << p2.use_count() << endl; // 2
    
    // 慎用get()返回的裸指针
    // 返回智能指针指向的对象的裸指针（有些第三方库的函数，可能需要穿递的参数就是裸指针）
    // get()返回的裸指针不能delete，否者会产生异常。（应该铭记一点，绑定上了智能指针，就应该使用之智能指针进行后续所有的对象内存管理实务）
    // shared_ptr<int>mu(new int(1000));
    // int *p = mu.get();
    // delete p; // 报异常 *** Error in `./a.out': double free or corruption (fasttop): 0x0000000002147c20 ***

    // 不能将其他智能指针绑到get返回的指针上
    // shared_ptr<int> pi(new int(232));
    // int *p = pi.get();
    // shared_ptr<int>newp(p);
    // cout << pi.use_count() << endl; // 1
    // cout << newp.use_count() << endl; // 1
    
    // shared_ptr<int> pi(new int(232));
    // int *p = pi.get();
    // {
    //     // newp离开这个大括号，（作用域），就会导致其内存被释放（也就是pi绑定的那块内存被释放，那么就会导致后续pi失效不可用）
    //      shared_ptr<int>newp(p);
    // }
    // cout << pi.use_count() << endl; // 1
    // cout << newp.use_count() << endl; // 1
    // 和上面的错误情况一样，都是出现了一个裸指针绑定了两个智能指针
    
    // 结论，永远不要用get得到的裸指针来初始化另一个智能指针，或给另一个智能指针赋值
    
    // 不要把类对象指针（this）作为shared_ptr返回，改用enable_shared_form_this
    // shared_ptr<CT>pct(new CT);
    // // shared_ptr<CT>pct2 = pct; // 这是两个强引用（引用计数2）这样写是可以的
    // shared_ptr<CT> pct2 = pct->getShelf(); // 出现问题
    // cout << pct.use_count() << endl; // 1 -> 2
    // cout << pct2.use_count() << endl; // 1 ->2
    // 发现两个引用计数都是一，又出现了使用一个裸指针（这里是this裸指针）来绑定两个智能指针的问题
    // 就会导致上面最开始的(new CT)出来的临时对象被析构两次
    
    // 解决办法就是，使用C++11里标准库里的类模板：enable_shared_form_this，让自定义的类继承自该模板
    // shared_ptr<CT>pct(new CT);
    // shared_ptr<CT> pct2 = pct->getShelf(); // 这样就不会出现问题，因为pct->getShelf()返回的是一个智能指针
    // cout << pct.use_count() << endl; // 2
    // cout << pct2.use_count() << endl; // 2
    
    // 用到c++标准库里的类模板，enable_shared_from_this
    // 现在，在外面创建CT类对象的智能指针，以及通过CT对象返回的this智能指针都是安全的
    // 这个enable_shared_from_this中有一个弱指针weak_ptr，他能够监视这个this
    // 在我们调用shared_from_this()这个方法的时候，这个方法内部实际上是调用了这个weak_ptr的lock()方法
    // 这个lock（）方法会让shared_ptr引用计数加1，同时返回这个shared_ptr，这就是enable_shared_from_this的工作原理
    
    
    // 避免循环引用（循环引用会导致内存泄漏）
    // shared_ptr<CA>pca(new CA);
    // shared_ptr<CB>pcb(new CB);
    
    // pca->m_cb_p = pcb; // 等价于指向CB对象有两个（改为弱引用之后变为：一个， 因为weak_ptr<CB>m_cb_p; ）强引用
    // pcb->m_ca_p = pca; // 等价于指向CA对象有两个强引用
    
    // cout << pca->m_cb_p.use_count() << endl; // 2  ====> 1
    // cout << pcb->m_ca_p.use_count() << endl; // 2  ====> 2
    
    // ~CB
    // ~CA
    
    // 这两个强引用就会导致，程序结束，引用计数减1，变为1，所以在程序结束时并不会进行内存释放（只有引用计数为0才会进行）
    
    // 程序运行结束，我们发现，并没有打印析构函数里的代码（也就是没有执行任何一个类的析构函数），这就说明，出现了异常，而且产生了内存泄漏
    // 改正方法：将上面两个类中任意一个改为弱引用
    // 改正之后，
    //  pca->m_cb_p = pcb; 离开作用域后，pcb的引用计数就变为0，就会去释放CB对象（也就是上面初始化new 出来的CA），
    // 然后CB的析构函数就被执行，表示CB对象即将被释放，导致CB内的成员变量m_ca_p引用计数减一，
    // 此时，指向CA对象的引用计数就由2变为1，那么当超出作用域（这里是程序结束）时，指向CA的引用计数会再减一参，然后变为0，就可以对CA对象进行内存释放（调用器析构函数）
    
    
    // 性能说明
    // shared_ptr的大小是裸指针的两倍
    // weak_ptr也是裸指针的两倍
    // char *p;
    // cout << sizeof(p) << endl; // 8
    // shared_ptr<char>pc(p);
    // cout << sizeof(pc) << endl; // 16
    // 无论是shared_ptr还是weak_ptr都有两个裸指针，一个指向T类型对象的指针，一个是指向控制块的指针
    // 控制块里包含有引用计数 、 弱引用计数 、其他数据（比如自定义的删除器，分配器等）
    // 这个控制块是由第一个指向某对象的shared_ptr创建的。weak_ptr也是指向该shared_ptr创建的控制块
    
    // 控制块创建时机
    // make_shared：分配并初始化一个对象，返回此对象的shared_ptr，所以，make_shared总是能够创建一个控制块
    // shared_ptr<int> = make_shared<int>(100);
    
    // 用裸指针创建一个shared_ptr对象时
    // int *p = new int(12);
    // shared_ptr<int> p1(p);
    // shared_ptr<int> p2(p); // 不允许，这样使用一个裸指针创建多个shared_ptr会创建多个控制块（也就是会造成多个引用计数都是1）
    // 析构的时候会导致析构多次
    // 最好使用 shared_ptr<int> p1(new int);
    
    // 移动语义
    // shared_ptr<int> p1(new int(100));
    // cout << p1.use_count() << endl; // 1
    // shared_ptr<int> p2(std::move(p1)); // 移动语义，移动构造一个新的智能指针对象
    //                                 // p1就不在指向该对象了（变为空）,引用计数仍然为1
    // cout << p2.use_count() << endl; // 1
                                    
    // shared_ptr<int> p3;
    // p3 = std::move(p2); // 移动赋值，p2指向空，p3指向该对象，整个引用计数仍旧为1
    // cout << p3.use_count() << endl; // 1
    // 移动肯定比复制快，复制需要增加引用计数，移动不需要
    // 移动构造函数快过复制构造函数，移动赋值预算符快过拷贝赋值运算符
    
    // 补充
    // 分配器（解决内存分配问题）
    // shared_ptr<int> p(new int(122), mymallocator<int>());
    
    // 奇怪用法，不要轻易尝试
    // new shared_ptr<T>, memcpy() 内存拷贝
    
    // 优先使用make_shared() 除了不可以定义自己的删除器
    shared_ptr<string> ps1(new string("ggggggg")); // 分配两次内存
    shared_ptr<string> ps2 = make_shared<string>("sdfsdfs") // 分配一次内存
    
    return 0;
}

