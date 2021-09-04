* （1）条件变量std::condition_variable、wait()、notify_one()
* （2）上述代码深入思考
* （3）notify_all()

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
