// MyArray.h

#include <string>
#include <iostream>

using namespace std;

#ifndef __MYARRAY__
#define __MYARRAY__

template<typename T=string, int size=5>
class MyArray
{
    private:
        T arr[size];
    
    public:
        void myfunc();
};

template<typename T, int size>
void MyArray<T, size>::myfunc()
{
    cout << size <<endl;
    return;
}

#endif