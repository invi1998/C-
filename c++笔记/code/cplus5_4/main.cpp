/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <memory>
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

int main()
{
    // weak_ptr
    // weak_ptr用来辅助shared_ptr，他是一种弱智能指针
    // 他也是一个类模板，他是用来指向一个有shared_ptr所管理的对象，但是weak_ptr这种指针不控制所指对象的生命周期
    // 换句话说，就是将weak_ptr绑定到shared_ptr的引用计数，更确切的说，weak_ptr的构造和析构不会增加或者减少所指对象的引用计数
    // 当shared_ptr需要释放所指对象的时候，是不会管是否有weak_ptr指向的。
    
    // 这个弱引用的作用，是用来监视shared_ptr的生命周期，是一种对shared_ptr的功能争强
    
    // weak_ptr的创建
    // 我们创建一个weak_ptr的时候一般使用一个shared_ptr来进行初始化
    // auto pi = make_shared<int>(100);
    // weak_ptr<int>piw(pi); // piw就是一个弱共享pi, 她的出现不会改变强引用（shared_ptr）的引用计数，但是会改变弱引用（weak_ptr）的引用计数
    // cout << pi.use_count() << endl; // 1
    // cout << piw.use_count() << endl; // 1
    // weak_ptr<int> piw2;
    // piw2 = pi;
    // weak_ptr<int> piw3;
    // piw3 = piw2;
    
    // cout << pi.use_count() << endl; // 1
    // cout << piw.use_count() << endl; // 1
    // cout << piw2.use_count() << endl; // 1
    
    // // lock() 功能就是检查weak_ptr所指向的对象是否存在，如果存在，那么lock就会返回一个指向该对象的shared_ptr（指向对象的强引用计数加1）
    // // 如果他所指向的对象不存在，lock就会返回一个空的shared_ptr
    // // 
    // pi.reset(); // 因为pi是唯一指向该引用对象的指针，所以reset的调用会释放该智能指针指向的对象，同时将pi置空
    // auto pi2 = piw.lock();
    // if (pi2) { // 注意，这种判断代码务必要写，特别是涉及到指针可能会产生空指针的情况
    //     cout << *pi2 << endl; // 100
    //     cout << pi2.use_count() << endl; // 2
    // } else {
    //     cout << "lock返回空" << endl;
    // }
    
    // weak_ptr 常用操作
    // use_count(); 获取与该弱指针共享对象的其他强指针shared_ptr的数量，或者说，获取当前所监视资源的强引用计数
    
    // auto pi = shared_ptr<int>(new int(2000));
    // auto pi2(pi);
    // weak_ptr<int> pw(pi);
    // cout << pw.use_count() << endl; // 2
    
    // // expired() 是否过期，若该指针的use_count返回0，则表示该弱指针指向的对象已经不存在了，则返回true，表示已过期。
    // // 换句话说，这个函数用来判断所观测的对象资源是否被释放
    // pi.reset();
    // cout << pw.expired() << endl; // 0 false
    // pi2.reset();
    // cout << pw.expired() << endl; // 1 true
    
    // // reset() 将该弱引用指针设置为空，不影响指向对象的强引用计数，但是指向该对象的弱引用数量会减少
    // piw.reset();
    
    // auto pi = make_shared<int>(240);
    // weak_ptr<int> pw;
    // pw = pi;
    // if (!pw.expired())
    // {
    //     auto p2 = pw.lock(); // 返回一个shared_ptr,并且此时强引用计数为2
    //     if (p2 != nullptr)
    //     {
    //         //
    //         int test;
    //         test = 122;
            
    //         // 离开这个作用域，强引用计数会恢复为1
    //     }
    // }
    // else
    // {
    //     // 已过期
    //     int test;
    //     test = 1;
    // }
    
    // 尺寸问题
    // weak_ptr<int> pw;
    // weak_ptr的尺寸和shared_ptr的尺寸一样大，都是裸指针的2倍
    weak_ptr<int>pw;
    int *p;
    cout << sizeof(p) << endl; // 8
    cout << sizeof(pw) << endl; // 16

    return 0;
}
