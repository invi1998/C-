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


```c++
/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <functional>
#include <vector>

using namespace std;

class CT
{
    public:
        int i;
        int j;
};

int testf()
{
    return 10;
}

const int &&myfunctest(void)
{
    // return 0;
}

template<typename T>
class CCT
{
    public:
        // typename T::iterator iter; // 定义一个T类型的迭代器类型
        decltype(T().begin()) iter;      
        // 如果 T = const std::vector<int> 那么，T().begin()返回的就是 std::vector<int>::const_iterator类型
        // 如果 T = std::vector<int> 那么，T().begin()返回的就是 std::vector<int>::iterator类型
        // 然后这里T后面加一个括号，表示生成该类型的一个临时对象，后面就可以直接用点来进行成员函数调用，所以就直接调用了该对象的.begin()
        void getBegin(T &t)
        {
            iter = t.begin();
        }
};

class AO
{
    public:
        AO()
        {
            std::cout << "AO()" << std::endl;
        }
        ~AO()
        {
            std::cout << "~AO()" << std::endl;
        }
        
        int func() const
        {
            std::cout << "A::func()" << std::endl;
        }
        int m_i = 99;
};

// c++98 使用偏特化解决
// template<typename T>
// class CCT<const T>
// {
//     public:
//         typename T::const_iterator iter; // 定义一个T类型的常量迭代器类型
//         void getBegin(const T &t)
//         {
//             iter = t.begin();
//         }
// };

auto funcBack_add(int a, int b) -> decltype(a + b)
{
    return a + b;
}

int &tf(int & i)
{
    return i;
}

double tf(double & d)
{
    return d;
}

template<typename T>
auto funcback(T &tv) -> decltype(tf(tv))
// 这里还就必须嘚使用功能返回类型后置语法，因为如果你改为
// decltype(tf(tv)) funcback(T &tv) 
// 编译会报错，因为你这行代码就表明，在使用功能tv的时候，还没有进行定义，他是在函数形参里面才完成的定义
{
    return tf(tv);
}

template<typename T>
// T& mydouble(T & v)
// 我们此时希望通过auto来推断这个函数返回类型
// auto mydouble(T &v) // 在调用的时候，编译报错（见下文）
decltype(auto) mydouble(T &v)
// 这里把auto理解为要推导的类型，推导的过程我们使用decltype
{
    v *= 2;
    return v;
}

decltype(auto) tff1()
{
    int i = 1;
    return i;
}

decltype(auto) tff2()
{
    int i = 1;
    return (i); // 加了括号， 导致返回的是一个int & 类型
    // 这样的代码会导致一个内存泄漏的问题：
    // 因为你返回的是一个引用，你在局部变量里返回一个引用（就会导致执行玩这个函数的时候，这个引用就被系统回收）
}

int main()
{
    // decltype含义和举例
    // decltype用于推导类型，对于一个给定的变量名或者表达式，decltype能够告诉你该名字或者表达式的类型
    // auto a = 10; // 我们并不想用表达式的值类初始化这个变量
    // c++11中引入了decltype（说明符）；他的主要作用就是返回操作数的数据类型
    // decltype特点
    // 1）decltype的自动类型推断会发生在编译期(和auto一样)
    // 2）decltype不会真正计算表达式的值
    // a) decltype后的圆括号是一个变量
    
    // const int i = 0;
    // const int &iv = i;
    // auto j1 = i;    // j1 = int;  传值方式推断（引用属性和const属性都会被丢弃）
    // decltype(i) j2 = 15;    // j2 = const int。如果decltype中是一个变量，则变量的const属性会返回
    // decltype(iv) j3 = j2;   // j3 = const int &; 如果decltype中是一个变量,则变量的const属性以及引用属性都会返回，
    //                         // decltype很循规蹈矩，有啥就返回啥
                            
    decltype(CT::i) a;  //a = int
    CT tmpct;
    decltype(tmpct) tmpct2; // tmpct2 = CT
    decltype(tmpct2.i) mv = 34; // mv = int
    int x = 1, y = 2;
    auto &&z = x; // x是一个左值，auto = int &; z = int &;万能引用
    decltype(z) &&h = y; // z = int &; h = int & (这里用到了引用折叠规则)
    
    
    // b) decltype后的圆括号中是非变量（是表达式）
    // decltype会返回表达式的结果对应的类型
    decltype(9) kk = 5; // kk = int
    int i = 0;
    int *pi = &i;
    int &iy = i;
    decltype(iy + 1) j; // j = int (因为iy + 1得到的是一个整形表达式)
    decltype(pi) k; // k = int * (因为pi是一个变量)
    *pi = 4;
    decltype(*pi) k2 = i;   // k2 = int &
    // *pi是指针指向的对象，而且能给对象赋值，所以*pi是一个左值
    // *pi是一个表达式不是一个变量，又因为他有*号,
    // 如果表达式的结果能够作为赋值语句左边的值，那decltype返回的就是一个引用
    // 所以这种情况需要专门记：decltype后面是一个非变量的表达式，并且表达式能够作为等号左边的内容，那么他的返回就类似一个左值引用 &
    
    decltype(i) k3; // k3 = int; 因为i只是一个变量
    
    decltype((i)) iy2 = i; // 如果在变量名外额外扩了一层或者多层括号，那么编译器就会把这个变量视为一个表达式
    // 又因为变量（表达式）可作为等号左边的内容，所以当前范例：iy2 = int &；
    
    // 结论decltype((变量))的结果永远都是一个引用
    
    
    // c) decltype后是函数
    decltype(testf()) tempv = 14; // tempv = int 
    // tempv 的类型就是函数testf()的返回类型，（注意：这里不会去真实的调用函数）
    // 这里编译器没有去调用函数testf（）,只是使用函数的返回值类型作为tempv的类型
    decltype(testf) tempv2; // tempv2 = int (void), 这个有返回类型，有参数类型，代表一种可调用对象
    // 标准库 function用法，类模板
    function<decltype(testf)> ftemp = testf; // 声明了一个function函数类型，用来代表一个可调用对象
    // 他所代表的可调用对象是 int(void)
    std::cout << ftemp() << std::endl;
    
    decltype(myfunctest()) myy = 0; // myy = const int &&
    
    
    // decltype的主要用途
    // 1）应付可变类型，一般decltype主要应用在模板编程中
    // using consttype = std::vector<int>; // 定义类型别名
    using consttype = const std::vector<int>; // 如果定义的类型别名设置为const
    consttype myvec = {12, 34, 53, 23, 55};
    CCT<consttype> cct;
    cct.getBegin(myvec);
    
    // 常量容器
    // const std::vector<int> myvec = {12, 34, 53, 23, 55};
    // 需要在定义时初始化，且后面不能修改（数目，值）
    // 如果想遍历这种常量容器，是需要使用常量迭代器来进行遍历
    // std::vector<int>::const_iterator iter;
    // 所以上文中我们类模板里定义的迭代器（typename T::iterator iter; // 定义一个T类型的迭代器类型），
    // 就不能用于遍历这种常量容器，故编译器报错
    // C++98的时代，如果需要解决这种问题，就需要实现对于该类型的类模板偏特化，这里是属于参数范围的偏特化
    
    // 测试类型后面加圆括号生成临时对象
    AO().func();    // 生成一个临时AO对象，并调用func成员函数
    // AO()
    // A::func()
    // ~AO()
    
    // (const AO()).func(); // 和上面打印一样
    decltype(AO().func()) newao = 999;  // newao = int
    // 注意，这里decltype虽然传入AO类并且使用其临时对象的func()函数的返回值类型作为类型推断，但是实际上根本没有构造过该AO类，也没有进一步去调用器func函数
    
    
    
    // 2）通过变量表达式抽取变量类型
    vector<int> ac;
    ac.push_back(12);
    ac.push_back(23);
    vector<int>::size_type acsize = ac.size();
    std::cout << acsize << std::endl; // 2
    decltype(ac)::size_type acsize2 = acsize; // 抽取ac的类型，也就是vector<int>,
    // 所以这行等价于 vector<int>::size_type acsize2 = acsize; 
    std::cout << acsize2 << std::endl;
    
    typedef decltype(sizeof(0)) size_t_myt; // size_t_myt = unsigned int  因为 sizeof(0) 返回的是一个无符号整形
    // 等价于 typedef decltype(sizeof(int)) size_t_myt;
    // 等价于 typedef unsigned int size_t_myt; // 也就是定义一个无符号整形的类型别名，叫size_t_myt
    size_t_myt abc;
    abc = 1;
    
    // 3) auto结合decltype构成返回类型后置语法(这里auto不是自动类型推断，而是返回类型后置语法的组成部分)
    // auto funcBack(int a, int b)->int {} 
    // 语法格式： auto + 函数名 + 形参列表 + -> + 函数返回类型 + 函数体
    
    int ii = 19;
    std::cout << funcback(ii) << std::endl; // 19
    double id = 123.34f;
    std::cout << funcback(id) << std::endl; // 123.34
    
    
    // 4) decltype(auto) 用法 c++14版本开始支持
    // a) 用于函数返回类型
    
    int aaa = 100;
    mydouble(aaa) = 122; // 因为模板函数mydouble返回的是一个引用，所以可以放在左边，用来接收左值
    // 函数模板改为 auto mydouble(T &v) 后，编译报错，
    // 因为此时mydouble返回类型变为了int（右值），不能给右值赋值（编译器会报错 = 左边必须为左值）
    
    std::cout << aaa << std::endl;  // 122
    
    decltype(mydouble(aaa)) ioio = aaa; // ioio = int &, 
    
    // b)用于变量声明中
    int xx = 1;
    const int & yy = 1;
    auto zz = yy;    // zz = int (注意：值传递的时候，const和&都会被auto丢弃)
    decltype(auto) zz2 = yy;  // zz2 = const int &;
    // auto丢弃掉的const &能够通过decltype捡回来
    
    // c) 再谈(x)
    int im = 10;
    decltype((im)) im2 = im; // im = int &
    
    decltype(tff1()) tesaa = 234; // 没问题 ，因为推断出返回类型就是 int
    int ag = 1;
    decltype(tff2()) testbb = ag;  // testbb = int &;
    // 编译没问题，
    tff2() = 1243; // 语法上没问题，因为这里返回的引用是局部变量的引用，在执行完tff2()函数的时候，这个内存就已经别回收了
    // 此时你在往这块内存写东西，就相当于往系统内存中写，这是非法的
    
    return 0;
}



```
