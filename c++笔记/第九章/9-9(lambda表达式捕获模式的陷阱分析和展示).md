# lambda常见陷阱_捕获列表中的引用_局部变量超出作用域

```c++
#include<iostream>
#include<iostream>
#include<cstdlib>
#include<functional>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

//全局变量，每个元素都是function，每个元素都是参数是int，返回值是bool类型
std::vector<std::function<bool(int)>> gv;

void myFunction()
{
 srand((unsigned)time(nullptr));
 int temValue = rand() % 6;//产生一个0-5之间的种子
 gv.push_back(
  [&](int tv)//这里改为按值捕获可以解决=
 {
  if (tv%temValue == 0)//如果tv是temValue的倍数
  {
   return true;

  }
  return false;
 }
 );//把lambda表达式插入vector容器中
}

int main(void)
{
 myFunction();
 cout << gv[0](10) << endl;
 /*
  * gv[0](10)调用的时候，temValue是个局部变量已经超出作用域了，这里调用有问题
  * temValue的内存空间已经释放了。解决办法，改为按值捕获。
  */
 
 system("pause");
 return 0;
}
/*
* (1)lambda常见陷阱_捕获列表中的&_局部变量超出作用域
* 捕获外部区域中的所有变量，并作为引用在lambda表达式中使用。
* 按照引用这种捕获方式，会导致lambda包含绑定到局部变量中的引用。
* (2)
* (3)
* (4)
* (5)
* (6)(7)
*/

```

# lambda常见陷阱_类中成员变量捕获问题_实际上是捕获的this指针

```c++
#include<iostream>
#include<iostream>
#include<cstdlib>
#include<functional>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

//全局变量，每个元素都是function，每个元素都是参数是int，返回值是bool类型
std::vector<std::function<bool(int)>> gv02;



class AT
{
public:
 int m_temvalue = 7;
 void addItem()
 {
  auto temvalue02 = this->m_temvalue;//成员变量转局部变量，lambda表达式捕获
  gv02.push_back(// c++4中允许在lambda表达式形参列表中使用auto
   //[=](auto tv)//按值捕获，解决引用捕获超出范围的问题（引用悬空）=按值，相当于有this
   //等价于
   [this](auto tv)
  {
   cout << "m_temvalue=" << m_temvalue << endl;//7
   if (tv%m_temvalue == 0)//如果tv是temValue的倍数
   {
    return true;

   }
   return false;
  }
  );
  /*
   * 我们会认为=是按值捕获，使我们能够访问成员变量m_temvalue,lambda表达式中使用的是m_temvalue的副本。
   *但是如果我们在调用lambda之前删除对象的指针，//46行，结果是：
   *m_temvalue=-572662307
   *0
   *也就是lambda执行的正确与否却决于pat对象是否存在，这个lambda表达式在对象存在的时候才能正确执行。
   */
  
 }
};

int main(void)
{

 AT *pat = new AT();
 pat->addItem();
 //delete pat;
 cout << gv02[0](10) << endl;//0
 delete pat;
 
 system("pause");
 return 0;
}
/*
* (1)lambda常见陷阱_形参列表可以使用auto
* c++4中允许在lambda表达式形参列表中使用auto。//18行
* (2)lambda常见陷阱_类中成员变量捕获问题_实际上是捕获的this指针
* 明确一点：捕获这个概念，只针对在创建lambda表达式的作用域内可见的，非静态 局部变量（包括形参）
* m_temvalue不是非静态局部变量，m——temvalue是AT类的成员变量，成员变量不能被捕获到的。
* 那么为什么可以在lambda中使用m_temvalue?
* this指针指向对象本身，这里使用的[=]捕获的是this指针值。
* [=](auto tv)等价于
* [this](auto tv)
* 解决办法：将成员变量赋值到一个局部变量中，捕获局部变量的值。
* (3)
* (4)
* (5)
* (6)(7)
*/


```

# 广义lambda捕获

```c++
#include<iostream>
#include<iostream>
#include<cstdlib>
#include<functional>
#include <vector>
#include <algorithm>
using namespace std;
//全局变量，每个元素都是function，每个元素都是参数是int，返回值是bool类型
std::vector<std::function<bool(int)>> gv02;

class AT
{
public:
 int m_temvalue = 7;
 void addItem()
 {
  gv02.push_back(// c++4中允许在lambda表达式形参列表中使用auto
    [abc=m_temvalue](auto tv)//将m_temvalue复制到闭包中来，lambda中使用的都是副本
        //等价于
   //[this](auto tv)
  {
   cout << "m_temvalue=" << abc << endl;//7
   if (tv%abc == 0)//如果tv是temValue的倍数
   {
    return true;

   }
   return false;
  }
  );
  /*
  * 我们会认为=是按值捕获，使我们能够访问成员变量m_temvalue,lambda表达式中使用的是m_temvalue的副本。
  *但是如果我们在调用lambda之前删除对象的指针，//46行，结果是：
  *m_temvalue=-572662307
  *0
  *也就是lambda执行的正确与否却决于pat对象是否存在，这个lambda表达式在对象存在的时候才能正确执行。
  */

 }
};

int main(void)
{
 AT *pat = new AT();
 pat->addItem();
 delete pat;//ok,可以提前删除
 cout << gv02[0](10) << endl;//0
 //delete pat;
 
 system("pause");
 return 0;
}
/*
* (1)广义lambda捕获generalized lambda capture
* c++14中引入，解决lambda表达式成员属性依赖this指针问题。

m_temvalue=7
0
请按任意键继续. . .

* 
* (3)
* (4)
* (5)
* (6)(7)
*/

```

# lambda中不捕获静态局部变量

```c++
#include<iostream>
#include<iostream>
#include<cstdlib>
#include<functional>
#include <vector>
#include <algorithm>
using namespace std;
std::vector<void(*)(int)>gv2;//用于存放一个int类型参数，返回值是void类型的lambda表达式

int main(void)
{
 static int temvalue = 4;
 gv2.push_back([](int value)//静态局部变量可以直接使用
 {
  cout << temvalue << endl;
  temvalue++;
 });
 
 gv2[0](12);//4
 gv2[0](123);//5
 gv2[0](123);//6
 
 system("pause");
 
 return 0;
}
/*
* (1)lambda中不捕获静态局部变量
* 捕获时间是不包括静态局部变量，也就是说静态局部变量是不能被捕获的。
* 但是可以在lambda表达式中使用静态局部变量，静态局部变量是保存在静态存储区，它的有效期是程序的运行期间。
*
* 静态变量不需要捕获，也捕获不到。但是这种对静态变量与按引用捕获效果类似。
*
* 将lambda匿名表达式存放在vector容器中，可以使用下表调用
*
* (2)
* (3)
* (4)
* (5)
* (6)(7)
*/

```

```c++
/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

std::vector<std::function<bool(int)>> gv;   // 全局变量，每个元素都是一个function，参数为int，返回类型bool

void func1()
{
    srand((unsigned)time(NULL));
    // int tmpvalue = rand() % 6; // 产生一个0-5之间的随机数
    
    // static int tmpvalue = rand() % 6; // 产生一个0-5之间的随机数
    static int tmpvalue = 4;
    
    // gv.push_back([&](int tv){
    gv.push_back(
        // [&](auto tv){ // 引用悬空
        // [=](auto tv){
        [](auto tv) { // 静态变量是不需要捕获的
        
            std::cout << tmpvalue << std::endl;
            
            if(tv%tmpvalue == 0) // 如果tv是tmpvalue的倍数
            {
                return true;
            }
            return false;
        }
    );
    tmpvalue++;
}

class AT
{
    public:
        int m_tempvalue =  7;
        // void addItem()
        // {
        //     gv.push_back(
        //         //  = 按值捕获
        //         // 我们会认为这个[=]是按值捕获，使得我们可以访问这个成员变量m_tmpvalue，
        //         // 所以我们就会顺理成章的认为这个lambda表达式所使用的m_tempvalue是按值捕获得到的
        //         // 但是实际上成员变量是不存在捕获的（lambda无法捕获成员变量），
        //         // 这里使用[=]捕获的是this指针值，
        //         // 所以这里lambda表达式实际上是这样子的
        //         [this](auto tv){
        //         // [=](auto tv){ // 有 = 就相当于有this
                    
        //             std::cout << m_tempvalue << std::endl; // this->m_tempvalue
                    
        //             if(tv % m_tempvalue == 0) // 如果tv是tmpvalue的倍数
        //             {
        //                 return true;
        //             }
        //             return false;
        //         }
        //     );
        // }
        
        
        // void addItem()
        // {
        //     auto tmpvalueCopy = m_tempvalue;
        //     gv.push_back(
        //         //  = 按值捕获
        //         // 我们会认为这个[=]是按值捕获，使得我们可以访问这个成员变量m_tmpvalue，
        //         // 所以我们就会顺理成章的认为这个lambda表达式所使用的m_tempvalue是按值捕获得到的
        //         // 但是实际上成员变量是不存在捕获的（lambda无法捕获成员变量），
        //         // 这里使用[=]捕获的是this指针值，
        //         // 所以这里lambda表达式实际上是这样子的
        //         [tmpvalueCopy](auto tv){
        //         // [=](auto tv){ // 有 = 就相当于有this
                    
        //             std::cout << tmpvalueCopy << std::endl; // this->m_tempvalue
                    
        //             if(tv % tmpvalueCopy == 0) // 如果tv是tmpvalue的倍数
        //             {
        //                 return true;
        //             }
        //             return false;
        //         }
        //     );
        // }
        
        
        // 广义lambda捕获
        void addItem()
        {
            gv.push_back(
                [abc = m_tempvalue](auto tv){ // 将m_tempvalue复制到闭包里来，lambda里使用的都是副本
                    std::cout << abc << std::endl; // this->m_tempvalue
                    
                    if(tv % abc == 0) // 如果tv是tmpvalue的倍数
                    {
                        return true;
                    }
                    return false;
                }
            );
        }
};

int main()
{
    // 捕获列表中的&
    // 捕获外部作用域中的所有变量，并作为引用在lambda表达式中使用
    // 按照引用方式捕获，会导致lambda表达式包含绑定到局部变量的引用
    // 引用捕获超出作用范围的情形也叫做 “引用悬空”
    // func1(); // 调用结束，tempvalue内存被回收
    // std::cout << gv[0](80) << std::endl; // 非法调用，会出现不可预料的后果
    // 因为此时此刻，你在进行调用的时候（调用容器里的lambda），会发现，lambda表达式里的tmpvalue值已经不在作用范围类了（tempvalue是定义在函数func1的作用域里的），
    // 一旦调用func1函数结束，那么内部的局部变量tempvalue就会被回收内存，
    // 此时此刻在调用lambda表达式就会导致调用了一个作用域范围外的变量，这就是使用&的一个坑的例子
    
    
    // 形参列表可以使用auto
    // c++14允许在lambda表达式中使用auto
    
    // 成员变量的捕获问题
    // AT *at = new AT();
    // at->addItem();
    // delete at;
    // std::cout << gv[0](10) << std::endl;    // 7 0
    // 结论：这里lambda表达式运算正确与否，取决去at对象是否存在，只有at对象存在，这个lambda表达式才执行正确
    // delete at;
    
    // 要明确一个概念，lambda的捕获的概念，只针对于在创建lambda表达式的作用域内可见的 非静态局部变量（包括形参）
    // 这里的m_tempvalue并不是非静态局部变量，它是AT类的成员变量，成员变量是不能被捕获到的
    // this: this指针用来指向对象本身，所以这里使用[=]捕获的是this指针值，
    
    // 那么我们如何改造，即使这个对象被销毁，lambda也能正常执行呢？（lambda表达式不依赖对象本身）
    // 解决办法就是将成员变量赋值给lambda内部的一个局部变量
    
    
    // 广义lambda捕获（generalized lambda capture）
    // 它也能解决lambda表达式执行依赖对象的问题
    // c++14
     
    
    // 静态局部变量（捕获这种事，事不包括静态局部变量的，也就是说，静态局部变量是不能被捕获的）
    // 虽然不能被捕获，但是是可以直接在lambda中直接使用的，静态局部变量是存储在静态存储区，他的有效期是一直到程序运行结束
    // 这种对static变量的使用，有点类似于按引用捕获这种效果
    func1();
    gv[0](10);  // 5
    func1();
    gv[0](10);  // 6
    gv[1](10);  // 6

    return 0;
}



```
