```c++
/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

shared_ptr<int> makes(int value)
{
    // return new int(value); // 不可以，无法将new得到的int *转换为shared_ptr
    return shared_ptr<int>(new int(value)); // 可以，显式的用int *创建shared_ptr<int>对象
}

int main()
{
    int *p = new int();
    int *q = p;
    int *r = q;
    // 只有pqr都不在使用了，才能进行内存释放
    // cout << p << endl;
    // cout << q << endl;
    // cout << r << endl;
    // 0x638c20
    // 0x638c20
    // 0x638c20
    
    // auto_ptr (C++98, 目前auto_ptr已经在新标准中被弃用，尽量避免使用)
    // unique_ptr (c++11)
    // shared_ptr (c++11)
    // weak_ptr (c++11)
    // 这三中智能指针都是类模板，我们可以将new获得的指针赋值给他们
    // shared_ptr 是共享式指针，多个指针指向同一个对象，最后一个指向被销毁时，这个对象才会被释放
    // unique_ptr 独占式指针，同一个时间内，只有一个指针指向对象。当然该独享的所有权是可以被移交的
    // weak_ptr 是辅助shared_ptr的
    
    shared_ptr<int> pi(new int(100)); // 指向一个值为100的int
    // shared_ptr<int> pi = new int(100); // 错误。因为智能指针时explicit，不可以进型隐式类型转换，必须使用直接初始化形式（一般带=的都是隐式类型转换）
    cout << pi << endl;
    
    
    shared_ptr<int> ptr = makes(400);
    cout << *ptr << endl;

    // 裸指针可以初始化shared_ptr,但是不推荐，智能指针和裸指针不要穿插使用
    // int *ppi = new int;
    // shared_ptr<int> spi(ppi); // 可以，但是不推荐，会有使用陷阱
    
    // 非要用，也得是使用new来进行
    shared_ptr<int> spi(new int);
    
    // make_shared 函数
    // 他是标准库里的一个函数模板。安全，高效
    // 他能在动态内存（堆）中，创建和初始化一个对象，并返回此对象的shared_ptr
    shared_ptr<int> p2 = make_shared<int>(100); // 这个shared_ptr 指向一个值为100的内存， 有点类似int *p1 = new int (100);
    shared_ptr<string> p3 = make_shared<string>(5, 'a');
    cout << *p3 << endl; // aaaaa
    
    shared_ptr<int> p4 = make_shared<int>(); // p4指向一个int，int里保存的值是0
    p4 = make_shared<int>(500); // p4指向一个新的int，int里保存的是500， P4首先先释放刚才指向0的内存，然后再指向500的int内存
    
    return 0;
}

```

newdelete探秘
```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>

using namespace std;

class A
{
public:
	
	A()
	{
		cout << "构造函数被执行" << endl;
	}
	~A();
	
};
A::~A()
{
	cout << "析构函数被执行" << endl;
}


int main(void)
{
	//(1)
	A*a1 = new A();//调用类A的构造函数
	delete a1;//该类的析构函数被执行

	int *p1 = new int(100);
	delete p1;
	//operator new()  和operator delete()是函数
	/*
	 * new干了两个事情
	 * （1）在堆上分配内存，就是通过operator new()来分配内存
	 * （2）调用构造函数来初始化内存。
	 *delete也干了两个事情：
	 *（1）调用析构函数，做一些对象释放前的操作
	 *（2）释放内存 --调用operator delete()函数来释放内存
	 */

	//operator new()函数可以直接调用
	void* myPoint = operator new (100);//直接分配100个字节


	//(2)
	int *p3 = new int(23);//分配4个字节
	delete p3;//删除的时候，编译器是怎么知道要释放4个字节？？？
	/*
	 * new出的内存空间中单独分配空间来记录分配的内存空间的大小，自己内部有记录机制。不需要深入了解。
	 */
	
	//(3)申请和释放一个数组  这些可以在MFC框架下演示
	int *p4 = new int[2];//int类型数组，有2个元素，占有8个字节 没有回收

	A a2;
	cout << sizeof(a2) << endl;//1  泄漏一个字节
	cout << sizeof(A) << endl;//1

	A*p5 = new A();//泄漏1字节

	A*p6 = new A[2]();//泄漏6个字节，比我们想的多出来4个字节
	
	/*
	 * 为什么空A对象a2占有1个字节？
	 * 因为每个对象分配了内存空间，至少占有一个地址空间，至少占有1个字节的地址。
	 *
	 *
	 * 疑问：为什么定义类型A数组会多泄漏，也就是给类型A对象数组多分配4个字节，而内置类型int并没有多分配？
	 * 定义类对象数组，需要额外的4个字节来记录是多少个数组元素，以便后面调用多少次析构函数，
	 */

	int *p7 = new int[3];
	delete p7;//没有使用[]，也可以释放，没有内存泄漏，但是建议使用delete []p7;

	A*p8 = new A[2]();//泄漏6字节，其中2个字节是对象数组，另外4个。。。是一个整形，记录的是数组元素的个数，这里是数字2
	delete[]p8;//类类型必须使用[]，不使用会报异常  ----本来应该调用2次析构函数，却只调用一次，就会导致系统异常


	/*A*p9 = new A;
	delete[]p9;*/
	//系统一直调用析构函数，因为不知道数组元素的个数
	
	system("pause");
	return 0;
}

/*
*(1)new delete探秘
*	1.sizeof new delete都是运算符，不是运算符
*	2.new delete是一对，malloc() free()是一对，new和delete会调用构造函数和析构函数。
*		也就是说new和delete具备对堆上所分配内存初始化，反初始化的能力。malloc 和free不具有这种能力。
*	总结：进行运算符的重载来进行函数operator new() operator delete()来进行不同数据类型的分配和释放。
*	
*(2)基本new如何记录分配的内存大小供delete使用？
*	不同编译器new内部有不同的实现方式。
*
*(3)申请和释放一个数组
*	new进行类对象数组分配的时候会额外占用内存空间记录数组个数，这里是占用4个字节记录类对象数组的元素个数，
*	以便后面调用析构函数释放多少次析构函数。
*	如果没有析构函数，系统就不会额外分配内存空间记录额外的类对象数组个数。
*
*(4)为什么对数组释放内存不加[]就会报异常？？
*A*p8 = new A[2]();
*delete[]p8;//这里使用delete p8;就会报异常
*写成delete p8;
*	1.只会调用一次析构函数只析构了一个对象，有内存泄漏
*	2.调用operator delete(p8)释放内存的时候，因为不知道数组元素的个数，会导致释放内存错乱。不知道释放多少个内存空间。
*总结:new delete 和new[] delete[]配套使用
*
*如果一个对象，使用new[]来进行分配内存，但是却单独的使用delete来释放内存，需要满足一下条件
 1、内置的对象类型
 2、自定义类 无自定义的析构函数
*
*/
```



在C++中，`operator new` 和 `operator delete` 是用于动态内存管理的运算符。它们分别用于分配和释放内存。与 `new` 和 `delete` 关键字不同，`operator new` 和 `operator delete` 是可以被重载的，这使得开发者可以在特定情况下自定义内存管理的行为。

### 1. `operator new`

`operator new` 是一个全局运算符，用于分配内存。默认情况下，`operator new` 分配内存并返回指向该内存的指针。如果分配失败，`operator new` 会抛出 `std::bad_alloc` 异常。

#### 基本用法

```cpp
#include <iostream>
#include <new>  // 包含 std::bad_alloc

int main() {
    try {
        int* ptr = static_cast<int*>(operator new(sizeof(int)));
        *ptr = 10;
        std::cout << "*ptr: " << *ptr << std::endl;
        operator delete(ptr);
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
    }

    return 0;
}
```

#### 重载 `operator new`

可以为特定类重载 `operator new`，以实现自定义的内存分配策略。

```cpp
#include <iostream>
#include <new>  // 包含 std::bad_alloc

class MyClass {
public:
    void* operator new(size_t size) {
        std::cout << "Custom operator new called, allocating " << size << " bytes" << std::endl;
        void* ptr = ::operator new(size);  // 调用全局 operator new
        if (ptr == nullptr) {
            throw std::bad_alloc();
        }
        return ptr;
    }

    void operator delete(void* ptr) noexcept {
        std::cout << "Custom operator delete called" << std::endl;
        ::operator delete(ptr);  // 调用全局 operator delete
    }
};

int main() {
    try {
        MyClass* obj = new MyClass();
        delete obj;
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
    }

    return 0;
}
```

### 2. `operator delete`

`operator delete` 是一个全局运算符，用于释放由 `operator new` 分配的内存。默认情况下，`operator delete` 接受一个指针并释放该指针指向的内存。

#### 基本用法

```cpp
#include <iostream>
#include <new>  // 包含 std::bad_alloc

int main() {
    try {
        int* ptr = static_cast<int*>(operator new(sizeof(int)));
        *ptr = 10;
        std::cout << "*ptr: " << *ptr << std::endl;
        operator delete(ptr);
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
    }

    return 0;
}
```

#### 重载 `operator delete`

可以为特定类重载 `operator delete`，以实现自定义的内存释放策略。

```cpp
#include <iostream>
#include <new>  // 包含 std::bad_alloc

class MyClass {
public:
    void* operator new(size_t size) {
        std::cout << "Custom operator new called, allocating " << size << " bytes" << std::endl;
        void* ptr = ::operator new(size);  // 调用全局 operator new
        if (ptr == nullptr) {
            throw std::bad_alloc();
        }
        return ptr;
    }

    void operator delete(void* ptr) noexcept {
        std::cout << "Custom operator delete called" << std::endl;
        ::operator delete(ptr);  // 调用全局 operator delete
    }
};

int main() {
    try {
        MyClass* obj = new MyClass();
        delete obj;
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
    }

    return 0;
}
```

### 3. 数组形式的 `operator new[]` 和 `operator delete[]`

除了单个对象的 `operator new` 和 `operator delete`，C++ 还提供了数组形式的 `operator new[]` 和 `operator delete[]`，用于分配和释放数组的内存。

#### 基本用法

```cpp
#include <iostream>
#include <new>  // 包含 std::bad_alloc

int main() {
    try {
        int* arr = static_cast<int*>(operator new[](10 * sizeof(int)));
        for (int i = 0; i < 10; ++i) {
            arr[i] = i * 10;
        }
        for (int i = 0; i < 10; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
        operator delete[](arr);
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
    }

    return 0;
}
```

#### 重载 `operator new[]` 和 `operator delete[]`

可以为特定类重载 `operator new[]` 和 `operator delete[]`，以实现自定义的数组内存管理策略。

```cpp
#include <iostream>
#include <new>  // 包含 std::bad_alloc

class MyClass {
public:
    void* operator new[](size_t size) {
        std::cout << "Custom operator new[] called, allocating " << size << " bytes" << std::endl;
        void* ptr = ::operator new[](size);  // 调用全局 operator new[]
        if (ptr == nullptr) {
            throw std::bad_alloc();
        }
        return ptr;
    }

    void operator delete[](void* ptr) noexcept {
        std::cout << "Custom operator delete[] called" << std::endl;
        ::operator delete[](ptr);  // 调用全局 operator delete[]
    }
};

int main() {
    try {
        MyClass* arr = new MyClass[10];
        delete[] arr;
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
    }

    return 0;
}
```

### 总结

- **`operator new`**：用于分配内存，默认情况下会抛出 `std::bad_alloc` 异常。
- **`operator delete`**：用于释放内存，默认情况下不会抛出异常。
- **重载 `operator new` 和 `operator delete`**：可以在特定类中重载这些运算符，以实现自定义的内存管理策略。
- **数组形式的 `operator new[]` 和 `operator delete[]`**：用于分配和释放数组的内存，也可以在特定类中重载。

通过重载这些运算符，可以实现更高效的内存管理和特定的内存分配策略，满足特定应用的需求。

---



智能指针概述和shared_ptr基础

```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
#include <memory>

using namespace std;

shared_ptr<int> testFunction(int value)
{
	//return new int();//error，不可以将new得到的int*类型转换为shared_ptr
	return shared_ptr<int>(new int(value));//OK，强制类型转换
}

int main(void)
{
	//shared_ptr指针用法
	shared_ptr<int>p1(new int(100));//p1指向值为100的int类型内存空间
									//shared_ptr<int>p2 = new int(100);//error智能指针是explicit,不可以进行隐式类型转换，
									//=一般用于隐式类型zhuanhaun


									//返回值是shared_ptr
	shared_ptr<int> p2 = testFunction(234);

	//裸指针可以初始化shared_ptr,但是不推荐
	string* s1 = new string("23432434");
	shared_ptr<string>s2(s1);
	//建议使用
	shared_ptr<string>s3(new string("sdfdsf"));

	//空的shared_ptr指针
	shared_ptr<int>p3;//指向int的智能指针，但还是目前为空==nullptr

					  //(3)make_shared
	shared_ptr<int>p4 = make_shared<int>(1345);//shared_ptr指向一个值为100的整形内存空间。
	shared_ptr<string>p5 = make_shared<string>(5, 'a');//p5指向aaaaa的字符串内存空间
	shared_ptr<int>p6 = make_shared<int>();//p6指向一段内存空间int=0
	p6 = make_shared<int>(1000);
	/*p6指向一段新的int，先释放指向0的内存空间，然后指向int值为1000的内存空间。
	*
	*/

	system("pause");
	return 0;
}

/*
*ptr==pointer的缩写
*
*(1)智能指针综述
*常见错误
*if()
*{
*	.....
*	return ;
*}
*delete p；//执行不到这里
*
*多个指针指向同一块内存空间，释放内存只能释放一次。
*所以new delete的写法要十分小心，要小小早早释放，也小心忘记释放。
*
*裸指针：直接new返回的指针。没有经过任何包装，这种指针强大，灵活，但是开发者要全程维护。
*引入智能指针解决裸指针的各种问题。
*智能指针：就理解成对裸指针进行了包装，包装后：
*最突出优点：智能指针“能够自动释放所指向的对象内存。”不用担心new出来的内存忘记释放。
*
*建议：优先使用智能指针，使用智能指针的程序更容易编写和调试。
*
*c++中有4种智能指针：
*auto_ptr（c++98）
*unique_ptr(c++11)
*shared_ptr(c++11)
*weak_ptr(c++11)
*帮助我们进行动态分配对象的生命周期的管理，能够有效防止内存泄漏。
*
*c++11标准中反对使用auto_ptr;目前auto——ptr已经完全被unique_ptr取代。
*c++11中的3种智能指针都是类模板，我们可以将new获得的地址赋值给他们
*重点是shared_ptr：共享式指针--多个指针指向同一个对象，最后一个指针被销毁时候，释放对象的内存空间。
*weak_ptr是辅助shared_ptr;
*unique_ptr独占式指针，在一个时间内，只有一个指针能够指向该对象，该对象的所有权是可以移交出去的。
*
*你忘记delete的时候，智能指针帮你delete，或者说，你不需要手动delete了。
*
*(2)shared_ptr基础
*共享所有权--不是被一个shared_ptr拥有，而是多个shared_ptr之间相互协作。
*工作原理：引用计数，每个shared_ptr都指向相同的内存，所以最后一个指向不需要指向该对象的时候。
*那么最后一个share_ptr去释放该对象。
*
*最后一个指向对象的shared_ptr在什么情况下释放该对象（shared_ptr所指向的对象）？
*	1.shared_ptr声明周期结束，被析构时候
*	2.这个shared_ptr指向其他对象的时候。
*	这种操作类似于java中的垃圾回收机制。
*是类模板,<>里面就是指针可以指向的类型，后边再跟智能指针名字
*格式：shared_ptr<指向的类型>智能指针名字
*
*
*(3)make_shared函数，标准库里面的函数模板，安全高效分配和使用shared_ptr
*能够在堆中分配内存并初始化一个对象，然后返回指向此对象的shared_ptr
*make_shared不能自定义删除器，因此需要自定义删除器的时候，不能使用。
*
*/
```





在C++中，智能指针是一种用于管理动态分配内存的工具，它们自动管理内存的生命周期，避免了手动管理内存带来的风险，如内存泄漏、双重删除等问题。C++标准库提供了几种常用的智能指针，包括 `std::unique_ptr`、`std::shared_ptr` 和 `std::weak_ptr`。

### 1. `std::unique_ptr`

`std::unique_ptr` 是一种独占所有权的智能指针。它确保在同一时间只有一个 `std::unique_ptr` 指向某个对象。当 `std::unique_ptr` 被销毁时，它所管理的对象也会被自动删除。

#### 基本用法

```cpp
#include <iostream>
#include <memory>

int main() {
    // 创建一个 unique_ptr
    std::unique_ptr<int> ptr(new int(10));

    // 访问指针所指向的对象
    std::cout << *ptr << std::endl;  // 输出 10

    // 释放内存
    ptr.reset();  // 也可以让 unique_ptr 自动管理内存

    return 0;
}
```

#### 转移所有权

`std::unique_ptr` 不支持复制，但支持移动语义，可以通过 `std::move` 转移所有权。

```cpp
#include <iostream>
#include <memory>

int main() {
    std::unique_ptr<int> ptr1(new int(10));
    std::unique_ptr<int> ptr2 = std::move(ptr1);  // 转移所有权

    if (ptr1 == nullptr) {
        std::cout << "ptr1 is null" << std::endl;  // 输出 "ptr1 is null"
    }

    std::cout << *ptr2 << std::endl;  // 输出 10

    return 0;
}
```

### 2. `std::shared_ptr`

`std::shared_ptr` 是一种共享所有权的智能指针。多个 `std::shared_ptr` 可以同时指向同一个对象，当最后一个 `std::shared_ptr` 被销毁时，对象才会被删除。

#### 基本用法

```cpp
#include <iostream>
#include <memory>

int main() {
    // 创建一个 shared_ptr
    std::shared_ptr<int> ptr1 = std::make_shared<int>(10);

    // 访问指针所指向的对象
    std::cout << *ptr1 << std::endl;  // 输出 10

    // 创建另一个 shared_ptr 指向同一个对象
    std::shared_ptr<int> ptr2 = ptr1;

    // 检查引用计数
    std::cout << "Use count: " << ptr1.use_count() << std::endl;  // 输出 "Use count: 2"

    return 0;
}
```

#### 自定义删除器

`std::shared_ptr` 支持自定义删除器，可以在对象被删除时执行特定的操作。

```cpp
#include <iostream>
#include <memory>

void customDeleter(int* ptr) {
    std::cout << "Custom deleter called" << std::endl;
    delete ptr;
}

int main() {
    std::shared_ptr<int> ptr(new int(10), customDeleter);

    std::cout << *ptr << std::endl;  // 输出 10

    return 0;
}
```

### 3. `std::weak_ptr`

`std::weak_ptr` 是一种不控制对象生命周期的智能指针。它主要用于解决 `std::shared_ptr` 之间的循环引用问题。`std::weak_ptr` 不能直接访问对象，需要先转换为 `std::shared_ptr`。

#### 基本用法

```cpp
#include <iostream>
#include <memory>

int main() {
    // 创建一个 shared_ptr
    std::shared_ptr<int> sharedPtr = std::make_shared<int>(10);

    // 创建一个 weak_ptr
    std::weak_ptr<int> weakPtr = sharedPtr;

    // 检查 weak_ptr 是否仍然有效
    if (!weakPtr.expired()) {
        std::shared_ptr<int> sharedPtr2 = weakPtr.lock();  // 转换为 shared_ptr
        std::cout << *sharedPtr2 << std::endl;  // 输出 10
    } else {
        std::cout << "weak_ptr has expired" << std::endl;
    }

    return 0;
}
```

### 4. 总结

- **`std::unique_ptr`**：独占所有权，确保在同一时间只有一个指针指向某个对象。支持移动语义，但不支持复制。
- **`std::shared_ptr`**：共享所有权，多个指针可以同时指向同一个对象。当最后一个 `std::shared_ptr` 被销毁时，对象才会被删除。支持自定义删除器。
- **`std::weak_ptr`**：不控制对象生命周期，主要用于解决 `std::shared_ptr` 之间的循环引用问题。需要先转换为 `std::shared_ptr` 才能访问对象。

通过使用这些智能指针，可以有效地管理动态分配的内存，避免内存泄漏和其他内存管理问题。选择合适的智能指针取决于具体的应用场景和需求。