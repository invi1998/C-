# lambda表达式和捕获列表

```c++
#include<iostream>
#include<cstdlib>

using namespace std;

class CT
{
public:
	int m_i;
	void myFunction(int x,int y)
	{
		auto mylambda01 = [this]()//=或者&默认就有this访问
		{
			return m_i;//因为前面已经有this，访问合法
		};
	}
};

int main(void)
{
	auto f=[](int a=45)->int//后置返回类型  ，并且擦拭农户可以有默认值
	{
		return a + 1;
	};//这里必须有分号
	//调用lambda表达式
	cout << f(12) << endl;//13

	static int numnber01 = 12;
	auto f1=[]()
	{
		return  numnber01;//需要捕获前面定义的number01,可以直接捕获静态局变量
	};
	//[&]
	int number02 = 02;
	auto f2=[&]()
	{
		number02++;
		return number02;
	};
	cout << f2() << endl;//3
	//2.3 [=]
	auto f3 = [=]()
	{
		cout << number02 << endl;
		//number02 = 12;//error 不能修改
	};
	//2.4[this]
	CT ct;
	ct.myFunction(3, 4);

	//2.5[变量名字1，变量名字2...]
	int number03 = 3;
	auto f4 = [number02]()
	{
		cout << number02 << endl;
		//cout << number03 << endl;//error
	};
	system("pause");
	return 0;
}

/*lambda表达式和捕获列表
* (1)c++11中的新特性，也是一种可调用对象
* lambda表达式，定义了一个匿名函数，并且可以捕获一定范围内的变量。[]--标志是一个lambad表达式
* 特点：
*	1.是个匿名函数，也可以理解为“可调用代码单元”，或者“未命名的内联函数”。
*	2.也有一个返回类型，一个参数列表，一个函数体
*	3.与函数不同的是，lambda可以在函数内部定义，常规函数不允许。
*格式：
*auto[捕获列表](参数列表)->返回类型后置
*{函数体}
*这是一个返回类型后置语法，lambda表达式的返回类型后置是强制要求，语法就是这么要求的。
*因为很多时候lambda表达式返回值特别明显，所以允许lambda表达式返回值类型省略。编译器可以自动推导。
*
*lambda表达式参数列表可以有默认值。
*
*1.2参数列表中没有参数的时候，参数列表可以省略，甚至()可以省略，如下格式都合法：
*auto f1=[](){return 1;}
*auto f2=[]{return 1;}
*
*1.3捕获列表[]和函数体{}不能省略。
*
*1.4lambda调用方法和普通函数相同，都是使用()这种函数调用运算符。
*1.5lambda表达式可以不返回任何类型给你，不返回任何类型就是void类型。
*auto f3=[]{};//ok
*1.6函数体末尾的分号不能省略。
* (2)捕获列表[]--通过捕获列表来捕获一定范围内的变量
*	2.1[]空表示不捕获任何变量,但是不包括静态局部变量。
*	2.2[&] 表示以"引用"方式捕获外部作用域中的所有变量。
*	2.3[=]表示以“值传递”的方式捕获外部变量，只能使用，修改不了外部变量。
*	2.4[this]一般用于类中，捕获当前类中this指针，让lambda表达式有和当前类成员函数具有同样的
*		访问权限。如果中括号中已经使用&，或者=，就已经默认使用了this，就是让lambda使用当前类中的成员变量。
*		用在类中定义lambda匿名函数
*	2.5[变量名字1，变量名字2...]多个变量使用逗号分开，按照值捕获列出的变量，没有列出的变量，不捕获。
*	2.6[&变量名字1，&量名字2...]用引用捕获多个变量，同时不捕获没有蕾出的变量
*	2.7[=,&变量名]  按值捕获所有外部变量，同时按应用捕获&列出的变量。=必须写在开头位置，第一个位置表示默认捕获方式。
*	2.8[&,变量名字] 按引用捕获外部变量，同时按值捕获指定的外部变量。
*总结：lambda表达式对于外部捕获控制特别细致。
*	
* (3)
* (4)
* (5)
* (6)(7)
*/

```

# lambda表达式延迟调用易出错细节分析

```c++
#include<iostream>
#include<cstdlib>

using namespace std;

int main(void)
{
	int x = 5;
	auto f1=[=]()//在捕获的这一时刻，x的值已经复制到lambda里面了
	{
		return x;
	};
	x = 10;
	cout << f1() << endl;//5,不是我们认为的10.
	//凡是按值捕获的外部变量，在lambda表达式定义的时刻就被赋值一份存储在lambda表达式中，这里输出的是lambda
	//表达式内部的值输出。这里的解决办法就是使用&引用
	system("pause");
	return 0;
}
/*
* (1)lambda表达式延迟调用易出错细节分析
* 从这个例子理解lambda的延迟调用
* 
* (2)
* (3)
* (4)
* (5)
* (6)(7)
*/


```

# Lambda表达式中的mutable

```c++
#include<iostream>
#include<cstdlib>

using namespace std;

int main(void)
{
	int x = 10;
	auto f1 = [=]()mutable //加上mutable可以更改x的值，虽然是按值捕获的x
	{
		x = 234;
	};
	
	
	system("pause");
	return 0;
}
/*
* (1)ambda表达式中的mutable
* mutable容易改变的，易变的
* 不管是不是一个const属性的变量，加上mutable就可以修改它的值。
* (2)
* (3)
* (4)
* (5)
* (6)(7)
*/

```

# lambda表达式的类型和存储_创建匿名类对象，捕获列表当作类成员属性

```c++
#include<iostream>
#include<cstdlib>
#include<functional>


using namespace std;

int main(void)
{
	auto f1 = [] {};//f1就相当于一个未命名的类 类型对象

	int x = 5;
	auto f2 = [=]()
	{
		return x;
	};//f2要生成一个类类型变量，这些捕获的外部变量要在类内部作为成员变量存在。

	//lambda和function结合使用例子
	std::function<int(int)>f3 = [](int value)->int
	{
		value++;
		return value;
	};
	cout << f3(15) << endl;//16

	std::function<int(int)>f4 = std::bind(
		[](int value)
	{
		return value + 1;
		
	},placeholders::_1
	);
	//bind第一个参数是可调用对象或者函数指针，第二个参数是函数参数
	cout << f4(23) << endl;//24

	//捕获列表为空的lambda表达式，可以转换成一个普通的函数指针。
	using funcpType = int(*)(int);//定义一个函数指针类型
	funcpType fp = [](int value) {return value + 1; };
	cout << fp(123) << endl;//124
	
	system("pause");
	return 0;
}
/*
* (1)lambda表达式的类型和存储
* c++11中lambda表达式的类型被成为“闭包类型（closure type）”。
* 闭包--函数内部的函数（可调用对象），本质上就是lambda表达式创建的运行时期的对象。
* lambda表达式是一种比较特殊的，匿名的，类 类型（闭包类）的对象。
* 我们也可以认为他是一个带有operator()类型的对象。也就是仿函数（函数对象）
*
* 所以我们可以使用std::function  std::bind来保存和调用lambda表达式，每个lambda都会
* 触发编译器生成一个独一无二的类类型。
* 
* lambda表达式这种语法我们可以就地定义匿名函数，就地封装短小的功能闭包。
* 
* (2)语法糖的概念
*	就是一种便捷写法
*	int a[4];
*	a[0]=5;//编译器内部的写法*(a+0)=5;
*	语法糖的目的就是让我们写的代码更加简单，容易理解，减少代码出错的机率。
*	语法糖是基于语言现有的特性，构建出一个东西，使用起来更简单，但是没有增加语言的原有功能。
*	lambda表达式可以堪称定义仿函数闭包（函数中函数）的语法糖。
* (3)
* (4)
* (5)
* (6)(7)
*/

```

# for_each()例子

```c++
#include<iostream>
#include<cstdlib>
#include<functional>
#include <algorithm>
#include <vector>
using namespace std;

void myFunction(int i)
{
	cout << "i=" << i << endl;
}

int main(void)
{
	vector<int>myVector = { 10,20,30,40 };
	for_each(myVector.begin(), myVector.end(), myFunction);//起始迭代器，结束迭代器，操作
	
	system("pause");
	return 0;
}
/*
* for_each()是个函数模板
* (2)
* (3)
* (4)
* (5)
* (6)(7)
*/

```

# for_each()使用lambda表达式

```c++
#include<iostream>
#include<cstdlib>
#include<functional>
#include <algorithm>
#include <vector>
using namespace std;

void myFunction(int i)
{
	cout << "i=" << i << endl;
}

int main(void)
{
	vector<int>myVector = { 10,20,30,40 };
	int iSum = 0;
	for_each(myVector.begin(), myVector.end(), [&iSum](int value)
	{
		cout << "value=" << value << endl;
		iSum += value;
	});
	cout << "iSum=" << iSum << endl;//100
	system("pause");
	return 0;
}
/*
* for_each()使用lambda表达式
* (2)
* (3)
* (4)
* (5)
* (6)(7)
*/

```

# find_if()用法

```c++
#include<iostream>
#include<cstdlib>
#include<functional>
#include <vector>
#include <algorithm>
using namespace std;

int main(void)
{
	vector<int>myVector = { 10,20,30,40 };
	auto result = find_if(myVector.begin(), myVector.end(), [](int value)
	{
		cout << "value=" << value << endl;
		if (value>20)
		{
			return true;//返回true停止遍历。
		}
		return false;//只要返回false就一直遍历，遍历完毕为止。一直到返回true或者遍历完毕位置
		//find_if第三个参数这个可调用对象返回true，find_if就停止遍历。
		//find_if返回一个迭代器，指向第一个满足条件的元素，如果不存在这样的元素，则这个迭代器会指向myVector.end()
	});
	system("pause");
	return 0;
}
/*
* (1)find_if()
* 函数模板，按条件查找，按照第三个参数给定的函数对象查找
* 
* (2)总结：多学习别人写的代码学习find_if()用法
* 
* (3)
* 
* (4)
* 
* (5)
* 
* (6)
* 
* (7)
*/

```
