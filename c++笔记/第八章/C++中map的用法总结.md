# C++中map的用法总结

## 一、map简介

map是STL（中文标准模板库）的一个关联容器。

可以将任何基本类型映射到任何基本类型。如int array[100]事实上就是定义了一个int型到int型的映射。
map提供一对一的数据处理，key-value键值对，其类型可以自己定义，第一个称为关键字，第二个为关键字的值
map内部是自动排序的

## 二、map的用法

必须引入包
```c++
#include<map>
```
2.map的定义

map<type1name,type2name> maps;//第一个是键的类型，第二个是值的类型

```c++
map<string,int> maps;
```

3.map容器内元素的访问

通过下标进行访问
如：maps['c']=5;

通过迭代器进行访问
map可以使用it->first来访问键，使用it->second访问值

```c++
#include<map>
#include<iostream>
using namespace std;
int main()
{
   map<char,int>maps;
   maps['d']=10;
   maps['e']=20;
   maps['a']=30;
   maps['b']=40;
   maps['c']=50;
   maps['r']=60;
   for(map<char,int>::iterator it=mp.begin();it!=mp.end();it++)
   {
       cout<<it->first<<" "<<it->second<<endl;
   }
   return 0;
}
```
4.map的常用用法

maps.insert() 插入
```c++
// 定义一个map对象
map<int, string> m;
 
//用insert函数插入pair
m.insert(pair<int, string>(111, "kk"));
 
// 用insert函数插入value_type数据
m.insert(map<int, string>::value_type(222, "pp"));
 
// 用数组方式插入
m[123] = "dd";
m[456] = "ff";
maps.find() 查找一个元素
find(key): 返回键是key的映射的迭代器

map<string,int>::iterator it;
it=maps.find("123");
maps.clear()清空


maps.erase()删除一个元素
//迭代器刪除
it = maps.find("123");
maps.erase(it);

//关键字删除
int n = maps.erase("123"); //如果刪除了返回1，否则返回0

//用迭代器范围刪除 : 把整个map清空
maps.erase(maps.begin(), maps.end());
//等同于mapStudent.clear()
maps.szie()长度
int len=maps.size();获取到map中映射的次数
maps.begin()返回指向map头部的迭代器
maps.end()返回指向map末尾的迭代器
//迭代
map< string,int>::iterator it;
for(it = maps.begin(); it != maps.end(); it++)
    cout<<it->first<<" "<<itr->second<<endl;//输出key 和value值
maps.rbegin()返回指向map尾部的逆向迭代器
maps.rend()返回指向map头部的逆向迭代器
//反向迭代
map<string,int>::reverse_iterator it;
for(it = maps.rbegin(); it != maps.rend(); it++)
    cout<<it->first<<' '<<it->second<<endl;
maps.empty()判断其是否为空
maps.swap()交换两个map
```