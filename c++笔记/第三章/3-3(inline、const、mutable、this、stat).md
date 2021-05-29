（1）在类定义中实现成员函数inline
（2）成员函数末尾的const
（3）mutable
（4）返回自身对象的引用，this
（5）static成员

# 在类定义中实现成员函数inline

```c++
/*
*(1)在类定义中实现成员函数inline，类内的成员函数实现其实也叫类的成员函数定义。
*	AddInline()这种函数直接在类的定义中实现的成员函数，会被当做inline内联函数来处理。
*	能不能内联成功，取决于具体的编译器。
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
*	作用：告诉系统，这个成员函数，不会修改该对象里面任何成员变量的值，
*		也就是说，这个成员函数不会修改类Time的任何状态。
*		后面加一个const的成员函数也叫常量成员函数
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
	//Hour = Hour + TemHour;	//error不能修改成员变量的属性
	
}

int main(void)
{
	Time myTime01 = Time(12, 12, 12);
	const Time myTime02;	//定义const对象，这种对象一旦初始化就不能修改。
	myTime02.AddHour(12);
	//myTime02.test();	//不兼容的修饰符号，不能调用非const函数。
	
	/*1.s声明为const的对象不能调用普通的成员函数
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
*	mutable的引入就是为了突破const的限制。
*	当成员函数后面有const，是不能修改成员变量的值，如果想在const常成员函数中修改成员变量的值。就是用mutable。
*(2)mutalbe是为了突破const限制而开的一个后门。
*2019年11月16日21:15:45
* Sunrise于东北电力大学第二教学楼1121实验室
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

	mutable int Hour;	//mutable修饰
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
*	每一个成员对象都有一个隐含的成员属性this指针，指向自己本身的地址，当进行函数调用的时候，把this指针
*	（自己本身的地址）当做第一个参数传递过去，后面依次是自己写的参数。
*	对系统看来，任何对成员属性的访问都是通过this指针来访问的。
*(3)
*	1.隐藏的this指针只能在成员函数中，全局函数和静态函数（static）中不存在this指针
*	2.在普通成员函数中，this是一个指向非const对象的const指针。this只能指向当前对象，指向不能改变。
*	3.在const成员函数中，this指针就是一个指向const对象的const指针。const Time *const this
*
*总结：
*	返回对象本身可以实现函数连续调用，多值函数
*2019年11月16日21:18:54
* Sunrise于东北电力大学第二教学楼1121实验室
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
	Time&AddHour(int TemHour);
	//Time&AddHour(Time *const this,int TemHour);	//等价于这种，隐藏this指针
	Time&AddMinute(int TemMinute);
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
	//Hour = (Hour + TemHour) % 24;	//时间是0-24小时
	this->Hour = (this->Hour + TemHour) % 24;
	cout << this->Hour << endl;	//10
	return *this;	//返回对象自身
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
*	属于整个类的成员变量叫静态成员变量，static成员变量
*	特点：不属于某个对象，而是属于整个类，一旦在某个对象中修改了static成员变量的值。
*		其他对象可以直接访问。
*		2。这种成员函数只有一个副本，使用时  类名：：成员变量名
*		3. 成员函数前面也可以加static，构成静态成员函数，属于类的成员，调用时候
*			类名:: 成员函数名字
*			静态成员函数只能操作静态成员属性.
*(3)
*
*/
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
using namespace std; 
static int number02 = 10; 	//全局静态变量的作用域是定义的文件。
//限制在本文件中，不能在其他文件中使用extern来声明调用。
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
	static int myCount;	//静态成员函数的声明，没有分配内存，所以在这里不能初始化,在类外面显式初始化
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
	//Hour = 12;	//error不是静态成员属性，不能在静态成员函数访问

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
