# 一：原子操作std::atomic续谈

一般atomic原子操作，针对++，–，+=，&=，|=，^=是支持的。

std::atomic<int> g_mycout = 0;//这是个原子整型类型变量；可以向使用整型变量一样使用

```c++
void mythread()
{
	for(int i = 0;i<1000000;i++)
	{
		g_mycout++;	//对应的操作就是原子操作，不会被打断；
		g_mycout +=1;
		g_mycout = g_mycout + 1;//结果不对
	}
}
void main()
{
	thread myobj1(mythread);
	thread myobj2(mythread);
	myobj1.join();
	myobj2.join();
	cout<<"两个线程都执行完毕，最终的g_mycout的结果是"<<g_mycout<<endl;

}
```

# 二：std::async深入谈

2.1、std::async参数详述，async用来创建一个异步任务；
参数std::launch::deferred[延迟调用]，以及std::launch::async[强制创建一个线程]
std::thread()如果系统资源紧张，那么可能创建线程就会失败，那么执行std::thread()时整个程序可能崩溃。
std::async()我们一般不叫创建线程（解释async能够创建线程），我们一般叫它创建一个异步任务。
std::async和std::thread最明显的不同，就是async有时候并不创建新线程。
a、如果你用std::launch::deferred来调用 async会怎么样？
std::launch::deferred延迟调用，并且不创建新线程，延迟到future对象调用.get()或者.wait的时候才执行mythread()，如果没有调用get()或wait，那么这个mythread（）不会执行。
b、std::launch::async：强制这个异步任务在新线程上执行，这意味着，系统必须要给我创建出新线程来运行mythread()；
c、std::launch::async|std::launch::deferred
这里这个|：意味着调用async的行为可能是”创建新线程并立即执行“或者没有创建新线程并且延迟到调用result.get()才开始执行任务入口函数，两者居其一；
d、我们不带额外参数；只给async函数一个入口函数名；
默认值应该是std::launch::async|std::launch::deferred;和c效果完全一致。
换句话说：系统会自行决定是异步（创建新线程）还是同步（不创建新线程）方式运行。

自行决定是啥意思？系统如何决定是异步（创建新线程）还是同步（不创建新线程）方式运行
2.2、std::async和std::thread的区别
std::thread创建线程，如果系统资源紧张，创建线程失败，那么整个程序就会报异常崩溃（有脾气）

```c++
int mythread(){return 1;}
std::thread mytobj(mythread);
myobj.join();
```

std::thread创建线程的方式，如果线程返回值，你想拿到这个值也不容易；
std::async创建异步任务。可能创建也可能不创建线程。并且async调用方法很容易拿到线程的返回值；
由于系统资源限制：
1、如果用std::thread创建的线程太多，则可能创建失败，系统报告异常，崩溃。
2、如果用std::async，一般就不会报异常不会崩溃，因为如果系统资源紧张导致无法创建新线程的时候，std::async这种不加额外参数的调用就不会创建新线程。而是后续谁调用了result.get()来请求结果，那么这个异步任务mythread就运行在执行这条get()语句所在的线程上。
如果你强制std::async一定要创建新线程，那么就必须使用std::launch::async。承受的代价就是系统资源紧张时，程序崩溃。
3、经验：一个程序里，线程数量不宜超过100-200，时间片。
2.3、std::async不确定性问题的解决
不加额外参数的std::async调用，让系统自行决定 是否创建新线程。
问题焦点在于std::future result = std::async(mythread); //写法
这个异步任务到底有没有被推迟执行，（std::launch::async还是std::launch::deferred)
std::future对象的wait_for函数

```c++
void main()
{
	cout<<"main start"<<"threadid ="<<std::this_thread::get_id()<<endl;
	std::future<int> result = std::async(mythread);
	std::future_status status = result.wait_for(std::chrono::seconds(0));
	if(status == std::future_status::deferred)
	{
		//线程被延迟执行了（系统资源紧张了，它给我采用std::launch::deferred策略了）
		cout<<result.get()<<endl;
	}
	if (status == std::future_status::timeout)
	{
		//超时线程还没执行完
	}
}
```