# 一：传递临时对象作为线程参数

1.1、要避免的陷阱1（解释1）
1.2、要避免的陷阱2（解释2）

```c++
class A
{
	public:
	mutable int m_i;	//const下也能修改
	//类型转换构造函数
	A(int a):m_i(a){cout<<"构造函数执行"<<this<<"threadid="<<std::this_thread::get_id()<<endl;}
	A(const A &a):m_i(a.m_i){cout<<"拷贝构造函数执行"<<this<<"threadid="<<std::this_thread::get_id()<<endl}
	~A(){cout<<"析构函数执行"<<this<<"threadid="<<std::this_thread::get_id()<<endl;}
	void thread_work(int num)	//来个参数
	{
		cout<<"子线程thread_work执行"<<this<<"thradid="<<std::this_thrad::get_id()<<endl;
	}
	void operator()(int num)
	{
		cout<<"子线程()执行"<<this<<"thradid="<<std::this_thrad::get_id()<<endl;
	}
};
//void myprint(const int &i, char *pmybuf)
void myprint(const int &i, const string &pmybuf)	//感觉可行其实还是不行
{
	cout<<i<<endl;	//分析认为，i并不是mvar的引用，实际是值传递，那么我们认为，即使主线程detach了子线程，那么子线程中用i仍然是安全的。
	cout<<pmybuf.c_str()<<endl;	//指针在detach子线程时，绝对会有问题
	cout<<*pmybuf<<endl;
	return;
}
void myprint(const int i, const A&pmybuf)
{
	cout<<&pmybuf<<endl;	//打印地址
}
//void myprint2(const A &pmybuf)
void myprint2(unique_ptr<int> pzn)
{
	pmybuf.m_i = 199;//我们修改该值不会影响main函数
	cout<<"子线程myprint2的参数地址是"<<&pmybuf<<"threadid="<<std::this_thread::get_id()<<endl;
}
int main()
{
	int mvar = 1;
	int &mvary = mvar;
	char mybuf[] = "this is a test!";
	thread mytobj(myprint,mvar,mybuf);//但是mybuf到底是在什么时候转成string.
	//事实上存在，mybuf都被回收了（main函数执行完了），系统才用mybuf去转string
	thread mytobj(myprint,mvar,string(mybuf));//这里直接将mybuf转换成string对象，这是一个可以保证在线程中用肯定有效的对象
	//mytobj.join();

	//1.2
	int mvar = 1;
	int mysecondpar = 12;
	thread myobj(myprint, mvar, A(mysecondpar));//我们是希望mysecondpar转成A类型对象传递给myprint的第二个参数
	//在创建线程的同时构造临时对象的方法传递参数是可行的；
	//mytojb.join;

	cout<<"主线程id是"<<std::this_thread::get_id()<<endl;
	int myar = 1;
	std::thread myobj(myprint2, A(mvar));	//这样显示转换是安全的
	
	A myobj(10);	//生成一个类对象
	std::thread myobj(myprint2, myobj);//myobj将类对象作为线程参数
	unique_ptr<int> myp(new int(100));
	std::thread mytobj(myprint2, std::move(myp));
	
	//用成员函数指针做线程函数
	A myobj(10);	//生成一个类对象
	std::thread myobj(&A::thread_work, myobj, 15);//&myobj==std::ref(myobj)
	mytobj.join();	//用成员函数必须用join
	
	//用（）作为线程入口函数
	A myobj(10);	//生成一个类对象
	std::thread myobj( myobj, 15);//&myobj==std::ref(myobj)//用std::ref后就不调用拷贝构造函数了，那么后续如果调用mytobj.detach()就不安全了；
	mytobj.join();
	
	mytobj.detach();	//子线程和主线程分别执行。
	cout<<"I Love China!"<<endl;
	return 0;
}
```

# 1.3、总结

a、若传递int这种简单类型参数，建议都是值传递，不要用引用。防止节外生枝。
b、如果传递类对象，避免隐式类型转换。全部都在创建线程这一行就构建出临时对象来，然后在函数参数里，用引用来接，否则系统还会多构造一次临时对象
c、建议不使用detach(),，只使用join()；这样就不存在局部变量失效导致线程对内存的非法引用问题；

二：临时对象作为线程参数继续讲，常用测试大法；
2.1、线程id概念：ID是个数字，每个线程（不管是主线程还是子线程）实际上都对应着一个数字，而且每个线程对应的数字都不同。也就是说，不同的线程，它的线程id(数字）必然是不同；
线程id可以用C++标准库里的函数来获取。std::this_thread::get_id()来获取。
2.2、临时对象构造时机捕获

三：传递类对象作为线程参数
正常子线程下修改类成员变量不会影响主线程类成员变量，不管有没有引用在线程都以拷贝接收类对象
用std::ref可以使对象真正传引用

四：用成员函数指针做线程函数
```