# decltype含义和举例_用于推导类型

```c++
#include<iostream>
#include<cstdlib>
#include <functional>
using namespace std;

class CT
{
public:
	int i;
	int  j;
};


int testFunction()
{
	return 10;
}

const int&&myfunctionTest(void)
{
	return 0;
}
int main(void)
{
	//1.1decltype后的圆括号是个变量
	const int i = 0;
	const int&reference01 = i;
	auto number01 = i;//number01=int 传值方式推断，引用属性和const属性都会被抛弃
	decltype(i) number02 = 23;//number02=const int类型。。i是什么类型，number02就是什么类型
	//如果decltype是个变量，则变量中的const属性会返回。
	decltype(reference01) number03 = number02;//const属性和引用属性都会保留   int const&类型
	//用于类
	decltype(CT::i) number04;//number04=int
	CT temCT;
	decltype(temCT) temCT02;//temCT02=CT类型
	decltype(temCT02.j)number05;//number05=int

	auto &&number06 = i;//万能引用  --auto =int,number06=int&
	decltype(number06) && number07 = i;//这里用到引用折叠，折叠成了左值， int &number07=i;
	
	//1.2decltype后的圆括号是个不是个变量，表达式。返回表达式结果对应类型
	decltype(34)number08 = 5;//number08=5;

	int j = 0;
	int*p1 = &j;
	int&reference02 = j;
	decltype(reference02 + 1)number09 = 454;//j=int ()内是整形表达式
	decltype(p1)p2=&j;//p2=int*类型
	*p2 = 43;
	decltype(*p2) number10 = j;
	/*
	 *number10=int&类型
	 **p2指针p2所指向的内存空间，而且能够给内存空间赋值，所以是个左值
	 **p2是个表达式，不是个变量，如果表达式的结果能作为赋值语句左边的值，那么decltype后返回的就是个引用。
	 *专门记：
	 *decltype后边是个非变量表达式，并且表达式能够作为等号左边的内容，返回的就是一个类似int&的引用。
	 */
	//在变量名字外面多加一层括号，编译器会把变量当作一个表达式
	decltype((i))reference03 = j;//最终结果是个引用
	//结论：decltype((变量))的结果永远是个引用

	//1.3decltype后的圆括号是个函数
	decltype(testFunction())temv = 14;//temv是函数的返回值类型，但是不会真正调用函数
	decltype(testFunction)temv2;//不加()返回的是int(void),是一个可调用函数对象。

	function<decltype(testFunction)> func01 = testFunction;//声明一个function（）函数类型，代表一个可调用对象  int(void)
	cout << func01() << endl;//10

	decltype(myfunctionTest()) myreturn = 0;//const int&&类型 函数返回值

	


	
	system("pause");
	return 0;
}
/*
* (1)decltype含义和举例：用于推导类型
*	1 decltype对于给定的变量名或者表达式，decltype能够告诉你该名字或者表达式的类型
*		从表达式的类型推断出要定义的变量类型，并且不想初始化变量--使用decltype
*		decltype说明符：主要作用：返回操作数的数据类型
*
*		decltype的特点：
*			1.decltype的自动类型推断会发生在编译期间
*			2.decltype不会真正计算表达式的值
*	1.1decltype后的圆括号是个变量
*	1.2decltype后的圆括号是个不是个变量，表达式。返回表达式结果对应类型
*	1.3decltype后的圆括号是个函数--返回值是函数返回值类型
* declare声明
* (2)
* (3)
* (4)
* (5)
* (6)(7)
*/

```

# decltype的主要用途_函数模板中

```c++
#include<iostream>
#include<cstdlib>
#include <vector>

using namespace std;

//2.1应付可变类型--一般应用于模板类型编程中
template<typename T>
class CT
{
public:
	//typename T::iterator iter;//迭代器类型
	decltype(T().begin())  iter;//如果T类型是const std::vector<int>，返回常量迭代器类型
	//const std::vector<int>()表示生成该类型的临时对象(加括号)，调用临时对象的begin()，得到一个常量迭代器
	void getBegin(T&tem)
	{
		iter = tem.begin();
	}
public:
	CT(){}
};

//类模板偏特化
//template<typename T>
//class CT<const T>
//{
//public:
//	typename T::const_iterator iter;//迭代器类型
//	void getBegin(T&tem)
//	{
//		iter = tem.begin();
//	}
//};

int main(void)
{
	//2.1应付可变类型--一般应用于模板类型编程中
	//using conttype =std::vector<int>;
	using conttype =const std::vector<int>;
	
	conttype myVector = { 12,23,34 };//定义该变量类型，myVector是个容器
	CT<conttype>ct;
	ct.getBegin(myVector);

	//常来容器遍历使用常量迭代器
	const std::vector<int> myvector02 = { 23,345,45 };
	vector<int>::const_iterator iter = myvector02.begin();
	
	system("pause");
	return 0;
}
/*
* (1)decltype的主要用途
*		2.1应付可变类型--一般应用于模板类型编程中
*			生成临时对象使用decltype获取类型
* (2)
* (3)
* (4)
* (5)
* (6)(7)
* 错误，vs2015没能编译通过，不知道vs2017能否编译通过
*	二进制“=”: 没有找到接受“std::_Vector_const_iterator<std::_Vector_val<std::_Simple_types<int>>>”类型的右操作数的运算符(或没有可接受的转换)	others	e:\projectcode\第9章未归类知识点\others\源15.cpp	17	

*/

```

# 使用括号()构造类的临时对象调用类中的成员函数

```c++
#include<iostream>
#include<cstdlib>

using namespace std;
class A
{
public:
	A()
	{
		cout << "执行A的构造函数" << endl;
	}
	~A()
	{
		cout << "执行A的析构函数" << endl;
	}
	int myFunction()
	{
		cout << "指向A的myFunction函数" << endl;
		return 0;
	}
};

int main(void)
{
	A().myFunction();//生成一个临时对象，调用临时对象的函数

	(const A()).myFunction();//ok,但是resharp会提示错误

	decltype(A().myFunction()) number01;//number01的类型是int类型
	//没有构造过临时对象，也没有真正调用过函数
	/*
	 * 执行A的构造函数
		指向A的myFunction函数
		执行A的析构函数
		执行A的构造函数
		指向A的myFunction函数
		执行A的析构函数
	 */
	system("pause");
	return 0;
}
/*
* (1)使用括号()构造类的临时对象调用类中的成员函数
* (2)
* (3)
* (4)
* (5)
* (6)(7)
*/

```

# decltype的主要用途_通过变量表达式抽取变量类型

```c++
#include<iostream>
#include<cstdlib>
#include <vector>

using namespace std;


int main(void)
{
	//2.2decltype的主要用途_通过变量表达式抽取变量类型
	vector<int> myV;
	myV.push_back(1);
	myV.push_back(2);
	myV.push_back(3);
	vector<int>::size_type mySize = myV.size();
	cout << mySize << endl;//3
	decltype(myV)::size_type mysize02 = mySize;//抽取myV的类型，
	cout << mysize02 << endl;//3

	typedef decltype(sizeof(0)) size__t;//等价与size__t是int类型
	/*
	 * typedef decltype(sizeof(int)) size__t
	 * typedef unsigned int size__t
	 */

	system("pause");
	return 0;
}
/*
* 2.2decltype的主要用途_通过变量表达式抽取变量类型
*
*/

```

# decltype的主要用途_auto结合decltype构成返回类型后置语法

```c++
#include<iostream>
#include<cstdlib>
#include <vector>

using namespace std;

//后置语法
auto func(int a,int b)->int
{
	return 0;
}

auto add(int i,int k)->decltype(i+k)
{
	return i + k;
}

int&myfunction01(int&i)
{
	return i;
}
double myfunction01(double&i)
{
	return i;
}

//函数模板
template<typename T>
auto myFunTemplate(T&tv)->decltype(myfunction01(tv))//返回类型后置，要现有tv，才能用tv，这里必须用返回类型后置这种用法
{
	return myfunction01(tv);
}

int main(void)
{
	//2.3decltype的主要用途_decltype(auto)用法
	int i = 12;
	double d = 23.34;
	cout << myFunTemplate(i) << endl;//12
	cout << myFunTemplate(d) << endl;//23.34

	
	system("pause");
	return 0;
}
/*
* (2)2.3decltype的主要用途_auto结合decltype构成返回类型后置语法
*auto这里没有自动类型推断的意思，只是返回类型后置语法的组成部分
* 
*/

```

# decltype的主要用途_decltype(auto)用法_用于函数的返回类型

```c++
#include<iostream>
#include<cstdlib>
#include <vector>

using namespace std;

template<typename T>
T& DoubleMyself(T&value)
{
	value *= 2;
	return value;
}


template<typename T>
decltype(auto) DoubleMyself02(T&value)//使用auto推断返回值结果
{//把auto理解成要推导的类型，推导过程理解为我们采用decltype推导
	value *= 2;
	return value;
}

int main(void)
{
	//2.4 decltype的主要用途_decltype(auto)用法
	//1.用于函数返回类型
	int number01 = 100;
	DoubleMyself(number01) = 20;//返回的是左值引用，所以可以赋值
	cout << "number01=" << number01 << endl;//20
	//使用auto推断返回值结果,DoubleMyself02的返回值结果是int，是一个右值，不能给右值赋值。加上decltype ok
	DoubleMyself02(number01) = 43;
	cout << number01 << endl;//43

	decltype(DoubleMyself02(number01))number02 = number01;
	cout << "number02=" << number02 << endl;//43
	
	system("pause");
	return 0;
}
/*
*2.4 decltype的主要用途_decltype(auto)用法
*c++14中存在这种语法，编译器要支持c++14语法
*	1.用于函数返回类型
* (3)
* (4)
* (5)
* (6)(7)
*/

```

# decltype的主要用途_decltype(auto)用法_用于变量的声明中_保存const和引用属性

```c++
#include<iostream>
#include<cstdlib>

using namespace std;
decltype(auto) myFunction()
{
	int i = 1;
	return i;
}

decltype(auto)myFunciton02()
{
	int i = 1;
	return(i);//加了()就会变成导致返回一个引用 int&,但是i是局部变量，使用会导致不可预料的结果。
	
}

int main(void)
{
	//用于变量的声明中
	int number01 = 12;
	const int &reference = 23;
	auto number02 = reference;//const属性和引用都没有传递
	decltype(auto)number03 = reference;//number03=const int& .auto丢掉的东西（const和引用），能够通过decltype(auto)捡回来

	//1.3再说(x)
	int number04 = 234;
	decltype((number04))rere = number04;//rere=int&类型

	decltype(myFunction()) number05 = 23;//int类型
	int a = 1;
	decltype(myFunciton02())number06 = a;//这里调用函数返回的是int&类型，但是i是局部变量
	myFunciton02() = 234;//语法上没问题，但是局部量i已经被系统回收了，会导致未预料的结果
	
	system("pause");
	return 0;
}
/*
* (1)decltype的主要用途_decltype(auto)用法
*	1.2用于变量的声明中_保存const和引用属性
*	1.3再说(x) 使用不当decltype会导致返回局部引用
* (2)decltype总结
*	
* (3)
* (4)
* (5)
* (6)(7)
*/

```
