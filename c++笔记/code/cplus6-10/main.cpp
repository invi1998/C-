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

using namespace std;

int mythread ()
{
    cout << "线程开始执行" << std::this_thread::get_id() << endl;
    std::chrono::milliseconds dura(5000);
    std::this_thread::sleep_for(dura); // 休息5秒
    cout << "线程执行结束" << std::this_thread::get_id() << endl;
    return 5;
}

int mythread1 (int mypar)
{
	cout<<"mythread() start"<<"threadid ="<<std::this_thread::get_id()<<endl;
	std::chrono::milliseconds dura(5000);//1秒=1000毫秒，所以5000毫秒=5秒
	std::this_thread::sleep_for(dura);//休息一定的时长
	return 5;
}

void mythread2 (std::future<int> &tmpf)//注意参数
{
	cout<<"mythread2() start"<<"threadid="<<std::this_thread::get_id()<<endl;
	auto result = tmpf.get();//获取值，只能get一次否则会报异常
				//为什么第二次get这个future我们会得到一个异常；主要是因为get函数的设计是一个移动语义
				// 他会做的事就是将future里的对象移动出来，第二次调用的时候，这里的对象就已经被移动走了，为空了，就不能再调用一次移动（get()）
	cout<<"mythread2 result="<<result<<endl;
	return;
}

void mythread3 (std::shared_future<int> &tmpf)//注意参数
{
	cout<<"mythread2() start"<<"threadid="<<std::this_thread::get_id()<<endl;
	auto result = tmpf.get();//获取值，只能get一次否则会报异常
				//为什么第二次get这个future我们会得到一个异常；主要是因为get函数的设计是一个移动语义
				// 他会做的事就是将future里的对象移动出来，第二次调用的时候，这里的对象就已经被移动走了，为空了，就不能再调用一次移动（get()）
	cout<<"mythread3 result="<<result<<endl;
	return;
}

// int g_atomicValue = 0; // 定义一个全局量
std::atomic<int> g_atomicValue = 0; // 定义一个原子全局量，这里封装了一个类型为int的对象，我们可以像操作int一样来操作这个对象

void thread_write()
{
    for (int i = 0; i < 100000; i++)
    {
        g_atomicValue ++; // 符合预期
        // g_atomicValue += 1; // 符合预期
        // g_atomicValue = g_atomicValue + 1; // 使用原子操作时，发现结果和预期不符
    }
    return;
}

std::atomic<bool> g_ifend = false; // 线程退出标记

void thread_bool()
{
    std::chrono::seconds dura(1);
    while(g_ifend == false)
    {
        // 系统没要求线程退出
        std::cout << "id" << std::this_thread::get_id() <<std::endl;
        std::this_thread::sleep_for(dura);
    }
    std::cout << std::this_thread::get_id() << "运行结束" << std::endl;
}

int main()
{
    
    std::cout << "main 线程执行" << std::this_thread::get_id() << std::endl;
    // // std::future<int> futureNUM = std::async(mythread); // 创建一个线程，并开始执行，将这个future对象和这个线程绑定到一起
    // std::future<int> futureNUM = std::async(std::launch::deferred, mythread);
    // std::cout << "continue1..." << std::endl;
    // cout << futureNUM.get() << endl; // 卡在这里等待mythread执行完毕，拿到结果后才会往下走
    // // futureNUM.wait(); // 等待线程执行结束，本身并不返回线程结果
    // cout << "打印" << endl;
    
    // future_status是一个枚举类型
    // wait_for（等待一个时间）
    // std::future_status status = futureNUM.wait_for(std::chrono::seconds(6)); // 等待线程执行 1 / 6秒钟
    // if (status == std::future_status::timeout)
    // {
    //     // 等待超时，表示线程还没执行完毕
    //     std::cout << "等待超时，线程还没执行完毕" << std::endl;
    // }
    // else if (status == std::future_status::ready)
    // {
    //     // 线程成功返回，
    //     std::cout << "线程成功返回" << std::endl;
    //     cout << futureNUM.get() << endl;
    // }
    // else if (status == std::future_status::deferred)
    // {
    //     // 如果async的第一个参数被设置为std::launch::deferred，那么本条件成立
    //     std::cout << "线程被延迟执行" << std::endl;
    //     cout << futureNUM.get() << endl;
    // }
    
    
    // std::packaged_task<int(int)> mypt(mythread1);
    // std::thread t1(std::ref(mypt), 100);
    // t1.join();
    // // std::future<int> result = mypt.get_future();
    // // std::cout << result.valid() << std::endl; // 1
    // // std::shared_future 也是一个类模板，对比之前的std::future 的get()移动数据，这里语义上理解get()应该是复制数据
    // // std::shared_future<int> result_shared(std::move(result));
    // // 或者 std::shared_future<int> result_shared(result.share()); 或者
    // std::shared_future<int> result_shared(mypt.get_future()); // 通过get_future返回值直接构造了一个shared_future对象
    // // 执行完毕之后，result_shared里有值了，而原先的result值被移走，变为空
    // // valid 判断future对象里是否有有效值
    // // std::cout << result.valid() << std::endl; // 0
    // std::cout << result_shared.valid() << std::endl; // 1
    // std::cout << result_shared.get() << std::endl;
    // std::cout << result_shared.valid() << std::endl; // 1
    // std::thread t2(mythread3, std::ref(result_shared));
    // t2.join();
    
    
    // 原子操作 atomic
    // 互斥量，多线程编程中保护共享数据，先锁，操作共享数据，解锁
    // 有两个线程，他们只对一个变量进行操作，一个线程读，一个线程写
    // 可以把原子操作理解为一种不需要加锁解锁的（无锁）技术的多线程并发编程方式
    // 或者可以理解为：原子操作时在多线程中不会被打断的程序执行片段
    // 互斥量的加锁解锁针对的一般都是一个代码段，而原子操作针对的一般都是一个变量
    // 一般都是指不可分割的操作，这类操作要么完成，要么失败，没有中间状态
    // C++11中引入了std::atomic来代表原子操作
    // std::atomic是一个类模板，他是用来封装某个类型的值的
    
    // std::thread obj1(thread_write);
    // std::thread obj2(thread_write);
    // obj1.join();
    // obj2.join();
    
    // std::cout << "两个线程执行完毕，按照常理，全局量g_atomicValue值应该为200000，但是实际值为：" << g_atomicValue << std::endl;
    // // 两个线程执行完毕，按照常理，全局量g_atomicValue值应该为200000，但是实�值为：139708
    
    // std::thread obj3(thread_bool);
    // std::thread obj4(thread_bool);
    // std::chrono::milliseconds dura(5000);
    // std::this_thread::sleep_for(dura);
    // g_ifend = true; // 对原子对象写操作，让线程自行结束
    // obj3.join();
    // obj4.join();
    
    
    // std::async参数详谈
    // std::async用来创建一个异步任务
    // std::launch::deferred 延迟调用
    // std::launch::async 强制创建线程
    // std::thread()如果系统资源紧张，那么可能创建线程就会失败，那么执行std::thread程序就会崩溃
    // std::async()我们一帮不叫他创建线程，（解释async能够创建线程），我们一般称呼为创建一个异步任务，
    // std::async和std::thread最明显的区别就是async有时候并不会创建新线程
    // 1）如果使用std::launch::deferred来调用async会怎样？
    // deferred会延迟到future对象调用get()或者wait()的时候才调用，并且不会创建新线程，如果没有调用get或者wait，那么线程不会执行
    // std::future<int> result = std::async(std::launch::deferred, mythread);
    // std::cout << result.get() << std::endl;
    // 2）如果使用std::launch::async来调用async会怎样？
    // std::launch::async强制这个异步任务在新线程上执行，这意味着系统必须给创建出一个新线程来运行mythread()；
    // std::future<int> result = std::async(std::launch::async, mythread);
    // std::cout << result.get() << std::endl;
    // 3）如果使用std::launch::async | std::launch::deferred 位或 async来调用async会怎样？
    // 这个 | 就意味着你调用async的行为可能是 async(创建新线程并立即执行) 也可能是 deferred （没有创建新线程并延迟调用），两者行为取其一
    // std::future<int> result = std::async(std::launch::async | std::launch::deferred, mythread);
    // std::cout << result.get() << std::endl;
    // 4）如果不带额外参数，
    // 那么他会以std::launch::async | std::launch::deferred为默认值进行调用，系统会自行决定是以异步还是同步方式运行
    // std::future<int> result = std::async(mythread);
    // std::cout << result.get() << std::endl;
    
    // 系统如何自行决定同步还是异步执行async?
    // 由于系统资源限制
    // 1）如果使用std::thread创建的线程过多，则可能创建线程失败，系统报告异常，崩溃
    // 2）如果用std::async一般就不会崩溃，报告异常，因为如果系统资源紧张导致无法创建新线程的时候，
    // std::async这种不加参数的调用就不会创建新线程，而是后续谁调用future对象的get()来请求结果，
    // 那这个异步任务就运行在执行这条get()语句的线程中
    // std::async不确定性问题解决
    // 不加额外参数的std::async调用，让系统自行决定 是否创建新线程。
    // 问题焦点在于std::future result = std::async(mythread); //写法
    // 这个异步任务到底有没有被推迟执行，（std::launch::async还是std::launch::deferred)
    // 使用 std::future对象的wait_for函数
    
    std::future<int> futureNUM = std::async(mythread);
    // 想判断这个async有没有立即执行（有没有立即创建线程并执行）
    std::future_status status = futureNUM.wait_for(std::chrono::seconds(0)); // 等待线程执行 1 / 6秒钟
    if (status == std::future_status::timeout)
    {
        // 等待超时，表示线程还没执行完毕
        std::cout << "等待超时，线程还没执行完毕" << std::endl;
    }
    else if (status == std::future_status::ready)
    {
        // 线程成功返回，
        std::cout << "线程成功返回" << std::endl;
        cout << futureNUM.get() << endl;
    }
    else if (status == std::future_status::deferred)
    {
        // 线程被延迟执行，（系统资源紧张，采用std::launch::deferred 策略)
        std::cout << "线程被延迟执行" << std::endl;
        cout << futureNUM.get() << endl;
    }



    return 0;
}

