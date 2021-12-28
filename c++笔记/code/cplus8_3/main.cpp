/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>

using namespace std;

namespace _nmsp1
{
    void func()
    {
        map<char, int> maps;
        maps['d']=10;
        maps['e']=123;
        maps['a']=120;
        maps.insert(make_pair('s', 34));
        maps.insert(make_pair('z', 45));
        maps.insert(make_pair('t', 277));
        maps.insert(pair<char, int>('r', 900));
        maps.insert(pair<char, int>('n', 322));
        maps.insert(pair<char, int>('l', 88));
        maps.insert(map<char, int>::value_type('w', 435));
        maps.insert(map<char, int>::value_type('o', 765));
        maps.insert(map<char, int>::value_type('v', 1));
        
        
        for(map<char,int>::iterator it=maps.begin(); it!=maps.end(); it++)
        {
            std::cout << it->first << "-------" << it->second << std::endl;
        }
        
        unordered_set<int> myset;
        std::cout << "bucket_count()" << myset.bucket_count()  << std::endl;
        for (int i=0; i<8; ++i)
        {
            myset.insert(i);
        }
        std::cout << "bucket_count()" << myset.bucket_count()  << std::endl;
        std::cout << "max_bucket_count()" << myset.max_bucket_count()  << std::endl;
        std::cout << "所有篮子（本容器）里的元素数量为" << myset.size() << std::endl;
        for (int i = 0; i< myset.bucket_count(); ++i)
        {
            std::cout << "第" << i << "个篮子里有的元素数量为：" << myset.bucket_size(i) << std::endl;
            auto pyfind = myset.find(i);
            // 直接查找比较费时，如果容器里有find，那么尽量用容器里的find，因为有全局find,如果容器本身没有find，在考虑
            if(pyfind!=myset.end())
            {
                std::cout << "pyfind --------------" << *pyfind << std::endl;
            }
            
        }
        
    }
}

int main()
{
    _nmsp1::func();

    return 0;
}
