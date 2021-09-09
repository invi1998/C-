* （1）windows临界区
* （2）多次进入临界区试验
* （3）自动析构技术
* （4）recursive_mutex递归的独占互斥量
* （5）带超时的互斥量std::timed_mutex和std::recursive_timed_mutex

# 一：windows临界区

# 二：多次进入临界区试验

在“同一个线程”（不同线程就会卡住等待）中，windows中的"相同临界区变量“代表的临界区的进入(EnterCriticalSection)可以被调用多次
但是你调用了几次EnterCriticalSection，你就得调用几次LeaveCriticalSection(&my_winsec);

```c++
class A
{
public:
	//把收到的消息（玩家命令）入到一个队列的线程
	void inMsgRecvQueue()
	{
		for(int i = 0;i<100000;++i)
		{
			cout<<"inMsgRecvQueue()执行，插入一个元素"<<i<<endl;
#ifdef_WINDOWSJQ_
	EnterCriticalSection(&my_winsec);	//进入临界区（加锁）
	EnterCrticalSection(&my_winsec);
	msgRecvQueue.push_back(i);
	LeaveCriticalSection(&my_winsec);	//离开临界区（解锁）
	LeaveCriticalSection(&my_winsec);
#endif
			//my_mutex2.lock();		//死锁,这边先2后1，下边先1后2
			//my_mutex1.lock();	//实际工程中这两个锁头不一定挨着，们需要保护不同的数据共享块；
			std::lock(my_mutex1,my_mutex2);	//相当于每个互斥量都调用了.lock();
			std::lock_guard<std::mutex> sbguard(my_mutex1,std::adopt_lock);	//这样能省略自己unlock的步骤
			std::lock_guard<std::mutex> sbguard(my_mutex2,std::adopt_lock);
			std::lock_guard<std::resursive_mutex> sbguard(my_mutex);
			std::chrono::milliseconds timeout(100);	//100毫秒
			//if(my_mutex.try_lock_for(timeout))	//等待100毫秒来尝试获取锁
			if(my_mutex.try_lock_unitl(chrono::steady_clock::now()+timeout))
			{
				//在这100毫秒之内拿到了锁
			}
			else
			{
				//这次没拿到锁头
				std::chrono::microseconds sleeptime(100);
				std::this_thread::sleep_for(sleeptime);
			}
			msgRecvQueue.push_back(i);//假设这个数字i就是我收到的命令，我直接弄到消息队列里边来；
			//my_mutex.unlock();
		}
		return;
	}
	bool out MsgLULProc(int &command)
	{
		std::lock_guard<std::mutex> sbguard(my_mutex);//sbguard是随便起的对象名
		//lock_guard构造函数里执行了mutex::lock(),析构函数里执行了mutex::unlock();
		//my_mutex1.lock();		//死锁
		//my_mutex2.lock();
		//std::lock(my_mutex1,my_mutex2);	//相当于每个互斥量都调用了.lock();
		if(!msgRecvQueue.empty())
		{
			//消息不为空
			int command = msgRecvQueue.front();	//返回第一个元素，但不检查元素是否存在
			msgRecvQueue.pop_front();	//移除第一个元素
			//my_mutex.unlock();	//一定不能忘记return前的解锁
			return true;
		}
		//my_mutex.unlock();
		return false;
	}
	//把数据从消息队列中取出的线程
	void outMsgRecvQueue()
	{
		int command = 0;
		for(int i = 0;i<10000;i++)
		{
			bool result = outMsgLULProc(command);
			if(result == true)
			{
				cout<<"out执行了，取出一个元素"<<command<<endl;
			}
			else
			{
				cout<<"消息队列为空"<<i<<endl;
			}
			/*if(!msgRecvQueue.empty())
			{
				//消息不为空
				int command = msgRecvQueue.front();//返回第一个元素，但不检查元素是否存在；
				msgRecvQueue.pop_front();	//移除第一个元素，但不返回；
				//这里就考虑处理数据....
				//....
			}
			else
			{
				//消息队列为空
				cout<<"消息队列为空"<<i<<endl;
			}*/
		}
		cout<<"end"<<endl;
	}
private:
std::list<int> msgRecvQueue;//容器，专门用于代表玩家给咱们发送过来的命令。
std::mutex my_mutex;	//创建了一个互斥量
std::recursive_mutex my_mutex;	//递归式独占互斥量
std::timed_mutex my_mutex;	//带超时功能的独占互斥量；
};

int main()
{
	A myobja;
	std::thread myOutnMsgObj(&A::outMsgRecvQueue, &myobja);//第二个参数是引用才能保证用的是同一个对象
	std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);//
	myOutnMsgObj.join();
	myInMsgObj.join();
}
```

# 三：自动释锁技术

//本类用于自动释放windows下的临界区，防止忘记LeaveCriticalSection导致死锁情况的发生，类似于C++11中的lock_guard

```c++
classCWinLock{		//叫RAII类（Resource Acquisition is initialization)中文”资源获取即初始化“;
public:
	CWinLock(CRITICAL_SECTION *pCritmp)	//构造函数
	{
		m_pCritical = pCritmp;
		EnterCriticalSection(&m_pCritical);
	}
	~CWinLock()	//析构函数
	{
		LeaveCriticalSection(m_pCritical);
	}

}
```

# 四：recursive_mutex递归的独占互斥量

std::mutex:独占互斥量，自己lock时别人lock不了；
recursive_mutex:递归的独占互斥量：允许同一个线程，同一个互斥量多次被.lock()
recursive_mutex也有lock()，也有unlock()；
考虑代码是否有优化空间。
递归次数据说有限制，递归太多次可能报异常。

# 五：带超时的互斥量std::timed_mutex和std::recursive_timed_mutex

std::timed_mutex：是带超时功能的独占互斥量；
try_lock_for()： 参数是一段时间，是等待一段时间。如果我拿到了锁，或者或者等待超过时间没拿到锁，就走下来；
try_lock_unit()：参数是一个未来的时间点，在这个未来的时间没到的时间内
如果时间到了没拿到锁，程序流程也走下来；

std::recursive_timed_mutex:带超时功能的递归独占互斥量（允许同一个线程多次获取这个互斥量）


```c++
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

```