（1）范围for语句；
（2）动态内存分配问题；
（3）nullptr；

# 一：范围for语句：用于遍历一个序列

```c++
int v[]
{

 12,13,14,15,16

}; // 初始化一个数组
// for(auto x : v) // 数组V中每个元素，一次放入x中并打印X的值。把v中每个元素拷贝到x中，打印x的值
for(auto &x : v) // 采用引用省了拷贝动作，提升了系统效率
{

 cout<<x<<endl;

}

for(auto x:{11, 13, 14, 15, 16})
{

 cout<<x<<endl;

}

```

# 二：动态内存分配问题

C中供程序使用的存储空间有程序区，静态存储区，动态存储区。
C++中我们把内存进一步详细分为5个区域；
1、栈：一般函数内的局部变量都会放在这里，由编译器自动分配和释放。
2、堆：程序员malloc/new分配，用free/delete来释放。忘记释放后，系统会回收。
3、全局/静态存储区：放全局变量和静态变量static。程序结束时系统释放。
4、常量存储区：“I love china”;
5、程序代码区

堆和栈不同的用途和区别
1、栈空间有限的。这是系统 int=4; 分配速度快，程序员也控制不了。
2、堆：只要不超出你实际拥有的物理内存，也在操作系统允许你能够分配的最大内存大小之内，都可以分配给你。
分配速度比较慢，你可以随时用malloc/new来分配，free/delete。非常灵活。

malloc和free：在C语言中，用malloc和free从堆中来分配和释放内存用。malloc()和free()是函数；
malloc(memory allocation) ：动态内存分配
一般形式：

```c++
void *malloc(int NumBytes)：//NumBytes：要分配的字节数。分配成功则返回指向被分配内存的指针，分配失败慢返回NULL。
```

当这段分配的内存你不使用的时候，你应该用free()函数来将内存释放掉，供其他地方使用。
free：

```c++
void free(void *FirstByte)：将之前用malloc分配的内存空间还给程序（操作系统），也就是说释放了这块内存，这样这块内存就被系统回收，并在需要的时候由系统分配出去再给其他释放。

```

```c++
int *p = NULL;
p = (int *)malloc(sizeof(int));
int *p - NULL;  //C语言写法 等价于数字0
p=(int *)malloc(sizeof(int));  //在堆中分配4个字节。
if(p!=NULL)
{

 //分配成功
 *p=5;
 cout<<*p<<endl;
 free(p); //释放掉，千万不要忘记

}

char *point =NULL; 
point =(char *)malloc(100*sizeof(char));  //100个位置
if(point !=NULL) //if(point)
{

 strcpy(point,"hello world!"); // 往point里拷贝字符串
 strcpy_s(point,100,"hello world!"); //strcat--->strcat_s(); // 链接字符串
 cout<<point<<endl;
 free(point);

}

int *p = (int *)malloc(sizeof(int) *100);  //分配可以放得下100个整数的内存空间
if(p!=NULL)
{

 int *q = p;
 *q++ =1; //==>*(q++); -->*q = 1; q++;
 *q++ =5;
 cout<<*p<<endl; //1
 cout<<*(p+1)<<endl; //5
 free(p);

}
```

new 和delete：是运算符（标识符）。C++中就用new/delete分配和释放内存，不再使用malloc和free来分配和释放内存
new，delete也和malloc，free干了一样的事就是分配和释放内存。同时new，delete还干了更多的事。
new一般使用格式：
1、指针变量名 = new 类型标识符
2、指针类型名 = new 类型标识符（初始值）； //注意这里是圆括号括起来，表示初始值
3、指针类型名 = new 类型标识符【内存单元个数】；//注意，这里是[]

```c++
int *myint = new int(18);   //int *p = (int *) malloc(sizeof(int)); 
if(myint !=NULL)
{

 *myint = 8; //*myint带包指针指向的变量
 cout<<*myint<<endl;
 delete myint; //释放单个myint的空间

}
int *pa = new int[100];  //开辟一个大小为100的整形数组空间
if(pa !=NULL)
{

 int *q = pa;
 *q++=12; //[0] = 12;
 *q++=18; //[1] = 18,执行完这行，这个q其实已经指向[2]
 
 cout<<*pa<<endl; //12
 cout<<*(pa+1)<<endl; //18

 //释放内存了
 delete[] pa; //释放int pa数组空间
     //new时候我们用[]，那么delete就必须用[],[]不写数组大小

}

```

额外补充知识
1、配对使用有malloc成功必然有free，有new成功必须有delete; ;
2、free/delete，不要重复调用。

malloc/free和new/delete区别
new/delete内部还调用了构造函数等等区别

# 三: nullptr C++11中引入新关键字

nullptr代表的也是空指针。

```c++
char *p = NULL;  //NULL实际就0
char *q = nullptr; 
int *a = nullptr; 

int a=nullptr //不可以
int b= NULL //可以

if( p == nullptr)
{

 cout<<"NULL == nullptr"<<endl;

}
if(q == NULL)
{

 cout<<"q == NULL"<<endl;

}

//使用nullptr能够避免在整数和指针之间发生混淆。
cout<<typeid(NULL).name()<<endl;  //打印类型名typeid().name // int 型
cout<<type(nullptr).name()<<endl;  // std::nullptr_t 类型

//NULL和nullptr实际上是不同的类型，所以就会在调用重载函数的时候因为参数类型不同而调用不同的函数
// 结论：对于指针的初始化和用到指针相关的场合，能用nullptr的就用nullptr取代NULL
```
