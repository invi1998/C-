* （1）unique_lock取代lock_guard
* （2）unique_lock的第二个参数
* （2.1）std::adopt_lock
* （2.2）std::try_to_lock
* （2.3）std::defer_lock
* （3）unique_lock的成员函数
* （3.1）lock()
* （3.2）unlock()
* （3.3）try_lock()
* （3.4）release()
* （4）unique_lock所有权的传递

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
			//my_mutex2.lock();		//死锁,这边先2后1，下边先1后2
			//my_mutex1.lock();	//实际工程中这两个锁头不一定挨着，们需要保护不同的数据共享块；
			//std::lock(my_mutex1,my_mutex2);	//相当于每个互斥量都调用了.lock();
			//std::lock_guard<std::mutex> sbguard(my_mutex1,std::adopt_lock);	//这样能省略自己unlock的步骤
			//std::lock_guard<std::mutex> sbguard(my_mutex2,std::adopt_lock);
			//std::unique_lock<std::mutex> sbguard1(my_mutex1, std::try_to_lock);
			//if(sbguard1.owns_lock()
			//{}else[]
			//std::unique_lock<std::mutex> sbguard1(my_mutex1,std::defer_lock);//没有加锁的my_mutex1
			//sbguard1.lock();//用了defer_lock后要自己加锁解锁
			//sbguard1.unlock();
			//if(sbguard1.try_lock() == true)	//表示尝试加锁成功
			//{
			//}
			//else
			//{
				//没拿到锁会干别的事，不会卡住
			//}
			//std::unique_lock<std::mutex> sbguard1(my_mutex1);
			//std::unique_lock<std::mutex> sbguard2(sbguard1);//复制所有权是非法的
			//std::unique_lock<std::mutex> sbguard2(std::move(sbguard1));//移动语义，现在相当于sbguard2和my_mutex1绑定到一起了，现在sbguard1指向空，sbguard2指向my_mutex1
			//std::mutex *ptx = sbguard1.release();	//现在你有责任自己解锁这个my_mutex1;
			//ptx->unlock();	//ptx相当于my_mutex1
			std::unique_lock<std::mutex> sbguard1 = rtn_unique_lock();
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

# 一：unique_lock取代lock_guard

unique_lock是个类模板，工作中，一般lock_guard(推荐使用）；lock_guard取代了mutex的lock()和unlock();
unique_lock比lock_guard灵活很多；效率上差一点，内存占用多一点。

# 二：unique_lock的第二个参数

lock_guard可以带第二个参数：
std::lock_guardstd::mutex sbguard1(my_mutex1,std::adopt_lock); //adopt_lock标记作用；
2.1、std::adopt_lock：表示这个互斥量已经被lock了（你必须要把互斥量提前lock了，否则会报异常）
std::adopt_lock标记的效果就是“假设调用方线程已经拥有了互斥的所有权（已经lock()成功了）；通知lock_guard不需要在构造函数中lock这个互斥量了;
unique_lock也可以带std::adopt_lock标记，含义相同，就是不希望在unique_lock()的构造函数中lock这个mutex。
用这个adopt_lock的前提是，你需要自己先把mutex先lock上；
2.2、std::try_to_lock
我们会尝试用mutex的lock()去锁定这个mutex，但如果没有锁定成功，我也会立即返回，并不会阻塞在那里；
用这个try_to_lock的前提是你自己不能先去lock。
2.3、std::defer_lock
用这个defer_lock的前提是你不能自己先lock，否则会报异常。
defer_lock的意思就是并没有给mutex加锁：初始化了一个没有加锁的mutex。
我们借着defer_lock的话题，来介绍一些unique_lock的重要成员函数

# 三：unique_lock的成员函数

3.1、lock()，加锁
3.2、unlock()，解锁
3.3、try_lock()，尝试给互斥量加锁，如果拿不到锁，则返回false，如果拿到了锁，返回true，这个函数不阻塞的；
3.4、release()，返回它所管理的mutex对象指针，并释放所有权；也就是说，这个unique_lock和mutex不再有关系。
严格区分unlock()和release()的区别，不要混淆。
如果原来mutex对象处于加锁状态，你有责任接管过来并负责解锁。（release返回的是原始mutex的指针）
为什么有时候需要unlock()：因为你lock锁住的代码段越少，执行越快，整个程序运行效率越高。
有人也把锁头锁住的代码多少称为锁的粒度，粒度一般用粗细来描述；
a、锁住的代码少，这个粒度叫细。执行效率高；
b、锁住的代码多，粒度叫粗，那执行效率就低；
要学会尽量选择合适粒度的代码进行保护，粒度太细，可能漏掉共享数据的保护，粒度太粗，影响效率。
选择合适的粒度，是高级程序员实力的体现。

# 四：unique_lock所有权的传递，mutex

std::unique_lockstd::mutex sbguard1(my_mutex1)：所有权概念
sbguard1拥有my_mutex1的所有权
sbguard1可以把自己对mutex(my_mutex1)的所有权转移给其他的unique_lock对象；
所以，unique_lock对象这个mutex的所有权是属于可以转移的，但是不能复制。

a、std::move
b、return unique_lock

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
                std::unique_lock<std::mutex>sbguard(my_mutex, std::defer_lock);
                // 创建了一个没有加锁的mutex
                // sbguard.lock(); // 注意，这里调用的是unique_lock这个的对象的成员函数lock，而不是mutex的
                // 这里我们不需要关心这个互斥量的解锁,因为unique_lock的功能就是能实现自动解锁
                // 当然我们也可以使用它的另一个成员函数unlock来进行解锁
                
                // sbguard.unlock();
                
                // 处理一些非共享代码
                cout << "inMsgRecvQueue执行中...." << i << endl;
                // sbguard.lock();
                
                if (sbguard.try_lock())
                {
                    msgRecvQueue.push_back(i); // 这个i就是命令
                } else {
                    cout <<"没拿到锁" << endl; 
                }
            }
            return;
        }
        bool outMsgNullProc(int &command)
        {
            // 要先lock()，后续才能使用第二个参数std::adopt_lock
            // my_mutex.lock();
            // std::unique_lock<std::mutex>sbguard(my_mutex, std::adopt_lock);
            // std::unique_lock<std::mutex>sbguard(my_mutex);
            std::unique_lock<std::mutex>sbguard = retur_unique_lock();
            std::mutex *pt = sbguard.release(); // 这个指针实际上指的就是解绑的my_mutex
            
            
            
            std::chrono::milliseconds dura(2000); // 
            std::this_thread::sleep_for(dura); // 让这个线程执行到这休息20秒
            
            // std::lock_guard<std::mutex>sbguard(my_mutex); // sbguard 自己随便取的变量名
            // 这里这个lock_guard需要使用std::mutex作为类模板的类型参数，然后将自己定义的mutex变量作为实参传入
            
            //工作原理：lock_guard这个构造函数里执行了mutex::lock()这个函数 
            // 然后因为lock_guard是一个局部对象在这里，所以在退出作用域的时候，就执行了lock_guard的析构函数
            // 然后析构函数里执行了mutex::unlock()这个函数，这样就达到了取代自己写lock（）和unlock()的作用
            
            // my_mutex2.lock();
            // my_mutex.lock();
            // std::lock(my_mutex, my_mutex2);
            if (!msgRecvQueue.empty())
            {
                // 消息队列不为空
                command = msgRecvQueue.front();
                // list 的front方法，返回第一个元素(只是得到元素，并不会改变原list)，（不会检查元素是否存在，所以需要事先判空）
                msgRecvQueue.pop_front(); // 移除第一个元素，但是不返回
                cout << "消息为： " << command << endl;
                pt->unlock();
                // my_mutex.unlock();
                // my_mutex2.unlock();
                return true;
            }
            // my_mutex2.unlock();
            // my_mutex.unlock();
            pt->unlock();
            return false;
        }
        
        // 把命令从队列中取出的线程
        void outMsgRecvQueue()
        {
            int command = 0;
            for (int i = 0; i < 100; ++i)
            {
                // if (!msgRecvQueue.empty())
                // {
                //     my_mutex.lock();
                //     // 消息队列不为空
                //     int command = msgRecvQueue.front();
                //     // list 的front方法，返回第一个元素(只是得到元素，并不会改变原list)，（不会检查元素是否存在，所以需要事先判空）
                //     msgRecvQueue.pop_front(); // 移除第一个元素，但是不返回
                //     my_mutex.lock();
                //     cout << "消息为： " << command << endl;
                // }
                // else
                // {
                //     cout << "msgRecvQueue消息队列为空...." << i << endl;
                // }
                if (outMsgNullProc(command))
                {
                    cout << "取到的值为： " << command << endl;
                } else {
                    cout << "null" << endl;
                }
                
            }
            
            cout << "end!" << endl;
            return;
        }
        
    private:
        std::list<int> msgRecvQueue;
        std::mutex my_mutex; // 创建一个互斥量
        std::mutex my_mutex2; // 创建第二个互斥量，用于演示死锁
};

int main()
{
    
    // 假定: 网络服务器，两个线程，一个线程用于从玩家收集命令，并将命令写到队列中，
    // 另一个线程负责从命令队列中取出命令并执行命令动作
    
    // list:在频繁的按顺序传入和删除数据时效率高，vector 对于随机的插入和删除数据效率高
    // 这里准备使用成员函数作为线程入口函数
    
    A mymsgOBJ;
    std::thread myoutmsg(&A::outMsgRecvQueue, &mymsgOBJ); // 第二个参数要使用引用，才能保证我们在线程中使用的是mymsgObj这个同一个对象
    std::thread myinmsg(&A::inMsgRecvQueue, &mymsgOBJ);
    
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
    // sbguard可以
    
    cout << "main return ... " << endl;
    

    return 0;
}

```