（1）函数回顾与后置返回类型；
（2）内联函数；
（3）函数杂合用法总结；
（4）const char *、char const*、 char * const 三者的区别；
（5）函数形参中带const；

# 一：函数回顾与后置返回类型

函数定义中，形参如果在函数体内用不到的话，则可以不给形参变量名字，只给其类型。
函数声明时，可以只有形参类型，没有形参名。

- 把函数返回类型放在函数名字之前，这种写法，叫前置返回类型。

- C++11中，后置返回类型。就是在函数声明和定义中，把返回类型参数列表之后。

- 前面放auto，表示函数返回类型放到参数列表之后，而放在参数列表之后的返回类型是通过->开始的。

auto func123(int a, int b) ->void; //后置返回类型的函数声明
func123(12, 13)；

# 二：内联函数

在函数定义前增加了关键字inline，将该函数声明成内联函数。
函数体很小，调用有很频繁的这种函数。咱们引入inline(内联函数)
<font style="color: red">1、inline影响编译器，在编译阶段对inline这种函数进行处理，系统尝试将调用该函数的动作替换为函数本体。通过这种方式，来提升性能。</font>
int abc = func123(5); //int abc = 1;

2、inline只是我们开发者对编译器的一个建议，编译器可以尝试去做，也可以不去做，这取决于编译器的的诊断功能，也就是说，决定权在编译器

3、<font style="color: red">内联函数的定义就要放在头文件</font>。这样需要用到这个内联函数的.cpp文件都能够通过#include 把这个内联函数的源代码#include进来，以便找到这个函数的本体源代码并尝试将该函数的调用替换为函数体内的语句。

优缺点：
代码膨胀的问题；所以内联函数函数体尽量要小。

注意：各种编译器对inline的处理各不相同。inline函数尽量简单，代码尽可能少。循环，分支，递归调用尽量不要出现在inline函数中。否则的话，编译器很可能会因为你写这些代码的原因拒绝让这个函数成为一个inline函数。

constexpr函数 可以看成是更严格的一种内联函数。

```c++
constexpr int func() {

    // 给一个函数添加constexpr的目的就是为了将一个函数用在常量表达式的调用中，所以就要求他的函数体写的足够简洁

}

```

# define宏展开也类似于inline

# 三：函数杂合用法总结

1、函数返回类型为void，表示函数不返回任何类型。但是我们可以调用一个返回类型是void的函数
return func(); //其中func定义为 void func(){}

```c++
void func() {

}

void funcV() {
    return func(); // 你可以返回另一个返回值为空（void)的函数
}
```

2、函数返回指针和返回引用的情况

```c++
int *myfunc() // 返回指针的函数
{

 int tmpvalue = 9;
 return &tmpvalue; //虽然编译通过但存在极大隐患,返回了局部变量的地址

}

int *p = myfunc();  //指针和引用都会有隐患
*p = 6;  //你往一个不属于你的地址写了数字（因为这个函数返回了局部变量的地址，局部变量在离开函数体的时候就已经被系统回收了地址）。

int &myfunc() // 返回引用的函数
{

 int tmpvalue = 9;
 return tmpvalue;

}
int &k = myfunc(); 
cout<<&k<<endl; 
k = 10;  //你往一个不属于你的地址写了数字。
int k = myfunc();  //这种写法是可以。
cout<< &k<< endl; 
k = 10;  //安全

```

3、没有形参可以保持形参列表为空（），或者int myfunc(void)

4、如果一个函数我们如果不调用的话，则该函数可以只有声明部分，没有定义部分。

5、普通函数，定义只能定义一次（定义放在.cpp文件中），声明可以声 明多次。一般函数定义.cpp文件会#include自己的函数声明文件(.h)

6、void func(int &ta，int &tb)，在C++中，更习惯用引用类型的形参来取代指针类型的形参。提倡在C++中，多使用引用类型的形参。

```c++
student abc; 
abc.num = 100; 
fs(abc); 
cout<<abc.num<<endl;  //1010

```

7、C++中，函数允许同名，但是形参列表的参数类型或者数量应该有明显的区别。

# 四：const char *、char const*、char * const 三者的区别

```c++
const int abc = 12; 
char *p：const混用；
// 4.1：const char *p; 
char str[] = "I Love China!"; 
//const char *p;  P指向的东西不能通过p来修改（p所指向的目标，那个目标中的内容不能通过p来改变）；
char const *p; 
p = str; 
//*p = 'Y';  //语法错；
p++; 
p++; 
str[0] = 'Y';  //这是OK的
//char const *等价于const char *p; 

char str[] = "I love China!"; 
char * const p = str;  //定义的时候必须初始化

 //p一旦指向了一个东西之后，就不可以再指向其他东西了。

//p++ //不可以，p一旦指向了一个东西之后，就不可以再指向其他东西了。
*p = 'Y';  //但可以修改p指向的目标中的内容。

//4.2
const char * const p = str; 
char const * const p = str;  //p的指向也不能变。p指向的内容也不能通过p来改变。

int i = 100; // 可以通过i修改
const int &a = i;  //代表a的内容不能通过a自己来修改。
i = 100; 
a= 500;  //不合法

int &b = 31;  //错误的（因为&引用类型是左值，不可以使用右值进行赋值）
const int &b = 31;  //分配了内存的；
b=45;  //不可以，错误
cout<<b<<endl; 

```

# 五：函数形参中带const

```c++
struct student {
    int num;
}

// void fs(student &temp) { // 如果一个函数不想修改参数的值的话，引用类型就改为常量引用
void fs(const student &temp) { 
    temp.num = 1010;
}

int main() {
    student abc; 
    abc.num = 100; 
    fs(abc); 
    cout<<abc.num<<endl;  //1010
}

```

把形参写成const的形式有很多好处
1、可以防止你无意中修改了形参值导致实参值被无意修改。
2、实参类型可以更灵活。

```c++
student abc; 
abc.num = 100; 
fs(abc); 
cout<<abc.num<<endl;  //1010
const student &def = abc; 
fs(def); 

fs(13) //需要 void func2(const int &a)才可以或者用move

```

```c++
struct student {int num; } //定义一个结构
void fs(student &stu)
void fs(const student &stu) //学习这种在形参中增加const的写法
{

 stu.num = 1010;

}

void fs2(const int i)
{

}
```
