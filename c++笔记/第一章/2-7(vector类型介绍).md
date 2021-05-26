（1）vector类型简介；
（2）定义和初始化vector对象；
（3）vector对象上的操作；
（3.1）范围for进一步讲解；

# 一：vector类型简介：

标准库：集合或者动态数组。我们可以把若干对象放在里边。
vector它能把其他对象装进来，也被称为容器。

vector<int> vjihe; //表示这个vjihe里边保存的是int型数据（int型对象）；

<int>：类模板，vector本身就是个类模板，实际上就是类模板的实例化的过程。

vector 当成类型（残缺的类类型）

vector：在vector之后加一对<>，<>内部放上类型信息。（完整的类类型）

```c++
vector<student> studlist; 

vector< vector<string> > strchuan; //可以，该集合里边的每一个元素又都是一个vector对象：集合套集合。
vector<int *> vjihe2; 
//不能往vector里装引用
vector<int &> vjihe3; //引用是个别名，不是个对象。不是对象不能往vector里边放。

```

# 二：定义和初始化vector对象

a)空vector

```c++
vector<string> mystr; //创建一个string类型的空的vector对象（容器），目前这个mystr里不包含任何元素；
// push_back(); 
mystr.push_back("abcde")
mystr.push_back("def"); 
```

b)元素拷贝的初始化方式
```c++
vector<string> mystr2(mystr); //把mystr元素拷贝给了mystr2
vector<string> mystr3 = mystr; //把mystr元素拷贝给了mystr3; 

```
c)c++11标准中，用列表初始化方法给值，用{}括起来；
```c++
vector<string> mystr4 = {"aaa", "bbb", "ccc"}
```

d)创建指定数量的元素。有元素数量概念的东西，一般会用圆括号；
```c++
vector<int> ijihe(15, -200); //创建15个int类型的元素，每个元素的值是-200; 
vector<string> sjihe(5, "hello"); //创建5个string类型的元素，每个元素的值是"hello".
vector<int> ijihe2(20); //20个元素，【0】---【19】，每一个元素都为0；
vector<string> sjihe2(5); //5个元素，下标【0】。。。【4】，每个元素都的值都为""；

```
e)多种初始化方式，（）一般表示对象中元素数量的概念。{}一般表示元素内容的概念。但不绝对。
```c++
vector<int> i1(10); //表示10个元素，每个元素值都缺省的0；
vector<int> i2{10}; //表示是一个元素，该元素的值为10；
vector<string> snor{"hello"}; //一个元素，内容是hello; 
vector<string> s22{10}; //10个元素，每个元素""。首先，这里容器s22存的是字符串，不能直接把10当成字符串存到字符串容器中去，所以就和上面的情形不一样，就会把这里的10当为10个元素，每个元素都是空字符串""
vector<string> s23{10, "hello"}; //10个元素，每个元素的内容都是"hello"
vector<int> i3(10, 1); //10个元素。每个元素的值为1；等同于初始化列表
vector<int> i4{10, 1}; //2个元素，第一个元素值为10，第二个元素值为1
// 要想正常的通过{}进行初始化，那么{}里边的值的类型，必须要跟vector后边这个<>里边元素类型相同。
vector<int> i22{"hello"}; //语法错误
```

# 三：vector对象上的操作

最常用的是不知道vector里有多少个元素。需要动态增加/减少。
所以一般来讲，先创建空vector。
vector很多操作string很类似
* a)判断是否为空empty()，返回的是布尔值。
```c++
vector<int> ivec; 
if(ivec.empty())
{
cout<<"ivec为空"<<endl; 
}

```
* b)push_back：常用方法，用于向vector中的末尾增加一个元素；
```c++
ivec.push_back(1); 
ivec.push_back(2); 
for(int i = 3; i<= 100; i++)
{
ivec.push_back(i); 
}
```

* c)size：返回元素个数；
```c++
std::cout << ivec.size() << endl; // 100

```

* d)clear移除所有元素，将容器清空
```c++
ivec.clear();
std::cout << ivec.size() << endl; // 0
```

* e)v[n]：返回V中第n个元素（n是个整形值）代表位置，位置从0开始，必须 小于size()，如果引用的下标超过这个范围，或者用[]访问了一个空的vector，那么就会产生不可预测的结果。这种数组越界是运行时错误，编译器发现不了。
```c++
std::cout << ivec[0] << endl; 

```

* f)=赋值
```c++
vector<int> ivec2; 
ivec2.push_back(111); 
ivec2 = ivec; //ivec2得到了100个元素，ivec2原来的元素就消失（被冲掉了）；
ivec2 = {12, 13, 14, 15}; //用{}中的值取代了ivec2原有值；
cout<<ivec2.size()<<endl; 

```

* g) == , !=：相等，不相等；
两个vector相等：元素数量相同，对应位置的元素值也得一样。否则就是不等
```c++
vector<int> ivec2; 
ivec2 = ivec; 
if(ivec2 == ivec)
cout<<"ivec2==ivec"<<endl; //成立
ivec2.push_back(12345); 
if (ivec2 != ivec)
cout<<""<<endl; //成立

```

* h)范围for的应用
```c++
vector<int> vecvalue{1, 2, 3, 4, 5}; 
for(auto & vecitem : vecvalue)
{
    vecitem *=2; //扩大一倍
}
for(auto vecitem:vecvalue)
{
    cout<<vecitem<<endl; 
}

```

3.1范围for进一步讲解
<font style="color: red; font-weight: blod; ">在for语句中（遍历一个容器等等类似操作中），千万不要改动vector容器的容量，增加/删除都不可以。</font>
```c++
for(auto vecitem:vecvalue)
{

    vecvalue.push_back(888); //导致输出彻底乱套
    cout<<vecitem<<endl; 

}
```
