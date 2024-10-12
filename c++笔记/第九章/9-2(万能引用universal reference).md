在C++中，万能引用（Universal References）和完美转发（Perfect Forwarding）是两个重要的概念，它们主要用于实现模板函数中的高效和灵活的参数传递。下面详细介绍这两个概念及其使用方法。

### 1. 万能引用（Universal References）

#### 1.1 定义

万能引用是指可以绑定到左值（lvalue）和右值（rvalue）的引用类型。这种引用类型通常出现在模板参数中，形式为 `T&&`，其中 `T` 是一个模板参数。

#### 1.2 判断规则

- 如果 `T` 是一个左值引用类型（如 `int&`），那么 `T&&` 仍然是一个左值引用。
- 如果 `T` 是一个右值引用类型（如 `int&&`），那么 `T&&` 仍然是一个右值引用。
- 如果 `T` 是一个非引用类型（如 `int`），那么 `T&&` 是一个右值引用，但可以绑定到左值和右值。

#### 1.3 示例

```cpp
#include <iostream>

template <typename T>
void printType(T&& param) {
    std::cout << "Type of param: " << __PRETTY_FUNCTION__ << std::endl;
}

int main() {
    int x = 42;
    printType(x);      // 左值引用
    printType(42);     // 右值引用
    return 0;
}
```

### 2. 完美转发（Perfect Forwarding）

#### 2.1 定义

完美转发是指在模板函数中，将参数的类型和值类别（左值或右值）完全保留并传递给另一个函数。这样可以避免不必要的拷贝和临时对象的创建，提高代码的效率。

#### 2.2 实现

完美转发通常使用 `std::forward` 函数来实现。`std::forward` 是一个模板函数，它可以将万能引用转换为正确的引用类型（左值引用或右值引用）。

#### 2.3 示例

```cpp
#include <iostream>
#include <utility>

// 目标函数
void targetFunction(int& x) {
    std::cout << "Left value reference: " << x << std::endl;
}

void targetFunction(int&& x) {
    std::cout << "Right value reference: " << x << std::endl;
}

// 模板函数，使用完美转发
template <typename T>
void forwardFunction(T&& param) {
    targetFunction(std::forward<T>(param));
}

int main() {
    int x = 42;
    forwardFunction(x);      // 左值引用
    forwardFunction(42);     // 右值引用
    return 0;
}
```

### 3. 万能引用和完美转发的结合

万能引用和完美转发通常结合使用，以实现高效和灵活的参数传递。下面是一个更复杂的例子，展示了如何在模板函数中使用万能引用和完美转发。

#### 3.1 示例

```cpp
#include <iostream>
#include <vector>
#include <utility>

// 目标函数
template <typename T>
void addElement(std::vector<T>& vec, T&& element) {
    vec.push_back(std::forward<T>(element));
}

// 模板函数，使用完美转发
template <typename T, typename... Args>
void populateVector(std::vector<T>& vec, Args&&... args) {
    (addElement(vec, std::forward<Args>(args)), ...);
}

int main() {
    std::vector<int> vec;

    populateVector(vec, 1, 2, 3, 4, 5);

    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

### 4. 总结

- **万能引用**：可以绑定到左值和右值的引用类型，形式为 `T&&`，其中 `T` 是一个模板参数。
- **完美转发**：在模板函数中，将参数的类型和值类别完全保留并传递给另一个函数，使用 `std::forward` 函数实现。

通过理解和使用万能引用和完美转发，可以编写更高效、更灵活的模板代码，避免不必要的拷贝和临时对象的创建，提高程序的性能。

---





# 万能引用

```c++
#include<iostream>
#include<cstdlib>

using namespace std;

template <typename T>
void myFunction(const T&value)//value的类型是const T&
{
	return;
}

void myFunction02(int&& tem)//参数是右值引用类型
{
	cout << "右值引用函数" << endl;
	return;
}

/*修改函数模板后，tem既可以接受左值，又可以接受右值。
*tem的类型是T&&
*从编译未报错可以判断：T被推断出来不仅仅是int类型，（int&&tem,导致tem只能接受右值）
*传递左值，编译器推断为T&类型 int&
*传递右值，编译器推断为T&&类型 int&&
*/
template<typename T1>
//void myFunction03(const T1 &&tem)
void myFunction03(T1 &&tem)//&&属于tem类型的一部分，&&和T类型没有关系
{
	cout << "右值引用模板函数" << endl;
	return;
}

//类模板
template<typename T2>
class myTest
{
public:
	void testFunction(T&& tem)//这个是右值引用
	{
		
	}

	template<typename T3>
	void testFunction02(T3&& tem)//这个是万能引用
	{
		
	}
};

int main(void)
{
	myFunction(12);//T是什么类型，value是什么类型？  T和value的类型往往不同。
	/*
	 * T是int类型，value的类型是const int&类型
	 * 调用函数模板时候给的参数10会对T的类型产生影响。
	 * value的类型会对T的类型产生影响。--value的类型为万能引用，就会对T类型产生影响
	 */

	//(2)右值引用&&
	int&&number01 = 23;
	myFunction02(123);
	//myFunction02(number01);//error number01是一个左值

	myFunction03(123);
	myFunction03(number01);//函数模板时候可以传递左值
	
	int number02 = 12;
	myFunction03(number02);//int&&右值引用类型

	//其他万能引用 auto
	auto &&number03 = 234;


	//(5)
	myTest<int>mt;
	int i = 100;
	//mt.testFunction(i);//error，因为定义对象的时候已经指定类型为int，里面不存在类型模板参数。本身没有涉及到类型推断。
	//类模板实例化后，成员函数有具体类型，这里为void testFunction(int&& tem)。
	
	cout << "helloWorld" << endl;
	system("pause");
	return 0;
}

/*
 *(1)类型区别基本概念
 *类型是按照规定的规则读取内存块而划分的。
 *
 *(2)universal reference 万能引用，未定义引用 进本认识
 *	万能引用需要的语境：
 *		1.必须是函数模板
 *		2.必须发生了模板类型推断并且函数形参的的样子是 T&&（auto也存在万能引用）  T模板类型参数
 *	T&&就是万能引用类型
 *(3万能引用和右值引用的区别：
 *	1.右值引用，编译器需要传递右值，否则编译器报错
 *	2.万能引用作为函数形参时，我们可以给他传递左值，也可以传递右值
 *		如果我们传递进去左值，那么这个万能引用就变成了左值引用
 *		如果我们传递过去右值，这个万能引用就变成右值引用
 *	左值转换为右值std::move()
 *		
 *(4)判断下面是不是万能引用
 *		1.void func(int&&parm)//右值引用，因为没有模板
 *		2.template<typename T>
 *		  void func(T&&parm)
 *		  {}//是万能引用，有模板参数
 *
 *		3.template<typename T>
 *		  void func(std::vector<T>&& parm)
 *		  {}//右值引用，模板类型T，必须和&&挨着，是一个右值引用
 *
 *(5)万能引用资格的剥夺和辨认
 *	1.剥夺：const会剥夺成为万能引用的资格，使其成为右值引用  void myFunction03(const T1 &&tem)
 *	2.辨认：类的成员函数辨认
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
#include <vector>

using namespace std;

template<typename T> // 改造为函数模板
// void func(const int &abc)  // abc这个变量时一个什么类型？ const int &
void func(const T &abc)
{
    
}


// void myfunc(int &&tmprv) // 参数tmprv 是一个右值引用类型
template<typename T>
void myfunc(T &&tmprv) // 注意，这里的&&是属于形参tmprv的一部分。&&和T类型没有关系
// void myfunc(const T &&tmprv) // const 剥夺万能引用资格
// 修改为函数模板之后
// 1）看到tmprv既能接收左值，又能接收右值
// 2）tmprv的类型是 T&&
// 从编译不报错，T被推断出来的，不仅仅是int型（因为如果是int ,就会导致 int && tmprv 不能接收左值）
{
    // T&&或者说tmprv才是万能引用，而单独的T不是万能引用
    // 如果传递进来的是整形左值，tmprv就该推断为int &类型；
    // 如果传递进来的是整形右值，tmprv就该推断为int&&类型；
    
    cout << tmprv << endl;
    return;
}
template<typename T> void funcVec(std::vector<T>&& t)
{
    
}

template<typename T>
class Mytest
{
    public:
        void testFunc(T&& x)
        // 这个x不是万能引用，只是一个右值引用
        // 因为这个testFunc是一个成员函数，成员函数并不涉及到模板类型推断
        // 我们在调用这个类模板的时候，需要先对这个类模板进行实例化，在这个时候，这个类就已经被实例化出来了
        // 那么他的成员函数就已经确定了参数类型了
        {
            std::cout << x << std::endl;
        }
        
        template<typename T2>
        void testFunc2(T2 && x)
        // 是万能引用，因为他需要在实际调用的时候才能进行类型推断
        {
            std::cout << x << std::endl;
        }
        
};

int main()
{
    // printf("Hello World");
    // 类型区别基本概念

    // func(10);
    // T 是什么类型？abc是什么类型？
    // T的类型和abc的类型往往不相同
    // T 的类型 int, abc 的类型 const int &
    // 调用函数模板时给的参数10会对T的类型产生影响，abc的类型会对T的类型产生影响
    
    // universal reference / 万能引用 / 未定义引用
    // &&表示右值引用。
    // int &&rv = 100;
    
    myfunc(100);
    int i = 100;
    // myfunc(i); // cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’
    // 错误，右值引用不能用来接收左值（引用）
    myfunc(i); // myfunc()为函数模板时，就可以正常调用
    
    
    // 万能引用语境
    // 1）必须是函数模板
    // 2）必须发生了模板类型推断，并且函数模板形参的样子是：T&&， auto也存在万能引用的概念
    // T&& 就是万能引用
    
    // 万能引用和右值引用解释起来的区别
    // 1）右值引用得传递右值，否者编译器报错
    // 2）万能引用作为函数形参时，可以传递左值，也可以传递右值
    // 如果我们传递的是左值，那么这个万能引用就变成了左值引用
    // 如果我们传递的是右值，那么这个万能引用就变成了右值引用
    
    // 1）void func(int &&t); 右值引用
    // 2）template<typename T> void func(T&& t); 万能引用
    // 3）template<typename T> void func(std::vector<T>&& t); 右值引用(因为 T 和 && 没有紧挨着)
    
    vector<int> av = {12,12,1234,4};
    // funcVec(av); // 无法将左值转为右值
    funcVec(std::move(av));
    
    // 万能引用资格剥夺与辨认
    // 剥夺
    // const 修饰符会剥夺一个引用成为万能引用的资格，会被打回原形，变成普通的右值引用
    // 所以，函数模板类型参数T前面不要加任何修饰词
    // 辨认
    
    Mytest<int> mt;
    int i0 = 100;
    // mt.testFunc(i0); // cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’
    mt.testFunc2(i0);
    mt.testFunc(std::move(i0));

    return 0;
}


```
