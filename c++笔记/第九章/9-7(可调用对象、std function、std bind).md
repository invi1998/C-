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

```c++
/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <functional>
#include <vector>

using namespace std;

void func1(int v)
{
    std::cout << "void func1(int v); v = " << v << std::endl;
}

class TC
{
    public:
        void operator()(int tv)
        {
            std::cout << "void operator()(int tv)， TV = " << tv << std::endl;
        }
        void putong(int tv)
        {
            std::cout << "void putong(int tv)； tv = " << tv << std::endl;
        }
        int m_i = 100;
        
    public:
        static int stcFunc(int tv)
        {
            std::cout << "static int stcFunc(int tv)" << tv << std::endl;
            return tv;
        }
};

class TC2
{
    public:
        using fpoint = void(*)(int); // 定义一个函数指针fpoint
        static void mysfunc(int tv)
        {
            std::cout << "static void mysfunc(int tv), tv = " << tv << std::endl;
        }
        operator fpoint()
        {
            return mysfunc;
        }
};

class CB
{
    std::function<void()>funcCallback;
    
    public:
        CB(const std::function<void()> &fv):funcCallback(fv)
        {
            std::cout << "CB(const std::funcCallback<void()> &fv):funcCallback(fv)" << std::endl;
        }
        
        void runCallback(void)
        {
            funcCallback();   
        }
};

class CT
{
    public:
        void operator()(void)
        {
            std::cout << "void operator()(void)" << std::endl;
        }
};

void myCallBack(int cs, const std::function<void(int)> &f)
{
    f(cs);
}

void run(int x)
{
    std::cout << x << std::endl;
}

void myfuncBind(int x, int y, int z)
{
    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
    std::cout << "z = " << z << std::endl;
}

void func4(int &a, int &b)
{
    a++;
    b++;
}

class CTC
{
    public:
        CTC()
        {
            std::cout << "CTC()" << std::endl;
        }
        
        ~CTC()
        {
            std::cout << "~CTC()" << std::endl;
        }
        
        CTC(const CTC&temv):m_i(temv.m_i)
        {
            std::cout << "CTC(const CTC&temv):m_i(temv.m_i)" << std::endl;
        }
        
        void operator()()
        {
            std::cout << "void operator()()" << this << std::endl;
        }
        
        void func(int v1, int v2) // 普通成员函数
        {
            std::cout << "v1 = " << v1 << ", v2 = " << v2 << std::endl;
            m_i = v1;
        }
        int m_i = 0;
};

int main()
{
    // 可调用对象
    // 函数，重载()运算符类的对象
    // 1）函数指针
    void(*pmf1)(int) = func1; // 定义一个函数指针pmf1，并给初值func1 
    // void(*pmf1)(int) = &func1; // 这两种写法一样,加不加&(地址符)效果一样
    pmf1(535);  // 调用函数，这就是一个可调用对象
    // void func1(int v); v = 535
    // 2）具有operator()成员函数的类对象（仿函数）
    // 仿函数 （functor） 行为类似于一个函数的行为（something that performs a function）
    // c++11中，仿函数是通过在类中重载()运算符实现，所以仿函数有称为函数对象（function object）
    // 能行使函数功能的类
    
    TC tc1;
    tc1(788); // 调用（）操作符，这也是一个可调用对象
    // void operator()(int tv)， TV = 788
    // 等价于 
    tc1.operator()(788);
    // void operator()(int tv)， TV = 788
    
    //  3)可被转换为函数指针的类对象（仿函数/函数对象）
    TC2 tc2;
    tc2(588);   // 先调用fpoint,再调用mysfunc()，这个也是一个可调用对象
    // static void mysfunc(int tv), tv = 588
    // 等价于
    tc2.operator TC2::fpoint()(346);
    // static void mysfunc(int tv), tv = 346
    
    // 4）类成员函数指针
    TC tc3;
    void(TC::*myfuncPointTC)(int) = &TC::putong;
    // 定义一个 类成员函数指针变量 myfuncPointTC 并给初始值
    (tc3.*myfuncPointTC)(8888);
    // void putong(int tv)； tv = 8888
    
    // 总结：
    // 1）看成对象
    // 2）我们可以对其使用()调用运算符，如果a是可调用对象，那么我们就可以编写a(parm...)代码
    // 
    
    // 如何把各种不同形式的可调用对象的调用形式统一一下？方便调用
    // std::function(可调用对象包装器) c++11
    // std::function是一个类模板，用来装各种可调用对象
    // 不能装类成员函数指针
    // std::function类模板的特点，就是能够通过给他指定模板参数，他就能够用统一的方式来处理函数
    // 1）绑定普通函数
    std::function<void(int)> f1 = func1;
    f1(222);
    // void func1(int v); v = 222
    // 2) 绑定类的静态成员函数
    std::function<int(int)> f2 = TC::stcFunc;
    std::cout << f2(1123) << std::endl;
    // static int stcFunc(int tv)1123
    // 1123
    
    // 3）绑定仿函数
    TC tc4;
    std::function<void(int)> f3 = tc4;
    f3(999);
    // void operator()(int tv)， TV = 999
    
    TC2 tc5;
    std::function<void(int)> f4 = tc5;
    f4(10101010);
    // static void mysfunc(int tv), tv = 10101010
    
    // 范例演示
    CT ct;  // 可调用对象
    CB cb(ct);  // cb需要可调用对象来做参数进行构造，ct因为有operator()，所以可以转为可调用对象
    cb.runCallback();
    // CB(const std::funcCallback<void()> &fv):funcCallback(fv)
    // void operator()(void)
    
    for(auto i = 0; i < 3; ++i)
    {
        myCallBack(i, run); 
        // 0
        // 1
        // 2
    }
    
    
    // std::bind
    // std::bind绑定器，他也是一个类模板， c++11引入的
    // 他能将对象及其相关的参数绑定到一起，绑定完之后可以进行直接调用，也可以用std::function进行保存，在需要的时候进行调用
    // 格式
    // std::bind(待绑定的函数对象/函数指针/成员函数指针, 参数绑定值...)
    // 1）将可调用对象和参数绑定到一起，构成一个仿函数，所以可以直接调用
    // 2）如果函数有多个参数，可以只绑定一部分参数，其他参数在调用的时候载补充完整（指定）
    auto bf1 = std::bind(myfuncBind, 12, 23, 34);
    // auto表示不关心bind返回类型，其实这里bind返回的是一个仿函数对象，可以直接调用，也可以赋给std::function
    bf1(); // 因为我们在上文中已经把所有形参进行了绑定，这里就可以不用传任何参数直接调用
    
    auto bf2 = std::bind(myfuncBind, placeholders::_1, placeholders::_2, 34);
    bf2(99, 88);
    // placeholders 占位符
    // 表示绑定函数myfuncBind的第三个参数绑定为34， 但是第一个和第二参数由调用 bf()的时候的第一个和第二参数来指定
    // _1, _2这些是标准库里定义的，占位符的含义
    // 这里placeholders::_1 表示这个位置的参数（当前placeholders::_1所在位置）将由函数调用时传入的第一个参数代替
    // placeholders::_2同理
    std::bind(myfuncBind, placeholders::_1, placeholders::_2, 34)(2222,33333); // 直接bind调用
    std::bind(myfuncBind, placeholders::_2, placeholders::_1, 34)(2222,33333); // 调用顺序 33333， 2222， 34
    
    
    int q = 100;
    int p = 200;
    
    auto bf4 = std::bind(func4, q, placeholders::_1);
    bf4(p);
    std::cout << "q = " << q << " p = " << p << std::endl;
    // q = 100 p = 201
    // 这说明bind对于预先绑定的函数参数是通过值传递的，所以这个q（值传递）的值不会改变
    // bind对于不事先绑定的参数，通过placeholders::n来进行绑定的参数，是通过运用传递的，所以这里p是引用传递
    
    
    // bind绑定成员函数
    CTC ctc;
    auto bf5 = std::bind(&CTC::func, ctc, placeholders::_1, placeholders::_2);
    // 成员函数是有绝对地址的，跟着类走，跟成员对象无关，但是如果你要调用这个成员函数，是需要使用类对象进行调用的
    bf5(444, 555);
    std::cout << ctc.m_i << std::endl; // 0
    // v1 = 444, v2 = 555
    // 上文中的参数 ctc ， 会导致调用CTC的拷贝构造函数来生成一个CTC类的临时对象，作为std::bind的返回值（bind返回的是仿函数类型对象）
    // 后续的func调用，修改的是临时的CTC对象的成员变量m_i值,并不影响真实的ctc对象的m_i的值，所以这就是为啥上文中 ctc.m_i == 0
    
    std::bind(&CTC::func, &ctc, placeholders::_1, placeholders::_2)(444, 555);
    std::cout << ctc.m_i << std::endl; // 444
    // 当把ctc改为引用，那么就会真实改变到对象的成员变量值
    
    // 结论：bind绑定成员函数，第二个参数如果前面添加 &，那么就不会生成临时对象，后续成员函数func修改的也是实际ctc对象的成员值
    // 这说明此时bind返回的这个对象其实是ctc对象本身（仿函数类型对象）
    
    // bind和function 配合使用
    std::function<void(int, int)> bfc = std::bind(&CTC::func, ctc, placeholders::_1, placeholders::_2);
    bfc(10, 20);
    
    // 把成员变量地址当函数一样进行绑定，绑定的结果放在std::function<int &(void)>里面进行保存、
    // 说白了就是使用可调用对象的方式保存一个变量
    std::function<int&()> bf7 = std::bind(&CTC::m_i, &ctc);
    // 如果bind第二个参数不是引用，那么这个bind会导致系统会调用两次拷贝构造函数
    // 第一次执行拷贝构造函数是系统要利用ctc产生临时CTC对象
    // 第二次拷贝构造函数执行是因为std::bind要返回一个CTC对象（仿函数，拷贝自临时CTC对象）
    // 执行完std::bind之后，临时的CTC对象会被释放，返回测这个临时CTC对象（仿函数）会被bf7接着
    bf7() = 900;
    std::cout << ctc.m_i << std::endl; // 900
    
    
    auto rt = std::bind(CTC());
    // CTC()构造临时对象，然后又调用拷贝构造函数生成了一个可调用对象，作为std::bind的返回内容，
    rt(); // 调用的是operator()
    
    auto bf8 = std::bind(run, std::placeholders::_1);
    for(int i = 0; i < 4; i++)
    {
        myCallBack(i, bf8);
        // 0
        // 1
        // 2
        // 3
    }
    
    // 总结：
    // 1）bind思想，所谓的延迟调用，将可调用对象同意格式，保存起来，需要的时候再进行调用
    // 2）我们有用std::function绑定一个可调用对象，但是类成员不能绑，std::bind成员函数，成员变量都能够绑定
    
    
    return 0;
}


```
