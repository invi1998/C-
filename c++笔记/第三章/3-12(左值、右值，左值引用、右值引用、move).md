在C++中，左值（lvalue）和右值（rvalue）是表达式分类的重要概念，它们在语言的许多方面都有重要的作用，包括函数参数传递、返回值处理、移动语义等。

### 左值 (lvalue)

- **定义**：左值是指具有持久存储的值，可以取地址，并且可以在赋值操作的左侧出现。
- **特点**：
  - 可以取地址（使用 `&` 运算符）。
  - 通常表示一个对象，这个对象有确定的存储位置。
  - 可以作为赋值操作的目标。

### 右值 (rvalue)

- **定义**：右值是指临时的、即将销毁的值，不能取地址，通常出现在赋值操作的右侧。
- **特点**：
  - 不能取地址。
  - 表示一个临时对象或字面量。
  - 通常用于初始化或赋值操作的源。

### 左值引用和右值引用

- **左值引用**：用 `&` 表示，只能绑定到左值。
- **右值引用**：用 `&&` 表示，只能绑定到右值。

### 示例

```cpp
#include <iostream>

int main() {
    int x = 10;  // x 是左值
    int y = 20;  // y 是左值

    // 左值引用
    int &ref_x = x;  // ref_x 是 x 的左值引用
    ref_x = y;       // 等价于 x = y

    // 右值引用
    int &&rref_y = 30;  // rref_y 是 30 的右值引用
    rref_y = 40;        // 修改 rref_y，即修改临时对象 30

    std::cout << "x: " << x << std::endl;  // 输出 20
    std::cout << "y: " << y << std::endl;  // 输出 20
    std::cout << "rref_y: " << rref_y << std::endl;  // 输出 40

    return 0;
}
```

### 左值应用和右值应用

- **左值应用**：通常涉及对持久对象的操作，如赋值、取地址等。
- **右值应用**：通常涉及对临时对象的操作，如移动语义、资源转移等。

### 移动语义

右值引用的一个重要应用是实现移动语义，这可以提高性能，特别是在处理大型对象（如 `std::vector`）时。

```cpp
#include <iostream>
#include <vector>

class MyClass {
public:
    std::vector<int> data;

    // 拷贝构造函数
    MyClass(const MyClass &other) : data(other.data) {
        std::cout << "Copy constructor called" << std::endl;
    }

    // 移动构造函数
    MyClass(MyClass &&other) noexcept : data(std::move(other.data)) {
        std::cout << "Move constructor called" << std::endl;
    }
};

MyClass createObject() {
    MyClass obj;
    obj.data.push_back(1);
    obj.data.push_back(2);
    return obj;  // 返回临时对象，触发移动构造函数
}

int main() {
    MyClass obj1 = createObject();
    return 0;
}
```

在这个例子中，`createObject` 函数返回一个临时对象，这个临时对象会被移动到 `obj1` 中，而不是拷贝。这通过移动构造函数实现，提高了效率。

### 总结

- **左值**：持久对象，可以取地址，可以作为赋值目标。
- **右值**：临时对象，不能取地址，通常用于初始化或赋值源。
- **左值引用**：绑定到左值。
- **右值引用**：绑定到右值，用于实现移动语义。

---





左值、右值，左值引用、右值引用、move

（1）左值和右值

#### 概念

    一般说法，编译器为其单独分配了一块存储空间，可以取出其地址的，左值可以放在运算符左边，
    
    右值指的是其数据本身，不能得到其自身的地址，右值只能放在赋值运算符右边

具体分析：

    左值最常见的情况如函数和数据成员的名字；
    
    右值是没有标识符，不可以取地址的表达式，一般也称之为“临时对象”
    
    比如： a = b + c ;
        &a 是允许的操作，而&(b+c)不能通过编译，因此，a是一个左值，而(b+c)是一个右值

（2）引用分类
（3）左值引用
（4）右值引用
（4.1）右值引用的引入目的
（5）std::move函数
（6）左值右值总结说明

左值和右值问题

```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include <vector>

using namespace std; 

int main(void)
{

 int number01 = 10; //每个对象占有一块内存空间

 number01 = 22;
 //左值当右值使用，
 number01 = number01 + 1;//number01本质上是左值。虽然它出现在等号右侧。
 /*
 //number01用在等号右侧的时候，我们说number01具有右值属性，不是右值。
 *number01出现在左边，用的是内存中的地址，nubmer01具有左值属性
 *用在右侧，用的是nubmer01的值。
 *总结：一个左值，可以同时具有左值属性和右值属性。
  */

    //  用到左值的运算符有哪些？
 //1.赋值运算符结果是左值
 printf("%d", number01 = 4);
 //整个赋值语句的结果是个左值，也就是(number01 = 4)这个整体是一个地址，可以赋值
 (number01 = 4) = 23; // 结果就是 number01值为23

 //2、取地址符 &
 &number01;
 cout << &number01 << endl;
    // &23; // 报错，编译不通过（取地址符需要一个左值）

 //3、string vector 等下标运算符[]都需要左值
 string str = "jisuanji";
 str[1];
 //迭代器也是左值（同时也就说明，迭代器的++ --运算符需要的是一个左值）
 vector<int>::iterator iter;
 iter++;
 
 system("pause");
 return 0;

    // 4、通过看一个运算符在一个字面值上能不能操作，我们也可以判断运算符是否用到的是左值

    // 左值表达式 也可以叫 左值，右值表达式 就是 右值
    // 

}

/*
*(1)左值：能在赋值语句等号左侧的东西，它能够代表一段内存空间
* 右值：不能作为左值的值就是右值，右值不能出现在赋值语句中等号的左侧。
* 结论：c++中的一条表达式，要么是左值，要么是右值，不可能两者都不是。、
* 但是有时候左值也可以当做右值使用。
*
*用到左值的运算符：
* 1.=赋值运算符 左侧是左值、
* 2.&取地址运算符
* 3.cstring vector 下标[]都是左值
* 4. 迭代器的递增，递减运算符等
* 一般在字面值上不能操作，就需要用到左值。
* 在资料上叫左值表达式就是左值，，右值表达式就是右值
* 左值：代表一个地址，所以左值表达式求值结果，就是一个对象，就需要有地址。变量名为内存空间的别名。
*  求值结果为对象的表达式，不一定是左值，需要具体分析。
*
*(2)
*
*(3)
*
*
*/

```

左值_右值_左值引用_右值引用

```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>

using namespace std;

int main(void)
{
 //左值引用（绑定到左值）
 int number01 = 10;
 int &reference_number01 = number01; // 这里的 & 不是代表取地址的意思，而是作为一个标识，标识这是一个引用
 reference_number01 = 20;

 //2const 引用（常量引用）也是左值引用，我们不希望该变值的对象
 const int &reference_number02 = number01;
    // reference_number02 = 1222; //报错，必须是一个可修改的左值引用

 //右值引用-绑定到右值  （怎么理解右值引用，首先，他是一个引用，他一般侧重于那种临时的变量）
 int && reference_number03 = 3;//使用完3之后不需要保留，注意：右值引用绑定到一个常量上
 reference_number03 = 12;

 //左值引用   
    //指针可以指向一个空，可以不初始化，但是引用必须初始化
 //没有空引用，左值引用初始化的时候就绑定到左值
 int number04 = 200;
 int&Reference_number05 = number04;
 //int &number06 = 4;//error 左值引用不能绑定到右值，必须绑定到左值
 const int &tem = 3;//const引用可以绑定到右值。const引用比较特殊。系统内部生成了一个临时变量
    // 相当于 int tempvalue = 3; const int &tem = tempvalue;
 /*
  * temValue=3;
  * int&tem=temValue;
  */

 //右值引用--必须是绑定到右值的引用。使用&&、--绑定临时对象上。一般希望使用右值引用绑定一些即将销毁的或者临时的对象
 int &&reference_number07 = 3; //右值引用可以理解成一个对象的名字。即对reference_number07进行取地址操作，是可以拿到他的地址的 &reference_number07

 //其他范例
 string str{ "jisuanjizuchengyaunli" };
 string &reference_str = str; // 或者 string &reference_str{str}; 不同的写法，一样的意思，左值引用绑定到左值
 //string &reference_str02{ "jisuanizuchengyaunli" };//error不可以，左值引用不能绑定到临时对象
 //临时变量被系统当做右值
 //
 const string &reference_str03 = "jisuanjizuchengyuanli";//ok
 //const不但可以绑定到右值，而且可以执行隐式类型转换（比如这里是string的隐式类型装换），经临时对象绑定到string左值。
    // 创建一个临时变量，绑定到左值reference_str03上
 
 //string &&reference_str04{ str };//error,右值引用不能绑定到左值
 string &&reference_str05{ "jisuanjizjiifisidf" };//可以绑定到临时对象

 int number06 = 100;
 int&reference_number08 = number06;
 //int&&reference_number09 = number06;//error

 //右值结果绑定到右值
 int &&reference_nubmer10 = number06 * 100;//ok，右值引用绑定到右值
    // int &reference_number11 = number06 * 100; // error ,左值引用不能绑定到右值
    const int &reference_number12 = number06 * 100; // 可以绑定右值

 //为什么前置运算符是一个左值表达式??
 /*++i;
  *系统直接在内部给变量i加1，然后返回i本身
  * 因为i是变量，所以可以被赋值，赋值后还是一个对象
  *
  * i++为什么是一个右值表达式？？？
  * 先用后加，
  * 先产生一个临时变量，记录i的值用于使用，再给i加1，接着返回这个临时变量（临时变量是右值）
  */

 int number11 = 300;
 (++number11) = 199;//number11=199
 //(number11++) = 2000;//error, 右值必须放在赋值语句的右边（很好理解，你不能 写出这样一个语句 100 = 200；）
 int&&reference13 = number11++;//成功绑定右值，但是reference11绑定的不是number11,绑定的是返回的临时量，比如这里，reference13的值就是199 + 1 = 200；
    number11 += 5;
    // reference13的值还是199 + 1 = 200；

    // 同理，绑定左值的情况
    int &reference14 = ++number11; // 此时，reference14就绑定到number11上去了，也就是reference14就编程number11的别名，
    number11 += 5; // reference14的值和number11的值都为210

    // 重点强调：
    //（1）、reference13虽然是右值引用（绑定到了右值），但是他reference13本身还是一个左值，（我们需要把reference13看成一个变量），因为他在 = 左边待着。
    int &reference15 = reference13; // 可以，左值引用可以绑定到左值上
    // int &&reference17 = reference13; // error,右值引用无法绑定到左值上
    // （2）、所有变量，看成左值。因为他们都是有地址的
    // （3）、任何函数里的形参都是左值。void f(int i, int &&w) {} 如这里的w是右值引用，但是w本身是左值
    // （4）、临时对象都是右值

 //std::move()函数--左值转换为右值
 int number14 = 14;
 int&&reference_number15 = std::move(number14);//ok
 reference_number15 = 15;
 cout << reference_number15 << endl; //15,可以绑定

 int &&reference_nubmer16 = 100;
 int&&reference_number17 = std::move(reference_nubmer16); //ok 将一个左值引用转化为右值

 string str18 = "jisuanjizuchegnaun";//没有移动，触发了string类的移动构造函数。移动后把str18清空了。reference19是重新开辟的内存。
 //string reference19 = std::move(str18);
 
 //把一段右值绑定到右值上
 string &&str20 = std::move(str18);
 
 system("pause");
 return 0;
}

/*
*(1)引用分类
* 1.左值引用-绑定到左值
* 2.常量引用-不改变原来的值
* 3.右值引用  使用&& c++11标准
*(2)左值引用是什么？
* 引用左右，绑定到左值上
*
*(3)右值引用，必须绑定到右值上
*为什么引入右值引用？
* 希望右值引用来绑定一些即将销毁的或者一些临时对象上。
*总结：能绑定到左值上的对象一般不能绑定到右值上。
* 返回左值引用的函数连同赋值，下标，解引用，和前置递增递减运算符（--i, ++i）都是返回表达式的例子。
* 我们可以将一个左值引用绑定到这列表达式的结果上。
*
* 返回非引用类型的函数，连同算术，关系，以及后置递增递减运算符（i--）,不能将一个左值引用绑定到这类
* 表达式上，但是可以将一个const或者右值引用绑定到这列表达式上。
*
*1.所有的变量，看成左值，因为他们是由地址的
*2.任何函数的形参都是左值。如void function(int&a);a虽然是引用，但是是左值
*3.临时对象是右值。
*
*
*(4)右值引入目的
* 1.右值引入是c++11的新概念，可以认为&&是一种新的数据类型，引入这种新的数据类型目的见2.
* 2.引入&&是为了提高程序运行效率，把拷贝对象变成移动对象来提高运行效率。拷贝时候需要分配内存，
*  然后再一个一个项拷贝，移动直接把内存块指针转给B，而不需要分配内存。具体见移动构造函数章节。
* 3.移动对象是如何发生的？怎么触发移动对象？
*  当用一个对象给另一个对象赋值，移动移动构造函数，移动赋值运算符（需要的参数是右值引用）
*  拷贝构造函数，赋值运算符需要的参数是左值引用
*
*(5)std::move()函数
* std::move()只有一个目的：把一个左值，强制转换为一个右值。--结果：一个右值可以绑定上去了。
*
*/
```

I++与++i的实现

```cpp
// 前缀形式：++i
int& int::operator++() //这里返回的是一个引用形式，就是说函数返回值也可以作为一个左值使用
{//函数本身无参，意味着是在自身空间内增加1的
  *this += 1;  // 增加
  return *this;  // 取回值
}


//后缀形式: i++
const int int::operator++(int) //函数返回值是一个非左值型的，与前缀形式的差别所在。
{//函数带参，说明有另外的空间开辟
  int oldValue = *this;  // 取回值
  ++(*this);  // 增加
  return oldValue;  // 返回被取回的值
}

//前置-- (--i)
	int& operator -- ()
	{
		(*this)--;
		return *this;
	}

	//后置-- (i--)
	int operator -- (int)
	{
		int tmp = *this;
		(*this)--;
		return tmp;
	}
```
