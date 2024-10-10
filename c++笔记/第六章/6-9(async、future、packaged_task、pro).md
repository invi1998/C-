<!-- 
（1）std::async、std::future创建后台任务并返回值
（2）std:: packaged_task
（3）std:: promise
（4）小结
 -->



在C++中，`std::async` 和 `std::thread` 都是用于并发编程的工具，但它们在设计理念、使用方式和行为上有显著的区别。以下是 `std::async` 和 `std::thread` 的主要区别：

### 1. 设计理念

#### `std::thread`
- **底层控制**：`std::thread` 提供了对操作系统线程的直接控制，允许开发者手动管理线程的生命周期。
- **灵活性**：`std::thread` 适合需要细粒度控制线程的应用，如手动调度、线程池等。

#### `std::async`
- **高层抽象**：`std::async` 提供了一个更高层次的抽象，用于异步任务的执行。它隐藏了线程管理的细节，简化了异步编程。
- **自动管理**：`std::async` 自动管理任务的执行和结果的获取，适合需要异步执行任务但不需要手动管理线程的应用。

### 2. 使用方式

#### `std::thread`
- **创建线程**：通过 `std::thread` 构造函数创建线程，传递一个可调用对象（如函数、lambda 表达式等）。
- **手动管理**：需要手动调用 `join` 或 `detach` 来管理线程的生命周期。

```cpp
#include <iostream>
#include <thread>

void threadFunction() {
    std::cout << "Thread function running" << std::endl;
}

int main() {
    std::thread t(threadFunction);
    t.join();  // 等待线程完成

    return 0;
}
```

#### `std::async`
- **创建异步任务**：通过 `std::async` 创建异步任务，返回一个 `std::future` 对象，用于获取任务的结果。
- **自动管理**：`std::async` 会自动管理任务的执行和结果的获取，无需手动管理线程。

```cpp
#include <iostream>
#include <future>

int asyncFunction() {
    std::this_thread::sleep_for(std::chrono::seconds(1));  // 模拟耗时操作
    return 42;
}

int main() {
    std::future<int> result = std::async(std::launch::async, asyncFunction);
    std::cout << "Doing some work in the main thread" << std::endl;
    int value = result.get();  // 等待任务完成并获取结果
    std::cout << "Async function returned: " << value << std::endl;

    return 0;
}
```

### 3. 行为差异

#### `std::thread`
- **立即执行**：`std::thread` 创建的线程会立即开始执行。
- **手动同步**：需要手动使用互斥量、条件变量等同步机制来协调线程之间的通信和同步。

#### `std::async`
- **延迟执行**：`std::async` 创建的任务可能不会立即执行，具体行为取决于 `std::launch` 策略。
- **自动同步**：`std::async` 通过 `std::future` 自动管理任务的同步，调用 `get` 时会阻塞，直到任务完成。

### 4. 性能和资源管理

#### `std::thread`
- **资源管理**：需要手动管理线程资源，如调用 `join` 或 `detach`。
- **开销**：创建和销毁线程的开销较大，适合长期运行的任务。

#### `std::async`
- **资源管理**：`std::async` 自动管理任务的资源，适合短小的异步任务。
- **开销**：`std::async` 的开销相对较小，但具体行为取决于实现（如可能使用线程池）。

### 5. 使用场景

#### `std::thread`
- **适合场景**：需要细粒度控制线程的应用，如手动调度、线程池、长时间运行的任务。
- **示例**：网络服务器、实时数据处理等。

#### `std::async`
- **适合场景**：需要异步执行任务但不需要手动管理线程的应用，如异步IO、短小的任务。
- **示例**：文件读写、网络请求、计算密集型任务等。

### 总结

- **`std::thread`**：
  - **底层控制**：直接管理操作系统线程。
  - **灵活性**：适合需要细粒度控制线程的应用。
  - **手动管理**：需要手动管理线程的生命周期和同步。

- **`std::async`**：
  - **高层抽象**：提供异步任务的自动管理。
  - **自动管理**：自动管理任务的执行和结果的获取。
  - **适合场景**：适合需要异步执行任务但不需要手动管理线程的应用。

选择 `std::thread` 还是 `std::async` 取决于具体的应用需求和开发者的偏好。对于需要细粒度控制和高性能的应用，`std::thread` 是更好的选择；对于需要简单异步任务管理的应用，`std::async` 更为合适。

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

# 二：std::package_task: 打包任务，把任务包装起来

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

`std::packaged_task`，这是一个 C++ 标准库中的类模板，用于封装可调用对象（如函数、lambda 表达式等），并将其转换为异步任务。`std::packaged_task` 与 `std::future` 和 `std::promise` 一起使用，可以实现异步任务的创建、执行和结果的获取。

### 1. `std::packaged_task` 基本概念

`std::packaged_task` 是一个类模板，用于封装一个可调用对象，并提供一个 `std::future` 来获取任务的结果。`std::packaged_task` 的主要用途是将任务的执行与结果的获取分离，使得任务可以在一个线程中执行，而在另一个线程中获取结果。

### 2. 基本用法

#### 2.1 包含头文件

```cpp
#include <iostream>
#include <future>
#include <thread>
```

#### 2.2 创建 `std::packaged_task`

```cpp
std::packaged_task<int()> task([]() {
    std::this_thread::sleep_for(std::chrono::seconds(1));  // 模拟耗时操作
    return 42;
});
```

#### 2.3 获取 `std::future`

```cpp
std::future<int> result = task.get_future();
```

#### 2.4 执行任务

```cpp
std::thread t(std::move(task));
t.join();
```

#### 2.5 获取结果

```cpp
int value = result.get();
std::cout << "Task returned: " << value << std::endl;
```

### 3. 完整示例

以下是一个完整的示例，展示了如何使用 `std::packaged_task` 创建和执行异步任务，并获取结果。

```cpp
#include <iostream>
#include <future>
#include <thread>

int main() {
    // 创建一个 packaged_task，封装一个 lambda 表达式
    std::packaged_task<int()> task([]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));  // 模拟耗时操作
        return 42;
    });

    // 获取 future 对象，用于获取任务的结果
    std::future<int> result = task.get_future();

    // 在新线程中执行任务
    std::thread t(std::move(task));
    t.join();

    // 获取任务的结果
    int value = result.get();
    std::cout << "Task returned: " << value << std::endl;

    return 0;
}
```

### 4. 详细说明

#### 4.1 创建 `std::packaged_task`

`std::packaged_task` 的构造函数接受一个可调用对象（如函数、lambda 表达式等），并将其实例化为一个特定类型的 `std::packaged_task`。

```cpp
std::packaged_task<int()> task([]() {
    std::this_thread::sleep_for(std::chrono::seconds(1));  // 模拟耗时操作
    return 42;
});
```

#### 4.2 获取 `std::future`

`std::packaged_task` 提供了 `get_future` 方法，用于获取一个 `std::future` 对象，该对象可以用来获取任务的结果。

```cpp
std::future<int> result = task.get_future();
```

#### 4.3 执行任务

`std::packaged_task` 可以通过调用其 `operator()` 来执行任务。通常情况下，任务会在一个新线程中执行，因此需要将 `std::packaged_task` 移动到新线程中。

```cpp
std::thread t(std::move(task));
t.join();
```

#### 4.4 获取结果

`std::future` 对象的 `get` 方法会阻塞，直到任务完成并返回结果。

```cpp
int value = result.get();
std::cout << "Task returned: " << value << std::endl;
```

### 5. 优点

- **异步任务**：`std::packaged_task` 使得任务的执行和结果的获取分离，适合异步编程。
- **灵活性**：可以封装任何可调用对象，包括函数、lambda 表达式、函数对象等。
- **线程安全**：`std::future` 和 `std::packaged_task` 的组合提供了线程安全的机制，确保任务的执行和结果的获取不会产生竞态条件。

### 6. 注意事项

- **移动语义**：`std::packaged_task` 不支持拷贝，只能通过移动语义传递。
- **资源管理**：`std::packaged_task` 的生命周期需要谨慎管理，确保在任务完成之前不被销毁。

通过使用 `std::packaged_task`，可以更方便地实现异步任务的创建和管理，提高程序的并发性能。

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



`std::promise` 是 C++ 标准库中用于异步编程的一个类模板，它提供了一种机制来设置异步操作的结果。`std::promise` 与 `std::future` 一起使用，`std::promise` 用于设置结果，而 `std::future` 用于获取结果。这种方式使得异步任务的执行和结果的获取分离，提高了代码的灵活性和可维护性。

### 1. `std::promise` 基本概念

- **`std::promise`**：用于设置异步操作的结果。
- **`std::future`**：用于获取异步操作的结果。

### 2. 基本用法

#### 2.1 包含头文件

```cpp
#include <iostream>
#include <future>
#include <thread>
```

#### 2.2 创建 `std::promise` 和 `std::future`

```cpp
std::promise<int> promise;
std::future<int> future = promise.get_future();
```

#### 2.3 设置结果

在异步任务中，使用 `std::promise` 的 `set_value` 方法设置结果。

```cpp
void asyncFunction(std::promise<int> p) {
    std::this_thread::sleep_for(std::chrono::seconds(1));  // 模拟耗时操作
    p.set_value(42);  // 设置结果
}
```

#### 2.4 获取结果

在主线程中，使用 `std::future` 的 `get` 方法获取结果。

```cpp
int main() {
    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    std::thread t(asyncFunction, std::move(promise));
    t.join();

    int value = future.get();  // 获取结果
    std::cout << "Async function returned: " << value << std::endl;

    return 0;
}
```

### 3. 完整示例

以下是一个完整的示例，展示了如何使用 `std::promise` 和 `std::future` 创建和执行异步任务，并获取结果。

```cpp
#include <iostream>
#include <future>
#include <thread>

void asyncFunction(std::promise<int> p) {
    std::this_thread::sleep_for(std::chrono::seconds(1));  // 模拟耗时操作
    p.set_value(42);  // 设置结果
}

int main() {
    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    std::thread t(asyncFunction, std::move(promise));
    t.join();

    int value = future.get();  // 获取结果
    std::cout << "Async function returned: " << value << std::endl;

    return 0;
}
```

### 4. 详细说明

#### 4.1 创建 `std::promise` 和 `std::future`

```cpp
std::promise<int> promise;
std::future<int> future = promise.get_future();
```

- **`std::promise<int>`**：创建一个 `std::promise` 对象，用于设置 `int` 类型的结果。
- **`promise.get_future()`**：从 `std::promise` 对象中获取一个 `std::future` 对象，用于获取结果。

#### 4.2 设置结果

在异步任务中，使用 `std::promise` 的 `set_value` 方法设置结果。

```cpp
p.set_value(42);  // 设置结果
```

- **`set_value`**：设置 `std::promise` 的结果。一旦设置，关联的 `std::future` 对象就可以通过 `get` 方法获取结果。

#### 4.3 获取结果

在主线程中，使用 `std::future` 的 `get` 方法获取结果。

```cpp
int value = future.get();  // 获取结果
```

- **`get`**：阻塞当前线程，直到结果可用，然后返回结果。如果任务抛出异常，`get` 也会重新抛出该异常。

### 5. 优点

- **异步任务**：`std::promise` 和 `std::future` 使得任务的执行和结果的获取分离，适合异步编程。
- **灵活性**：可以用于多种异步操作，如网络请求、文件读写等。
- **线程安全**：`std::promise` 和 `std::future` 的组合提供了线程安全的机制，确保任务的执行和结果的获取不会产生竞态条件。

### 6. 注意事项

- **移动语义**：`std::promise` 不支持拷贝，只能通过移动语义传递。
- **资源管理**：`std::promise` 和 `std::future` 的生命周期需要谨慎管理，确保在任务完成之前不被销毁。
- **异常处理**：如果异步任务抛出异常，可以使用 `set_exception` 方法设置异常，`std::future` 的 `get` 方法会重新抛出该异常。

### 7. 其他方法

- **`set_exception`**：设置 `std::promise` 的异常，关联的 `std::future` 对象在调用 `get` 时会重新抛出该异常。
- **`get_future`**：从 `std::promise` 对象中获取一个 `std::future` 对象。

### 8. 示例：设置异常

```cpp
#include <iostream>
#include <future>
#include <thread>
#include <exception>

void asyncFunction(std::promise<int> p) {
    try {
        std::this_thread::sleep_for(std::chrono::seconds(1));  // 模拟耗时操作
        throw std::runtime_error("An error occurred");
    } catch (...) {
        p.set_exception(std::current_exception());  // 设置异常
    }
}

int main() {
    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    std::thread t(asyncFunction, std::move(promise));
    t.join();

    try {
        int value = future.get();  // 获取结果
        std::cout << "Async function returned: " << value << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
```

通过使用 `std::promise` 和 `std::future`，可以更方便地实现异步任务的创建和管理，提高程序的并发性能和可维护性。