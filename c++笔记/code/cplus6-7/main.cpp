/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <list>

using namespace std;

std::mutex resource_mutex;
std::once_flag cas_flag;

class MyCAS
{
    static void CreateInstance() // 只被调用一次的函数
    {
        // 设置一个睡眠时间
        std::chrono::milliseconds sleepTime(2000);
        std::this_thread::sleep_for(sleepTime);
        
        std::cout << "隋明" << std::endl;
        
        m_instance = new MyCAS();
        static CGRealese cg;
    }
    
    private:
        // 单例模式的核心就是私有化构造函数
        MyCAS()
        {
            
        }
    private:
        static MyCAS *m_instance; // 申明一个静态成员变量
    
    public:
        static MyCAS *GetInstance()
        {
            // if (m_instance == NULL) // 提高效率
            // {
            //     std::unique_lock<std::mutex> CGmutex(resource_mutex);
            //     if (m_instance == NULL)
            //     {
            //         m_instance = new MyCAS();
            //         static CGRealese cg;
            //     }
            // }
            std::call_once(cas_flag, CreateInstance);
            // 假设两个线程同时执行到这里，其中一个线程要等待另一个线程执行完才能继续执行，但是他看到第一个线程执行完设置的已经执行完设置的标记，
            // 那么他就会跳过执行
            std::cout << "call_once执行完毕（。。。）" << std::endl;
            return m_instance;
        }
        
        class CGRealese
        {
            // 类中套一个回收类，用来释放对象
            public:
                ~CGRealese()
                {
                    if (MyCAS::m_instance)
                    {
                        delete MyCAS::m_instance;
                        MyCAS::m_instance = nullptr;
                        std::cout << "析构执行" << std::endl;
                    }
                }
        };
        
        void testFunc()
        {
            std::cout << "测试" << std::endl;
        }
};

// 类静态成员变量初始化
MyCAS *MyCAS::m_instance = NULL;

void mythred()
{
    // 线程入口函数
    std::cout << "线程入口函数执行...." << std::endl;
    MyCAS *mythreadCAS = MyCAS::GetInstance(); // 这里可能就会有问题
}

int main()
{
    // 单例设计模式
    // MyCAS *p_a = MyCAS::GetInstance(); // 返回该类（MyCAS的对象指针）
    // MyCAS *p_b = MyCAS::GetInstance();
    
    // std::cout << p_b << std::endl;
    // std::cout << p_a << std::endl;
    // 0x25bac20
    // 0x25bac20
    // 发现返回的对象的指针完全一样，说明指向的是同一个对象实例
    
    // p_a->testFunc();
    // MyCAS::GetInstance()->testFunc();
    
    // 单例设计模式共享数据问题分析，解决
    // 需要在自己创建的线程中来创建或者使用单例类
    
    std::thread myobj1(mythred);
    std::thread myobj2(mythred);
    
    myobj1.join();
    myobj2.join();
    
    // std::call_once() c++11引入的函数模板， 该函数的第二个参数是一个函数名a()
    // call_once()功能是保证函数a()只被调用一次
    // 他具备互斥量的能力。效率上，比互斥量消耗的资源更少
    // call_once()需要与一个标记结合使用，这个标记std::once_flag; once_flag是一个解构
    // call_once()就是通过这个标记来决定对应的函数a（）是否被执行，调用call_once()成功之后，call_once()就会把这个标记设置为一种状态
    // 后续再次调用call_once的时候，只要once_flag被设置为了"已调用”状态，那么这个a()函数就不会再被调用执行了

    return 0;
}

