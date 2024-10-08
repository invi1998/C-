```c++
/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <memory>
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

int main()
{
    // weak_ptr
    // weak_ptr用来辅助shared_ptr，他是一种弱智能指针
    // 他也是一个类模板，他是用来指向一个有shared_ptr所管理的对象，但是weak_ptr这种指针不控制所指对象的生命周期
    // 换句话说，就是将weak_ptr绑定到shared_ptr的引用计数，更确切的说，weak_ptr的构造和析构不会增加或者减少所指对象的引用计数
    // 当shared_ptr需要释放所指对象的时候，是不会管是否有weak_ptr指向的。
    
    // 这个弱引用的作用，是用来监视shared_ptr的生命周期，是一种对shared_ptr的功能增强
    
    // weak_ptr的创建
    // 我们创建一个weak_ptr的时候一般使用一个shared_ptr来进行初始化
    // auto pi = make_shared<int>(100);
    // weak_ptr<int>piw(pi); // piw就是一个弱共享pi, 她的出现不会改变强引用（shared_ptr）的引用计数，但是会改变弱引用（weak_ptr）的引用计数
    // cout << pi.use_count() << endl; // 1
    // cout << piw.use_count() << endl; // 1
    // weak_ptr<int> piw2;
    // piw2 = pi;
    // weak_ptr<int> piw3;
    // piw3 = piw2;
    
    // cout << pi.use_count() << endl; // 1
    // cout << piw.use_count() << endl; // 1
    // cout << piw2.use_count() << endl; // 1
    
    // // lock() 功能就是检查weak_ptr所指向的对象是否存在，如果存在，那么lock就会返回一个指向该对象的shared_ptr（指向对象的强引用计数加1）
    // // 如果他所指向的对象不存在，lock就会返回一个空的shared_ptr
    // // 
    // pi.reset(); // 因为pi是唯一指向该引用对象的指针，所以reset的调用会释放该智能指针指向的对象，同时将pi置空
    // auto pi2 = piw.lock();
    // if (pi2) { // 注意，这种判断代码务必要写，特别是涉及到指针可能会产生空指针的情况
    //     cout << *pi2 << endl; // 100
    //     cout << pi2.use_count() << endl; // 2
    // } else {
    //     cout << "lock返回空" << endl;
    // }
    
    // weak_ptr 常用操作
    // use_count(); 获取与该弱指针共享对象的其他强指针shared_ptr的数量，或者说，获取当前所监视资源的强引用计数
    
    // auto pi = shared_ptr<int>(new int(2000));
    // auto pi2(pi);
    // weak_ptr<int> pw(pi);
    // cout << pw.use_count() << endl; // 2
    
    // // expired() 是否过期，若该指针的use_count返回0，则表示该弱指针指向的对象已经不存在了，则返回true，表示已过期。
    // // 换句话说，这个函数用来判断所观测的对象资源是否被释放
    // pi.reset();
    // cout << pw.expired() << endl; // 0 false
    // pi2.reset();
    // cout << pw.expired() << endl; // 1 true
    
    // // reset() 将该弱引用指针设置为空，不影响指向对象的强引用计数，但是指向该对象的弱引用数量会减少
    // piw.reset();
    
    // auto pi = make_shared<int>(240);
    // weak_ptr<int> pw;
    // pw = pi;
    // if (!pw.expired())
    // {
    //     auto p2 = pw.lock(); // 返回一个shared_ptr,并且此时强引用计数为2
    //     if (p2 != nullptr)
    //     {
    //         //
    //         int test;
    //         test = 122;
            
    //         // 离开这个作用域，强引用计数会恢复为1
    //     }
    // }
    // else
    // {
    //     // 已过期
    //     int test;
    //     test = 1;
    // }
    
    // 尺寸问题
    // weak_ptr<int> pw;
    // weak_ptr的尺寸和shared_ptr的尺寸一样大，都是裸指针的2倍
    weak_ptr<int>pw;
    int *p;
    cout << sizeof(p) << endl; // 8
    cout << sizeof(pw) << endl; // 16

    return 0;
}


```

在C++中，智能指针的大小与其内部实现有关。不同的智能指针类型（如 `std::unique_ptr`、`std::shared_ptr` 和 `std::weak_ptr`）有不同的内部结构，因此它们的大小也不同。了解这些大小有助于优化内存使用和性能。

### 1. `std::unique_ptr`

`std::unique_ptr` 是一个轻量级的智能指针，通常只包含一个指针成员。它的大小通常与普通指针相同。

#### 示例

```cpp
#include <iostream>
#include <memory>

int main() {
    std::cout << "Size of std::unique_ptr<int>: " << sizeof(std::unique_ptr<int>) << " bytes" << std::endl;
    std::cout << "Size of int*: " << sizeof(int*) << " bytes" << std::endl;
    return 0;
}
```

输出可能是：

```
Size of std::unique_ptr<int>: 8 bytes
Size of int*: 8 bytes
```

### 2. `std::shared_ptr`

`std::shared_ptr` 是一个更复杂的智能指针，因为它需要管理引用计数和控制块。控制块通常包含引用计数、弱引用计数和指向实际对象的指针。因此，`std::shared_ptr` 的大小通常大于 `std::unique_ptr`。

#### 示例

```cpp
#include <iostream>
#include <memory>

int main() {
    std::cout << "Size of std::shared_ptr<int>: " << sizeof(std::shared_ptr<int>) << " bytes" << std::endl;
    std::cout << "Size of int*: " << sizeof(int*) << " bytes" << std::endl;
    return 0;
}
```

输出可能是：

```
Size of std::shared_ptr<int>: 16 bytes
Size of int*: 8 bytes
```

### 3. `std::weak_ptr`

`std::weak_ptr` 也是复杂的智能指针，它需要与 `std::shared_ptr` 共享控制块。因此，`std::weak_ptr` 的大小通常与 `std::shared_ptr` 相同。

#### 示例

```cpp
#include <iostream>
#include <memory>

int main() {
    std::cout << "Size of std::weak_ptr<int>: " << sizeof(std::weak_ptr<int>) << " bytes" << std::endl;
    std::cout << "Size of std::shared_ptr<int>: " << sizeof(std::shared_ptr<int>) << " bytes" << std::endl;
    return 0;
}
```

输出可能是：

```
Size of std::weak_ptr<int>: 16 bytes
Size of std::shared_ptr<int>: 16 bytes
```

### 详细解释

#### `std::unique_ptr`

- **内部结构**：通常包含一个指针成员。
- **大小**：与普通指针相同，通常是8字节（在64位系统上）。

#### `std::shared_ptr`

- **内部结构**：
  - 指向控制块的指针。
  - 控制块包含：
    - 引用计数。
    - 弱引用计数。
    - 指向实际对象的指针。
- **大小**：通常为16字节（在64位系统上），因为需要存储指向控制块的指针。

#### `std::weak_ptr`

- **内部结构**：
  - 指向控制块的指针。
- **大小**：通常为16字节（在64位系统上），因为需要存储指向控制块的指针。

### 影响因素

- **平台和编译器**：不同平台和编译器可能会有不同的实现细节，导致智能指针的大小略有不同。
- **自定义删除器**：如果 `std::shared_ptr` 使用了自定义删除器，可能会增加控制块的大小，从而影响 `std::shared_ptr` 和 `std::weak_ptr` 的大小。

### 总结

- **`std::unique_ptr`**：轻量级，通常与普通指针大小相同。
- **`std::shared_ptr`**：复杂，包含控制块，通常为16字节（在64位系统上）。
- **`std::weak_ptr`**：复杂，与 `std::shared_ptr` 共享控制块，通常为16字节（在64位系统上）。



---





weak_ptr概述与常见操作

```c++
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
#include <memory>

using namespace std;


int main(void)
{
	//weak_ptr一般是使用shared_ptr来创建爱女
	auto p1 = make_shared < int>(100);
	weak_ptr<int>p2Weak(p1);//p2Weak是弱引用，强引用计数不会改变，一个强引用，一个弱引用指向100
	//只有强引用决定对象的生存期。若引用对对象生存期没有影响
	weak_ptr<int>p3Weak = p2Weak;//使用一个弱引用给另一个弱引用赋值。

	//lock()的功能：检查weak_ptr所指向的对象是否存在，如果存在，就返回一个指向该对象的shared_ptr指针，
	//如果不存在,返回的指针指向为空
	//p1.reset();//释放该对象
	auto p4shared_ptr = p2Weak.lock();//p4shared_ptr是一个返回的shred_ptr,强引用计数+1
	

	//常见函数用法
	int count = p2Weak.use_count();
	cout << count << endl;	//2

	//expired--过期判断
	//指向100的两个强引用都设置为过期
	p1.reset();
	p4shared_ptr.reset();
	if (p2Weak.expired())//返回值为true
	{
		cout << "对象已经过期" << endl;
	}

	//(4)尺寸问题
	weak_ptr<int>p5_weak;
	int*p6 = nullptr;
	int length1 = sizeof(p5_weak);	//8
	int length2 = sizeof(p6);	//4
	cout << "length1=" << length1 << endl;
	cout << "length2=" << length2 << endl;

	system("pause");
	return 0;
}

/*
*(1)weak_ptr概述
*用于辅助shared_ptr指针进行工作
*强指针strong--shared_ptr
*弱指针weak--weak_ptr
*weak_ptr也是一个类模板，这个指针指向一个有shared_ptr管理的对象，但是weak_ptr不控制生存周期。
*换句话说，将weak_ptr绑定到shared_ptr上不会改变shared_ptr对象的引用计数。
*当shared_ptr需要释放所指定对象时候，不管是否有weak_ptr是否指向该对象。
*
*weak_ptr为什么需要引入？？？
*weak--能力弱，也叫弱引用，弱共享
*作用：理解为监视shared_ptr的声明周期，是对shared_ptr的一种扩充。
*weak_ptr不是一种独立的智能指针，不能用来操作所指向的资源，所以它看起来是shared_ptr的一个助手
*weak_ptr用于监视它所指向的对象是否存在。
*
*
*(2)weak_ptr的创建、
*使用shared_ptr初始化weak_ptr
*
*(3)weak_ptr用法
*	可以使用weak_ptr为shared_ptr赋值
*	
*	1.use_count() 获取该弱指针共享对象的其他shared_ptr的数量，或者说获取当前观测支援的强引用数量。
*	2.expired()是否过期的意思，弱引用指针的use_count()==0，返回true，换句话说，这个函数用于判断所观测的资源是否已经被释放。
*(4)尺寸问题
*	weak_ptr<int>pw
*	weak_+ptr和shared_ptr尺寸一样大，是裸指针的两倍
*	一个智能指针包含两个指针，（1）第一个指针指向智能指针的对象
*		（2）第二个指针指向“智能指针控制块”的数据结构，控制块里面有“强引用计数”，“弱引用计数”“其他数据等如内存分配器，自定义删除器等”
*	详细见图
*		
*/
```

