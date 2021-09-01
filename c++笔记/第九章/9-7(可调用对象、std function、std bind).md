# 可调用对象

```c++
#include<iostream>
#include<cstdlib>

using namespace std;

void myFunciton(int value)
{
	cout << "myFunciton函数执行了" << endl;
}

class TESTCLASS
{
public:
	void operator()(int value)
	{
		cout << "这是类中重载了()运算符的函数执行了" << endl;
	}
	void myfunction02(int value)
	{
		cout << "testclass 类中的myfunction02函数执行了" << endl;
	}
public:
	int m_a;
};

//1.3可被转换为函数指针的类对象_仿函数_函数对象
class testclass02
{
public:
	using tfpoint = void(*)(int);
	static void myStaticFunction(int value)//静态成员函数执行了
	{
		cout << "testclass02静态成员函数执行了" << endl;
	}
	operator tfpoint()//类型转换函数
	{
		return myStaticFunction;//返回的是一个静态函数指针名字
	}
};
int main(void)
{
	//1.1函数指针
	void(*pMyFunc)(int) = myFunciton;//定义一个函数指针并给初值
	pMyFunc(23);//使用函数指针调用函数  这就是一个可调用对象

	//1.2operator()
	TESTCLASS ts;
	ts.operator()(3);
	//等价于下面的调用
	ts(23);

	//1.3可被转换为函数指针的类对象_仿函数
	testclass02 tc2;
	tc2(40);//先调用operator tfpoint,然后调用返回的函数
	//等价于
	tc2.operator testclass02::tfpoint()(40);

	//1.4类成员函数指针
	TESTCLASS tc03;
	void (TESTCLASS::*myfPoint)(int) = &TESTCLASS::myfunction02;//类成员函数指针定义变量并初始化
	//成员函数指针本身是有地址的，不依赖于类对象
	(tc03.*myfPoint)(234);//这里必须有*  ----也是一个可调用对象
	


	
	system("pause");
	return 0;
}
/*
* (1)可调用对象
*	函数和重载了()运算符的类对象
*	1.1函数指针
*	1.2具有operator()成员函数的类对象--重载了()运算符的类对象
*		仿函数的定义：仿函数funtor，他的行为类似于函数的东西(something that performs a function)
*		c++中仿函数是通过在类中重载()运算符实现的。仿函数又被称为函数对象（function obeject），能行使函数功能的类。
*	1.3可被转换为函数指针的类对象_仿函数_函数对象
*	1.4类成员函数指针
*总结：
*	1.所有可调用对象看成一个对象
*	2.我们对其使用()调用运算符，如果a是可调用对象，那么我们可以使用a(para,para...)调用
*	
*/

```

# function()可调用对象包装器

```c++
#include<iostream>
#include<cstdlib>
#include <functional>

using namespace std;

//普通函数可调用对象
void myFunciton(int value)
{
	cout << "myFunciton函数执行了" << endl;
}

class TC
{
public:
	void operator()(int value)
	{
		cout << "这是类中重载了()运算符的函数执行了" << endl;
	}
	void myfunction02(int value)
	{
		cout << "testclass 类中的myfunction02函数执行了" << endl;
	}

	static int stcfunc(int t)
	{
		cout << "类tc中的静态函数执行" << endl;
		return t;
	}
public:
	int m_a=0;
};

//1.3可被转换为函数指针的类对象_仿函数_函数对象
class testclass02
{
public:
	using tfpoint = void(*)(int);
	static void myStaticFunction(int value)//静态成员函数执行了
	{
		cout << "testclass02静态成员函数执行了" << endl;
	}
	operator tfpoint()//类型转换函数
	{
		return myStaticFunction;//返回的是一个静态函数指针名字
	}
};
int main(void)
{
	//1.1绑定普通函数
	std::function<void(int)> f1 = myFunciton;
	f1(123);
	
	//1.2绑定类中的静态成员函数
	std::function<int(int)>f2 = TC::stcfunc;
	cout << f2(234) << endl;

	//1.3绑定仿函数
	TC tc03;
	std::function<void(int)>f3 = tc03;//类中的成员属性要初始化
	f3(23);


	testclass02 tc02;
	std::function<void(int)>f4 = tc02;
	f4(23);
	system("pause");
	return 0;
}
/*
* (1)function()可调用对象包装器
*如何能把各种可调用对象统一一下，使用同一种方法调用？？？
*std::function()可调用对象包装器
*c++11中引入的新标准，std::function是一个类模板，用来装各种可调用对象(不能装类成员函数指针，因为需要类对象参与)
*
*std::function类模板的特点：就是通过给它指定模板参数，他就能够用统一的方式来处理函数。
*	1.绑定普通函数
*	1.2绑定类的静态成员函数
*	1.3绑定仿函数
*
*
*/

```

# function()可调用对象包装器_范例演示1

```c++
#include<iostream>
#include<cstdlib>
#include <functional>

using namespace std;

//普通函数可调用对象
void myFunciton(int value)
{
	cout << "myFunciton函数执行了" << endl;
}

class CB
{
private:
	std::function<void()>fcallBack;
	
public:
	CB(const std::function<void()>&f):fcallBack(f)//引用
	{
		int i = 1;
	}
	//调用回调函数的成员函数
	void runCallBack(void)
	{
		fcallBack();
	}
};

class CT
{
public:
	void operator()(void)//重载函数调用运算符
	{
		cout << "CT类operator函数执行" << endl;
	}
};
int main(void)
{
	//小案例演示
	CT ct;//可调用对象
	CB cb(ct);//cb需要可调用对象做参数来构造，ct因为有operator()所以可以转换为std::function<void()>&对象
	cb.runCallBack();//调用CT中的operator()函数

	system("pause");
	return 0;
}
/*
* (1)function()可调用对象包装器_范例演示1
* 
	CT类operator函数执行
	请按任意键继续. . .

*
*
*/

```

# function()可调用对象包装器_范例演示2

```c++
#include<iostream>
#include<cstdlib>
#include <functional>

using namespace std;

//
void myCallBack(int value,const std::function<void(int)>&f)
{
	f(value);
}

void runfunction(int value)
{
	cout << "value=" << value << endl;
}


int main(void)
{
	//小案例演示
	for (int i=0;i<10;++i)
	{
		myCallBack(i, runfunction);
	}
	system("pause");
	return 0;
}
/*
* (1)function()可调用对象包装器_范例演示2

value=0
value=1
value=2
value=3
value=4
value=5
value=6
value=7
value=8
value=9
*
*
*/

```

# std_bind绑定器

```c++
#include<iostream>
#include<cstdlib>
#include <functional>

using namespace std;

void myFunction(int x,int y,int z)
{
	cout << "x=" << x << endl;
	cout << "y=" << y << endl;
	cout << "z=" << z << endl;
}

void myFunction02(int x, int y, int z)
{
	cout << "x=" << x << endl;
	cout << "y=" << y << endl;
	cout << "z=" << z << endl;
}

void myFunction03(int&x,int&y)
{
	x++;
	y++;
}

int main(void)
{
	auto bf1 = std::bind(myFunction, 1, 2, 3);//auto表示不关心返回的类型，其实bind返回的是一个仿函数类型对象
	bf1();//1  2  3
	//placeholders::_2是占位符
	auto bf2 = std::bind(myFunction02, placeholders::_1, placeholders::_2, 34);
	//表示第一，第二个参数分别由调用者提供，这里只制定了第三个参数
	bf2(1, 2);// 1  2  34
	//直接调用
	std::bind(myFunction02, placeholders::_1, placeholders::_2, 34)(12, 23);


	auto bf3 = std::bind(myFunction02, placeholders::_2, placeholders::_1, 300);
	bf3(1, 2);//2	1	300 按照标号位置传递

	int a = 1;
	int b = 2;
	auto bf4 = std::bind(myFunction03, a, placeholders::_1);
	bf4(b);
	cout << "a=" <<a<< endl;//1不是2
	cout << "b=" <<b<< endl;//3
	/*
	 * 这说明，bind对于预先绑定的函数参数是通过值传递的，所以这里的a实际上是值传递
	 * 对于不预先绑定的参数，通过std::placeholde传递的参数，是通过引用传递的，b实际上是引用传递的。
	 */



	
	system("pause");
	return 0;
}
/*
* (1)std::bind绑定器
* 也是个类模板，是c++11中新特性
* std::bind能够将对象及其相关的参数绑定到一起，绑定完毕后可以直接调用，也可以使用std::function保存，在需要的时候调用
* 格式：
* std::bind(待绑定的函数对象/函数指针/成员函数指针，绑定参数值1，参数绑定值2，参数绑定....)
* 总结：
*	1.将可调用对象可参数绑定在一起，构成一个仿函数，可以直接调用
*	2.如果函数有多个参数，可以只绑定一部分，其他参数在调用时候指定
* (2)
* (3)
* (4)
* (5)
* (6)(7)
*/

```

# std_bind绑定器_绑定成员函数_第二个参数是类对象

```c++
#include<iostream>
#include<cstdlib>
#include <functional>

using namespace std;

class CT
{
public:
	void myFunction(int x,int y)//普通成员函数
	{
		cout << "x=" << x << endl;
		cout << "y=" << y << endl;
		m_a = x;
	}
public:
	int m_a = 0;
};

int main(void)
{
	CT ct;
	auto bf1 = std::bind(&CT::myFunction, ct,std::placeholders::_1, std::placeholders::_2);
	bf1(1, 2);
	/*
	 * bind中使用第二个参数ct会导致调用CT的拷贝构造函数来生成一个CT的拷贝构造函数生成一个临时对象，做为std::bind()的
	 * 返回值（仿函数类型），后续的myFunction调用修改的是这个临时对象的m_a的值，并不影响ct兑现的值。
	 * 解决办法：ct使用引用，就不会生成临时对象，会修改真正ct的值
	 * 
	 */
	auto bf2 = std::bind(&CT::myFunction, &ct, std::placeholders::_1, std::placeholders::_2);
	bf2(1, 2);

	system("pause");
	return 0;
}
/*
* (1)std::bind绑定器_绑定成员函数_第二个参数是类对象
* 
* std::bind(待绑定的函数对象/函数指针/成员函数指针，绑定参数值1，参数绑定值2，参数绑定....)
* 总结：
*	1.将可调用对象可参数绑定在一起，构成一个仿函数，可以直接调用
*	2.如果函数有多个参数，可以只绑定一部分，其他参数在调用时候指定
* (2)
* (3)
* (4)
* (5)
* (6)(7)
*/

```

# std_bind绑定器和function配合使用

```c++
#include<iostream>
#include<cstdlib>
#include <functional>

using namespace std;

class CT
{
public:
	void myFunction(int x, int y)//普通成员函数
	{
		cout << "x=" << x << endl;
		cout << "y=" << y << endl;
		m_a = x;
	}
	void operator()()
	{
		cout << "operator()函数被调用" << endl;
	}
public:
	int m_a = 0;
	
};

void myCallBack(int cs,const std::function<void(int)>&f)
{
	f(cs);
}

void runFunc(int x)
{
	cout << "runFunc函数执行了" <<x<< endl;
}

int main(void)
{
	//(1)std::bind绑定器和function配合使用
	CT ct;
	std::function<void(int, int)> bf1 = std::bind(&CT::myFunction, &ct, std::placeholders::_1, std::placeholders::_2);
	bf1(10, 20);

	//(2)把成员变量地址当函数一样绑定
	std::function<int&()>bf2 = std::bind(&CT::m_a, &ct);//如果第二个参数不用&引用，会调用两次拷贝构造函数
	//第一册是利用ct来产生一个临时对象，第二次是std::bind要返回一个CT类型对象，返回的CT对象拷贝自临时对象，std::bind执行完毕后临时对象会释放。
	bf2() = 53;//实际修改的是成员变量的值


	auto rt = std::bind(CT());//执行构造函数，拷贝构造，两次析构
	/*
	 * CT()构造临时对象，调用拷贝构造函数生成一个临时对象作为std::bind()的返回对象，
	 * bind返回仿函数类型对象，就是调用拷贝构造函数构造出来的对象
	 */

	auto bf4 = std::bind(runFunc, std::placeholders::_1);//调用时候指定第一个参数

	for (int i=0;i<10;i++)
	{
		myCallBack(i, bf4);//调用的runFunc
		
	}
	
	system("pause");
	return 0;
}
/*
* (1)std::bind绑定器和function配合使用
* (2)把成员变量地址当函数一样绑定，绑定结果放在std::function<int&(void)>保存，说白了就是用一个可调用对象类型保存。
* (3)总结：
*		bind思想，所谓的延迟调用，将可调用对象格式统一，保存起来，需要的时候在调用。
*		我们有std::function绑定一个可调用对象，类型成员不能绑定，std::bind成员函数，成员变量都可以绑定
*	2019年12月11日12点22分
* 
* (4)
* (5)
* (6)(7)
*/

```
