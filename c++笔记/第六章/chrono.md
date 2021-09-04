# C++11有了chrono库，可以很容易的实现定时功能。

## chrono：
　　chrono库主要包含了三种类型：时间间隔Duration、时钟Clocks和时间点Time point。
Duration：
　　duration表示一段时间间隔，用来记录时间长度，可以表示几秒钟、几分钟或者几个小时的时间间隔，duration的原型是：

```c++
              template<class Rep, class Period = std::ratio<1>> class duration;

```
　　第一个模板参数Rep是一个数值类型，表示时钟个数；第二个模板参数是一个默认模板参数std::ratio，它的原型是：

```c++
              template<std::intmax_t Num, std::intmax_t Denom = 1> class ratio;
```
　　它表示每个时钟周期的秒数，其中第一个模板参数Num代表分子，Denom代表分母，分母默认为1，ratio代表的是一个分子除以分母的分数值，比如ratio<2>代表一个时钟周期是两秒，ratio<60>代表了一分钟，ratio<60*60>代表一个小时，ratio<60*60*24>代表一天。而ratio<1, 1000>代表的则是1/1000秒即一毫秒，ratio<1, 1000000>代表一微秒，ratio<1, 1000000000>代表一纳秒。标准库为了方便使用，就定义了一些常用的时间间隔，如时、分、秒、毫秒、微秒和纳秒，在chrono命名空间下，它们的定义如下：

```c++
typedef duration <Rep, ratio<3600,1>> hours;
typedef duration <Rep, ratio<60,1>> minutes;
typedef duration <Rep, ratio<1,1>> seconds;
typedef duration <Rep, ratio<1,1000>> milliseconds;
typedef duration <Rep, ratio<1,1000000>> microseconds;
typedef duration <Rep, ratio<1,1000000000>> nanoseconds;
　　通过定义这些常用的时间间隔类型，我们能方便的使用它们，比如线程的休眠：
std::this_thread::sleep_for(std::chrono::seconds(3)); //休眠三秒
std::this_thread::sleep_for(std::chrono:: milliseconds (100)); //休眠100毫秒

```
## Time point：

　　time_point表示一个时间点，用来获取1970.1.1以来的秒数和当前的时间, 可以做一些时间的比较和算术运算，可以和ctime库结合起来显示时间。time_point必须要clock来计时，time_point有一个函数time_from_eproch()用来获得1970年1月1日到time_point时间经过的duration。下面的例子计算当前时间距离1970年1月一日有多少天：

```c++

#include <iostream>
#include <ratio>
#include <chrono>
int main ()
{
  using namespace std::chrono;
  typedef duration<int,std::ratio<60*60*24>> days_type;
  time_point<system_clock,days_type> today = time_point_cast<days_type>(system_clock::now());
  std::cout << today.time_since_epoch().count() << " days since epoch" << std::endl;
  return 0;
}

```

## Clocks：

　　表示当前的 系统时钟，内部有time_point, duration, Rep, Period等信息，它主要用来获取当前时间，以及实现time_t和time_point的相互转换。Clocks包含三种时钟： 
* system_clock：从系统获取的时钟；
* steady_clock：不能被修改的时钟；
* high_resolution_clock：高精度时钟，实际上是system_clock或者steady_clock的别名。
　　可以通过now()来获取当前时间点：

```c++
#include <iostream>
#include <chrono>
int main()
{
std::chrono::steady_clock::time_point t1 = std::chrono::system_clock::now();
std::cout << "Hello World\n";
std::chrono::steady_clock::time_point t2 = std::chrono:: system_clock::now();
std::cout << (t2-t1).count()<<” tick count”<<endl;
}

```
    通过时钟获取两个时间点之相差多少个时钟周期，我们可以通过duration_cast将其转换为其它时钟周期的duration：

```c++
cout << std::chrono::duration_cast<std::chrono::microseconds>( t2-t1 ).count() <<” microseconds”<< endl;
     system_clock的to_time_t方法可以将一个time_point转换为ctime，而from_time_t方法则是相反的，它将ctime转换为time_point：
std::time_t now_c = std::chrono::system_clock::to_time_t(time_point);

```
    可以利用high_resolution_clock来实现一个类似于boost.timer的定时器，这样的timer在测试性能时会经常用到，经常用它来测试函数耗时，可实现毫秒微秒级定时，它的基本用法是这样的：

```c++
#include<chrono>
usingnamespace std;
usingnamespace std::chrono;
classTimer
{
public:
    Timer() : m_begin(high_resolution_clock::now()) {}
    void reset() { m_begin = high_resolution_clock::now(); }
    //默认输出毫秒
    int64_t elapsed() const
    {
        return duration_cast<chrono::milliseconds>(high_resolution_clock::now() - m_begin).count();
    }
    //微秒
    int64_t elapsed_micro() const
    {
        return duration_cast<chrono::microseconds>(high_resolution_clock::now() - m_begin).count();
    } 
    //纳秒
    int64_t elapsed_nano() const
    {
        return duration_cast<chrono::nanoseconds>(high_resolution_clock::now() - m_begin).count();
    }
    //秒
    int64_t elapsed_seconds() const
    {
        return duration_cast<chrono::seconds>(high_resolution_clock::now() - m_begin).count();
    }
    //分
    int64_t elapsed_minutes() const
    {
        return duration_cast<chrono::minutes>(high_resolution_clock::now() - m_begin).count();
    }
    //时
    int64_t elapsed_hours() const
    {
        return duration_cast<chrono::hours>(high_resolution_clock::now() - m_begin).count();
    }
private:
    time_point<high_resolution_clock> m_begin;
};

```
测试代码：

```c++
void fun()
{
    cout<<”hello word”<<endl;
}
int main()
{
    timer t; //开始计时
    fun()
    cout<<t.elapsed()<<endl; //打印fun函数耗时多少毫秒
    cout<<t.elapsed_micro ()<<endl; //打印微秒
    cout<<t.elapsed_nano ()<<endl; //打印纳秒
    cout<<t.elapsed_seconds()<<endl; //打印秒
    cout<<t.elapsed_minutes()<<endl; //打印分钟
    cout<<t.elapsed_hours()<<endl; //打印小时
}
```
