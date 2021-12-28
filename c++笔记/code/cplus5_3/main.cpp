/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

using namespace std;

shared_ptr<int> myfunc (shared_ptr<int> &ptr)
// void myfunc (shared_ptr<int> &ptr) // 如果是使用引用作为参数，则智能指针的引用计数不会增加
{
    cout << *ptr << endl;
    return ptr;
}

void mm(shared_ptr<int> s)
{
    cout << s.use_count() << endl;
    return;
}

void myshareddelte(int *p) // 我们自定义的shared_ptr删除器，用于删除int型的对象指针，
// 当对象引用计数为0，就会自动调用该删除器函数来删除该对象
{
    cout << "删除器打印日志" << endl;
    delete p;
}

class A
{
    public:
        A()
        {
            
        }
        ~A(){}
};

// 实现一个函数模板来封装shared_ptr数组
template<typename T>
shared_ptr<T> make_shared_array(size_t size)
{
    return shared_ptr<T>(new T[size], std::default_delete<T[]>());
}

int main()
{
    // shared_ptr引用计数的增加和减少
    // auto p0 = make_shared<int>(1000); // 目前p0所指向的对象只有一个引用着
    // auto p1(p0); // 智能指针定义时初始化，p0和p1都指向了相同的对象，此对象目前有两个引用者
    
    // // 在如下情况下，所有指向这个对象的shared_ptr都会加1
    // // 1、像上面这样，使用一个shared_ptr来初始化另一个shared_ptr
    // // 2、把智能指针单做实参往函数里传递
    // myfunc(p1); // 此时 p0 ,p1的智能指针计数都是3
    
    // // 3、作为函数的返回值
    // auto p2 = myfunc(p1);
    // cout << p0.use_count() << endl; // 3
    // cout << p1.use_count() << endl; // 3
    // cout << p2.use_count() << endl; // 3
    
    // // 引用计数减少
    // 1、给shared_ptr赋予新值
    // p0 = make_shared<int>(200);
    // cout << p0.use_count() << endl; // 1
    // p1 = make_shared<int>(400); 
    // cout << p1.use_count() << endl; // 1
    // p2 = make_shared<int>(900);
    // cout << p2.use_count() << endl; // 1
    // mm(p2);
    // cout << p2.use_count() << endl; // 1
    
    // 2、局部的shared_ptr离开其作用域
    // auto p3 = make_shared<int>(9000);
    // auto p4(p3); // 引用计数2
    // cout << p4.use_count() << endl;
    // mm(p4);
    // // 2-> 3 -> 2 在外面，2个计数，通过值传递，引用计数加1，离开局部作用域范围，引用计数减一

    // // 3、当一个shared_ptr引用计数从1变为0，则他会自动释放自己所管理的对象内存
    // auto p5 = make_shared<int>(1000);
    // auto p6 = make_shared<int>(1000);
    // p5 = p6; // 给p5赋值会让P5指向p6所指的对象， 该对象引用计数变为2，则原来的p5所指向的对象的引用计数会变为从1变为0
    // cout << p5.use_count() << *p5 << endl;
    // cout << p6.use_count() << *p6 << endl;
    
    // // unique()；判断一个智能指针是否独占某一个对象，是返回true
    // cout << p5.unique() << endl;
    
    // cout << (0 == false) << endl;
    // reset() :恢复，复位重置
    // a)reset不带参数
    // 若pi是唯一指向该对象的指针，那么释放pi所指向的对象， 并将pi置空
    // 若pi不是唯一指向该对象的指针，那么不释放pi指向的对象，但是指向该独享的引用计数会减1，同时pi置空
    // shared_ptr<int> pi(new int(800));
    // // pi.reset();
    // // cout << pi.use_count() << endl; // 0
    // // cout << pi.unique() << endl; // false 0
    // auto pii(pi);
    // pi.reset();
    // cout << pi.use_count() << endl; // 0
    // cout << pi.unique() << endl; // false 0
    // cout << pii.use_count() << endl; // 1
    // cout << pii.unique() << endl; // true 1
    
    // b)reset带参数 一般这个参数是new出来的指针
    // 若pi是唯一指向该对象的指针，那么释放pi所指向的对象， 并将pi指向新对象
    // 若pi不是唯一指向该对象的指针，那么不释放pi指向的对象，但是指向该独享的引用计数会减1，同时pi指向新对象
    // shared_ptr<int>pi (new int(300));
    // // pi.reset(new int(7090));
    // // cout << pi.use_count() << endl; // 1
    // // cout << pi.unique() << endl; // true 1
    // // cout << *pi << endl; // 7090
    // auto pii(pi);
    // pi.reset(new int(2020));
    // cout << pi.use_count() << endl; // 1
    // cout << pi.unique() << endl; // true 1
    // cout << *pi << endl; // 7090
    // cout << pii.use_count() << endl; // 1
    // cout << pii.unique() << endl; // true 1
    // cout << *pii << endl; // 300
    
    // // 空指针也可以通过reset来重新初始化
    // shared_ptr<int> np;
    // np.reset(new int(9000));
    
    // 解引用 * 获取p指向的对象
    // shared_ptr<int> p(new int(2000));
    // cout << *p << endl; // 2000
    
    // get()  考虑到有些函数的参数需要的是一个内置的裸指针，而不能是智能指针
    // 返回p中保存的裸指针，小心使用，如果智能指针释放了所指向的对象，那么返回的裸指针也变得无效了
    // shared_ptr<int> pluo(new int(3400));
    // int *io = pluo.get();
    // cout << *io << endl; // 3400
    // *io = 45;
    // cout << *pluo << endl; // 45
    
    // swap() 交换两个智能指针所指向的对象
    // shared_ptr<string> p1(new string{"ptr"});
    // shared_ptr<string> p2(new string{"shared_"});
    
    // cout << *p1 << *p2 << endl; // ptrshared_
    // std::swap(p1, p2);
    // // 或者
    // // p1.swap(p2);
    // cout << *p1 << *p2 << endl; // shared_ptr
    
    // =nullptr
    // 将所指向的对象引用计数减1，若引用计数为0了，则将智能指针指向的对象释放。然后将智能指针设置为空
    // shared_ptr<string> p1(new string{"ptr"});
    // p1 = nullptr;
    
    // 智能指针名字作为判断条件
    
    // 指定删除器及其数组问题
    // 指定删除器
    // 1.定时机制帮助我们删除所指向的对象 delete 默认使用delete运算符作为默认的资源西沟方式
    // 我们可以指定自己的删除器取代系统的默认删除器
    // shared_ptr指定删除器的方式比较简单，一般只需要在测试中添加具体的删除器函数名即可
    // shared_ptr<int> pp(new int (122321), myshareddelte);
    // 删除器还可以是一个lambda表达式
    // shared_ptr<int> pp(new int (122321), [](int *p) {
    //     cout <<"调用lambda表达式删除器" << endl;
    //     delete p;
    // });
    // shared_ptr<int> pp2(pp); // 引用计数 2
    
    // pp2.reset(); // 剩下一个引用计数，pp2为nullptr
    // pp.reset(); // 调用完这行就会调用我们自定义的删除器
    
    // 有些情况下使用默认删除器处理不了（比如使用shared_ptr管理动态数组），这时需要我们提供自定义的删除器
    // shared_ptr<int> pparray(new int[10], [](int *p) {
    //     cout << "调用delete[]自定义删除器" << endl;
    //     delete[] p;
    // })
    
    // shared_ptr<A> pa(new A[10]);
    // *** Error in `./a.out': munmap_chunk(): invalid pointer: 0x0000000000ce9c28 *** 报异常
    // shared_ptr<A> pa(new A[10], [](A *p) {
    //     cout << "调用delete[]自定义删除器" << endl;
    //     delete[] p;
    // }); 
    // 调用delete[]自定义删除器
    
    // 使用default_delete来做删除器，default_delete是标准模板库里的模板类，她内部也是使用delete实现的，也可以实现数组删除
    // shared_ptr<A> pa(new A[10], std::default_delete<A[]>());
    
    // 定义数组的的时候，在尖括号中加中括号<[]>，就可以避免自己写删除器来进行数组对象的删除
    // shared_ptr<A[]> Pa(new A[10]);
    
    // 使用自定义的make_shared_array函数模板来生层一个智能指针对象数组
    shared_ptr<int> pintArray = make_shared_array<int>(5);
    shared_ptr<A> pintA = make_shared_array<A>(15);
    
    // 指定删除器额外说明， 就算是两个shared_ptr指定了不同的删除器，只要他们所指向的对象类型相同，那么这两个shared_ptr也属于同一个类型
    auto lambda1 = [](int *p){
        cout << "拉姆达表达式1执行了" << endl;
        delete p;
    };
    auto lambda2 = [](int *p){
        cout << "拉姆达表示2执行了" << endl;
        delete p;
    };
    
    shared_ptr<int> p1(new int[2], lambda1);
    shared_ptr<int> p2(new int[3], lambda2);
    // p2 = p1; // 这行代码会导致p2先调用lambda,把自己所指向的对象进行释放，然后指向p1所指向的对象，p1所指向的对象，引用计数加1 = 2
    // // 整个main执行王弼后，还会调用lambda1来释放p1, p2共同指向的对象
    // // 拉姆达表示2执行了
    // // 拉姆达表达式1执行了
    
    // 类型相同，就代表他们可以放到元素类型相同的容器里来
    vector<shared_ptr<int>> pvce{p1, p2};
    
    // make_shared是提倡的生成shared——ptr的方式，但是make_shared不支持我们自定义删除器



    return 0;
}
