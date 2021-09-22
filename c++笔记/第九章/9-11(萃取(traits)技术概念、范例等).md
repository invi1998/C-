# 类型萃取_迭代器萃取

```c++
#include<iostream>
#include<iostream>
#include<cstdlib>
#include<functional>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

template <typename T>
void printTraitsInfo(const T& t)
{
 cout << "--------------begin-----------" << endl;

 cout << "我们要萃取的类型名字是：" << typeid(T).name() << endl;

 cout << "is_void = " << is_void<T>::value << endl;                                    //类型是否是void
 cout << "is_class = " << is_class<T>::value << endl;                                  //类型是否是一个class
 cout << "is_object = " << is_object<T>::value << endl;                                //类型是否是 一个对象类型
 cout << "is_pod = " << is_pod<T>::value << endl;                                      //是否普通类（只包含成员变量，不包含成员函数）；   POD(plain old data)
 cout << "is_default_constructible = " << is_default_constructible<T>::value << endl;  //是否有缺省构造函数
 cout << "is_copy_constructible = " << is_copy_constructible<T>::value << endl;        //是否有拷贝构造函数
 cout << "is_move_constructible = " << is_move_constructible<T>::value << endl;        //是否有移动构造函数
 cout << "is_destructible = " << is_destructible<T>::value << endl;                    //是否有析构函数
 cout << "is_polymorphic = " << is_polymorphic<T>::value << endl;                      //是否含有虚函数
 cout << "is_trivially_default_constructible = " << is_trivially_default_constructible<T>::value << endl;      //缺省拷贝构造函数是否是可有可无的(没有也行的),返回1表示确实可有可无

 cout << "has_virtual_destructor = " << has_virtual_destructor<T>::value << endl;      //是否有虚析构函数

 cout << "--------------end-------------" << endl;

}

class A
{
public:
 A() = default;
 A(A&& ta) = delete;           //移动构造：你要不写delete，系统一般就会认为你有这个成员函数；
 A(const A& ta) = delete;      //拷贝构造 
 virtual ~A() {}
};
class B
{
public:
 int m_i;
 int m_j;
};
class C
{
public:
 C(int t) {} //有自己的构造函数，编译器不会给你提供缺省构造函数
};

void func()
{
 printTraitsInfo(int());     //扔一个临时对象进去
 printTraitsInfo(string());
 printTraitsInfo(A());
 printTraitsInfo(B());
 printTraitsInfo(C(1));
 printTraitsInfo(list<int>());
}
int main(void)
{
 func();
 system("pause");
 return 0;
}
/*
* (1)类型萃取概述  type traits
* 属于泛型编程技术，在stl实现源码中，这种类型萃取技术使用比较多。萃取：提取一下信息出来。
* c++标准库中提出了很多类型萃取接口，这些接口其实都是一些类模板。。
* https://en/cppreference.com/w/cpp/types
* 
* (2)类型萃取范例
* 通过萃取接口中的value值为true或者false就能萃取一些有用的信息。
* 
* (3)迭代器萃取概述(iteratirs traits)
* 给定迭代器类型  对象 能够给出迭代器的种类
*
* (4)总结
* c++中，模板与泛型编程，模板元编程常用于开发标准库和接口库开发，如果做这个方向工作需要专门学习。
* 我们一般都是做和业务逻辑相关的工作。
* 
*/



```

```c++
// trails.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <list>
#include <iostream>
#include <string>
#include <functional>
#include <queue>

using namespace std;

namespace _nmsp1
{
 // 卡类型萃取概述（type traits）:泛型编程，在stl实现源码中萃取技术用的比较多
 // 萃取：提取出一些信息出来
 //; c++11标准库中提供了很多类型的萃取接口， 这些接口的实质就是一些类模板
 // https://en.cppreference.com/w/cpp/types


 // 类型萃取范例
 // 通过萃取接口 中的 value和true,false就能够萃取出很多有用的信息

 template<typename T>
 void printTraitsInfo(const T& t)
 {
  cout << "--------------------begin----------------------" << endl;
  cout << "我们要萃取的类型名字是：" << typeid(T).name() << endl;  // 
  cout << "is_void = " << is_void<T>::value << endl;  // 类型是否是void
  cout << "is_class = " << is_class<T>::value << endl;  // 类型是否是一个class
  cout << "is_object = " << is_object<T>::value << endl;  // 类型是否是一个对象类型
  cout << "is_pod = " << is_pod<T>::value << endl; // 是否普通类（只包含成员变量 ， 不包含成员函数）
  cout << "is_default_constructible = " << is_default_constructible<T>::value << endl; // 是否含有缺省构造函数
  cout << "is_copy_constructible = " << is_copy_constructible<T>::value << endl;  // 是否含有拷贝构造函数
  cout << "is_move_constructible = " << is_move_constructible<T>::value << endl;  // 是否含有移动构造函数
  cout << "is_destructible = " << is_destructible<T>::value << endl;   // 是否含有析构函数
  cout << "is_polymorphic = " << is_polymorphic<T>::value << endl;  // 是否含有虚函数
  cout << "is_trivially_default_constructible = " << is_trivially_default_constructible<T>::value << endl;  // 缺省拷贝构造函数是否是可有可无的（返回1表示确实可有可无）
  cout << "has_virtual_destructor = " << has_virtual_destructor<T>::value << endl;  // 是否有虚析构函数
  cout << "---------------------end----------------------" << endl;
 }

 class A 
 {
 public:
  A() = default;
  A(A&& ta) = delete; // 移动构造，你要是不写delete，系统一般会认为你有这个成员函数
  A(const A& ta) = delete; // 拷贝构造
  virtual ~A() {}

 };

 class B
 {
 public:
  int m_i;
  int m_j;

 };

 class C
 {
 public:
  C(int t) {} // 有自己的构造函数，编译器不会给你提供缺省的构造函数
 };

 void func()
 {
  printTraitsInfo(int()); // 将临时对象做参数
  /*
  --------------------begin----------------------
  我们要萃取的类型名字是：int
  is_void = 0
  is_class = 0
  is_object = 1
  is_pod = 1
  is_default_constructible = 1
  is_copy_constructible = 1
  is_move_constructible = 1
  is_destructible = 1
  is_polymorphic = 0
  is_trivially_default_constructible = 1
  has_virtual_destructor = 0
  ---------------------end----------------------
  */
  printTraitsInfo(string());
  /*
  --------------------begin----------------------
  我们要萃取的类型名字是：class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >
  is_void = 0
  is_class = 1
  is_object = 1
  is_pod = 0
  is_default_constructible = 1
  is_copy_constructible = 1
  is_move_constructible = 1
  is_destructible = 1
  is_polymorphic = 0
  is_trivially_default_constructible = 0
  has_virtual_destructor = 0
  ---------------------end----------------------
  */
  printTraitsInfo(A());
  /*
  --------------------begin----------------------
  我们要萃取的类型名字是：class _nmsp1::A
  is_void = 0
  is_class = 1
  is_object = 1
  is_pod = 0
  is_default_constructible = 1
  is_copy_constructible = 0
  is_move_constructible = 0
  is_destructible = 1
  is_polymorphic = 1
  is_trivially_default_constructible = 0
  has_virtual_destructor = 1
  ---------------------end----------------------
  */
  printTraitsInfo(B());
  /*
  --------------------begin----------------------
  我们要萃取的类型名字是：class _nmsp1::B
  is_void = 0
  is_class = 1
  is_object = 1
  is_pod = 1
  is_default_constructible = 1
  is_copy_constructible = 1
  is_move_constructible = 1
  is_destructible = 1
  is_polymorphic = 0
  is_trivially_default_constructible = 1
  has_virtual_destructor = 0
  ---------------------end----------------------
  */
  printTraitsInfo(C(1));
  /*
  * --------------------begin----------------------
  我们要萃取的类型名字是：class _nmsp1::C
  is_void = 0
  is_class = 1
  is_object = 1
  is_pod = 0
  is_default_constructible = 0
  is_copy_constructible = 1
  is_move_constructible = 1
  is_destructible = 1
  is_polymorphic = 0
  is_trivially_default_constructible = 0
  has_virtual_destructor = 0
  ---------------------end----------------------
  */
  printTraitsInfo(list<int>());
  /*
  --------------------begin----------------------
  我们要萃取的类型名字是：class std::list<int,class std::allocator<int> >
  is_void = 0
  is_class = 1
  is_object = 1
  is_pod = 0
  is_default_constructible = 1
  is_copy_constructible = 1
  is_move_constructible = 1
  is_destructible = 1
  is_polymorphic = 0
  is_trivially_default_constructible = 0
  has_virtual_destructor = 0
  ---------------------end----------------------
  */

 }
}

int main()
{
 _nmsp1::func();

 return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件


```
