（1）前言；
（2）string类型简介；
（3）定义和初始化string对象；
（4）string对象上的操作；

# 一：前言int, float, char, C++标准库：string, vector.

string：可变长字符串的处理。vector一种集合或者容器的概念。

# 二：string类型简介

C++标准库中的类型，代表一个可变长字符串；
string这个类型，看成一个类类型（类似于struct）；
```c++
char str[100] = "I Love China"; //C语言中用法

```

# 三：定义和初始化string对象
```c++
string s1；//默认初始化，s1 = "";""空串，表示里边没有字符；
string s2= "I Love China!"; //把I Love China!这个字符串内容拷贝到了s2代表的一段内存中。拷贝时不包括末尾\0.
string s3('');
string s4 = s2; //把s2中的内容拷贝到了s2所代表的一段内存中。

int num = 6;
string s5(num, 'a'); //aaaaaa,把s5初始化为连接num个字符'a'组成的字符串。这种方式不太推荐，因为会在系统内部创建临时对象。
```

# 四：string对象上的操作

(a) 判断是否为空empty()，返回的布尔值

```c++
string s1; 
if(s1.empty()) //成立
{
cout<<"s1为空"<<endl; 
}

```

(b) size()或者length()：返回字节/字符数量 代表该字符串的长度。unsigned int (返回无符号整型)

```c++
string s1; 
cout<<s1.size()<<endl; //0
cout<<s1.length()<<endl; //0
string s2 = "我爱中国"; 
cout<<s2.size()<<endl; //8
cout<<s2.length()<<endl; //8
string s3 = "I Love China!"; //13
cout<<s3.size()<<endl; 
cout<<s3.length()<<endl; 
```

(c) s[n]：返回s中的第n个字符（n是个整型值），n代表的是一个位置，位置从0开始，到.size() - 1; 
如果用下标n超过这个范围的内容，或者本来一个空字符串，也用s[n]去访问，都会产生不可预测的结果；

```c++
string s3 = "I Love China!"; 
if ( s3.size() > 4)
{
cout<< s3[4]<<endl; //v
s3[4] = 'w'; 
}
cout<<s3<<endl; //I Love China!

```

(d) s1+s2：字符串的连接，返回连接后的结果，其实就是得到一个新的string对象。

```c++
string s4 = "abcd"; 
string s5 = "hijk"; 
string s6 = s4 + s5; 
cout<<s6<<endl; 
```

(e) s1 = s2; 字符串对象赋值，用s2里边的内容取代原来s1里的内容。

```c++
string s7 = "abcd"; 
string s8 = "de"; 
s7 = s8; 
cout<<s7<<endl; 

```

(f) s1 == s2：判断两个字符串是否相等。大小写敏感：也就是大写字符跟小写字符是两个不同的字符。abc abC
相等：长度相同。字符全相同。

```c++
string s9 = "abc"; 
string s10 = "abc"; 
if (s9==s10)
{
cout<<"s9 ==s10" <<endl; 
}
```

(g) s1!=s2：判断两个字符串是否不相等。

```c++
string s9 = "abc"; 
string s10 = "abC"; 
if(s9 != s10)
{
cout<<"s9!=s10"<<endl; 
}

```

(h) s.c_str()：返回一个字符串s中的内容指针。返回一个指向正规的C字符串的指针常量，也就是以\0结尾的。
这个函数的引入是为了与C语言兼容，在C语言中没有string类型，所以我们得通过string类对象的c_str()成员函数把string对象转换成C语言中的字符串样式；

```c++
string s9 = "abc"; 
string s10 = "abC"; 
const char *p = s10.c_str(); //abC

char str[100]; 
strcpy_s(str, sizeof(str), p); 
cout<<str<<endl; //"abC"

//记住这也是一种初始化string的方式。用的是C语言形式的字符数组（字符串）来初始化string.
string s11(str); //用C语言的字符数组初始化string类型。
```

(i) 读写string 对象：

```c++
string s1; 
cin >> s1; //从键盘输入：abc
cout<<s1<<endl; //abc

```

(j) 字面值和string相加+

```c++
string s1 = "abc"; 
string s2 = "defg"; 
string s3 = s1 + "and" + s2 + 'e'; 
cout<<s3<<endl; //abc and defge

string s5 = "abc" + "def"; //语法上不允许这么加
string s5 = "abc" + s1 + "def"; //中间夹一个string对象，语法上就允许
string s5 = "abc" + "def" + s2; 错误，看来两个字符串不能挨着相加，否则就会报语法错
```

(k) 范围for针对string的会用：C++11中提供了范围for：能够遍历一个序列中的每一个元素
string可以看成是一个字符序列；

```c++
string s1 = "I Love China"
for(auto c : s1) //auto：变量类型自动推断 char
{
cout<<c<<endl; //每次输出一个字符，后边跟一个换行符
}
for(auto & c ： s1)
{
//toupper()把小字符转成大写，大写字符没变化。
c= toupper(c); //因为c是一个引用，所以这相当于改变s1中的值; 
}
cout<<s1<<endl; //I LOVE CHINA
```
