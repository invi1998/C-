# 容器分类和array容器的使用

```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
#include <memory>
#include <array>
using namespace std;


int main(void)
{
	//包含5个元素的数组
	array<string, 5>mystring = { "fsdfd","fsfds","ds","dsf","dfd" };
	cout << mystring.size() << endl;//5
	mystring[0] = "fsdffffsdfsdfsdfsdfsfsfdf";
	cout << sizeof(string) << endl;//字符串类型的大小---28
	for (size_t i=0;i<mystring.size();i++)
	{
		cout << "------------------------------1" << endl;
		const char*p = mystring[i].c_str();
		cout << "元素内容为:" << p << endl;
		printf("对象地址为=%p", &mystring[i]);
		cout << endl;
		printf("指向的字符串地址=%p", p);
		cout << endl;
		cout << "------------------------------2" << endl;

	}
	
	/*
			 ------------------------------1
			元素内容为:fsdffffsdfsdfsdfsdfsfsfdf
			对象地址为=002EFB60
			指向的字符串地址=0063DA38
			------------------------------2
			------------------------------1
			元素内容为:fsfds
			对象地址为=002EFB7C
			指向的字符串地址=002EFB80
			------------------------------2
			------------------------------1
			元素内容为:ds
			对象地址为=002EFB98
			指向的字符串地址=002EFB9C		//string 比较短，直接在后面存储
			------------------------------2
			------------------------------1
			元素内容为:dsf
			对象地址为=002EFBB4
			指向的字符串地址=002EFBB8
			------------------------------2
			------------------------------1
			元素内容为:dfd
			对象地址为=002EFBD0
			指向的字符串地址=002EFBD4
			------------------------------2
			请按任意键继续. . .


	 */
	//总结：对象是连续的，但是对象指向的字符串的内存空间没有关系
	
	
	system("pause");
	return 0;
}

/*
*(1)容器的分类
*vector
*list
*map
*容易用于保存数据，
*STL中的容器可以分为3大类
*	1.顺序容器(Sequence containers)，放在那里就在那里，array,vector,deque ,list, Forward-List
*	2.关联容器（Associative Containers）,元素一般是hi键值对，使用键（key）来找值（value）特别适合查找
*		set /multiset map/multimap
*		根据键自动在内部排序，能控制插入内容，但是不能控制插入位置。
*		可以使用树，或者哈希来实现
*		
*	3.无序容器（unordered containers）c++11中推出的新名字。元素的位置不重要，重要的是元素是否在容器中
*		无序容易也是属于一种关联容器。
*		随着元素的的插入，每个元素在容器中的位置可能随着发生改变。内部可能使用哈希表来实现
*		常见的有：unordered_set unordered_multiset unordered_map  unordered_multimap
*	c++标准没有规定是用什么算法实现上面容器
*
*(2)容器的说明说明和应用事项例
*	2.1array本质上是一个内存空间连续的数组，大小固定，不能增加它的大小
*	对象是连续的，但是对象指向的字符串的内存空间没有关系
*
*(3)
*
*
*/
```


# vector容器

```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
#include <memory>

using namespace std;

class A
{
public:
	A(int a):m_i(a)
	{
		cout << "构造函数执行" << endl;
	}
	A(const A&a):m_i(a.m_i)
	{
		cout << "A的拷贝构造函数执行" << endl;
	}

	~A()
	{
		cout << "A的析构函数执行" << endl;
	}
public:
	int m_i;
};

int main(void)
{
	vector<A>myA;
	for (int i=0;i<5;i++)
	{
		cout << "----------begin-----------" << endl;
		myA.push_back(A(i));
		cout << "----------end-----------" << endl;

	}
	/*
	 * ----------begin-----------
		构造函数执行
		A的拷贝构造函数执行
		A的析构函数执行
		----------end-----------
		----------begin-----------
		构造函数执行
		A的拷贝构造函数执行
		A的析构函数执行
		A的拷贝构造函数执行
		A的析构函数执行
		----------end-----------
		----------begin-----------
		构造函数执行
		A的拷贝构造函数执行
		A的拷贝构造函数执行
		A的析构函数执行
		A的析构函数执行
		A的拷贝构造函数执行
		A的析构函数执行
		----------end-----------
		----------begin-----------
		构造函数执行
		A的拷贝构造函数执行
		A的拷贝构造函数执行
		A的拷贝构造函数执行
		A的析构函数执行
		A的析构函数执行
		A的析构函数执行
		A的拷贝构造函数执行
		A的析构函数执行
		----------end-----------
		----------begin-----------
		构造函数执行
		A的拷贝构造函数执行
		A的拷贝构造函数执行
		A的拷贝构造函数执行
		A的拷贝构造函数执行
		A的析构函数执行
		A的析构函数执行
		A的析构函数执行
		A的析构函数执行
		A的拷贝构造函数执行
		A的析构函数执行
		----------end-----------
		请按任意键继续. . .

	总结：先调用构造函数，创建一个临时地对象，然后使用拷贝构造函数，将对象拷贝进myA容器中，调用析构函数析构掉对象。
		
	 */

	//查看元素个数
	cout << myA.size() << endl;//5
	//查看容器空间个数
	cout << myA.capacity() << endl;//6

	//容器的地址和第一个元素的地址不相同
	printf("第一个元素的地址=%p\n", &myA[0]);//0073B730
	printf("容器的地址=%p\n", &myA);//0036F998

	//删除下标为2的元素，只调用一次析构函数
	int icount = 0;
	cout << "-------------" << endl;
	for (vector<A>::iterator iter=myA.begin();iter!=myA.end();iter++)
	{
		icount++;
		if (icount==2)
		{
			myA.erase(iter);
			break;
		}
	}

	cout << "****************************" << endl;
	for (vector<A>::iterator iter = myA.begin(); iter != myA.end(); iter++)
	{
		icount++;
		if (icount == 3)
		{
			myA.insert(iter, A(34));
			break;
		}
	}
	/*
	 *	 构造函数执行
		A的拷贝构造函数执行
		A的拷贝构造函数执行
		A的析构函数执行
		A的拷贝构造函数执行
		A的析构函数执行
		A的拷贝构造函数执行
		A的析构函数执行
		A的拷贝构造函数执行
		A的析构函数执行
		A的析构函数执行

	 */

	//设置容器的最大元素个数
	vector<int>myint;
	myint.reserve(10);
	cout << myint.capacity() << endl;//10
	system("pause");
	return 0;
}

/*
*(1)vector容器实现类似于一个栈，向顶部增加元素和删除元素都很快，删除中间的元素很慢，查找的速度也不会太快
*
*	vector容器内存是挨着的，vector有一个“空间”的概念，每一个空间可以装一个元素。
*	当只有一个元素的时候，只有一个空间。当再插入一个元素的时候，找一段可以装两个元素的“空间”，
*	将原来的对象在新空间构造，析构掉老空间的对象。所以插入元素会调用多次构造函数和析构函数。
*	但是容器观测到你多次插入元素，容器会给你多分配内存，以便不用每次新分配内存。
*
*	也就是说，vector容器插入元素是不断寻找新的内存空间，析构和构造的过程。
*
*为什么要析构老元素，构造新元素？
*	主要的原因是这种容器的实现要求内存是连续的。
*	
*(2)我们使用vector，一般需要提前知道元素不超过多少个，把capacity设置为最大值
*
*(3)
*
*
*/
```