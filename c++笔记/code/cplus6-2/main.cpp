/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <memory>
#include <thread>

using namespace std;

void myprint()
{
    for(int i = 0; i < 100; ++i)
    {
        cout << "线程开始执行" << i << endl;
    }
}

class TA
{
  public:
  int &m_i;
    TA(int &i):m_i(i) // 普通构造函数 注意这里使用引用传值，当调用线程的detach（）时，会存在内存泄漏的问题
    // 因为使用了detach，主线程结束，里面的内存被销毁，而这里使用的时引用，那么就相当于你在这里使用了一个被销毁的内存
    // 所以，如果是detach，这里要使用值传递（复制的方法）
    {
        cout << "ta ()构造函数执行" << endl;
    }
    TA(const TA &ta):m_i(ta.m_i) // 拷贝构造函数
    {
        cout << "ta()拷贝构造函数执行" << endl;
    }
    ~TA()
    {
        cout << "析构函数执行" << endl;
    }
    void operator() ()
    {
        cout << "类类型的可调用对象 的 线程执行了" << endl;
        cout << "m_i的值为：" << m_i << endl;
    }
    
};

int main()
{
    // 程序运行起来生成一个进程，同时这个进程的运行也代表所属的主线程开始自动运行
    
    cout << "thread" << endl;
    
    // 主线程从main函数开始运行，那么我们自己创建的线程也需要一个入口函数（初始函数）开始运行
    // 一旦这个函数运行完毕，就代表我们的线程执行完毕
    
    // 整个进程是否执行完毕的标志，是主线程是否执行完毕，如果主线程执行完毕了，就代表整个进程执行完毕
    // 此时如若其他子线程没有执行完毕，那么，一般情况下这些子线程也会被操作系统强行终止
    
    // 所以，一般情况下，如果想保持子线程的生命周期（运行状态），那么就该让主线程一直保持运行
    
    // thread 是标准库里的一个类
    // join()； 加入、回合。说白了就是阻塞，阻塞主线程，让主线程等待子线程执行完毕，然后子线程和主线程汇合
    std::thread mytobj(myprint); // 这个实参是一个可调用对象
    // 创建了线程，线程执行入口是myprint()， myprint()开始执行
    // mytobj.join(); // 主线程阻塞到这里，等待子线程mytobj执行完毕
    if (mytobj.joinable())
    {
        mytobj.detach();
        if (mytobj.joinable())
        {
            mytobj.join();
        }
    }
    
    cout << "test" << endl;
    
    // detach(); 
    // 传统规律，主线程需要等待子线程执行完毕，然后自己再最后退出
    // detach：分离，也就是主线程不和子线程汇合，子线程和主线程各自执行各自的，不必等待
    // 我们创建了很多子线程，让主线程挨个等待子线程执行完毕，当不需要这种等待的时候就可以使用detach
    // 一旦detach之后，与这个主线程关联的thread对象就会失去与这个主线程的关联关系，此时这个子线程就会驻留在后台运行
    // 这个子线程就相当于被C++运行时库接管，当这个子线程执行完毕之后，由运行时库负责清理该线程相关的资源（守护线程）
    // 一旦调用了detach()，我们之后就不能再使用join()，否者会爆异常
    
    // joinable(); 判断是否可以成功使用join或者detach();(返回true，可以join或者detach)
    
    // 其他创建线程的方法
    // 用类对象 的可调用对象重载
    // 一旦调用了这个detach，那主线程执行完毕，那这里用的这个ta这个对象还在吗？
    // 对象肯定是不在了的，但是为什么会依旧可以执行呢，因为实际上这里这个对象时被复制到线程中去的，ta在主线程执行完会被销毁
    // 但是所复制的ta对象依旧存在
    // 所以，只要你这个ta对象里没有引用，没有指针，那么就不会产生问题
    int mi = 66;
    TA ta(mi);
    thread mytobj2(ta); // ta可调用对象
    mytobj2.join();
    // mytobj2.detach();
    
    // 使用lambda表达式
    auto mylamdbathread = []{
        cout << "拉姆达创建的线程执行" << endl;
    };
    thread mytobj3(mylamdbathread);
    mytobj3.join();
 
    return 0;
}

