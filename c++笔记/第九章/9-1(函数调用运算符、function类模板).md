（1）学习c++体会、总述
（2）不同调用对象的相同调用形式
（3）标准库function类型介绍
（4）总结

重载（）函数对象
```c++
#include<iostream>
#include<cstdlib>

using namespace std;

void myFunction(int a_i)
{
	cout << "这是我自定义的函数myFunction" << endl;
	return;
}

//类实现的功能：返回一个大于等于0的数字，如果小于0，返回0
class BiggerThanZero
{
public:
	//带一个参数的构造函数
	BiggerThanZero(int a):m_i(a)
	{
		m_i = a;
	}
	
public:
	//重载()函数调用运算符
	int operator()(int value)const
	{
		if (value<=0)
		{
			return 0;
		}
		else
		{
			return value;
		}
	}

	int operator()(int a,int b)const
	{
		return 1;
	}
private:
	int m_i;
};

int main(void)
{
	myFunction(3);//()--函数调用运算符

	//类中重载函数调用运算符
	BiggerThanZero b1(23);//重载了函数调用运算符的对象,这里是对象定义并初始化（因为前面有类名），所以调用的是构造函数
	int number01=b1(-34);
	//重载的函数，operator 运算符就是函数名字  operator ()
	int number02 = b1.operator()(-45);

	//重载函数调用运算符--两个参数
	int number03 = b1(12, 33);
	
	
	system("pause");
	return 0;
}
/*
 *(1)学习c++体会、总数
 *	1.对语言本身的学习--学习语法，不可或缺的阶段
 *	2.大量的练习--有条件尽量去实践
 *	3.开始阅读优秀的人写的优秀代码--每个人写的代码有不同的习惯,每个人都有自己的习惯和擅长。
 *		
 *(2)圆括号()就是函数调用的明显标记,()有一个称呼叫“函数调用运算符”;
 *	如果我们在类中重载了函数调用运算符()，那么我们就可以像使用函数一样使用该类的对象了。对象名(实参)
 *	如何定义一个函数调用运算符？？
 *		1.定义一个该类的对象。
 *		2.像函数调用一样使用该对象，也就是在()中添加实参列表
 *(3)结论：
 *	只要这个对象所属的类重载了()"函数调用运算符"，那么这个类对象就变成可调用的了
 *	而且可以调用多个版本的()，只要在参数类型或者数量上构成函数重载即可。
 *(4)类重载了（），那该类就有一个新名字--"函数对象”,因为可以调用这种对象，或者说
 *	这些对象的行为像函数一样。
 *
 *
 *
*/
```

function函数模板与函数指针

```c++
#include<iostream>
#include<cstdlib>
#include <map>
#include <string>
#include <functional>

using namespace std;
typedef int (*myfunction)(int);//定义一个函数指针类型，这种指针可以指向返回值是int类型，且只有一个int类型参数的函数

//类实现的功能：返回一个大于等于0的数字，如果小于0，返回0
class BiggerThanZero
{
public:
	//带一个参数的构造函数
	BiggerThanZero(int a) :m_i(a)
	{
		m_i = a;
	}

public:
	//重载()函数调用运算符
	int operator()(int value)const
	{
		if (value <= 0)
		{
			return 0;
		}
		else
		{
			return value;
		}
	}


private:
	int m_i;
};

//普通函数,打印value值，并原样返回.--调用参数和返回值与类型中重载()相同
int echoValue(int value)
{
	cout << value << endl;
	return value;
}

int main(void)
{
	//通过函数指针调用函数
	myfunction myfucntionP1 = echoValue;//这里可以不加&
	int number01 = myfucntionP1(-23);
	int number02 = (*myfucntionP1)(234);//这里也可以不加*

	//通过map保存可调用对象的指针
	map<string, int(*)(int)>myMap;//这个map只能装函数指针类型
	myMap.insert({ "aa", echoValue });//插入一个函数指针

	BiggerThanZero b1(123);//含有函数调用运算符的对象
	//myMap.insert({ "bb",b1.operator() });//error,语法错误
	//通过function来声明一个可调用对象
	//function<int(int)>//声明了一个function()类型，用来代表一个可调用对象，代表的是返回值是int类型，接受一个int类型参数
	function<int(int)>f1 = echoValue;//函数指针，这里echoValue有函数重载，就会找不到地址，报错
	function<int(int)>f2 = b1;//放类对象也ok，类中有运算符重载
	function<int(int)>f3 = BiggerThanZero(12);//创建一个匿名对象来初始化一个函数模板对象
	f1(43);
	f2(-23);
	f3(-23);


	map<string, function<int(int)>>myMap02 = { {"aa",echoValue},{"bb",b1},{"cc",BiggerThanZero(23)}};
	myMap02["aa"](12);//容器名["键"]==值
	myMap02["bb"](234);
	myMap02["cc"](-213);
	
	system("pause");
	return 0;
}
/*
*(1)不同调用对象的相同调用形式--function模板
*int echoValue(int value)和类中的int operator()(int value)const，参数和返回值相同，就叫做“调用形式相同”
*一种调用形式对应一个函数类型（函数类型--返回值和参数）
*
*(2)可调用对象
*	eachValue()函数叫可调用对象
*	重载了函数调用运算符类对象
*	我们可以把这些可调用对象的指针保存起来，目的是方便我们随时调用这些可调用对象，类似于我们c语言中的函数指针
*	通过map键值对来保存，用字符串做键，用函数指针做值。
*
*(3)标准库functional类型介绍
*	function 类模板，要提供模板参数来表示该function类型能表示的调用形式
*
*注意：
*	如果函数有重载，就无法通过function来调用，解决办法是使用函数指针来解决。

*/
```