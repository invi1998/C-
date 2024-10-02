（1）构造函数；

（2）多个构造函数；

（3）函数默认参数；

（4）隐式转换和explicit；

（5）构造函数初始化列表；

构造函数初始化列表以一个冒号开始，接着是以逗号分隔的数据成员列表，每个数据成员后面跟一个放在括号中的初始化式。例如：

```c++
class CExample {
public:

    int a;
    float b;
    //构造函数初始化列表
    CExample(): a(0),b(8.8)
    {}
    //构造函数内部赋值
    CExample()
    {
        a=0;
        b=8.8;
    }

}; 

```

上面的例子中两个构造函数的结果是一样的。上面的构造函数（使用初始化列表的构造函数）显式的初始化类的成员；而没使用初始化列表的构造函数是对类的成员赋值，并没有进行显式的初始化。

初始化和赋值对内置类型的成员没有什么大的区别，像上面的任一个构造函数都可以。<font color="F44336">对非内置类型成员变量，为了避免两次构造，推荐使用类构造函数初始化列表。</font>但有的时候必须用带有初始化列表的构造函数：

- 1.成员类型是没有默认构造函数的类。若没有提供显示初始化式，则编译器隐式使用成员类型的默认构造函数，若类没有默认构造函数，则编译器尝试使用默认构造函数将会失败。
- 2.const 成员或引用类型的成员。因为 const 对象或引用类型只能初始化，不能对他们赋值。



初始化数据成员与对数据成员赋值的含义是什么？有什么区别？

首先把数据成员按类型分类并分情况说明:

- 1.内置数据类型，复合类型（指针，引用）- 在成员初始化列表和构造函数体内进行，在性能和结果上都是一样的
- 2.用户定义类型（类类型）- 结果上相同，但是性能上存在很大的差别。因为类类型的数据成员对象在进入函数体前已经构造完成，也就是说在成员初始化列表处进行构造对象的工作，调用构造函数，在进入函数体之后，进行的是对已经构造好的类对象的赋值，又调用个拷贝赋值操作符才能完成（如果并未提供，则使用编译器提供的默认按成员赋值行为）
注意点：

初始化列表的成员初始化顺序:

<font color="#2196F3">C++ 初始化类成员时，是按照声明的顺序初始化的，而不是按照出现在初始化列表中的顺序。</font>

```c++
class CMyClass {
    CMyClass(int x, int y);
    int m_x;
    int m_y;
};

CMyClass::CMyClass(int x, int y) : m_y(y), m_x(m_y)
{
};
```

你可能以为上面的代码将会首先做 m_y=I，然后做 m_x=m_y，最后它们有相同的值。但是编译器先初始化 m_x，然后是 m_y, ，因为它们是按这样的顺序声明的。结果是 m_x 将有一个不可预测的值。有两种方法避免它，一个是总是按照你希望它们被初始化的顺序声明成员，第二个是，如果你决定使用初始化列表，总是按照它们声明的顺序罗列这些成员。这将有助于消除混淆。

# 类的构造函数与函数的默认参数

```c++
/*

    (1)构造函数

        为什么需要构造函数？
        为了方便类对象的初始化操作。因为构造函数被系统自动调用，所以构造函数的目的就是初始化类对象的数据成员。
        在类中有一类特殊的成员函数，函数名字与类名相同，我们在创建类对象的时候，
        自动调用这个特殊的成员函数，这个成员函数就叫构造函数。

    总结：

        1.与类名字相同，并且没有返回值,连void也没有；
        2.不可以手动调用构造函数，否则编译会出错；
        3.构造函数应该声明为public，否则在类的外部不能调用。因为类内的属性（成员）默认是private，所以要显式写上public
        4.构造函数中如果有多个参数，那么我们创建对象的时候需要传递参数。
        5.写了带参数的构造函数，系统就不会添加默认构造函数。

    问题引入：一个类中是否可以存在多个构造函数？

            可以。多个构造函数可以为类对象提供多种初始化方法。但是各个构造函数之间要构成函数重载。

    (2)对象拷贝

        

    (3)函数的默认参数

        在函数的形式参数加上“=值”来显式的制定默认值
        （1）默认值只能放在函数的声明中，不要放在函数的定义中。除非该函数没有函数声明.
        (2)在具有多个参数的函数中制定默认值的时候，默认参数必须出现在参数列表的最右侧，
            一旦一个参数有默认值，它右侧的参数必须都是具有默认值.--默认参数都出现在不默认参数的右侧。
        (3)默认参数遇上函数重载。当三个参数有一个是默认参数遇见只有两个参数的函数，会调用哪一个？

*/
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
using namespace std; 

class Time
{
public:

 int Hour; //小时
 int Minute; //分钟
 int Second; //秒

public:

 //Time(int TemHour, int TemMinute, int TemSecond); //构造函数
 Time(int TemHour, int TemMinute, int TemSecond=30); //构造函数

 Time(); //无参构造函数

}; 

//构造函数的实现
Time:: Time(int TemHour, int TemMinute, int TemSecond)
{

 Hour = TemHour;
 Minute = TemMinute;
 Second = TemSecond;

}

//无参构造函数
Time:: Time() //默认时间是10：10：10
{

 Hour = 10;
 Minute = 10;
 Second = 10;

}

//普通函数带默认构造函数
int functionSum(int a, int b=10)
{

 return a + b;

}

//int functionSum(int a)
//{
// return a; 
//}

int main(void)
{

 //创建类对象并且均调用了构造函数
 Time myTime01(12, 34, 23);
 Time myTime02=Time(12, 13, 45);
 Time myTime03 = Time{ 13, 23, 45 };
 Time myTime04{ 12, 14, 56 };
 Time myTime05 = { 13, 13, 45 };

 

 //Time myTime06(); //vs1015编译通过，但是不是调用无参构造函数，也不是调用有参数构造函数，系统有处理
    //下面都是调用无参构造函数，
 Time myTime07;
    Time myTime06 = Time{};
 Time myTime08 = Time();
 Time myTime09{};
 Time myTime10={};

    //(2)对象拷贝
 Time myTime20 = {12,12,12}; //调用默认无参构造函数
 //如下四个对象并没有调用传统意义上的构造函数，他们调用的实际上是拷贝构造函数
 Time myTime21 = myTime20; //都是调用默认拷贝构造函数
 Time myTime22(myTime20);
 Time myTime23{ myTime20 };
 Time myTime24 = { myTime20 };

 //（3）构造函数带默认参数
 Time myTime25(12, 12);
 //(3)普通函数带默认参数
 int Number01 = functionSum(33,13); //错误--有多个函数实例与函数调用匹配
 int Number02 = functionSum(10);

 

 

 system("pause");
 return 0;

}

```

# 隐式类型转换与explicit

```c++
/*
 *(1)隐式类型转换和explicit
 *  编译系统私下做了很多我们不知道的事情，当参数不够或者类型不匹配时候就会进行隐式类型转换
 *(2)是否可以强制系统明确要求构造函数不做强制类型转换？
 * 可以。如果构造函数中声明explicit(显式的)。则这个构造函数只能用于初始化显式类型转换。不能进行隐式类型转化。
 *(3)
 * 
 */
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
using namespace std;

class Time
{
private:
 int Hour;
 int Minute;
 int Second;
public:
 explicit Time(int Hour, int Minute, int Second);
 Time(int Hour);

};

Time::Time(int Hour, int Minute, int Second)//把形式参数赋值给成员属性
{
 this->Hour = Hour;
 this->Minute = Minute;
 this->Second = Second;
}
Time::Time(int Hour)
{
 this->Hour = Hour;
}

void functionTransform(Time TemTime)
{
 return;
}

int main(void)
{
 Time myTime01 = 12;
 //Time myTime03(12, 23, 43, 54); //错误
 Time myTime04 = { 16 };
 //大括号内部可以当做一个对象· --如果带一个参数的构造函数加explicit就会进行隐式类型转换，都会错误
 Time myTime02=(12, 23, 43, 54); //会调用单参数的构造函数--相当于逗号表达式，最后是Hour=54

 functionTransform(12); //首先会调用一个参数的构造，构造一个Time临时对象

 //explicit防止隐式类型转化
 //Time myTime05 = { 12,12,12 }; //错误，不能隐式类型转换
 Time myTime06(2, 12, 34);//正确，调用构造函数
 Time myTime07=(2, 12, 34);//正确，调用构造函数
 /*
  * 总结：一般可以认为()是参数，{}是一个对象
  * (1)对于单个参数的构造函数，一般声明为explicit,除非有特殊原因。
  */

    Time myTime100 = {16}; // 这个写法还比较正常，可以明确的高数系统调用哪个构造函数（单参数的构造函数）
    Time myTime101 = 16; // 这种含糊不清的写法，就存在临时对象隐式转换，这个16就被系统隐式调用单参数的构造函数转为Time对象，然后通过拷贝构造函数赋值给myTime101
    functionTransform(16); // 也是一样的问题
    // 为了避免这种含糊不清的写法，可以强制系统明确要求构造函数不能进行隐式类型转换，只需要在构造函数声明中带上explicit即可，那么这个构造函数就只能用于初始化和显式类型转换

    // 声明为explicit后：
    Time myTime100 = Time{16};
    Time myTime101 = Time(106);
    functionTransform(Time(16));

 system("pause");
 return 0;

}

```

# 构造函数初始化列表

```c++
/*
 *(1)构造函数初始化列表。
 * 1. 直接在构造函数的实现中，在函数参数列表后面：成员变量（初始化形参变量(初始值)）, 成员变量值2（初始化形参变量2），
 * 这样写的执行时机是执行{}前开始执行，以后会遇到必须使用构造函数成员初始化列表形式的。
 * 2. 建议使用构造函数初始化列表形式。初始化列表形式叫初始化，写在{}中的叫赋值操作(在初始化的时候是个垃圾值)。写在{}
 *  相当于放弃了初始化，由系统给一个垃圾值。
 * 3. 每个成员变量的初始化顺序和系统定义顺序有关，而与构造函数初始化列表的写的顺序无关。
 *
 *(2)为什么需要构造函数初始化列表？
 * 1. 初始化而不是先存入一个垃圾值，然后再赋值。
 * 2. 有些场合只能使用初始化列表的方式。

*/
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
using namespace std; 

class Time
{
private:

 int Hour;
 int Minute;
 int Second;

public:

 explicit Time(int Hour, int Minute, int Second);
 Time(int Hour);

}; 

Time:: Time(int TemHour, int TemMinute, int TemSecond): Hour(TemHour), Minute(TemMinute), Second(TemSecond)//把形式参数赋值给成员属性
{
}
Time:: Time(int Hour)
{

 this->Hour = Hour;

}

int main(void)
{

 Time myTime01 = Time(12, 12, 12);

 system("pause");
 return 0;

}

```



在 C++ 中，初始化列表（Initialization List）是一种在构造函数中初始化成员变量的机制。有些情况下，只能使用初始化列表来初始化类对象。以下是一些常见的场景：

### 1. **常量成员变量（`const` 成员变量）**

常量成员变量必须在构造函数的初始化列表中初始化，因为它们在对象构造完成后不能被修改。

```cpp
class MyClass {
public:
    MyClass(int val) : myConst(val) {}  // 必须在初始化列表中初始化

private:
    const int myConst;
};

int main() {
    MyClass obj(42);
    return 0;
}
```

### 2. **引用成员变量**

引用成员变量必须在构造函数的初始化列表中初始化，因为引用一旦绑定就不能再改变。

```cpp
class MyClass {
public:
    MyClass(int& ref) : myRef(ref) {}  // 必须在初始化列表中初始化

private:
    int& myRef;
};

int main() {
    int value = 42;
    MyClass obj(value);
    return 0;
}
```

### 3. **没有默认构造函数的类成员**

如果类的成员变量是一个没有默认构造函数的类对象，那么必须在构造函数的初始化列表中初始化该成员变量。

```cpp
class NoDefaultCtor {
public:
    NoDefaultCtor(int val) : value(val) {}
private:
    int value;
};

class MyClass {
public:
    MyClass(int val) : noDefaultCtor(val) {}  // 必须在初始化列表中初始化

private:
    NoDefaultCtor noDefaultCtor;
};

int main() {
    MyClass obj(42);
    return 0;
}
```

### 4. **基类的构造函数需要参数**

如果派生类的基类构造函数需要参数，那么必须在派生类的构造函数初始化列表中调用基类的构造函数。

```cpp
class Base {
public:
    Base(int val) : baseValue(val) {}
private:
    int baseValue;
};

class Derived : public Base {
public:
    Derived(int val) : Base(val) {}  // 必须在初始化列表中调用基类构造函数
};

int main() {
    Derived obj(42);
    return 0;
}
```

### 5. **成员变量是 `std::unique_ptr` 或其他需要在构造函数中初始化的智能指针**

虽然 `std::unique_ptr` 有默认构造函数，但如果需要初始化为一个特定的值，通常在初始化列表中进行。

```cpp
#include <memory>

class MyClass {
public:
    MyClass() : ptr(std::make_unique<int>(42)) {}  // 在初始化列表中初始化

private:
    std::unique_ptr<int> ptr;
};

int main() {
    MyClass obj;
    return 0;
}
```

### 总结

在以下情况下，只能使用初始化列表来初始化类对象：

1. **常量成员变量（`const` 成员变量）**：必须在初始化列表中初始化。
2. **引用成员变量**：必须在初始化列表中初始化。
3. **没有默认构造函数的类成员**：必须在初始化列表中初始化。
4. **基类的构造函数需要参数**：必须在初始化列表中调用基类的构造函数。
5. **成员变量是 `std::unique_ptr` 或其他需要在构造函数中初始化的智能指针**：通常在初始化列表中进行。

使用初始化列表可以确保成员变量在对象构造时被正确初始化，避免潜在的未定义行为和错误。
