（1）隐式类型转换；
（2）显式类型转换；
（2.1）static_cast；
（2.2）dynamic_cast；
（2.3）const_cast；
（2.4）reinterpret_cast；
C风格的强制类型转换很简单，均用 Type b = (Type)a 形式转换。C++风格的类型转换提供了4种类型转换操作符来应对不同场合的应用，如下表：

转换类型操作符	作用
const_cast	去掉类型的const或volatile属性
static_cast	无条件转换，静态类型转换
dynamic_cast	有条件转换，动态类型转换，运行时检查类型安全（转换失败返回NULL）
reinterpret_cast	仅重新解释类型，但没有进行二进制的转换

# 1、const_cast

去掉类型的const或volatile属性（只能去除指针或者引用的const属性，编译时就会进行类型转换, 不能去掉普通变量的const属性）

```c++
int main() {

    struct T {
        int i;
    };

    const T a;
    //a.i = 10;  //直接修改const类型，编译错误
    T &b = const_cast<T&>(a);
    b.i = 10;

    return 0;

}

```
# 2、static_cast

类似C风格的强制转换，进行无条件转换，静态类型转换：

- 1）基类和子类之间的转换：其中子类指针转换为父类指针是安全的，但父类指针转换为子类指针是不安全的（基类和子类之间的动态类型转换建议用dynamic_cast）。

- 2）基本数据类型转换，enum，struct，int，char，float等。static_cast不能进行无关类型（如非基类和子类）指针之间的转换（也就是不能用于指针类型之间的转换，比如int *, double *, float *, string *等非类类型指针）。

- 3）把任何类型的表达式转换成void类型。

- 4）static_cast不能去掉类型的const、volatile属性（用const_cast）。

```c++
int main() {

    int n = 6;
    double d = static_cast<double>(n);     //基本类型转换
    int *pn = &n;
    double *d = static_cast<double*>(&n);  //无关类型转换，编译错误
    void *p = static_cast<void*>(pn);

    return 0;
}
```

# 3、dynamic_cast

有条件转换，动态类型转换，运行时检查类型安全（转换失败返回NULL）：

* 1）安全的基类和子类之间的转换。

* 2）必须有虚函数。

* 3）相同基类不同子类之间的交叉转换，但结果返回NULL。

```c++
class Base {
public:

    int _i;
    virtual void foo() {}; //基类必须有虚函数。保持多态特性才能使用dynamic_cast

}; 

class Sub : public Base {
public:

    char *_name[100];
    void Bar() {};

}; 

int main() {

    Base* pb = new Sub();
    Sub* ps1 = static_cast<Sub*>(pb);  //子类->父类，静态类型转换，正确但不推荐
    Sub* ps2 = dynamic_cast<Sub*>(pb); //子类->父类，动态类型转换，正确

    Base* pb2 = new Base();
    Sub* ps21 = static_cast<Sub*>(pb2); //父类->子类，静态类型转换，危险！访问子类_name成员越界
    Sub* ps22 = dynamic_cast<Sub*>(pb2);//父类->子类，动态类型转换，安全，但结果为NULL

    return 0;

}

```
# 4、reinterpret_cast

仅重新解释类型，但没有进行二进制的转换(编译时就会进行类型转换的检查，中文翻译：重新解释，将操作数的内容解释为另一种不同的类型，用于处理两种无关类型的转换)：

- 1）转换的类型必须是一个指针，引用、算术类型、函数指针或者成员指针。

- 2）在比特级别上进行转换，可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针（先把一个指针转换成一个整数，在把该整数转换成原类型的指针，还可以得到原先的指针值）。但不能将非32bit的实例转成指针。
- 3） 最普通的用途就是在函数指针类型之间进行转换。
- 4） 很难保证移植性。

```c++
int doSomething() { 
    return 0; 
};

int main() {
    typedef void(*FuncPtr)(); //FuncPtr is 一个指向函数的指针，该函数没有参数，返回值类型为 void
    FuncPtr funcPtrArray[10]; //10个FuncPtrs指针的数组 让我们假设你希望（因为某些莫名其妙的原因）把一个指向下面函数的指针存入funcPtrArray数组：

    funcPtrArray[0] = &doSomething;// 编译错误！类型不匹配，reinterpret_cast可以让编译器以你的方法去看待它们：funcPtrArray
    funcPtrArray[0] = reinterpret_cast<FuncPtr>(&doSomething); //不同函数指针类型之间进行转换

    return 0;
}
```

## 总结

* 去const属性用const_cast

* 基本类型转换用static_cast

* 多态类之间的类型转换用dynamic_cast

* 不同类型的指针类型转换用reinterpret_cast
# 未定义行为

概念：C语言中的未定义行为（Undefined Behavior）是指C语言标准未做规定的行为；同时，标准也从没要求编译器判断未定义行为，所以这些行为有编译器自行处理，在不同的编译器可能会产生不同的结果，又或者如果程序调用未定义的行为，可能会成功编译，甚至一开始运行时没有错误，只会在另一个系统上，甚至是在另一个日期运行失败。当一个未定义行为的实例发生时，正如语言标准所说，“什么事情都可能发生”，也许什么都没有发生。

　　所以，避免未定义行为，是个明智的决定。

* 1. 同一运算符中多个操作数的计算顺序（&&、||、? 和, 运算符除外）
例如：x = f()+g(); //错误

f()和g()谁先计算由编译器决定，如果函数f或g改变了另一个函数所使用变量的值，那么x的结果可能依赖于这两个函数的计算顺序。

* 2. 函数各参数的求值顺序
例如： printf("%d, %d\n", ++n, power(2, n)); //错误

在不同的编译器可能产生不同的结果，这取决于n的自增运算和power调用谁在前谁在后。

需要注意的是，不要和逗号表达式弄混，逗号表达式可以参考这篇文章：c语言中逗号运算符和逗号表达式

* 3. 通过指针直接修改 const 常量的值
直接通过赋值修改const变量的值，编译器会报错，但通过指针修改则不会，例如:

```c++
int main() {

	const int a = 1;
	int *b = (int*)&a;
	*b = 21; printf("%d, %d", a, *b);
	return 0;

}

int main ( )

{
const int a = 1 ; 

int * b = ( int * ) & a ; 

* b = 21 ; 

printf ( "%d, %d" , a , * b ) ; 

return 0 ; 

}

```
 

a输出值也由编译器决定。

<hr>

```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>

using namespace std; 

class A{}; 
class B:public A
{}; 

int main(void)
{

	//1隐式类型转换
	int number = 22;
	double number02 = 2.3333;
	double number03 = number + number02;
	int number04 = number + number02;
	cout << number03 << endl;
	cout << number04 << endl;
	//内存中的值和输出的值是不同的

	//2显式类型转换
	int number05 = 5 %(int) 3.2;
	int number06 = 5 % int(3.2);	//把类型或者数字()起来都可以--c语言风格的类型转换

	//static_cast
	double number07 = 100.4545;
	int number08 = (int)number07;	//c语言风格的类型转换
	int number09 = static_cast<int>(number07);//c++风格的类型转换

	//(2)子类和父类之间的类型转换
	B b;
	A a = static_cast<A>(b);	//把子类转换成父类对象   子类是特殊的父类对象

	//(3)void*(五类型指针)与其他类型指针之间的转换
	int number10 = 10;
	int*p = &number10;
	void*p2 = static_cast<void*>(p);
	int*p3 = static_cast<int*>(p2);
	cout << *p3 << endl;

	//const_cast()--只能修改指针和引用的const
	int number11 = 11;
	const int*p11 = &number11;
	int*p12 = const_cast<int*>(p11);
	*p12 = 100;	//正确--语法上没错，但是不建议修改const的变量的值，属于未定义的行为，可能写进去，也可能写不进去
	//--内存和打印出来的值不同
	
	const int& number12 = number11;
	int& number13 = const_cast<int&>(number12);

	//reinterpreter_cast类型转换
	int number14 = 14;
	int*p14 = &number14;
	char*p15 = reinterpret_cast<char*>(p14);
	double*p16 = reinterpret_cast<double*>(p14);
	cout << *p16 << endl;	//-9.25596e+61

	int number17 = 100;
	long long number18 = 8898899400;	//8个字节后面强制转换为4个字节的地址会丢失2==2 126A 6DC8
	int*p17 = (int*)number18;	//	c风格的转换
	int*p18 = reinterpret_cast<int*>(number18);	//4个字节，所以会丢失==126a 6dc8
	
	
	system("pause");
	return 0;

}

/*
* (1)隐式类型转换--系统自动进行，不需要程序开发人员介入
*
* （2）显式类型转换
*
* c++强制类型转换分为4种--我们要尽量使用c++风格的类型转换
* 这4中强制类型转换，分别用于不同的目的，提供4中轻质类型转换的目的：提供更加丰富的含义和功能，更好的类型检测机制
*	1.static_cast
*		静态转换理解为“正常转换”，编译的时候进行类型转换的检查。代码中要保证转换的安全性，如“asdf”字符串转换为int是错误的
*		与c语言的强制类型转换差不多。
*		c风格的强制类型转换以及编译器能够进行的隐式类型转换，都可以使用static_cast显式类型转换。
*		可用于：
*			（1）相关类型转换  整形--->浮点型
*			（2）后续学习类中的子类转换成父类
*			(3)void*(五类型指针)与其他类型指针之间的转换
*		不可用于：
*			（1）一般不可以用于指针类型之间的转换 float*--int*--double*等之间的转换
*	2.dynamic_cast
*		主要应用于运行时类型识别和检查
*		应用于父类型和子类型之间转换---父类型指针指向子类对象（父类指针用子类对象初始化）, 然后用dynamic_cast把父类指针转换为子类
*	3.const_cast
*		去除指针或者引用的const属性，也是属于编译时的转换。
*		不能去掉普通变量的。
*	4.reinterpret_cast
*		重新解释转换，将操作数重新解释为另一种类型--把操作数类型都转换了
*		处理无关类型的转换--两个转换之间没有关系。自由转换，非常随意。
*			1. 将一个整形转换成一个指针类型 。一种指针类型重新转换成另一种指针类型int*---float*类型--按照转换后的内容重新解释内存中的内容。
*			2. 也可以将一个指针类型转换为整形类型
*		这种自由转换安全性太差，都不建议使用。强制类型转换跳过  编译器的类型检查。
*		资料上说：reinterpret危险，使用const_cast意味着设计缺陷。
*		使用c++风格的类型转换，尽量不要使用c语言风格的类型转换。
*	这4种类被称为“命名的强制类型转换”--都有一个名字并且不同名字
*
*	通用形式：
*	强制类型转换名字 <type>(express)
*		--type转换的目标类型
*		--express  需要转换的变量或者表达式
*	
*/ 
```
