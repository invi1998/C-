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
#include <list>
#include <mutex>
#include <condition_variable>
#include <future>

#include<bits/stdc++.h>
// #include <windows.hpp>

using namespace std;

// #define __WINDOWSJQ__

#ifdef __WINDOWSJQ__
// 本类用于自动释放windows临界区，防止忘记释放EnterCriticalSection,功能类似于C++11中的lock_guard
class CWinLock // RAII类，（Resource Acquisition is innitalization）资源获取即值初始化。
// 利用C++的构造函数和析构函数来做一些自动处理，自动创建，自动消失
// 比如容器，智能指针这些类都属于RAII类
{
    private:
       CRITICAL_SECTION *winsecGuard;
    public:
        CWinLock(CRITICAL_SECTION *tmp)
        {
            winsecGuard = tmp;
            EnterCriticalSection(winsecGuard);
        }
        ~CWinLock()
        {
            LeaveCriticakSection(winsecGuard);
        }
};
#endif

class A
{
    private:
        std::list<int> msgRecvQueue;
        // std::mutex mutexA;
        // std::recursive_mutex mutexA;
        std::timed_mutex mutexA;
        
#ifdef __WINDOWSJQ__
        CRITICAL_SECTION my_winsec; // windows中的临界区，非常类似于C++中的互斥量
#endif
        
    public:
        A()
        {
#ifdef __WINDOWSJQ__
            InitializeCriticalSection(&my_winsec); // 使用临界区之前要先初始化
#endif
        }
        void inMsgRecvQuene()
        {
            for(int i = 0; i < 100; ++i)
            {
#ifdef __WINDOWSJQ__
                // EnterCriticalSection(&my_winsec);
                CWinLock winLoc(&my_winsec);
                std::cout << "inMsgRecvQuene...(windows中的临界区)" << std::endl;
                msgRecvQueue.push_back(i);
                // LeaveCriticakSection(&my_winsec);
#else
                // mutexA.lock();
                // std::lock_guard<std::mutex> guard_A(mutexA);
                // std::lock_guard<std::recursive_mutex> guard_A(mutexA);
                // std::lock_guard<std::recursive_mutex> guard_B(mutexA);
                
                // 希望线程执行到这里，等待100ms，如果100ms内没拿到锁，就继续往下走
                std::chrono::microseconds timeout(100);
                // if (mutexA.try_lock_for(timeout)) // 等待100ms尝试获取锁
                if (mutexA.try_lock_until(std::chrono::steady_clock::now() + timeout))
                // 尝试拿锁直到当前时间点后的100ms时间点
                {
                    // 拿到锁了
                    std::cout << "inMsgRecvQuene..." << std::endl;
                    msgRecvQueue.push_back(i);
                    mutexA.unlock();
                }
                else
                {
                    // 没拿到锁
                    std::cout << "GG，没拿到锁" << std::endl;
                    std::chrono::microseconds timeSleep(100);
                    std::this_thread::sleep_for(timeSleep);
                }
                // mutexA.unlock();
                #endif
            }
            return;
        }
        
        bool outMsgProc(int &command)
        {
#ifdef __WINDOWSJQ__
            EnterCriticalSection(&my_winsec);
            if (!msgRecvQueue.empty())
            {
                command = msgRecvQueue.front();
                msgRecvQueue.pop_front();
                LeaveCriticakSection(&my_winsec);
                return true;
            }
            LeaveCriticakSection(&my_winsec);
            return false;
#else
            mutexA.lock();
            std::chrono::microseconds timeSleep(3000);
            std::this_thread::sleep_for(timeSleep);
            if (!msgRecvQueue.empty())
            {
                command = msgRecvQueue.front();
                msgRecvQueue.pop_front();
                mutexA.unlock();
                return true;
            }
            mutexA.unlock();
            return false;
#endif
        }
        
        void outMsgRecvQuene()
        {
            int command = 0;
            for (int i = 0; i < 100; ++i)
            {
                bool result = outMsgProc(command);
                if (result)
                {
                    std::cout << "取出元素值：" << command << std::endl;
                }
                else
                {
                    std::cout << "消息队列为空" << std::endl;
                }
            }
            std::cout << "endl" << std::endl;
            return;
        }
};

int main()
{
    
   A objA;
   std::thread threadAin(&A::inMsgRecvQuene, &objA);
   std::thread threadAout(&A::outMsgRecvQuene, &objA);
   
   threadAin.join();
   threadAout.join();
   
//   windows临界区

// 自动析构技术
// std::lock_guard<std::mutex>

// 递归的独占互斥量 recursive_mutex
// std::mutex 独占式的互斥锁，自己拿到锁时，其他人lock不了
// recursive_mutex 递归的独占互斥量，可以允许同一个线程同一个互斥量被多次lock()
// recursive_mutex也有lock()和unlock()

// 带超时互斥量 std::timed_mutex   std::recursive_timed_mutex
// std::timed_mutex 带超时功能的独占互斥量，
// std::recursive_timed_mutex 带超时功能的递归式独占互斥量

// std::timed_mutex 提供了两个新接口
// try_Lock_for()  参数是一个时间段（一段时间） 等待一段时间，如果拿到了锁，或者等待超时没拿到锁，就都会走下去
// try_lock_until() 参数是一个时间点，他会一直等到这个时间点，一直尝试拿锁拿到了继续玩下走，没拿到会一直拿到这个点，入户继续往下走，不再卡住




    return 0;
}
