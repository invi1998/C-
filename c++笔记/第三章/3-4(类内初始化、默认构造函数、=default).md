（1）类相关非成员函数

（2）类内初始化

（3）const成员变量的初始化

const常量属性的成员属性必须在<font color="#4CAF50">构造函数初始化列表中初始化</font>，在{}后不能给值。或者在定义const常量的时候进行初始化（如果两种初始化操作都存在，那么常量值会以初始化列表的值为准）。
```c++
 Time:: Time(int tempHour):constValue(10) {

     

 }

```

（4）默认构造函数

（5）=default; ，=delete; 

=default: 用于显式要求编译器提供合成版本的四大函数(构造、拷贝、析构、赋值) 
例如：
```c++
class A{
public:
    A() = default;
    A(const A& a) = default;
    A& operator=(A&);
    ~A() = default;
}
```

我们只能对具有合成版本的成员函数使用=default；

=delete, 用于定义删除函数，在旧标准下，我们如果希望阻止拷贝可以通过显式声明拷贝 
构造函数和拷贝赋值函数为private，但新标准下允许我们定义删除函数，例如：

```c++
struct NoCopy{

    NoCopy() = default;
    NoCopy(const NoCopy&) = delete; // 阻止拷贝
    NoCopy& operator=(const NoCopy&) = delete; //阻止赋值
    ~NoCopy() = default;

}

```

=delete可以告知编译器，不生成默认的这些成员。 
这里有两点要注意： 
1、=delete可以使用在任意的函数上(可以引导函数匹配过程)，除了析构函数(析构函数为删除函数将 
导致该类型无法销毁);=default必须用在有默认合成函数的函数上。 
2、=delete必须出现在函数第一次声明的时候。

# 成员属性的类内初始化_常量成员属性初始化

```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
using namespace std; 

class Time
{
public:

	int Hour;
	int Minute={0};
	int Second=0;	//类内初始值
	//常量属性使用构造函数初始化列表方式初始化
	const int TestConst;

public:

	explicit Time(int Hour, int Minute, int Second);
	Time(int Hour);
	Time():TestConst(12) {}

}; 
//友元函数访问类中的私有数据成员。公有数据成员可以直接访问。
void ReadTime(Time&MyTime)
{

	cout << MyTime.Hour << endl;

}

Time:: Time(int TemHour, int TemMinute, int TemSecond) : Hour(TemHour), Minute(TemMinute), Second(TemSecond), TestConst(12)//把形式参数赋值给成员属性
{
}
Time:: Time(int Hour): TestConst(12)
{

	this->Hour = Hour;

}

int main(void)
{

	Time myTime01 = Time(12, 12, 12);

	//类相关的非成员函数调用
	ReadTime(myTime01);

	//类内初始化
	Time myTime02;	//Minute=Second=0  
	
	system("pause");
	return 0;

}

/*
*(1)类相关非成员函数
*	和类几乎无关的成员函数，访问私有数据成员---友元函数
*	访问公有数据成员，可以直接访问
*(2)类内初始化
*	在c++11中我们可以为类内成员变量提供一个初始值。创建对象的时候，使用初始值来初始化该成员变量。
*	
*(3)常量属性的成员属性必须在构造函数初始化列表中初始化，在{}后不能给值。
*
*/

```

# 默认构造函数

```c++
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
    Time(){} // 不带参数的默认构造函数

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

	//Time myTime01 = Time(12, 12, 12);

	Time myTime02;	//调用默认构造函数
	system("pause");
	return 0;

}

/*
*(1)默认构造函数--没有任何参数的构造函数
*	1. 如果没有任何构造函数，系统默认给一个默认构造函数，编译器自动定义一个默认构造函数，这个叫“合成的默认构造函数”
*	2. 一旦我们写了构造函数，系统就不会自动创建默认构造函数。
*(2)当进行类的组合时候，会包含其他类的对象，必须使用成员初始化列表和初始值，手工写自己的构造函数。
*(3)
*
*/

```

# =default =delete自动为构造函数生成函数体

```c++
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
	Time() = default;	//编译器为=default这种函数自动生成函数体
	//Time() = delete;	//让编译器禁止生成某个函数

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

	//默认构造函数
	Time myTime02;	//默认构造函数
	system("pause");
	return 0;

}

/*
*(1)=default =delete

	Time() = default; 编译器能够自动为我们生成函数体，用于特殊的函数
	只能用于构造函数，拷贝构造函数等特殊函数

*(2)Time() = delete; 	//让编译器禁止生成某个函数
*(3)
*
*/
```
