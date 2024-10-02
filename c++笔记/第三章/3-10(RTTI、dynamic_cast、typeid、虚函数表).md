

C++ 的 RTTI（Run-Time Type Information，运行时类型信息）是一种机制，允许程序在运行时查询对象的类型信息。RTTI 主要通过以下几种方式实现：

### 1. **`typeid` 运算符**

`typeid` 运算符用于获取对象或类型的类型信息。它返回一个 `type_info` 对象，该对象包含了类型的各种信息，如类型名称等。

#### 示例

```cpp
#include <iostream>
#include <typeinfo>

class Base {};
class Derived : public Base {};

int main() {
    Base base;
    Derived derived;

    std::cout << "Type of base: " << typeid(base).name() << std::endl;
    std::cout << "Type of derived: " << typeid(derived).name() << std::endl;

    Base* ptr = &derived;
    std::cout << "Type of ptr: " << typeid(ptr).name() << std::endl;
    std::cout << "Type of *ptr: " << typeid(*ptr).name() << std::endl;

    return 0;
}
```

### 2. **`dynamic_cast`**

`dynamic_cast` 用于在类层次结构中进行类型转换，特别是多态对象。它在运行时进行类型检查，如果转换失败，返回 `nullptr`（对于指针）或抛出异常（对于引用）。

#### 示例

```cpp
#include <iostream>

class Base {
public:
    virtual ~Base() {}
    virtual void print() const { std::cout << "Base" << std::endl; }
};

class Derived : public Base {
public:
    void print() const override { std::cout << "Derived" << std::endl; }
};

int main() {
    Base* basePtr = new Base();
    Derived* derivedPtr = dynamic_cast<Derived*>(basePtr);

    if (derivedPtr) {
        derivedPtr->print();  // 不会执行，因为 basePtr 不是 Derived 类型
    } else {
        std::cout << "Conversion failed" << std::endl;  // 输出 "Conversion failed"
    }

    Base* basePtr2 = new Derived();
    Derived* derivedPtr2 = dynamic_cast<Derived*>(basePtr2);
    derivedPtr2->print();  // 输出 "Derived"

    delete basePtr;
    delete basePtr2;
    return 0;
}
```

### 3. **`type_info` 类**

`type_info` 类提供了类型信息的接口，可以通过 `typeid` 运算符获取 `type_info` 对象。

#### 主要成员函数

- `const char* name() const;`：返回类型的名称。
- `bool before(const type_info& rhs) const;`：比较两个类型的顺序。
- `bool operator==(const type_info& rhs) const;`：检查两个类型是否相同。
- `bool operator!=(const type_info& rhs) const;`：检查两个类型是否不同。

#### 示例

```cpp
#include <iostream>
#include <typeinfo>

class Base {};
class Derived : public Base {};

int main() {
    Base base;
    Derived derived;

    const std::type_info& ti1 = typeid(base);
    const std::type_info& ti2 = typeid(derived);

    std::cout << "Type of base: " << ti1.name() << std::endl;
    std::cout << "Type of derived: " << ti2.name() << std::endl;

    if (ti1 == ti2) {
        std::cout << "Types are the same" << std::endl;
    } else {
        std::cout << "Types are different" << std::endl;  // 输出 "Types are different"
    }

    return 0;
}
```

### 4. **`type_index` 类**

`type_index` 类是 C++11 引入的，它提供了一个可以用于容器的 `type_info` 包装器。`type_index` 对象可以比较、排序和用作容器的键。

#### 主要成员函数

- `const std::type_info& type() const;`：返回 `type_info` 对象。
- `bool operator==(const type_index& rhs) const;`：检查两个类型是否相同。
- `bool operator<(const type_index& rhs) const;`：比较两个类型的顺序。

#### 示例

```cpp
#include <iostream>
#include <typeindex>
#include <map>

class Base {};
class Derived : public Base {};

int main() {
    Base base;
    Derived derived;

    std::type_index ti1(typeid(base));
    std::type_index ti2(typeid(derived));

    std::cout << "Type of base: " << ti1.name() << std::endl;
    std::cout << "Type of derived: " << ti2.name() << std::endl;

    if (ti1 == ti2) {
        std::cout << "Types are the same" << std::endl;
    } else {
        std::cout << "Types are different" << std::endl;  // 输出 "Types are different"
    }

    std::map<std::type_index, std::string> typeMap;
    typeMap[ti1] = "Base";
    typeMap[ti2] = "Derived";

    for (const auto& pair : typeMap) {
        std::cout << "Type: " << pair.first.name() << ", Value: " << pair.second << std::endl;
    }

    return 0;
}
```

### 5. **性能考虑**

RTTI 机制在编译时和运行时都会引入一些开销。特别是 `dynamic_cast` 在运行时进行类型检查，可能会比其他类型转换操作符（如 `static_cast`）慢。因此，在性能敏感的应用中，应谨慎使用 RTTI。

### 总结

C++ 的 RTTI 提供了在运行时查询对象类型信息的能力，主要通过 `typeid` 运算符、`dynamic_cast`、`type_info` 类和 `type_index` 类实现。这些机制在多态编程、类型安全的类型转换和类型信息的管理中非常有用。理解 RTTI 的工作原理和使用方法，可以帮助编写更安全和灵活的代码。

---



# RTTI概念

RTTI(Run Time Type Identification)即通过运行时类型识别，程序能够使用基类的指针或引用来检查着这些指针或引用所指的对象的实际派生类型。

## RTTI机制的产生

为什么会出现RTTI这一机制，这和C++语言本身有关系。和很多其他语言一样，C++是一种静态类型语言。其数据类型是在编译期就确定的，不能在运行时更改。然而由于面向对象程序设计中多态性的要求，C++中的指针或引用(Reference)本身的类型，可能与它实际代表(指向或引用)的类型并不一致。有时我们需要将一个多态指针转换为其实际指向对象的类型，就需要知道运行时的类型信息，这就产生了运行时类型识别的要求。和Java相比，C++要想获得运行时类型信息，只能通过RTTI机制，并且C++最终生成的代码是直接与机器相关的。

## typeid和dynamic_cast操作符

RTTI提供了两个非常有用的操作符：typeid和dynamic_cast。

typeid操作符，返回指针和引用所指的实际类型；

dynamic_cast操作符，将基类类型的指针或引用安全地转换为其派生类类型的指针或引用。

我们知道C++的多态性（运行时）是由虚函数实现的，对于多态性的对象，无法在程序编译阶段确定对象的类型。当类中含有虚函数时，其基类的指针就可以指向任何派生类的对象，这时就有可能不知道基类指针到底指向的是哪个对象的情况，类型的确定要在运行时利用运行时类型标识做出。为了获得一个对象的类型可以使用typeid函数，该函数反回一个对type_info类对象的引用，要使用typeid必须使用头文件<typeinfo>，因为typeid是一个返回类型为typ_info的引用的函数。因为type_info类的复制构造函数和赋值运算符都是私有的，所以不允许用户自已创建type_info的类。唯一要使用type_info类的方法就是使用typeid函数。
type_info 类的一些方法：
1、 .name  返回一个c风格的字符串
Human *phuman = new Man;
const type_info &tp = typeid(*phuman);
cout << tp.name() << endl;
2、 == 、 !=
Human *phuman2 = new Man;
const type_info &tp2 = typeid(*phuman2);
if (tp == tp2) {

    cout << "tp和tp2类型相同" << endl;

}
Human *phuman3 = new Woman;
const type_info & tp3 = typeid(*phuman3);
if (tp2 != tp3) {

    cout << "tp2和tp3类型不相同" << endl;

}

## typeid函数

typeid函数的主要作用就是让用户知道当前的变量是什么类型的，比如使用typeid(a).name()就能知道变量a是什么类型的。typeid()函数的返回类型为typeinfo类型的引用。

typeid函数是type_info类的一个引用对象，可以访问type_info类的成员。但因为不能创建type_info类的对象，而typeid又必须反回一个类型为type_info类型的对象的引用，所以怎样在typeid函数中创建一个type_info类的对象以便让函数反回type_info类对象的引用就成了问题。这可能是把typid函数声明为了type_info类的友元函数来实现的，默认构造函数并不能阻止该类的友元函数创建该类的对象。所以typeid函数如果是友元的话就可以访问type_info类的私有成员，从而可以创建type_info类的对象，从而可以创建返回类型为type_info类的引用。

## dynamic_cast强制转换运算符

该转换符用于将一个指向派生类的基类指针或引用转换为派生类的指针或引用，注意dynamic_cast转换符只能用于含有虚函数的类，其表达式为dynamic_cast<类型>(表达式)，其中的类型是指把表达式要转换成的目标类型，比如含有虚函数的基类B和从基类B派生出的派生类D，则B *pb; D*pd, md; pb=&md; pd=dynamic<D*>(pb); 最后一条语句表示把指向派生类D的基类指针pb转换为派生类D的指针，然后将这个指针赋给派生类D的指针pd，有人可能会觉得这样做没有意义，既然指针pd要指向派生类为什么不pd=&md; 这样做更直接呢？

因为有些时候我们需要强制转换，比如如果指向派生类的基类指针B想访问派生类D中的除虚函数之外的成员时就需要把该指针转换为指向派生类D的指针，以达到访问派生类D中特有的成员的目的，比如派生类D中含有特有的成员函数g()，这时可以这样来访问该成员dynamic_cast<D*>(pb)->g(); 因为dynamic_cast转换后的结果是一个指向派生类的指针，所以可以这样访问派生类中特有的成员。但是该语句不影响原来的指针的类型，即基类指针pb仍然是指向基类B的。

dynamic_cast转换符只能用于指针或者引用。dynamic_cast转换符只能用于含有虚函数的类。dynamic_cast转换操作符在执行类型转换时首先将检查能否成功转换，如果能成功转换则转换之，如果转换失败，如果是指针则反回一个0值，如果是转换的是引用，则抛出一个bad_cast异常，所以在使用dynamic_cast转换之间应使用if语句对其转换成功与否进行测试，比如pd=dynamic_cast<D*>(pb); if(pd){…}else{…}，或者这样测试if(dynamic_cast<D*>(pb)){…}else{…}。

## RTTI与虚函数表

C++中如果含所有虚函数，编译器就会对该类产生一个虚函数表。
 虚函数里面有很多项，每一项都是一个指针，每个指针指向这个类里面的虚函数的入口地址（一个类有多个虚函数）

 虚函数表项里面，第一个表项很特殊，它指向的不是虚函数的入口地址，它指向的实际上是这个类的type_info()对象。

  每个类对象隐藏一个指向类的虚函数表的指针。

  虚函数表第一列是type_info()类型，用于说明实际的类对象类型。调用子类或者父类的函数（虚函数）

# RTTI和dynamic_cast类型转换

```c++
#include<iostream>
#include<cstdlib>
#include<string>

using namespace std; 

class  Human
{
public:

 int age;
 string name;

public:

 //构造函数
 Human();
 Human(int age, string name);
 //析构函数
 ~Human()
 {
  cout << "Human析构函数" << endl;
 }

 virtual void eat()
 {
  cout << "Human eat()函数" << endl;
 }

}; 

class Man :public Human
{
public:

 Man();
 ~Man()
 {
  cout << "man的析构函数执行" << endl;
 }
 virtual void eat()
 {
  cout << "Man eat()函数" << endl;
 }

public:

}; 

class Woman :public Human
{
public:

 Woman();

public:

 ~Woman()
 {
  cout << "Woman析构函数执行" << endl;
 }

 virtual void eat()
 {
  cout << "woman eat()函数" << endl;
 }

}; 

Woman:: Woman()
{

 cout << "这是woman的构造函数" << endl;

}

Human:: Human()
{

 cout << "Human默认构造函数" << endl;

}

Human:: Human(int age, string name)
{

 this->age = age;
 this->name = name;

}

Man:: Man()
{

 cout << "Man类的默认构造函数" << endl;

}

int main(void)
{

 //(1)运行时类型识别
 Human *p_human = new Man;
 Human &q = *p_human;//*p_human表示指针所指向的对象。
 q.age = 12;
 q.name = "jisuanji";
 q.eat();

 //怎么将父类指针转换为子类对象
 Man *m1 = (Man*)(p_human);//c语言强制类型转换，可以成功--但是要求明确知道是什么类型才安全，否则不安全
 m1->eat();//调用Man中的eat()函数
 Woman *w1 = (Woman*)p_human; //转换为其他类型也成功，不安全
 
 //dynamic_cast可以帮助我们做安全检查
 Man*m2 = dynamic_cast<Man*>(p_human); //强父类指针转换为子类类型指针
    if (m2 != nullptr) {
     m2->eat();//执行man类的eat()函数
    } else {
        cout << "p_human不是一个man类型" << endl;
    }
 //对于引用，因为它不像指针一样，转换失败返回一个空指针，引用没有空引用这种说法，但是如果用dynamic_cast转换失败，系统会抛出一个std::bad_cast异常，可以用此来进行判断
 Human &m_reference = *p_human; // 1、先将human类型的指针指向的对象用一个human类型的变量进行引用
 try
 {
  Man m3 = dynamic_cast<Man &>(m_reference); //成功（2、再将human引用转为man类型的引用），转换成功，流程代码继续往下走，如果失败，则进入catch里面
  //(2)typeid运算符
  cout << typeid(m_reference).name() << endl; //class Man
  cout << typeid(m3).name() << endl;  //class Man
  
 }

 catch (std::bad_cast)
 {
  cout << "std::bad_cast" << endl;
 }

 Human h1 = Human(12, "jisuanjiii");
 cout << typeid(h1).name() << endl; //class Human
 int a = 10;
 cout << typeid(a).name() << endl; //int
 cout << typeid("jisuanjizuchengyuanli").name() << endl; //char const [22]

 //1.两个指针定义的类型相同
 Human*p_human02 = new Woman;
 if (typeid(p_human)==typeid(p_human02))
 {
  cout << "p_human和p_human02是同一种指针类型" << endl; //成立，和我们预料的不同
  /*比较的时候是比较的定义的类型，而不是new后面出的类型，这也可以验证前面的RTTI类型识别
   */ 
 }

 //我们如果想比较new出来的对象的实际类型进行比较，该怎么做？
 Woman*p_woman = new Woman;
 Human*p_human03 = p_woman; //
 /*
  * p_woman对象里面有一个我们看不见的指针，指向的是这个对象所在类的Woman的虚函数表。
  * 虚函数表的第一列是type_info()类对象，确定P-woman指向哪一个对象，去调用对应的虚函数。
  */
 if (typeid(*p_human)==typeid(*p_human03))//不成立，实际指向的对象类型比较
 {
  cout << "p_human和p_human03对象类型相同" << endl;

 }
 /* 基类中必须有虚函数，否则上边条件不成立
  * 总结：比较的时候要使用typeid(*指针)进行实际对象的比较，而不要进行定义指针类型的比较。
  * 切记：只有当基类中有虚函数的时候，编译器才会对typeid()的表达式求值，否则某个类型不含有虚函数
  * 则typeid()反悔的是静态类型（也就是定义时候的类型）,既然是定义的类型，编译器就不需要对表达式求只也知道表达式的类型。
  * 
  */

 //type_info类型
 int aa = 11;
 const type_info &aa_reference = typeid(aa);//返回值是type_info静态类型
 cout << aa_reference.name() << endl;
 
 
 system("pause");
 return 0;

}

/*
 *(1)Rtti runtime type identification 运行时类型识别
 *通过运行时类型识别，程序能够使用基类的指针或者引用来所指对象的实际派生类型。
 *RTTI我们看成一种系统提供给我们的能力，或者一种功能，这种功能是通过两个运算符来实现：
 * 1.dynamic_cast运算符：能够将基类的指针或者引用安全的转化为派生类的指针或者引用。
 * 2.typeid运算符，返回指针或者引用所指对象的实际类型。
 * 注意：要想两个运算符正常工作，在基类中必须有一个虚函数。不然的话，两个运算符工作的结果就和预测的不同。
 *  只有虚函数存在，这两个运算符才会使用指针或者引用所绑定的动态类型。
 *  否则，编译的时候就确定了调用哪个类型。
 *(2)typeid(类型(指针，引用))运算符
 * 也可以是typeid(表达式)
 * 拿到对象信息，typeid就会返回一个常量对象的引用，这个常量对象是一个标准库类型，type_info(类、类类型)
 * 主要方法：
 *  1.  typeid().name()返回常量对象的名字
 *  2.
 * 主要用于：用于比较两个指针是否指向同意类型的对象
 *  1. 两个指针定义的类型相同
 * 
 *(3)RTTI与虚函数表
 *C++中如果含所有虚函数，编译器就会对该类产生一个虚函数表。
 *虚函数里面有很多项，每一项都是一个指针，每个指针指向这个类里面的虚函数的入口地址（一个类有多个虚函数）
 *虚函数表项里面，第一个表项很特殊，它指向的不是虚函数的入口地址，它指向的实际上是这个类的type_info()对象。
 *
 *
 *
 * 每个类对象隐藏一个指向类的虚函数表的指针。
 * 虚函数表第一列是type_info()类型，用于说明实际的类对象类型。调用子类或者父类的函数（虚函数）
 */
```
