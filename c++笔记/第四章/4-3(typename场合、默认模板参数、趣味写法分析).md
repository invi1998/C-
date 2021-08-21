（1）typename的使用场合

（2）函数指针做其他函数的参数

（3）函数模板趣味用法举例
```c++
#include <cstdio>
#include <iostream>

using namespace std;

template <typename T, typename F>
void testFunc(const T&i, const T&j, F funcpoint)
{
	cout << funcpoint(i, j) << endl;
}

class Tc
{
	public:
  		Tc(){
        	cout << "构造函数执行" << endl;
        }
  		Tc(const Tc&tc)
        {
        	cout << "拷贝构造函数执行" << endl;
        }
  
  		int operator()(int v1, int v2)const
        {
        	return v1 * v2;
        }
};

int func2(int a, int b)
{
    return a * b;
}


int main()
{
	Tc testC;
  	testFunc(2,6,testC);
  	testFunc(2,6,func2);
  	testFunc(2,6,Tc());
}

/*
构造函数执行

拷贝构造函数执行

12

12

构造函数执行

12


*/
```

（4）默认模板参数
```c++
// MyArray.h

#include <string>
#include <iostream>

using namespace std;

#ifndef __MYARRAY__
#define __MYARRAY__

template<typename T=string, int size=5>
class MyArray
{
    private:
        T arr[size];
    
    public:
        void myfunc();
};

template<typename T, int size>
void MyArray<T, size>::myfunc()
{
    cout << size <<endl;
    return;
}

#endif
```

```c++
#include <cstdio>
#include <iostream>
#include <string>
#include "MyArray.h"

using namespace std;

class Tc
{
	public:
  		Tc(){
        	cout << "构造函数执行" << endl;
        }
  		Tc(const Tc&tc)
        {
        	cout << "拷贝构造函数执行" << endl;
        }
  
  		int operator()(int v1, int v2)const
        {
        	return v1 * v2;
        }
};


typedef int(*FunType)(int, int); // 定义一个函数指针类型

int func2(int a, int b)
{
    return a - b;
}

// template <typename T, typename F=Tc> // 第三个参数我们默认给他一个默认参数Tc
// void testFunc(const T&i, const T&j, F funcpoint=F()) // 如果不使用函数模板，这行代码就等价于
// void testFunc(const int &i, const int &j, Tc funcpoint = Tc())
// 同样我们也可以为该函数模板提供一个缺省的函数指针作为参数
template <typename T, typename F=FunType> // 第三个参数我们默认给他一个函数指针类型作为类型参数（函数指针类型需要实现定义好）
void testFunc(const T&i, const T&j, F funcpoint=func2) // 这里缺省的默认参数，因为是一个FuncType（自定义的函数指针类型）,所以这里需要传入一个函数名，代表函数的首地址
{
	cout << funcpoint(i, j) << endl;
}


int main()
{
	Tc testC;
  	testFunc(2,6,testC);
  	testFunc(2,6,func2);
  	testFunc(2,6,Tc());
  	
  	MyArray<>abc; //完全使用模板参数的缺省值
  	MyArray<int, 100>intTest;
  	
  	abc.myfunc();
  	intTest.myfunc();
  	
  	testFunc(2,6); //只传递前两个参数，第三个参数使用缺省的默认TC
  	
}
```

```c++

typename的应用场合_用在模板函数类型成员前表示是一个类型
```c++
// myVector.h
#ifndef __MYVCTOR__
#define __MYVCTOR__

template<typename T> // 定义一个名字为T的模板参数，表示mtvector这个容器所保存的元素类型
class myVector {
    public:
        typedef T* myiterator; // 迭代器
    public:
        myVector();
        myVector& operator = (const mtVector&); // 赋值运算符重载，在类模板内部使用模板名并不需要提供模板参数（当然，想使用也可以）
        // myVector& operator = (const mtVector<T>&)
    public:
        myiterator mybegin(); // 迭代器起始位置
        myiterator myend(); // 迭代器最后一个元素的后一个位置
    public:
        myfunc() {}; // 成员函数
};
//成员函数写在类外
template<typename T>
void myVector<T>::myfunc() {

}

template<typename T>
// typename必须写
// ::的第二个用法，访问类型成员myiterator (typename)
// 我们这里的迭代器mybegin返回的正好是这种类型
// typename这里的目的就是显示的告诉编译器myiterator是一个类型
typename myVector<T>::myiterator myVector<T>::mybegin()
{

};

#endif
```

```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>

using namespace std;

//求字符串的长度
template<typename T>
typename T::size_type GetLength(const T&c)
{
	if (c.empty())
	{
		return 0;
	}
	return c.size();
}


int main(void)
{
	string mytest = "jisuanjizuchegnyuanli";
	string::size_type size = GetLength(mytest);//等价于无符号整形

	
	system("pause");
	return 0;
}

/*
*(1)typename的使用场合
*	1.在模板定义里面。typename标明其后的参数是类型参数		template<typename T,int a,int b>
*	template<typename T>		//名字为T的模板参数
*	typename 可以使用class，这里的class不是类定义，表示类型参数
*	2.使用类的类型成员，用typename来表示这是一个类型
*	::可以表示类成员作用域 比如访问类的静态成员，可以使用 类名：：静态成员名
*	::还可以表示访问模板类的类型成员。
*				函数返回值
*		typename myVector<T>::myIterator   myVector<T>::myend()
*		typename 的用处就是显式告诉编译器myVector<T>::myIterator是一个类型
*
		typename T::size_type GetLength(const T&c)
		中的T::size_type也是类型，所以前面需要加上typename
*
*(2)
*
*(3)
*
*
*/
```

函数指针调用函数

```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>


using namespace std;
int myFunction(int a,int b)
{
	return  a + b;
}
//把函数指针作为另外一个函数的参数传递？？
//
//定义一个函数指针类型
typedef int(*FunType)(int, int);
//定义一个函数接收函数的指针
void TestFunction(int i,int j,FunType myfun)
{
	//这里可以通过函数指针调用函数
	int result = myfun(i, j);
	cout << result << endl;
}


int main(void)
{
	TestFunction(1, 2, myFunction);//函数名字就相当于一个函数地址
	TestFunction(1, 2, &myFunction);//ok。取地址也可以调用成功

	system("pause");
	return 0;
}

/*
*(1)函数指针做其他函数的参数
*
*
*(2)
*
*(3)
*
*
*/
```

函数模板趣味用法_传递一个可调用类对象作为类模板参数

```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>

using namespace std;

int myFunction(int a, int b)
{
	return  a + b;
}


//把函数指针作为另外一个函数的参数传递？？
//
//定义一个函数指针类型
typedef int(*FunType)(int, int);

template <typename T,typename F>
void testFunction(const T&i,const T&j,F myfunc)
{
	cout << myfunc(i, j) << endl;
}

class tc
{
public:
	tc()
	{
		cout << "无参构造函数执行" << endl;
	}
	tc(const tc&t)
	{
		cout << "拷贝构造函数执行" << endl;
	}
	//重载()函数调用运算符
	int operator()(int a,int b)
	{
		return a + b;
	}
public:
	
};



int main(void)
{
	testFunction(1, 2,myFunction);//自动推断 T为int，F为函数类型


	tc object01;
	testFunction(3, 4, object01);
	/*
	 * 1.首先调用拷贝构造函数执行，把一个object01拷贝给形参,判断为类 类型
	 * 2.然后调用重载的函数调用()运算符
	 */
	cout << "_______________________________" << endl;
	testFunction(5, 6, tc());//使用一个临时对象，少调用一个拷贝构造函数，只调用了无参构造函数

	
	system("pause");
	return 0;
}

/*
 *传递一个可调用类对象作为类模板参数
 *
*(1)可调用对象概念--具体见“未归类知识点1”
*	1.如果类重载了函数调用运算符（也就是括号运算符），则我们可以像使用函数一样使用该类的对象，
*		我们把这样的类对象称为函数对象（function object）
*	2.C++语言中有几种可调用对象：函数、函数指针、lamba表达式、
*		bind创建的对象以及重载了函数运算符的类。
*
*(2)
*
*(3)
*
*
*/
```

默认模板参数

```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
#include"myArray.h"
using namespace std;

int myFunction(int a, int b)
{
	return  a + b;
}


//把函数指针作为另外一个函数的参数传递？？
//
//定义一个函数指针类型
typedef int(*FunType)(int, int);

template <typename T, typename F=FunType>//声明的时候指定类型
void testFunction(const T&i, const T&j, F myfunc=myFunction)//有默认参数
{
	cout << myfunc(i, j) << endl;
}

int main(void)
{
	//a
	myArray<> my_array01;//完全用模板参数的默认缺省值
	myArray<int >my_array02;//第二个参数使用默认缺省值


	//b.函数模板的默认参数 testFunction最后参数有默认值
	testFunction(3, 4);
	
	system("pause");
	return 0;
}

/*
*(1)默认模板参数
*	a.类模板--类模板名字后面必须使用<>来提供额外的信息。<>表示这是一个模板
*	b.函数模板的默认参数
*		老标准只能为类模板提供默认模板参数，c++11新标准可以为函数模板提供默认参数
*(2)
*
*(3)
*
*
*/
```