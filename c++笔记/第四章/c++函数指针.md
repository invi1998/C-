# C/C++ 函数指针使用总结

## 一 函数指针介绍



  函数指针指向某种特定类型，函数的类型由其参数及返回类型共同决定，与函数名无关。举例如下：

```c++
int add(int nLeft,int nRight);//函数定义  
```
 该函数类型为int(int,int),要想声明一个指向该类函数的指针，只需用指针替换函数名即可：
```c++
int (*pf)(int,int);//未初始化  
```
  则pf可指向int(int,int)类型的函数。pf前面有*，说明pf是指针，右侧是形参列表，表示pf指向的是函数，左侧为int，说明pf指向的函数返回值为int。则pf可指向int(int,int)类型的函数。而add类型为int(int,int),则pf可指向add函数。
```c++
pf = add;//通过赋值使得函数指针指向某具体函数  
```
  注意：*pf两端的括号必不可少，否则若为如下定义：
```c++
int *pf(int,int);//此时pf是一个返回值为int*的函数，而非函数指针  
```

## 二 标准C函数指针

### 1函数指针的定义

   1.1 普通函数指针定义
```c++
int (*pf)(int,int);  
```
  1.2 使用typedef定义函数指针类型
```c++
typedef int (*PF)(int,int);  
PF pf;//此时，为指向某种类型函数的函数指针类型，而不是具体指针，用它可定义具体指针</span>  
```

### 2函数指针的普通使用

```c++
pf = add;  
pf(100,100);//与其指向的函数用法无异  
(*pf)(100,100);//此处*pf两端括号必不可少  
```
注意：add类型必须与pf可指向的函数类型完全匹配



### 3函数指针作为形参

```c++
//第二个形参为函数类型，会自动转换为指向此类函数的指针  
Void fuc(int nValue,int pf(int,int));  
  
//等价的声明，显示的将形参定义为指向函数的指针  
Void fuc(int nValue,int (*pf)(int,int));  
Void fuc(int nValue,PF);  
```

形参中有函数指针的函数调用，以fuc为例：

```c++
pf = add;//pf是函数指针  
fuc(1,add);//add自动转换为函数指针  
fuc(1,pf);  
```

### 4返回指向函数的指针

4.1 使用typedef定义的函数指针类型作为返回参数

```c++
PF fuc2(int);//PF为函数指针类型  
```
4.2 直接定义函数指针作为返回参数

```c++
int (*fuc2(int))(int,int);//显示定义  
```
   说明：按照有内向外的顺序阅读此声明语句。fuc2有形参列表，则fuc2是一个函数，其形参为fuc2(int),fuc2前面有*，所以fuc2返回一个指针，指针本身也包含形参列表（int，int），因此指针指向函数，该函数的返回值为int.

总结：fuc2是一个函数，形参为(int),返回一个指向int(int,int)的函数指针。



## 二 C++函数指针



### 1由于C++完全兼容C，则C中可用的函数指针用法皆可用于C++



#### 2 C++其他函数(指针)定义方式及使用



2.1 typedef与decltype组合定义函数类型
```c++
typedef decltype(add) add2;  
```
  decltype返回函数类型,add2是与add相同类型的函数，不同的是add2是类型，而非具体函数。



使用方法：
```c++
add2* pf;//pf指向add类型的函数指针，未初始化  
```

2.2 typedef与decltype组合定义函数指针类型

```c++
typedef decltype(add)* PF2;//PF2与1.1PF意义相同  
PF2 pf;// pf指向int(int,int)类型的函数指针，未初始化  
```

2.3 使用推断类型关键字auto定义函数类型和函数指针

```c++
auto pf = add;//pf可认为是add的别名(个人理解)   
auto *pf = add;//pf为指向add的指针   
```
### 3函数指针形参

```c++
typedef decltype(add) add2;  
typedef decltype(add)* PF2;  
void fuc2 (add2 add);//函数类型形参，调用自动转换为函数指针  
void fuc2 (PF2 add);//函数指针类型形参，传入对应函数(指针)即可  
```
     说明：不论形参声明的是函数类型：void fuc2 (add2 add);还是函数指针类型void fuc2 (PF2 add);都可作为函数指针形参声明，在参数传入时，若传入函数名，则将其自动转换为函数指针。

   
### 4  返回指向函数的指针

4.1 使用auto关键字    

```c++
auto fuc2(int)-> int(*)(int,int) //fuc2返回函数指针为int(*)(int,int)
```
4.2 使用decltype关键字
```c++
decltype(add)* fuc2(int)//明确知道返回哪个函数，可用decltype关键字推断其函数类型,  
```

###  5 成员函数指针

 5.1普通成员函数指针使用举例   

```c++
class A//定义类A  
{  
private:  
  
       int add(int nLeft, int nRight)  
  
       {  
              return (nLeft + nRight);  
       }  
  
public:  
  
       void fuc()  
  
       {  
              printf("Hello  world\n");  
             
       }  
};  
  
   
typedef void(A::*PF1)();//指针名前需加上类名限定  
  
PF1 pf1 = &A::fuc; //必须有&  
  
A a;//成员函数地址解引用必须附驻与某个对象地址，所以必须创建一个队形  
  
(a.*pf1)();//使用成员函数指针调用函数  
```

5.2继承中的函数指针使用举例

```c++
class A  
{  
public:  
       void fuc()  
       {  
              printf("Hello fuc()\n");  
       }  
  
       void fuc2()  
       {  
              printf("Hello A::fuc2()\n");  
       }  
};  
  
class B:public A  
{  
public:  
       virtual void fuc2()  
       {  
              printf("Hello B::fuc2()\n");  
       }  
  
};  
  
typedef void(A::*PF1)();  
typedef void(B::*PF2)();  
  
PF1 pf1 = &A::fuc;  
  
int main()         
{  
       A a;  
       B b;  
       (a.*pf1)();  //调用A::fuc  
       (b.*pf1)();   //调用A::fuc  
  
       pf1 = &A::fuc2;  
       (a.*pf1)();  //调用A::fuc2  
       (b.*pf1)();  //调用A::fuc2  
  
       PF2 pf2 = &A::fuc2;   
       (b.*pf2)(); //调用A::fuc2  
}  
```


### 6重载函数的指针

 6.1 重载函数fuc

```c++
Void fuc();  
Void fuc(int);  
```

  6.2 重载函数的函数指针

```c++
void (*PF)(int) = fuc;//PF指向fuc(int)  
int(*pf2)(int) = fuc;//错误没有匹配的类型  
```