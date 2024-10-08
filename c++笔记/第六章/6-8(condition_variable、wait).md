* （1）条件变量std::condition_variable、wait()、notify_one()
* （2）上述代码深入思考
* （3）notify_all()

在C++中，`std::condition_variable` 是一个用于线程间同步的工具，通常与 `std::mutex` 一起使用。条件变量允许一个或多个线程等待某个条件成立，然后由其他线程通知这些等待的线程继续执行。本文将详细介绍 `std::condition_variable` 的用法，包括 `wait` 和 `notify_one` 函数。

### 1. `std::condition_variable` 基本概念

`std::condition_variable` 提供了以下主要功能：

- **`wait`**：使当前线程进入等待状态，直到被通知或超时。
- **`notify_one`**：唤醒一个正在等待的线程。
- **`notify_all`**：唤醒所有正在等待的线程。

### 2. 基本用法

#### 2.1 包含头文件

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
```

#### 2.2 定义互斥量和条件变量

```cpp
std::mutex mtx;
std::condition_variable cv;
bool ready = false;
```

#### 2.3 生产者线程

生产者线程设置条件并通知消费者线程。

```cpp
void producer() {
    std::this_thread::sleep_for(std::chrono::seconds(1));  // 模拟耗时操作
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();  // 通知一个等待的线程
}
```

#### 2.4 消费者线程

消费者线程等待条件成立后继续执行。

```cpp
void consumer() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return ready; });  // 等待条件成立
    std::cout << "Consumer: Ready is true" << std::endl;
}
```

#### 2.5 主函数

创建生产者和消费者线程，并等待它们完成。

```cpp
int main() {
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();

    return 0;
}
```

### 3. 详细说明

#### 3.1 `wait`

`wait` 函数使当前线程进入等待状态，直到被通知或超时。`wait` 需要一个 `std::unique_lock` 对象作为参数，该锁必须在调用 `wait` 时持有互斥量。

```cpp
cv.wait(lock, [] { return ready; });
```

在这个例子中，`cv.wait(lock, [] { return ready; })` 会一直等待，直到 `ready` 变为 `true`。`lock` 是一个 `std::unique_lock` 对象，它在调用 `wait` 时会自动释放互斥量，在 `wait` 返回时重新获取互斥量。

#### 3.2 `notify_one`

`notify_one` 函数唤醒一个正在等待的线程。如果有多个线程在等待，选择哪个线程被唤醒是不确定的。

```cpp
cv.notify_one();
```

#### 3.3 `notify_all`

`notify_all` 函数唤醒所有正在等待的线程。所有被唤醒的线程都会竞争互斥量，只有获取到互斥量的线程才能继续执行。

```cpp
cv.notify_all();
```

### 4. 完整示例

以下是一个完整的示例，展示了如何使用 `std::condition_variable` 实现生产者-消费者模式。

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> data_queue;
bool done = false;

void producer() {
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 模拟耗时操作
        {
            std::lock_guard<std::mutex> lock(mtx);
            data_queue.push(i);
        }
        cv.notify_one();  // 通知一个等待的线程
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        done = true;
    }
    cv.notify_all();  // 通知所有等待的线程
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !data_queue.empty() || done; });

        if (!data_queue.empty()) {
            int data = data_queue.front();
            data_queue.pop();
            lock.unlock();
            std::cout << "Consumer: Consumed data " << data << std::endl;
        } else if (done) {
            break;
        }
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();

    return 0;
}
```

### 5. 总结

- **`std::condition_variable`**：用于线程间同步的工具。
- **`wait`**：使当前线程进入等待状态，直到被通知或超时。
- **`notify_one`**：唤醒一个正在等待的线程。
- **`notify_all`**：唤醒所有正在等待的线程。

通过合理使用 `std::condition_variable`，可以实现高效的线程间同步，避免忙等待和资源浪费。

# 一：条件变量std::condition_variable、wait()、notify_one()

线程A：等待一个条件满足

线程B：专门往消息队列中扔消息（数据）

std::condition_variable实际上是一个类，是一个和条件相关的一个类，说白了就是等待一个条件达成。

这个类是需要和互斥量来配合工作，用的时候我们要生成这个类的对象；

```c++
class A
{
public:
	std::unique_lock<std::mutex> rtn_unique_lock()
	{
		std::unique_lock<std::mutex> tmpguard(my_mutex1);
		return tmpguard;//从函数返回一个局部的unique_lock对象是可以的。
		//返回这种局部对象tmpguard会导致系统生成临时unique_lock对象，并调用unique_lock的移动构造函数
	}
	//把收到的消息（玩家命令）入到一个队列的线程
	void inMsgRecvQueue()
	{
		for(int i = 0;i<100000;++i)
		{
			cout<<"inMsgRecvQueue()执行，插入一个元素"<<i<<endl;
			
			std::unique_lock<std::mutex> sbguard1(my_mutex1);
		
			
			msgRecvQueue.push_back(i);//假设这个数字i就是我收到的命令，我直接弄到消息队列里边来；
			//假如ouMsgRecvQueue正在处理一个事务，需要一段时间，而不是正卡在wait()那里等待你唤醒，那么此时这个notify_one这个调用也许就没效
			my_cond.notify_one();//我们尝试把wait()的线程唤醒，执行完这行，那么outMsgRecvQueue()里边的wait就会被唤醒
			//唤醒之后的事情后续研究
			
			//my_mutex.unlock();
		}
		return;
	}
	bool out MsgLULProc(int &command)
	{
		if(!msgRecvQueue.empty())
		{
			std::lock_guard<std::mutex> sbguard(my_mutex);//sbguard是随便起的对象名
			if(!msgRecvQueue.empty())
			{
				//消息不为空
				int command = msgRecvQueue.front();	//返回第一个元素，但不检查元素是否存在
				msgRecvQueue.pop_front();	//移除第一个元素
				//my_mutex.unlock();	//一定不能忘记return前的解锁
				return true;
			}
		}
			//my_mutex.unlock();
		return false;
	}
	//把数据从消息队列中取出的线程
	void outMsgRecvQueue()
	{
		int command = 0;
		while(true)
		{
			std::unique_lock<std::mutex> sbguard1(my_mutex1);
			//wait()用来等一个东西
			//如果第二个参数lambda表达式返回值是true，那wait()直接返回；
			//如果第二个参数lambda表达式返回值是false,那么wait()将解锁互斥量，并堵塞到本行，
			//那堵塞到什么时候为止呢？堵塞到其他某个线程调用notify_one()成员函数为止；
			//如果wait()没有第二个参数：my_cond.wait(sbguard1):那么就跟第二个参数lambda表达式返回false效果一样
			//wait()将解锁互斥量，并堵塞到本行，堵塞到其他某个线程调用notify_one()成员函数为止；
			//当其他线程用notify_one()将本wait(原来睡着/堵塞）的状态唤醒后，wait就开始恢复干活了，恢复后wait干什么活？
			//a、wait()不断的尝试重新获取互斥量锁，如果还获取不到，那么流程就卡在wait这里等着获取，如果获取到了锁，那么wait就继续执行
			//b.1、如果wait有第二个参数（lambda)，就判断这个lambda表达式，如果lambda表达式为false,那wait又对互斥量解锁,然后又休眠这里等待下次被notify_one唤醒
			//b.2、如果lambda表达式为true，则wait返回，流程走下来（此时互斥锁被锁着）。
			//b.3、如果wait没有第二个参数，则wait返回，流程走下来。
			my_cond.wait(sbguard1,[this]{	//一个lambda就是一个可调用对象（函数）
				if(!msgRecvQueue.empty())
					return true;
				return false;
			}
			//执行一些动作
			
		}
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
std::condition_variable my_cond;	//生成一个条件对象
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


# 二：notify_all():唤醒所有线程


```c++
/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
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

using namespace std;

class A
{
    public:
        std::unique_lock<std::mutex> retur_unique_lock()
        {
            std::unique_lock<std::mutex>temguard(my_mutex);
            return temguard; // 从函数返回一个局部的unique_lock对象是可以的，第三章14节。移动构造函数
            // 返回着这种局部的对象会导致系统生成临时的unique_lock对象，并调用unique_lock的移动构造函数
        }
        // 收到的命令放到一个队列的线程
        void inMsgRecvQueue()
        {
            for (int i = 0; i < 100; ++i)
            {
                // my_mutex.lock();
                // my_mutex2.lock();
                // std::lock(my_mutex, my_mutex2);
                // std::unique_lock<std::mutex>sbguard(my_mutex, std::try_to_lock);
                // if (sbguard.owns_lock())
                // {
                //     cout << "inMsgRecvQueue执行中...." << i << endl;
                //     msgRecvQueue.push_back(i); // 这个i就是命令
                // } else {
                //     cout << "没有拿到互斥量mutex锁。。。。" << endl;
                // }
                // my_mutex2.unlock();
                // my_mutex.unlock();
                // std::unique_lock<std::mutex>sbguard(my_mutex, std::defer_lock);
                // // 创建了一个没有加锁的mutex
                // // sbguard.lock(); // 注意，这里调用的是unique_lock这个的对象的成员函数lock，而不是mutex的
                // // 这里我们不需要关心这个互斥量的解锁,因为unique_lock的功能就是能实现自动解锁
                // // 当然我们也可以使用它的另一个成员函数unlock来进行解锁
                
                // // sbguard.unlock();
                
                // // 处理一些非共享代码
                // cout << "inMsgRecvQueue执行中...." << i << endl;
                // // sbguard.lock();
                
                // if (sbguard.try_lock())
                // {
                //     msgRecvQueue.push_back(i); // 这个i就是命令
                // } else {
                //     cout <<"没拿到锁" << endl; 
                // }
                cout << "inMsgRecvQueue执行中..." << endl;
                std::unique_lock<std::mutex>sbguard(my_mutex);
                msgRecvQueue.push_back(i);
                // my_cond.notify_one(); // 我们尝试把wait（）的线程唤醒，执行完这行outMsgRecvQueue（）里的wait就会被唤醒
                my_cond.notify_all();
                // 假如outmsgRecvQueue（）正在处理一个事务，需要执行一段时间，而不是正卡在wait等待
                // 、那么此时notify就没有效果
                
            }
            return;
        }
        // bool outMsgNullProc(int &command)
        // {
        //     // 要先lock()，后续才能使用第二个参数std::adopt_lock
        //     // my_mutex.lock();
        //     // std::unique_lock<std::mutex>sbguard(my_mutex, std::adopt_lock);
        //     // std::unique_lock<std::mutex>sbguard(my_mutex);
        //     // std::unique_lock<std::mutex>sbguard = retur_unique_lock();
        //     // std::mutex *pt = sbguard.release(); // 这个指针实际上指的就是解绑的my_mutex
            
            
            
        //     // std::chrono::milliseconds dura(2000); // 
        //     // std::this_thread::sleep_for(dura); // 让这个线程执行到这休息20秒
            
        //     // std::lock_guard<std::mutex>sbguard(my_mutex); // sbguard 自己随便取的变量名
        //     // 这里这个lock_guard需要使用std::mutex作为类模板的类型参数，然后将自己定义的mutex变量作为实参传入
            
        //     //工作原理：lock_guard这个构造函数里执行了mutex::lock()这个函数 
        //     // 然后因为lock_guard是一个局部对象在这里，所以在退出作用域的时候，就执行了lock_guard的析构函数
        //     // 然后析构函数里执行了mutex::unlock()这个函数，这样就达到了取代自己写lock（）和unlock()的作用
            
        //     // my_mutex2.lock();
        //     // my_mutex.lock();
        //     // std::lock(my_mutex, my_mutex2);
            
        //     if (!msgRecvQueue.empty())
        //     {
        //         std::unique_lock<std::mutex> sbguard(my_mutex);
        //         if (!msgRecvQueue.empty())
        //         {
        //             // 消息队列不为空
        //             command = msgRecvQueue.front();
        //             // list 的front方法，返回第一个元素(只是得到元素，并不会改变原list)，（不会检查元素是否存在，所以需要事先判空）
        //             msgRecvQueue.pop_front(); // 移除第一个元素，但是不返回
        //             cout << "消息为： " << command << endl;
        //             // pt->unlock();
        //             // my_mutex.unlock();
        //             // my_mutex2.unlock();
        //             return true;
        //         }
        //     }
        //     // my_mutex2.unlock();
        //     // my_mutex.unlock();
        //     // pt->unlock();
        //     return false;
        // }
        
        // 把命令从队列中取出的线程
        void outMsgRecvQueue()
        {
            // int command = 0;
            // for (int i = 0; i < 100; ++i)
            // {
            //     // if (!msgRecvQueue.empty())
            //     // {
            //     //     my_mutex.lock();
            //     //     // 消息队列不为空
            //     //     int command = msgRecvQueue.front();
            //     //     // list 的front方法，返回第一个元素(只是得到元素，并不会改变原list)，（不会检查元素是否存在，所以需要事先判空）
            //     //     msgRecvQueue.pop_front(); // 移除第一个元素，但是不返回
            //     //     my_mutex.lock();
            //     //     cout << "消息为： " << command << endl;
            //     // }
            //     // else
            //     // {
            //     //     cout << "msgRecvQueue消息队列为空...." << i << endl;
            //     // }
            //     if (outMsgNullProc(command))
            //     {
            //         cout << "取到的值为： " << command << endl;
            //     } else {
            //         cout << "null" << endl;
            //     }
                
            // }
            
            int command = 0;
            while(1)
            {
                std::unique_lock<std::mutex>sbguard(my_mutex);
                // 这行只有unique_lock拿到锁才会走下来
                
                // wait()是用来等待一个东西，
                // 如果wait第二个参数（lambda）的返回值是false，那么wait将解锁互斥量并堵塞到本行；
                // 那么堵塞到什么时候为止呢？堵塞到其他线程函数调用notify_one()成员函数为止
                // 如果wait第二个参数返回true，那么wait就直接返回
                // 如果wait没有第二个参数， my_cond.wait(sbguard）
                // 那么就跟第二个参数返回false效果一样。
                my_cond.wait(sbguard, [this]{ // 一个lambda表达式就相当于一个可调用对象
                    if(!msgRecvQueue.empty())
                        return true;
                    return false;
                });
                
                // 当其他线程用notify_one唤醒本wait之后
                // wait会不断尝试重新获取互斥量锁，如果获取不到，流程会卡住wait这里不断获取
                // 获取到锁后，就继续执行
                // wait获取到锁之后，就等于上了锁
                // 如果wait有第二个参数，就判断这个lambda表达式，如果lambda表达式为false，那么wait又对互斥量解锁
                // 然后进入休眠，等待再次被notify_ONe唤醒
                // 如果表达式返回true，那么wait就返回，然后流程继续往下走，（此时互斥量任然是被锁住的）
                // 如果wait没有第二个参数，那么wait返回，流程继续往下走，（此时互斥量任然是被锁住的）
                command = msgRecvQueue.front();
                msgRecvQueue.pop_front();
                std::cout << "取出一个元素：" << command << std::this_thread::get_id() << std::endl;
                
                sbguard.unlock(); // 因为unique_lock的灵活性，所以我们可以随时的unlock（）解锁
            }
            
            cout << "end!" << endl;
            return;
        }
        
    private:
        std::list<int> msgRecvQueue;
        std::mutex my_mutex; // 创建一个互斥量
        std::mutex my_mutex2; // 创建第二个互斥量，用于演示死锁
        std::condition_variable my_cond; // 生成一个条件变量对象
};

int main()
{
    
    // 假定: 网络服务器，两个线程，一个线程用于从玩家收集命令，并将命令写到队列中，
    // 另一个线程负责从命令队列中取出命令并执行命令动作
    
    // list:在频繁的按顺序传入和删除数据时效率高，vector 对于随机的插入和删除数据效率高
    // 这里准备使用成员函数作为线程入口函数
    
    A mymsgOBJ;
    std::thread myoutmsg(&A::outMsgRecvQueue, &mymsgOBJ); // 第二个参数要使用引用，才能保证我们在线程中使用的是mymsgObj这个同一个对象
    std::thread myoutmsg2(&A::outMsgRecvQueue, &mymsgOBJ);
    std::thread myinmsg(&A::inMsgRecvQueue, &mymsgOBJ);
    std::thread myinmsg2(&A::inMsgRecvQueue, &mymsgOBJ);
    
    myoutmsg.join();
    myinmsg.join();
    
    // 保护共享数据，操作时，某个线程用代码把共享数据锁住，操作数据之后，解锁
    // 互斥量（mutex）
    // 互斥量是一个类对象，理解为一把锁，多个线程尝试用lock()函数来加锁这个锁头
    // 只有一个线程能锁定成功，解锁使用unlock（）
    
    // 互斥量使用要小心，保护数据多了，影响程序执行效率，保护少了，程序运行出现异常
    
    // 互斥量使用
    // #include <mutext>
    // lock() unlock()
    // 先lock(),操作完共享数据之后，再unlock()
    // lock()和unlock()必须成对对称数量的调用
    
    // 为了防止忘记unlock()，c++引入了一个std::lock_guard的类模板
    // 可以自动帮助unlock()
    
    // std::lock_guard类模板 可以直接取到lock和unlock()
    // 使用了lock_guard之后，就不能再使用lock()和unlock()了；
    //工作原理：lock_guard这个构造函数里执行了mutex::lock()这个函数 
    // 然后因为lock_guard是一个局部对象在这里，所以在退出作用域的时候，就执行了lock_guard的析构函数
    // 然后析构函数里执行了mutex::unlock()这个函数，这样就达到了取代自己写lock（）和unlock()的作用
    
    // 所以，其实客观上来说，lock_guard没有lock()和unlock()灵活
    
    // 死锁
    // 比如我有两把锁，（死锁这个问题，是至少由两个互斥量才能产生的）
    // 
    
    // 只要保证两个互斥量的上锁顺序一致就不会死锁
    
    // std::lock()函数模板 （用来处理多个互斥量）
    // 能力： 一次锁住两个或者两个以上的互斥量（至少两个，一个不行）
    // std::lock() 如果一个互斥量没锁住，他就会放开锁住的互斥量，他就会在哪里等待，等下次加锁所有互斥量都锁住才会往下走
    // 所以，她他要么全锁住要么全都不锁住，不会说锁着一个然后拿着不放影响其他人加锁造成死锁
    
    
    // nuique_lock()取代lock_guard()
    // unique_lock()是一个类模板，工作中一般使用lock_guard足够，lock_guard
    // unique_lock比lock_guard灵活很多，但是内存占用多一点，效率上差一点
    
    // unique_lock第二个参数
    // std::lock_guard<std::mutex>sbguard(my_mutex, std::adopt_lock); // adopt_lock 标记作用
    // 使用std::adopt_lock的前提是，你需要事先把互斥量进行lock
    // std::adopt_lock 她无论是lock_guard还是unique_lock()都用在第二个参数
    // 用来标记这个互斥量已经被lock()了（你必须要提前把这个互斥量lock()住，否者会报异常）
    // std::adopt_lock这个标记的效果就是，假设调用方线程已经拥有了互斥量的所有权（已经lock成功了）
    // 他就会通知lock_guard不需要再构造函数中lock（）这个互斥量了
    // unique_lock也可以带这个参数，他表示的含义相同，表示不需要在构造函数中lock这个互斥量了
    
    // std::try_to_lock
    // 我们会尝试去用mutex的lock去锁定这个mutex，但是如果没有锁定成功，会立即返回，并不会阻塞在那里
    // 用这个try_to_lock的前提是没有事先使用lock()去锁死mutex
    
    // std::defer_lock
    // 使用std::defer_lock的前提是你不能自己先lock
    // defer_lock的意思就是并没有给mutex加锁，初始化了一个没有加锁的mutex
    // 借着defer_lock来介绍一些unique_lock的成员函数
    
    // lock()
    // unlock()
    // try_lock() // 尝试给互斥量加锁，如果拿不到锁，那就返回false，如果加锁成功，返回true，这个函数是不阻塞线程的
    // release() // 返回她锁管理的mutex指针，并释放所有权，也就是说，这个unique_lock和mutex不再有关系
    // 要严格区分unlock和release的区别，不要混淆
    // release之后，如果原来的mutex对象处于加锁状态，那么你负责使用完之后对齐进行解锁
    // release返回的是原始的mutex指针
    
    
    // unique_lock所有权的传递
    // std::unique_lock<std::mutex>sbguard(my_mutex);
    // sbguard拥有my_mutex的所有权
    // std::unique_lock<std::mutex>sbguard2(sbguard); // 这条代码是非法的。所有权不可以进行复制操作
    // std::unique_lock<std::mutex>sbguard3(std::move(sbguard)); // 使用移动语义，左值转右值，相当于将my_mutex的所有权从sbguard转移到sbguard3.这是可以的
    // 这样，那sbguard就指向了空，相当于sbguard release()了
    
    
    
    // 条件变量std::condition_variable, wait(),notify_one()
    // 线程A：等待一个条件满足
    // 线程B：专门往消息队列中仍消息
    // std::condition_variable实际上是一个类，是一个和条件相关的类，说明白了就是等待一个条件达成
    // 这个类是需要和互斥量配合工作，用的时候是需要生成这个类的对象
    
    // notify_all()
    
    
    
    
    cout << "main return ... " << endl;
    

    return 0;
}

```
