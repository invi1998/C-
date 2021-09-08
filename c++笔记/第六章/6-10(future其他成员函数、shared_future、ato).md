* （1）std::future的其他成员函数
* （2）std::shared_future
* （3）原子操作std::atomic
* （3.1）原子操作概念引出范例
* （3.2）基本的std:: atomic用法范例

# 一：std::future的其他成员函数

```C++
int mythread()
{
	cout<<"mythread() start"<<"threadid="<<std::this_thread::get_id()<<endl;//新的线程id
	std::chrono::milliseconds dura(5000);//1秒=1000毫秒，所以5000毫秒=5秒
	std::this_thread::sleep_for(dura);	//休息一定的时长
	cout<<"mythread() end"<<"threadid ="<<std::this_thread::get_id()<<endl;
	return 5;
}
int main()
{
	cout<<"main"<<"threadid="<<std::this_thread::get_id()<<endl;
	std::future<int> result = std::async(std::launch::deferred,mythread);	//流程并不会卡在这里
	cout<<"continue....!"<<endl;
	//cout<<result.get()<<endl;	//卡在这里等待线程执行完，
							//但是这种get因为一些内部特殊操作，不能get多次，只能get一次
	//枚举类型：
	std::future_status status = result.wait_for(std::chrono::seconds(1));//等待1秒
	if(status == std::future_status::timeout)//超时，我想等待你1秒钟，希望你返回，你没返回，那么status = timeout
	{
		//表示线程还没执行完；
		cout<<"超时，线程还没有执行完毕"<<endl;
	}
	else if(status == std::future_status::ready)
	{
		//表示线程成功返回
		cout<<"线程成功执行完毕，返回"<<endl;
		cout<<result.get()<<endl;
	}
	else if(status == std::future_status::deferred)//延迟
	{
		//如果async的第一个参数被设置为std::launch::deferred,则本条件成立
		cout<<"线程被延迟执行"<<endl;
		cout<<result.get()<<endl;
	}
	cout<<"I Love China!"<<endl;
	return 0;
}
```

# 二：std::shared_future

也是个类模板，get()函数复制数据


```c++
int mythread(int mypar)
{
	cout<<"mythread() start"<<"threadid ="<<std::this_thread::get_id()<<endl;
	std::chrono::milliseconds dura(5000);//1秒=1000毫秒，所以5000毫秒=5秒
	std::this_thread::sleep_for(dura);//休息一定的时长
	return 5;
}
void mythread2(std::future<int> &tmpf)//注意参数
{
	cout<<"mythread2() start"<<"threadid="<<std::this_thread::get_id()<<endl;
	auto result = tmpf.get();//获取值，只能get一次否则会报异常
				//为什么第二次get这个future我们会得到一个异常；主要是因为get函数的设计是一个移动语义
	cout<<"mythread2 result="<<result<<endl;
	return;
}
int main()
{
	cout<<"main"<<"threadid="<<std::this_thread::get_id()<<endl;
	std::packaged_task<int(int)> mypt(mythread);//我们把函数mythread通过packaged_task包装起来；
	std::thread t1(std::ref(mypt),1);	//线程直接开始执行，第二个参数作为线程入口函数的参数；
	t1.join();//我们可以调用这个等待线程执行完毕，不调用这个不太行，会崩溃
	//std::future<int>result = mypt.get_future();//std::future大家不陌生了，这个对象里含有线程入口函数

	//bool ifcanget = result.valid();
	//std::shared_future<int> result_s(result.share());//执行完毕后result_s里有值，而result里空了
	//ifcanget = result.valid();
	std::shared_future<int> result_s(mypt.get_future());//通过get_future返回值直接构造了一个shared_future对象。
	auto mythreadresult = result_s.get();
	mythreadresult = result_s.get();

	std::thread t2(mythread2,std::ref(result));
	t2.join();	//等线程执行完毕
	cout<<"I Love China!"<<endl;
	return 0;
}
```

# 三：原子操作std::atomic

3.1、原子操作概念引出范例
互斥量：多线程编程中保护共享数据：锁，操作共享数据，开锁
有两个线程，对一个变量进行操作，这个线程读该变量值，另一个线程往这个变量中写值。
int atomvalue = 5;
//读线程
int tmpvalue = atomvalue; //这里这个atomvalue代表的是多个线程之间要共享的变量；
//写线程B
atomvalue = 6; //汇编代码的话；
大家可以把原子操作理解成一种：不需要用到互斥量加锁（无锁）技术的多线程并发编程方式
原子操作：是在多线程中不会被打断的程序执行片段；原子操作，比互斥量效率上更胜一筹。
互斥量的加锁一般是针对一个代码段（几行代码），而原子操作针对的一般都是一个变量，而不是一个代码段；
原子操作，一般都是指“不可分割的操作”；也就是说这种操作状态要么是完成的，要么是没完成的，不可能出现半完成状态；
std::atomic来代表原子操作，std::atomic是个类模板。其实std::atomic这个东西是用来封装某个类型的值的；

3.2、一般用于计数或者统计(累计发送出去了多少个数据包，累计接收到了多少个数据包；）


```c++
//int g_mycout = 0;//定义一个全局量
//std::mutex g_my_mutex;	//互斥量
std::atomic<int> g_mycout = 0;//我们封装了一个类型为int的对象（值);我们可以像操作一个int类型变量一样来操作这个g_mycout

void mythread()	//线程入口函数
{
	for(int i = 0;i<100000;i++)
	{
		//g_my_mutex.lock();
		g_mycout++;
		//g_my_mutex.unlock();
	}
	return;
}
void main()
{
	thread mytobj1(mythread);
	thread mytobj2(mythread);
	mytobj1.join();
	mytobj2.join();
	cout<<"两个线程执行完毕，最终的g_mycount的结果是："<<g_mycout<<endl;

}
```

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
        g_atomicValue ++;
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
    
    std::thread obj1(thread_write);
    std::thread obj2(thread_write);
    obj1.join();
    obj2.join();
    
    std::cout << "两个线程执行完毕，按照常理，全局量g_atomicValue值应该为200000，但是实际值为：" << g_atomicValue << std::endl;
    // 两个线程执行完毕，按照常理，全局量g_atomicValue值应该为200000，但是实�值为：139708
    
    std::thread obj3(thread_bool);
    std::thread obj4(thread_bool);
    std::chrono::milliseconds dura(5000);
    std::this_thread::sleep_for(dura);
    g_ifend = true; // 对原子对象写操作，让线程自行结束
    obj3.join();
    obj4.join();


    return 0;
}


```