/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

using namespace std;

class A
{
    public:
        A()
        {
            cout << "构造函数" << endl;
        }
        
        ~A()
        {
            cout << "析构函数" << endl;
        }
    private:
        int m_a;
};

int main()
{
    // new/delete 探秘
    // 
    
    // A *a = new A();
    
    // cout << sizeof(a) << endl;
    
    
    // delete a;
    
    
    A *pa_c = new A[2]();
    
    delete pa_c; // 正确写法 delete[] pa_c;
    
    // 构造函数

    // 构造函数
    
    // 析构函数
    
    // *** Error in `./a.out': munmap_chunk(): invalid pointer: 0x0000000001861c28 ***




    
    // operator new 和 operator delete
    int *pi = new int;
    delete pi;
    // new 干了两个事情，a）分配内存，通过operator new()来分配内存，b)调用构造函数来初始化内存
    // void *myorgpoint = operator new(100) // 分配100字节，（一般没人这么写）
    // delete 也干了两个事，a)调用析构函数， b) 释放内存，（调用operator delete（）来释放内存）

    // 基本new 如何记录分配内存大小供delete使用？
    int *p = new int(); // 4字节
    delete p; // 删除的时候，编译器怎么知道要回收4字节？new 内部有自己的记录机制，记录了分配出去多少内存
    
    int *pp = new int[2]; // 8字节
    delete[] pp;
    return 0;
}
