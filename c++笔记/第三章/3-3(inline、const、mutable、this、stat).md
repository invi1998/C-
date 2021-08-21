（1）在类定义中实现成员函数inline

（2）成员函数末尾的const

* 含义：成员函数不会修改该对象的成员变量
* const 成员函数， 不管是const对象还是非const对象，都可以调用const成员函数
* 非const成员函数，不能被const对象调用，只能被非const对象调用
* 普通函数（非类里的函数）后面不能加const。（非成员函数不允许使用const修饰符）

const关键字所起作用的本质，就是把隐藏着的默认的this指针参数，改成const类型。也就是说：
假如void    foo( )函数被编译器改写为 void foo(T*pThis)，则void foo( ) const将会被改写为void foo(const T* pThis) 。在函数末尾添加一个const，就相当于在隐藏的this参数类型前加一个const.

这样做有两个效果，第一：编译器将不允许foo()const修改pThis指向的对象的成员。第二、const对象只能调用const成员函数，否则就会报错说把const T*转化为T* 会丢失qualifier

（3）mutable（不稳定，容易改变）
const的反义词，mutable的存在就是为了突破const的限制

用mutable修饰一个成员变量，就表示这个成员变量永远处于可以修改状态，及时在const常成员函数中.

（4）返回自身对象的引用，this

如何理解this，程序在调用成员函数的时候，编译器负责把这个对象的地址传递给这个成员函数中的一个隐藏的this形参。
比如有个类，他的成员函数add_hour.
Time mytime;
mytime.add_hour(13);
程序执行到这，实际上编译器是重写了这个add_hour成员函数。
Time& Time::add_hour(Tiem *const this, int hour)
编译器就会多加了这个Time* const this这个参数，然后在调用的时候，编译器实际上是把你所调用的对象的地址作为第一个参数，进行调用。即:
mytime.add_hour(&mytime, 13);
这也就解释了为什么成员函数里可以直接调用成员变量，因为在编译器看来，成员函数里的成员变量都是通过this来进行调用的，即你也可以显示的写为
this->Hour  (this->成员白能量)

* <font color="#03A9F4">1. 隐藏的this指针只能在成员函数中，全局函数和静态函数（static）中不存在this指针</font>
* <font color="#03A9F4">2. 在普通成员函数中，this是一个指向非const对象的const指针(比如说：类型为Time, 那么this就是 Time * const this )。this只能指向当前对象，指向不能改变。</font>
* <font color="#03A9F4">3. 在const成员函数中，this指针就是一个指向const对象的const指针。const Time *const this, 即this指向的对象不能变外，同时this指向的对象的成员变量的值也不能变</font>

（5）static成员

* 静态成员函数

属于整个类，不属于具体的某个类对象，我们一旦在某个对象里修改了这个成员变量值，其他对象里的这个静态成员函数也会被修改。对于这种静态成员变量的调用 类名：：静态成员变量名

成员函数前面也可以加static，构成静态成员函数。属于整个类的成员函数，调用时 类名：：成员函数名(....)

如何定义静态成员变量：一般在一个.cpp文件的开头定义静态成员变量。这样可以在任何函数前面进行分配内存。
<font color="#9c27b0">注意：定义(实现)静态成员变量时，前面不能添加static，只能在声明时添加static；定义时可以不给初始值，系统默认给0</font>

但是毕竟静态成员变量也是属于成员变量，所以也可以使用( 对象名. 静态成员名 ) 来进行访问；

静态成员函数不能访问普通成员变量，只能访问静态成员变量；

# 在类定义中实现成员函数inline

```c++
/*
*(1)在类定义中实现成员函数inline，类内的成员函数实现其实也叫类的成员函数定义。
* AddInline()这种函数直接在类的定义中实现的成员函数，会被当做inline内联函数来处理。
* 能不能内联成功，取决于具体的编译器。
*
*(2)
*(3)
*
*/
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
using namespace std; 

class Time
{
private:

 int Hour;
 int Minute;
 int Second;

public:

 explicit Time(int Hour, int Minute, int Second);
 Time(int Hour);

public:

 void AddHour(int TemHour)
 {
  Hour = Hour + TemHour;
 }

}; 

Time:: Time(int TemHour, int TemMinute, int TemSecond) : Hour(TemHour), Minute(TemMinute), Second(TemSecond)//把形式参数赋值给成员属性
{
}
Time:: Time(int Hour)
{

 this->Hour = Hour;

}

int main(void)
{

 Time myTime01 = Time(12, 12, 12);

 system("pause");
 return 0;

}

```

# 成员函数末尾的const

```c++
/*
*(1)在成员函数(声明和定义都需要)后面增加const
* 作用：告诉系统，这个成员函数，不会修改该对象里面任何成员变量的值，
*  也就是说，这个成员函数不会修改类的任何状态。
*  后面加一个const的成员函数也叫常量成员函数
*(2)
*(3)
*
*/
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
using namespace std;

class Time
{
private:
 int Hour;
 int Minute;
 int Second;
public:
 explicit Time(int Hour, int Minute, int Second);
 Time(int Hour);
 Time()
 {}
public:
 void AddHour(int TemHour)const;
 void test(string& str)
 {
  cout << str << endl;
 }
};

Time::Time(int TemHour, int TemMinute, int TemSecond) :Hour(TemHour), Minute(TemMinute), Second(TemSecond)//把形式参数赋值给成员属性
{
}
Time::Time(int Hour)
{
 this->Hour = Hour;
}

//常量成员函数--不能修改成员变量的值
void Time::AddHour(int TemHour) const
{
 // Hour = Hour + TemHour; //error不能修改成员变量的属性
 
}

int main(void)
{
 Time myTime01 = Time(12, 12, 12);
 const Time myTime02; //定义const对象，这种对象一旦初始化就不能修改。
 myTime02.AddHour(12);
 //myTime02.test(); //不兼容的修饰符号，不能调用非const函数。
 
 /*1.声明为const的对象不能调用普通的成员函数
  *2.但是这种对象可以调用const常量成员函数
  *3.const不能放在普通函数的末尾
  * 
  */
 system("pause");
 return 0;

}

```

# mutable修饰成员变量_永远处于可修改状态

```c++
/*
*(1)mutable 不稳定，容易改变的<---->const
* mutable的引入就是为了突破const的限制。
* 当成员函数后面有const，是不能修改成员变量的值，如果想在const常成员函数中修改成员变量的值。就是用mutable。
*(2)mutalbe是为了突破const限制而开的一个后门。
*(3)
*
*/
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
using namespace std; 

class Time
{
private:

 mutable int Hour; //mutable修饰
 int Minute;
 int Second;

public:

 explicit Time(int Hour, int Minute, int Second);
 Time(int Hour);

 Time(){}

public:

 void AddHour(int TemHour)const
 {
  //用mutable修饰一个成员变量，就表示这个成员变量永远处于可以修改状态，及时在const常成员函数中.
  Hour = Hour + TemHour;
 }

}; 

Time:: Time(int TemHour, int TemMinute, int TemSecond) : Hour(TemHour), Minute(TemMinute), Second(TemSecond)//把形式参数赋值给成员属性
{
}
Time:: Time(int Hour)
{

 this->Hour = Hour;

}

int main(void)
{

 Time myTime01 = Time(12, 12, 12);

 const Time myTime02;
 myTime02.AddHour(12);

 

 

 

 system("pause");
 return 0;

}

```

# 返回对象本身实现函数连调与this指针

```c++

/*
*(1)返回自身对象的引用  *this
*(2)this指针
* 每一个成员对象都有一个隐含的成员属性this指针，指向自己本身的地址，当进行函数调用的时候，把this指针
* （自己本身的地址）当做第一个参数传递过去，后面依次是自己写的参数。
* 对系统看来，任何对成员属性的访问都是通过this指针来访问的。
*(3)
* 1.隐藏的this指针只能在成员函数中，全局函数和静态函数（static）中不存在this指针
* 2.在普通成员函数中，this是一个指向非const对象的const指针。this只能指向当前对象，指向不能改变。
* 3.在const成员函数中，this指针就是一个指向const对象的const指针。const Time *const this
*
*总结：
* 返回对象本身可以实现函数连续调用，多值函数
*/
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
using namespace std;

class Time
{
private:
 mutable int Hour;
 int Minute;
 int Second;
public:
 explicit Time(int Hour, int Minute, int Second);
 Time(int Hour);
 Time(){}
public:
 //返回对象自身的应用，返回对象自身
 Time& AddHour(int TemHour);
 //Time& AddHour(Time *const this,int TemHour); //等价于这种，隐藏this指针
 Time& AddMinute(int TemMinute);
};

Time::Time(int TemHour, int TemMinute, int TemSecond) :Hour(TemHour), Minute(TemMinute), Second(TemSecond)//把形式参数赋值给成员属性
{
}
Time::Time(int Hour)
{
 this->Hour = Hour;
}
//返回对象自身的引用
Time& Time::AddHour(int TemHour)
{
 //Hour = (Hour + TemHour) % 24; //时间是0-24小时
 this->Hour = (this->Hour + TemHour) % 24;
 cout << this->Hour << endl; //10
 return *this; //返回对象自身
}

Time& Time::AddMinute(int TemMinute)
{
 this->Minute = (this->Minute + TemMinute) % 60;
 cout << this->Minute << endl;
 return *this;
}

int main(void)
{
 Time myTime01 = Time(12, 12, 12);
 myTime01.AddHour(22);
 //返回对象本身，就可以实现多值函数的调用，类似于cout<<可以实现调用多次，返回的就是ofstream对象本身
 //返回对象本身可以实现函数连续调用，多值函数
 myTime01.AddHour(13).AddMinute(54);
 
 system("pause");
 return 0;

}

```

# 类中的静态属性和静态方法

```c++

/*
*(1)static成员，全局静态变量与局部静态变量
*
*(2)有没有一个成员变量或者属性是属于某个类，不属于某个对象？？
* 属于整个类的成员变量叫静态成员变量，static成员变量
* 特点：不属于某个对象，而是属于整个类，一旦在某个对象中修改了static成员变量的值。
*  其他对象可以直接访问。
*  2。这种成员函数只有一个副本，使用时  类名：：成员变量名
*  3. 成员函数前面也可以加static，构成静态成员函数，属于类的成员，调用时候
*   类名:: 成员函数名字
*   静态成员函数只能操作静态成员属性.
*(3)
*
*/
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
using namespace std; 
static int number02 = 10;  //全局静态变量的作用域是定义的文件，也就是说，在该cpp文件中定义的全局静态变量，不能再其他cpp文件中使用。
// 对于普通的全局变量就可以在其他cpp文件中使用 extern 关键字来声明调用
// externa int g_abc; 
//全局静态变量限制在本文件中，不能在其他文件中使用extern来声明调用。
//
//局部静态变量保存上一次调用的时候结果的值，再次进入时候不会执行初始化工作
void myFunction()
{

 static int number1 = 5;//局部静态变量，跳出函数后值保存在静态存储区中
 //第二次调用的时候不执行上面操作，值只被初始化一次，不会再次执行初始化，但是可以修改
 number1 = 10;

}

class Time
{
public:

 int Hour;
 int Minute;
 int Second;

public:

 explicit Time(int Hour, int Minute, int Second);
 Time(int Hour);

public:

 //静态成员属性和静态成员函数
 static int myCount; //静态成员函数的声明，没有分配内存，所以在这里不能初始化,在类外面显式初始化
 static void myStaticFunction(int testValue);

}; 

Time:: Time(int TemHour, int TemMinute, int TemSecond) : Hour(TemHour), Minute(TemMinute), Second(TemSecond)//把形式参数赋值给成员属性
{
}
Time:: Time(int Hour)
{

 this->Hour = Hour;

}
//静态成员函数
void Time::myStaticFunction(int testValue)
{

 myCount = testValue;
 //Hour = 12; //error不是静态成员属性，不能在静态成员函数访问

}

//静态成员属性显式初始化-分配内存
int Time::myCount = 0; 
int main(void)
{

 

 Time myTime01 = Time(12, 12, 12);

 Time myTime02(02, 34, 54); 

 Time::myCount = 100;
 cout << myTime01.myCount << endl;

 //如何定义静态成员变量：一般在一个.cpp文件的开头定义静态成员变量。这样可以在任何函数前面进行分配内存。

 //静态成员函数
 //使用类调用
 Time::myStaticFunction(14); 
 //使用类对象调用
 myTime01.myStaticFunction(45); 

 

 system("pause");
 return 0;

}

```
