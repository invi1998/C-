<!-- 
（1）std::async、std::future创建后台任务并返回值
（2）std:: packaged_task
（3）std:: promise
（4）小结
 -->

# 一：std::async、std::future创建后台任务并返回值

希望线程返回一个结果；
std::async是个函数模板，用来启动一个异步任务，启动起来一个异步任务之后，他返回一个std::future对象，std::future是个类模板。
什么叫”启动一个异步任务“，就是自动创建一个线程并开始执行对应的线程入口函数，它返回一个std::future对象，这个std::future对象里边就含有线程入口函数所返回的结果（线程返回的结果）；我们可以通过调用future对象的成员函数get()来获取。
future:将来的意思，有人也称呼std::future提供了一种访问异步操作结果的机制，就是说这个结果你可能没有办法马上获取。
在线程执行完毕的时候，你就能够拿到结果了，所以大家就这么理解：这个future(对象）里会保存一个值，在将来的某个时刻获取到.

```c++
int mythread()	//线程入口函数
{
	cout<<"mythread() start"<<"threadid="<<std::this_thread::get_id()<<endl;//打印新线程 id
	std::chrono::milliseconds dura(5000);//顶一个5秒的时间
	std::this_thread::sleep_for(dura);//休息了一定时长
	cout<<"mythread() end"<<"threadid="<<std::this_thread::get_id()<<endl;//打印新线程id
	return 5;
}
//下列程序通过std::future对象的get()成员函数等待线程执行结束并返回结果；
//这个get()函数很牛，不拿到将来的返回值誓不罢休，不拿到值我就卡在这里等待拿值；
//std::thread
void main()
{
	//我们通过额外向std::async()传递一个参数，该参数类型是std::lunch类型(枚举类型），来达到一些特殊的目的。
	//std::launch::deferred:表示线程入口函数调用被延迟到std:future的wait()或get()函数调用时才执行；
	//那如果wait()或者get()没有被调用那么线程会执行吗？没执行。实际上，线程根本就没创建
	//std::launch::deferred:延迟调用，并且没有创建新线程，是在主线程中调用的线程入口函数；
	//b、std::launch::async,在调用async函数的时候就开始创建线程；
	//async()函数，默认用的是async|deferred标记，看CPU利用率
	
	A a;
	int tmppar = 12;
	cout<<"main"<<"threadid="<<std::this_thread::get_id()<<endl;
	std::future<int> result = std::async(mythread);	//创建一个线程并开始执行
	std::future<int> result = std::async(std::launch::deferred,&a::mythread,&a,tmppar);// 第二个参数是对象引用才能保证线程里用的是同一个对象
	
	cout<<"continue....!"<<endl;
	int def;
	def = 0;
	cout<<result.get()<<endl;//卡在这里等待mythread()执行完毕，拿到结果
	//get只能调用一次，不能调用多次。
	//result.wait();	//等待线程返回，本身并不返回结果；
	cout<<"I Love China!"<<endl;
	return 0;
}
```

# 二：std::package_tack: 打包任务，把任务包装起来

是个类模板，它的模板参数是各种可调用对象；通过std::package_task来把各种可调用对象包装起来，方便将来作为线程入口函数

```c++
std::packaged_task<int(int)> mypt([](int mypar){
	cout<<"mythread() start"<<"threadid="<<std::this_thread::get_id()<<endl;//打印新线程 id
	std::chrono::milliseconds dura(5000);//顶一个5秒的时间
	std::this_thread::sleep_for(dura);//休息了一定时长
	cout<<"mythread() end"<<"threadid="<<std::this_thread::get_id()<<endl;//打印新线程id
	return 5;
}

cout<<"main"<<"threadid="<<std::this_thread::get_id()<<endl;
std::package_task<int(int)> mypt(mythread);	//我们把函数mythread通过packaged_task包装起来
std::thread t1(std::ref(mypt),1);	//线程直接开始执行，第二个参数作为线程入口函数的参数。
t1.join();//等待线程执行完毕。
std::future<int> result = mypt.get_future();	//std::future对象里包含有线程入口函数的返回结果，这里result保存mythread返回值
cout<<result.get()<<endl;
cout<<"I Love China!"<<endl;

mytasks.push_back(std::move(mypt));	//入容器，这里用了移动语义，入进去之后mypt就为空
//...
std::packaged_task<int(int)> mypt2;
auto iter = mytasks.begin();
mypt2 = std::move(*iter);//移动语义
mytasks.erase(iter);	//删除第一个元素，迭代已经失效了，所以后续代码不可以再使用iter
mypt2(123);
std::future<int> result = mypt2.get_future();
cout<<result.get()<<endl;
```


# 三：std::promise类模板

我们能够在某个线程中给它赋值，然后我们可以在其他线程中，把这个值取出来用；

```c++
void mythread(std::promise<int> &tmpp,int calc)	//大家注意第一个参数
{
	//做一系列复杂的操作
	calc++;
	calc *= 10;
	//做其他运算，比如整整花费了5秒钟；
	std::chrono::milliseconds dura(5000);	//顶一个5秒的时间
	std::this_thread::sleep_for(dura);	//休息了一定时长

	//终于计算出结果了
	int result = calc;	//保存结果
	tmpp.set_value(result);	//结果我保存到了tmpp这个对象中；
}

void mythread2(std::future<int> & tmpf)
{
	auto result = tmpf.get();
	cout<<"mythread2 result"<<result<<endl;
	return;
}
void main()
{
	//std::promise<int> myprom;	//声明一个std::promise对象myprom,保存的值类型为int;
	std::thread t1(mythread,std::ref(myprom),180);
	t1.join();

	//获取结果值
	std::future<int> fu1 = myprom.get_future();	//promise和future绑定，用于获取线程返回值
	//auto result = fu1.get();
	//cout<<"result = "<<result<<endl;
	
	std::thread t2(mythread2, std::ref(fu1));
	t2.join();//等mythread2线程执行完毕
	cout<<"I Love China!"<<endl;
}
```

四：小结：到底怎么用，什么时候用；
我们学习这些东西的目的，并不是要把他们都用在咱们自己的实际开发中。
相反，如果我们能够用最少的东西能够写出一个稳定、高效的多线程程序，更值得赞赏；
我们为了成长，必须要阅读一些高手些的代码，从而快速实现自己代码的积累，我们的技术就会有一个大幅度的提升；
为我们将来能读懂高手代码铺路。

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

int mythread (int tmp)
{
    cout << tmp << endl;
    cout << "线程开始执行" << std::this_thread::get_id() << endl;
    std::chrono::milliseconds dura(5000);
    std::this_thread::sleep_for(dura); // 休息5秒
    cout << "线程执行结束" << std::this_thread::get_id() << endl;
    return 5;
}

class THA
{
    public:
        int mythread (int mpr)
        {
            cout << mpr << endl;
            cout << "线程开始执行" << std::this_thread::get_id() << endl;
            std::chrono::milliseconds dura(5000);
            std::this_thread::sleep_for(dura); // 休息5秒
            cout << "线程执行结束" << std::this_thread::get_id() << endl;
            return 5;
        }
};

vector<std::packaged_task<int(int)>>mytask;

void myPromiseThread(std::promise<int> &temp, int calc)
{
    calc++;
    calc *= 100;
    std::chrono::milliseconds dura(5000);
    std::this_thread::sleep_for(dura); // 休息5秒
    int result = calc;
    temp.set_value(result); // 线程函数运算结果通过set_value就保存在了一个promise对象temp里
}

void myPromiseFutureThread(std::future<int> &tempF)
{
    auto result = tempF.get();
    std::cout << "线程1的结果为，" << result << std::endl;
    result;
}

int main()
{
    // std::async,std::future创建后台任务并返回
    // 希望线程返回一个结果
    
    // std::async是一个函数模板，用来启动一个异步任务，启动起来异步任务之后没返回一个std::future对象
    // 什么叫启动一个异步任务，就是创建一个线程并开始执行对应的线程入口函数，她返回的std::future对象
    // 里面就有线程入口函数返回的结果（线程返回结果）
    // 可以调用std::future对象的get()成员函数获取这个结果
    // std::future提供了一种访问异步操作结果的机制，就是说这个结果你可能没有办法马上拿到
    // 只能在线程执行完毕之后才能获得，这个future对象里保存了一个值，在将来的某个时刻能够拿到
    // get()函数只能调用一次，不可以连续多次调用
    
    std::cout << "main 线程执行" << std::this_thread::get_id() << std::endl;
    // std::future<int> futureNUM = std::async(mythread); // 创建一个线程，并开始执行，将这个future对象和这个线程绑定到一起
    // std::cout << "continue1..." << std::endl;
    // int def;
    // def = 0;
    // cout << futureNUM.get() << endl; // 卡在这里等待mythread执行完毕，拿到结果后才会往下走
    // // futureNUM.wait(); // 等待线程执行结束，本身并不返回线程结果
    // cout << "打印" << endl;
    
    // 通过std::future对象的get()成员函数等待线程执行结束并返回结果
    // get()函数会一直等到拿到线程执行结果
    
    // THA mytha_obj;
    // int temp = 123;
    // // std::future<int> futureTHA = std::async(&THA::mythread, &mytha_obj, temp);
    // // std::future<int> futureTHA = std::async(std::launch::deferred, &THA::mythread, &mytha_obj, temp);
    // std::future<int> futureTHA = std::async(std::launch::async, &THA::mythread, &mytha_obj, temp);
    // // 注意这里，如果使用的是类成员函数做线程函数入口，
    // // 第一个参数是对象的成员函数引用，
    // // 第二个参数是这个对象的引用，才能保证线程里面使用的是同一个对象，而不会单独重新复制出一个对象
    // // 第三个参数是这个对象的成员函数的参数
    // std::cout << "continue2..." << std::endl;
    // int def2;
    // def2 = 0;
    // cout << futureTHA.get() << endl; // 卡在这里等待mythread执行完毕，拿到结果后才会往下走
    // cout << "打印2222" << endl;
    
    // 我们可以额外向std::async传递一个参数，该参数类型是std::launch类型（枚举类型），来达到一些特殊目的
    // 1）std::launch::deferred; 表示线程入口函数调用被延迟到std::future的get()或者wait()才执行,
    // 此时，如果wait()或者get()没有被调用，那么线程不会被执行，实际上线程甚至都没被创建
    // std::launch::deferred实际上，延迟调用，并且没有创建新线程，都是在主线程中调用线程入口函数
    // 2）std::launch::async；异步，表示在调用async函数的时候就开始创建线程并且立即开始执行，不会等待get()或者wait（）
    // async()函数，系统默认使用的就是这个参数标记（std::launch::async）
    
    
    // std::packaged_task 任务包装，任务打包
    // std::packaged_task是一个类模板，他的模板参数是各种可调用对象
    // 通过std::packaged_task可以把各种可调用对象包装起来，方便日后作为线程入口函数
    // packaged_task包装起来的可调用对象还可以直接调用，从这个角度来讲，packaged_task对象也是一个可调用对象
    
    
    std::packaged_task<int(int)> mpt(mythread); // 把函数通过packaged_task包装起来
    // std::thread t1(std::ref(mpt), 1232); // 线程开始执行，第二个参数作为线程入口函数的参数
    // t1.join(); // 等待线程执行完毕
    // std::future<int> future1 = mpt.get_future(); // std：：future对象里包含有线程入口函数的返回值
    // // 这里future1保存的就是mythread返回的结果
    // std::cout << future1.get() << std::endl;
    
    std::packaged_task<int(int)> mpt2([](int temp){
        cout << temp << endl;
        cout << "线程开始执行" << std::this_thread::get_id() << endl;
        std::chrono::milliseconds dura(5000);
        std::this_thread::sleep_for(dura); // 休息5秒
        cout << "线程执行结束" << std::this_thread::get_id() << endl;
        return 5;
    });
    // std::thread t1(std::ref(mpt2), 1232); // 线程开始执行，第二个参数作为线程入口函数的参数
    // t1.join(); // 等待线程执行完毕
    // std::future<int> future2 = mpt2.get_future(); // std：：future对象里包含有线程入口函数的返回值
    // // 这里future1保存的就是mythread返回的结果
    // std::cout << future2.get() << std::endl;
    
    // 直接调用
    // mpt2(2344); // 直接调用，相当于函数调用，并没有创建线程
    // std::future<int> future3 = mpt2.get_future(); // std：：future对象里包含有线程入口函数的返回值
    // // 这里future1保存的就是mythread返回的结果
    // std::cout << future3.get() << std::endl;
    
    // mytask.push_back(std::move(mpt)); // 使用移动语义，移动进去以后，mpt就为空了
    // mytask.push_back(std::move(mpt2));
    
    // std::packaged_task<int(int)> mypt;
    // auto iter = mytask.begin(); // 定义迭代器
    // mypt = std::move(*iter); // 移动语义
    // mytask.erase(iter); // 移动完之后，原来的指针不能继续使用，应该进行释放。erase（）删除迭代器指向的元素
    // // 同时，这个迭代器也已失效，后续也不可以继续使用这个迭代器
    // mypt(2344); // 直接调用，相当于函数调用，并没有创建线程
    // std::future<int> future3 = mypt.get_future(); // std：：future对象里包含有线程入口函数的返回值
    // // 这里future1保存的就是mythread返回的结果
    // std::cout << future3.get() << std::endl;
    
    
    
    
    // std::promise，类模板
    // 通过这个类模板，我们可以在某个线程中给他赋值，然后在其他线程中把这个值取出来
    // 总结：通过promise保存一个值，在将来某个时刻我们通过把一个future绑定到这个promise上来得到这个绑定的值
    
    std::promise<int> mypro; // 声明一个std::promise对象，保存的值的类型为int
    std::thread tp1(myPromiseThread, std::ref(mypro), 10086);
    tp1.join();
    // 获取结果值
    std::future<int> fue = mypro.get_future(); // future 和 promise进行绑定，用于获取线程返回值
    // cout << fue.get() << endl;
    
    std::thread tp2(myPromiseFutureThread, std::ref(fue));
    tp2.join(); // 等待线程2执行完毕
    

    return 0;
}

```