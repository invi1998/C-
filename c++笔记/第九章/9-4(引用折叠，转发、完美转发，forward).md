# 引用折叠规则与引用的引用

```c++
#include<iostream>
#include<cstdlib>
#include <boost/type_index.hpp>
using namespace std;

template<typename T>
void myFunction01(T &&tem)//T是类型模板参数，T是有类型的,tem是形参，tem也是有类型的
{
	using boost::typeindex::type_id_with_cvr;
	cout << "T type=" << type_id_with_cvr<T>().pretty_name() << endl;//显示T类型
	cout << "tem type=" << type_id_with_cvr<decltype(tem)>().pretty_name() << endl;//显示tem类型

}

int main(void)
{
	int i = 18;//i的类型是int类型，i是个左值。
	myFunction01(i);//实例化后,我们认为编译器推断后的结构为void myFunction(int& &&tem){}
	myFunction01(12);
	/*
	 *	T type=int &
		tem type=int &
		T type=int
		tem type=int &&
	 */
	//编译器真正实例化出来的myFunction是什么类型 void myFunction(int&tem){}--引用折叠


	//1.1引用的引用
	int number = 500;
	int &number_reference = number;//引用
	//int & &rr = b;//链接在一起叫右值引用，分开交引用的引用，是非法的。但是编译器内部可以出现，它自己推导，使用引用折叠处理。


	
	system("pause");
	return 0;
}
/*
 * (1)引用折叠规则
 * 引用折叠是c++s11新标准，是一条规则，是一条规则reference-collapsing rules 引用坍塌，&&&--->&
 * c++标准中有明确含义的引用只有两种：一种是&左值引用，一种是&&右值引用。
 *	void myFunction(int& &&tem)//两组 int&是左值引用，第二组&&tem是右值引用类型
 *	
 *	什么情况下出现引用折叠？？
 *	分成两组，第一组是左值引用/右值引用		第二组 左值引用/右值引用
 *			左值引用  左值引用	&		&
 *			左值引用	右值引用	&		&&	当前的情况
 *			右值引用	左值引用	&&		&
 *			右值引用	右值引用	&		&
 *		折叠规则：如果任何一个为左值引用，那么结果就是左值引用，否则就是右值引用。--左值具有传染性。
 *
 *		1.1引用的引用
 *		不可以显示写，但是编译器内部进行函数模板类型推断的时候可以出现，编译器使用引用折叠规则进行处理。
 *		编译器不允许程序开发者写出引用的引用的代码。
 *	需要引用折叠的场景有：
 *		1.函数模板实例化
 *		2.auto自动类型推断的时候
 *		
 * (2)
 * (3)
 * (4)
 * (5)
 * (6)
 */


```

# 转发函数和完美转发
```c++
#include<iostream>
#include<cstdlib>
#include <boost/type_index.hpp>
using namespace std;
//普通函数
void myFunction(int value01,int value02)
{
	++value02;
	cout <<"Sum="<< value02 + value01 << endl;
	return;
}

//左值引用
void myFunction02(int value01, int &value02)
{
	++value02;
	cout << "Sum=" << value02 + value01 << endl;
	return;
}

//改为右值引用
void myFunction03(int &&value01, int &value02)
{
	++value02;
	cout << "Sum=" << value02 + value01 << endl;
	return;
}


//模板函数，要把收到的参数以及这些参数相对应的参数保持不变转发为其他函数myFunction  --这就是转发
template<typename F,typename T1,typename T2>
void myFunctionTramsmit(F f,T1 t1,T2 t2)//F就是要调用的第三方函数,要转发的目标函数
{
	f(t1, t2);
	

}

//将函数模板改为万能引用类型
template<typename F, typename T1, typename T2>
void myFunctionTramsmit02(F f, T1&& t1, T2 &&t2)//F就是要调用的第三方函数,要转发的目标函数
{
	/*针对主函数中的调用myFunctionTramsmit02(myFunction02, 34, i);
	 *T1=int, t1=int&&  t1是右值引用类型，t1本身是左值
	 *T2=int&,t2=int&
	 * 
	 */
	//f(t1, t2);
	//“void(int &&, int &)”: 无法将参数 1 从“int”转换为“int &&”
	//调用处的是一个右值，但是通过万能引用转换为一个左值了，t1本身是一个左值，但是函数myFunction03需要的是一个右值
	f(std::forward<T1>(t1), std::forward<T2>(t2));
	//t1中原来是左值还是右值信息存在在T1模板参数中，这里会将t1转换为原始实参的右值类型
}

int main(void)
{	int i = 50;
	myFunction(34, i);

	//通过转发函数调用
	myFunctionTramsmit(myFunction, 34, i);
	myFunctionTramsmit(myFunction02, 34, i);//这里返回后i=50,并不是我们期望的51.也就是说，T2并没有实例化为int&类型
	//void myFunctionTramsmit(void(*f)(int,int&),int t1,, int t2){}//实例化后的样子，t2这个参数并不是真正传递过去引用类型
	//怎么修改传递函数，让模板函数的参数能够保持给定实参的左值性（i有const属性也传递到模板函数中）？？？？
	/*
	 *万能引用  T&& 保存实参i的所有属性，从而让编译器推导出来函数模板最终的形参类型
	 *如果使用T&普通引用，则实参中只有const属性可以传递到函数模板，而实参中的左值和右值性就不能传递到函数模板
	 * 
	 */

	//万能引用后调用
	myFunctionTramsmit02(myFunction02, 34, i);//i的值是51

	//右值引用
	int &&number01 = 80;//右值引用绑定右值
	/*
	 * 虽然&&number01是绑定到右值的，但是number01本身是个左值，因为number01本在在等号左边。
	 * &&number01叫右值引用，number01是个左值，在内存中有地址。
	 * number01本身是个左值，但是他的类型是右值引用。也就是说左值引用，右值引用概念说的是他的类型。
	 *void myFunctionTramsmit02(F f, T1&& t1, T2 &&t2)函数形参中的变量都是左值，本身类型是右值引用
	 */
	//myFunction03(number01, i);//error，实参中要给右值，number01本质上是一个左值，形参中是一个右值引用，必须绑定一个右值
	int j = 50;
	myFunctionTramsmit02(myFunction03, 20, j);
	
	system("pause");
	return 0;
}
/*
* (1)转发和完美转发
*	通过函数指针调用函数，并把函数参数作为参数传递，就是转发。
*
*	完美转发：让我们可以写接受任意类型实参的函数模板，并将其转发到目标函数，
*		目标函数会接收到与转发函数所接收到的参数类型完全相同（左值性，右值性）
*		通过std::forword()来实现
*	
* (2)std::forward()
*	c++11中的新函数，专门为转发而存在。
*	它要么返回一个左值，要么返回一个右值。
*	使用条件：
*		调用模板函数，模板函数参数是万能引用类型，模板参数负责转发
*		void myFunctionTramsmit02(F f, T1&& t1, T2 &&t2)
*		f(std::forward<T1>(t1), std::forward<T2>(t2));
*		void myFunction03(int &&value01, int &value02)//value01要绑定一个右值
*		//调用
*		myFunctionTramsmit02(myFunction03, 20, j);
*	std::forword()的能力就是按照实参本身的类型转发。
*		1.实参原来是左值，到形参中还是左值,forward是按照形参原来的类型处理，所以std::forward处理后还是左值
*		2.实参原来是右值，到形参中变成左值,forward是按照形参原来的类型处理，所以std::forward处理后是个右值
*		所以这里看来std::forward()有强制把左值转换为右值的能力，所以forward()其实只对原来是右值的情况有用。
*
*		std::forward()的能力就是保持原始实参的类型（左值还是右值）
* (3)完美转发是如何保存原始实参的左值和右值信息？
*	是保存在转发函数（函数模板中）的模板参数T中，所以转发函数的参数类型必须是万能引用类型。
* (4)总结
*forward是强制把一个左值转换为右值，如果你的原始形参就是一个左值，那我的forward啥也不干，
*move()无条件强制类型转换，转换后原始对象名字不能使用。
* 
* (5)再谈万能引用
*	万能引用不是一种新的引用类型。是一种程序的写法。但是万能引用概念的存在是有意义的。
* 
* 1.转发函数
* 2.左值和右值
* 3.完美转发std::forward()
 
* (6)
*/

```

# forward和move使用例子
```c++
#include<iostream>
#include<cstdlib>

using namespace std;


void printInformation(int&tem)//类型是左值引用的形参
{
	cout << "形参类型为左值引用的函数" << endl;
}
void printInformation(int&&tem)//类型是左值引用的形参
{
	cout << "形参类型为右值引用的函数" << endl;
}

template<typename T>
void testTransmit(T&&t)//万能引用
{
	printInformation(t);//如果t是右值，T=int t=int&&类型，t本身是左值	如果t是左值，T=int&,t=int&类型，t本身是左值
	printInformation(std::forward<T>(t));
	printInformation(std::move(t));//move将左值转换为右值
	
}

int main(void)
{
	testTransmit(1);//传递右值
	/*
	 *  形参类型为左值引用的函数
		形参类型为右值引用的函数
		形参类型为右值引用的函数
	 */

	int i = 123;
	testTransmit(i);//传递左值
	/*

	形参类型为左值引用的函数
	形参类型为左值引用的函数
	形参类型为右值引用的函数
	 */
	system("pause");
	return 0;
}
/*
 * (1)std::forward()小例子
 * (2)
 * (3)
 * (4)
 * (5)
 * (6)(7)
 */

```

# forward与万能引用比较
```c++
#include<iostream>
#include<cstdlib>

using namespace std;

template<typename T>
void myFunction(T&&tem)
{}

int main(void)
{
	int ix = 12;//ix是左值
	int&&reference = std::move(ix);//左值转换为右值绑定
	int&&reference02 = std::forward<int>(ix);//ok 可以把左值成功的转换为右值

	myFunction(ix);//i是左值，T=int&,tem=int&
	myFunction(100);//100是右值，T=int,tem=int&&

	/*std::forward<int>(ix);转换还是不转换，取决于<>里面的值，可以类比于万能引用，
	 *14行是T=int,tem=int&&类型
	 * 
	 */
	
	system("pause");
	return 0;
}
/*
* (1)std::forward用法补充
*		应用于模板转发函数	把一个值转换为原始实参的类型
* (2)
* (3)
* (4)
* (5)
* (6)(7)
*/

```


```c++
namespace _nmsp2
{
    
    // 定义一个模板函数 ： 要把收到的参数以及这些参数相对应的类型不变（比如左值引用。右值引用，const）的转发给其他函数（转发给myfunc）
    // 这就叫转发
    template<typename F, typename T1, typename T2>
    // void myfuncTmp(F f, T1 t1, T2 t2) // F就是第三方我们要调用的函数（要转发到的目标函数）
    void myfuncTmp(F f, T1 &&t1, T2 &&t2)
    {
        
        // 万能引用后（针对 myfuncTmp(myfunc, 40, j); ）
        // T1 = int, t1 = int &&
        // T2 = int &, t2 = int &
        
        // 针对 myfuncTmp(myfunc2, 30, k); 
        // T1 = int, t1 = int &&。但是，因为函数里的形参总是左值，所以t1是左值（类型是右值引用）
        // 所以在调用myfunc2的时候，往里面的v1(右值引用，注定其只能绑定右值)，t1绑定到右值引用，就报错了
        // f(t1, t2);
        // 改造为完美转发
        f(std::forward<T1>(t1), std::forward<T2>(t2));
    }
    
    // void myfunc(int v1, int v2)
    void myfunc(int v1, int & v2)
    {
        ++v2; // 改变V2的值，让其自加1
        std::cout << v1 + v2 << std::endl;
    }
    
    void myfunc2(int &&v1, int & v2)
    {
        std::cout << v1 << std::endl;
        std::cout << v2 << std::endl;
    }
    
    void printInfo(int &i) // 类型是左值引用的形参
    {
        std::cout << "printInfo()的参数类型是左值引用" << std::endl;   
    }
    
    void printInfo(int &&i) // 类型是右值引用的形参
    {
        std::cout << "printInfo()的参数类型是右值引用" << std::endl;   
    }
    
    template<typename T>
    void printTmp(T && t)
    {
        printInfo(t);
        printInfo(std::forward<T>(t));
        printInfo(std::move(t));
    }
    
    void func()
    {
        // 转发 完美转发
        
        int i = 10;
        myfunc(20, i);  // 20 + 11 = 31
        std::cout << i << std::endl;    // 10 ==变成引用后==> 11
        
        int j = 70;
        myfuncTmp(myfunc, 40, j);   // 40 + 71 = 111
        std::cout << j << std::endl;    // 70 ==变成引用后==> 70（错误） ===变成万能引用后===> 71（正确）
        
        // 这里j不是71，说明我们的转发模板出了问题（当我们遇到引用的时候就不能正常工作）
        // 想象中myfuncTmp在被调用后实例化的结果是
        // void myfuncTmp(void (*myfunc)(int, int &), int t1, int t2)
        // 所以，这里就导致我们函数模板里传递给目标函数的是t2，而不是我们传递的 引用j(&j),
        // 那么我们如何通过函数模板给目标函数传递一个引用呢？（修改转发函数模板的参数，让其能够保持给定实参的左值性，当然，如果实参有const属性，我们也希望const属性能被传递进去）
        // 通过 万能引用 实现（实参的所有信息都能传递到万能引用中，从而让编译器推导出函数模板的最终形参类型）
        // 如果不用万能引用，只用普通的引用传值 ，则只有const属性能传递到函数模板中，而实参中的左值和右值无法传递
        
        int k = 100;
        myfunc2(200, k);
        
        // 定义一个右值引用类型(右值引用绑定右值)
        int &&youzhi = 90;
        // 虽然 &&youzhi是绑定到右值的，但是 youzhi 本身他是一个左值，因为 youzhi 是在等号左边待着的
        int &z = youzhi; // 证明 youzhi 是一个左值，因为youzhi可以绑定到左值引用
        // &&youzhi 叫右值引用，youzhi是一个左值（有地址）
        // youzhi 是一个左值，但是他的类型是右值引用（&&）
        // 也就是说，左值引用 还是 右值引用 这种概念说的是他的类型，而不是它本身
        // void fc(int &&v){}  // 注意：函数中的形参总是左值，即使他的类型是右值引用
        // myfunc2(youzhi, k); // cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’
        // 无法将参数1从int转为int &&
        // myfunc2(std::move(youzhi), k); // 可以使用move左值转右值
        
        // 同样的，通过转发模板，也会报错无法将左值转右值
        // myfuncTmp(myfunc2, youzhi, k); // 报错
        myfuncTmp(myfunc2, 30, k); // 报错
        
        
        // 完美转发  让我们可以写能够接收任意类型的实参的函数模板，并将其转发到目标函数，目标函数会接收到与转发函数完全相同的参数
        // std::forward
        // c++11 中专门为转发而存在的一个新函数，要么返回左值，要么返回右值
        // f(std::forward<T1>(t1), std::forward<T2>(t2));
        // 发挥作用的条件
        // 调用模板函数，模板函数参数是万能引用，模板函数负责转发
        // std::forward的能力就是按照实参本来的类型进行转发
        // 对std::forward的理解：
        // 1）实参如果是一个左值，forward是按照形参原来的类型进行处理，所以std::forward之后还是左值
        // 2）实参如果原来是一个右值，到了形参中变成了左值，forward是按照形参原来的类型进行处理，所以std::forward之后变成右值
        // 所以，这里看来std::forward有强制将左值转为右值的能力，forword只对原来是个右值的情况有用
        // forward的能力就是保持原始实参的类型
        
        printTmp(100);
        // 100 右值， T = int；  t = int &&  但是t本身是左值
        // printInfo()的参数类型是左值引用
        // printInfo()的参数类型是右值引用
        // printInfo()的参数类型是右值引用
        printTmp(k);
        // k 左值， T = int &；  t = int &  但是t本身是左值
        // printInfo()的参数类型是左值引用
        // printInfo()的参数类型是左值引用
        // printInfo()的参数类型是右值引用
        
        // 实参是左值还是右值，这个信息会被保存到转发函数模板里的万能引用里面的模板类型参数里去的
    
        // std::move()和std::forward的区别
        // forward 就是强制把一个左值转为右值，但是如果你实参就是左值，那forward啥也不干（有条件的类型转换）
        // move() 无条件的强制类型转换（强制将左值转右值）
        
        // 万能引用 并不是一种新的引用类型，他只是一种写代码的表象
        // 但是万能引用这个概念的存在是很有意义的
        
    }
}


```


