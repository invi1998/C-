****（1）typename的使用场合

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

typename的应用场合_用在模板函数类型成员前表示是一个类型

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
* 1.在模板定义里面。typename标明其后的参数是类型参数  template<typename T,int a,int b>
* template<typename T>  //名字为T的模板参数
* typename 可以使用class，这里的class不是类定义，表示类型参数
* 2.使用类的类型成员，用typename来表示这是一个类型
* ::可以表示类成员作用域 比如访问类的静态成员，可以使用 类名：：静态成员名
* ::还可以表示访问模板类的类型成员。
*    函数返回值
*  typename myVector<T>::myIterator   myVector<T>::myend()
*  typename 的用处就是显式告诉编译器myVector<T>::myIterator是一个类型
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



在C++中，`typename` 关键字主要用于两种场合：一是模板参数列表中，二是模板内部的嵌套类型声明。`typename` 的主要作用是告诉编译器某个标识符是一个类型，而不是一个值或成员变量。下面是这两种场合的具体解释和示例。

### 1. 模板参数列表中

在模板参数列表中，`typename` 和 `class` 关键字可以互换使用，用于声明类型参数。不过，`typename` 更加通用，因为它也可以用于嵌套类型声明。

#### 示例

```cpp
template <typename T>
class MyTemplate {
public:
    void process(T value) {
        // 处理 T 类型的值
    }
};

int main() {
    MyTemplate<int> intTemplate;
    intTemplate.process(10);

    MyTemplate<double> doubleTemplate;
    doubleTemplate.process(3.14);

    return 0;
}
```

### 2. 模板内部的嵌套类型声明

在模板内部，当你需要引用一个依赖于模板参数的类型时，必须使用 `typename` 关键字。这是因为编译器在解析模板时，无法确定某个标识符是类型还是值。使用 `typename` 可以明确告诉编译器这是一个类型。

#### 示例

假设我们有一个类模板 `MyClass`，它包含一个嵌套的类型 `NestedType`。我们在另一个模板 `AnotherTemplate` 中需要引用这个嵌套类型。

```cpp
template <typename T>
class MyClass {
public:
    typedef T NestedType;  // 嵌套类型
};

template <typename T>
class AnotherTemplate {
public:
    void process() {
        typename MyClass<T>::NestedType value;  // 必须使用 typename
        // 处理 value
    }
};

int main() {
    AnotherTemplate<int> intTemplate;
    intTemplate.process();

    AnotherTemplate<double> doubleTemplate;
    doubleTemplate.process();

    return 0;
}
```

### 详细解释

在 `AnotherTemplate` 中，`MyClass<T>::NestedType` 是一个依赖于模板参数 `T` 的类型。编译器在解析 `MyClass<T>::NestedType` 时，无法确定 `NestedType` 是一个类型还是一个值。为了消除这种不确定性，我们需要使用 `typename` 关键字来明确告诉编译器 `NestedType` 是一个类型。

### 3. 模板特化中的使用

在模板特化中，`typename` 也可以用于声明类型参数。

#### 示例

```cpp
template <typename T>
class MyTemplate {
public:
    void process(T value) {
        std::cout << "General template: " << value << std::endl;
    }
};

// 特化模板
template <>
class MyTemplate<int> {
public:
    void process(int value) {
        std::cout << "Specialized template for int: " << value << std::endl;
    }
};

int main() {
    MyTemplate<double> doubleTemplate;
    doubleTemplate.process(3.14);

    MyTemplate<int> intTemplate;
    intTemplate.process(10);

    return 0;
}
```

### 总结

- **模板参数列表中**：`typename` 和 `class` 可以互换使用，用于声明类型参数。
- **模板内部的嵌套类型声明**：当需要引用一个依赖于模板参数的类型时，必须使用 `typename` 关键字。
- **模板特化**：`typename` 也可以用于声明特化模板的类型参数。

通过使用 `typename`，可以确保编译器正确解析模板中的类型，避免类型解析的二义性问题。这对于编写复杂的模板代码非常重要。

---



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
* 1.如果类重载了函数调用运算符（也就是括号运算符），则我们可以像使用函数一样使用该类的对象，
*  我们把这样的类对象称为函数对象（function object）
* 2.C++语言中有几种可调用对象：函数、函数指针、lamba表达式、
*  bind创建的对象以及重载了函数运算符的类。
*
*(2)
*
*(3)
*
*
*/
```

在C++中，可调用对象（Callable Object）是指可以像函数一样调用的对象。可调用对象可以是函数指针、函数对象（functor）、lambda表达式或绑定表达式（如 `std::bind` 的结果）。C++标准库提供了多种工具来处理和操作可调用对象，使其在函数式编程和泛型编程中非常有用。

### 可调用对象的类型

1. **函数指针**：
   - 指向函数的指针，可以直接调用。
   - 示例：
     ```cpp
     void foo(int x) {
         std::cout << "foo called with " << x << std::endl;
     }
     
     int main() {
         void (*func_ptr)(int) = foo;
         func_ptr(10);  // 调用 foo
         return 0;
     }
     ```

2. **函数对象（Functor）**：
   - 重载了 `operator()` 的类对象。
   - 示例：
     ```cpp
     class Adder {
     public:
         int operator()(int a, int b) const {
             return a + b;
         }
     };
     
     int main() {
         Adder adder;
         int result = adder(10, 20);  // 调用 adder
         std::cout << "Result: " << result << std::endl;  // 输出 30
         return 0;
     }
     ```

3. **Lambda表达式**：
   - 匿名函数，可以捕获外部变量。
   - 示例：
     ```cpp
     int main() {
         auto lambda = [](int a, int b) {
             return a + b;
         };
         int result = lambda(10, 20);  // 调用 lambda
         std::cout << "Result: " << result << std::endl;  // 输出 30
         return 0;
     }
     ```

4. **绑定表达式**：
   - 使用 `std::bind` 绑定函数或函数对象。
   - 示例：
     ```cpp
     #include <functional>
     #include <iostream>
     
     void foo(int x, int y) {
         std::cout << "foo called with " << x << " and " << y << std::endl;
     }
     
     int main() {
         auto bound_func = std::bind(foo, 10, std::placeholders::_1);
         bound_func(20);  // 调用 foo(10, 20)
         return 0;
     }
     ```

### 标准库中的可调用对象

C++标准库提供了一些工具来处理可调用对象，例如 `std::function` 和 `std::invoke`。

1. **`std::function`**：
   - 一个通用的多态函数包装器，可以存储任何可调用对象。
   - 示例：
     ```cpp
     #include <functional>
     #include <iostream>
     
     void foo(int x) {
         std::cout << "foo called with " << x << std::endl;
     }
     
     class Adder {
     public:
         int operator()(int a, int b) const {
             return a + b;
         }
     };
     
     int main() {
         std::function<void(int)> func1 = foo;
         func1(10);  // 调用 foo
     
         Adder adder;
         std::function<int(int, int)> func2 = adder;
         int result = func2(10, 20);  // 调用 adder
         std::cout << "Result: " << result << std::endl;  // 输出 30
     
         auto lambda = [](int a, int b) {
             return a + b;
         };
         std::function<int(int, int)> func3 = lambda;
         result = func3(10, 20);  // 调用 lambda
         std::cout << "Result: " << result << std::endl;  // 输出 30
     
         return 0;
     }
     ```

2. **`std::invoke`**：
   - 用于调用可调用对象，可以处理函数指针、成员函数指针、成员变量指针等。
   - 示例：
     ```cpp
     #include <functional>
     #include <iostream>
     
     void foo(int x) {
         std::cout << "foo called with " << x << std::endl;
     }
     
     class MyClass {
     public:
         void bar(int x) {
             std::cout << "bar called with " << x << std::endl;
         }
     };
     
     int main() {
         std::invoke(foo, 10);  // 调用 foo
     
         MyClass obj;
         std::invoke(&MyClass::bar, obj, 20);  // 调用 obj.bar
     
         auto lambda = [](int a, int b) {
             return a + b;
         };
         int result = std::invoke(lambda, 10, 20);  // 调用 lambda
         std::cout << "Result: " << result << std::endl;  // 输出 30
     
         return 0;
     }
     ```

### 总结

- **函数指针**：指向函数的指针，可以直接调用。
- **函数对象（Functor）**：重载了 `operator()` 的类对象。
- **Lambda表达式**：匿名函数，可以捕获外部变量。
- **绑定表达式**：使用 `std::bind` 绑定函数或函数对象。
- **`std::function`**：通用的多态函数包装器，可以存储任何可调用对象。
- **`std::invoke`**：用于调用可调用对象，可以处理多种类型的调用。

通过使用这些可调用对象和相关的标准库工具，可以编写更加灵活和强大的C++代码，特别是在需要函数式编程和泛型编程的场景中。

---



invoke

`std::invoke` 是 C++17 引入的一个非常有用的工具，它提供了一种通用的方式来调用可调用对象。虽然在许多情况下，我们可以直接调用函数，但 `std::invoke` 在某些特定场景下提供了更大的灵活性和便利性。以下是 `std::invoke` 的一些典型使用场景以及为什么在这些场景中使用 `std::invoke` 而不是直接调用函数。

### 1. 通用性

#### **调用多种类型的可调用对象**

`std::invoke` 可以处理多种类型的可调用对象，包括函数指针、成员函数指针、成员变量指针、函数对象（functor）和 lambda 表达式。这使得它在编写泛型代码时非常有用。

#### 示例

```cpp
#include <iostream>
#include <functional>

class MyClass {
public:
    void memberFunction(int x) {
        std::cout << "Member function called with " << x << std::endl;
    }

    int memberVariable = 10;
};

void freeFunction(int x) {
    std::cout << "Free function called with " << x << std::endl;
}

int main() {
    MyClass obj;
    int x = 20;

    // 调用成员函数
    std::invoke(&MyClass::memberFunction, obj, x);

    // 调用自由函数
    std::invoke(freeFunction, x);

    // 调用成员变量
    int value = std::invoke(&MyClass::memberVariable, obj);
    std::cout << "Member variable value: " << value << std::endl;

    // 调用 lambda 表达式
    auto lambda = [](int a, int b) { return a + b; };
    int result = std::invoke(lambda, x, x);
    std::cout << "Lambda result: " << result << std::endl;

    return 0;
}
```

### 2. 间接调用

#### **通过变量或参数调用**

在某些情况下，你需要根据运行时信息来决定调用哪个函数。`std::invoke` 可以帮助你在这种情况下编写更简洁的代码。

#### 示例

```cpp
#include <iostream>
#include <functional>

void function1(int x) {
    std::cout << "Function 1 called with " << x << std::endl;
}

void function2(int x) {
    std::cout << "Function 2 called with " << x << std::endl;
}

int main() {
    int x = 10;
    bool useFunction1 = true;

    auto func = useFunction1 ? function1 : function2;
    std::invoke(func, x);

    return 0;
}
```

### 3. 通用算法

#### **在标准库算法中使用**

许多标准库算法（如 `std::for_each`、`std::transform` 等）需要一个可调用对象作为参数。`std::invoke` 可以在这种情况下提供更大的灵活性。

#### 示例

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

class MyClass {
public:
    void process(int x) {
        std::cout << "Processing " << x << std::endl;
    }
};

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    MyClass obj;

    // 使用 std::invoke 作为可调用对象
    std::for_each(numbers.begin(), numbers.end(), std::bind(std::invoke, &MyClass::process, &obj, std::placeholders::_1));

    return 0;
}
```

### 4. 解决二义性

#### **处理成员函数指针的二义性**

在某些情况下，直接调用成员函数指针可能会导致二义性。`std::invoke` 可以帮助解决这种二义性。

#### 示例

```cpp
#include <iostream>
#include <functional>

class Base {
public:
    void func(int x) {
        std::cout << "Base::func called with " << x << std::endl;
    }
};

class Derived : public Base {
public:
    void func(int x) {
        std::cout << "Derived::func called with " << x << std::endl;
    }
};

int main() {
    Derived obj;
    int x = 10;

    // 使用 std::invoke 调用基类的成员函数
    std::invoke(static_cast<void (Base::*)(int)>(&Derived::func), obj, x);

    return 0;
}
```

### 总结

- **通用性**：`std::invoke` 可以处理多种类型的可调用对象，适用于泛型编程。
- **间接调用**：在需要根据运行时信息调用不同函数的场景中，`std::invoke` 提供了更大的灵活性。
- **通用算法**：在标准库算法中，`std::invoke` 可以作为可调用对象的通用接口。
- **解决二义性**：在处理成员函数指针的二义性时，`std::invoke` 可以提供清晰的解决方案。

通过使用 `std::invoke`，你可以编写更加灵活、通用和健壮的代码，特别是在需要处理多种类型可调用对象的场景中。

---



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
* a.类模板--类模板名字后面必须使用<>来提供额外的信息。<>表示这是一个模板
* b.函数模板的默认参数
*  老标准只能为类模板提供默认模板参数，c++11新标准可以为函数模板提供默认参数
*(2)
*
*(3)
*
*
*/
```
