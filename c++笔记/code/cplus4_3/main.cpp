#include <cstdio>
#include <iostream>
#include <string>
#include "MyArray.h"

using namespace std;

class Tc
{
	public:
  		Tc(){
        	cout << "构造函数执行" << endl;
        }
  		Tc(const Tc&tc)
        {
        	cout << "拷贝构造函数执行" << endl;
        }
  
  		int operator()(int v1, int v2)const
        {
        	return v1 * v2;
        }
};


typedef int(*FunType)(int, int); // 定义一个函数指针类型

int func2(int a, int b)
{
    return a - b;
}

// template <typename T, typename F=Tc> // 第三个参数我们默认给他一个默认参数Tc
// void testFunc(const T&i, const T&j, F funcpoint=F()) // 如果不使用函数模板，这行代码就等价于
// void testFunc(const int &i, const int &j, Tc funcpoint = Tc())
// 同样我们也可以为该函数模板提供一个缺省的函数指针作为参数
template <typename T, typename F=FunType> // 第三个参数我们默认给他一个函数指针类型作为类型参数（函数指针类型需要实现定义好）
void testFunc(const T&i, const T&j, F funcpoint=func2) // 这里缺省的默认参数，因为是一个FuncType,所以这里需要传入一个函数指针
{
	cout << funcpoint(i, j) << endl;
}


int main()
{
	Tc testC;
  	testFunc(2,6,testC);
  	testFunc(2,6,func2);
  	testFunc(2,6,Tc());
  	
  	MyArray<>abc; //完全使用模板参数的缺省值
  	MyArray<int, 100>intTest;
  	
  	abc.myfunc();
  	intTest.myfunc();
  	
  	testFunc(2,6); //只传递前两个参数，第三个参数使用缺省的默认TC
  	
}