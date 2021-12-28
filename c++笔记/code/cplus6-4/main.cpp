/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <list>

using namespace std;

// vector<int> g_v = {1, 2, 3}; // 定义一个共享数据区

// void myprint(int num)
// {
//     cout << "myprint开始执行，线程id === " << std::this_thread::get_id() << "编号 === " << num << endl;
    
//     cout << "id === " << std::this_thread::get_id() << "打印共享数据区数据：" << g_v[0] << g_v[1] << g_v[2] << endl;
//     return;
// }

class A
{
    public:
        // 收到的命令放到一个队列的线程
        void inMsgRecvQueue()
        {
            for (int i = 0; i < 1000000; ++i)
            {
                cout << "inMsgRecvQueue执行中...." << i << endl;
                msgRecvQueue.push_back(i); // 这个i就是命令
            }
        }
        
        // 把命令从队列中取出的线程
        void outMsgRecvQueue()
        {
            for (int i = 0; i < 1000000; ++i)
            {
                if (!msgRecvQueue.empty())
                {
                    // 消息队列不为空
                    int command = msgRecvQueue.front();
                    // list 的front方法，返回第一个元素(只是得到元素，并不会改变原list)，（不会检查元素是否存在，所以需要事先判空）
                    msgRecvQueue.pop_front(); // 移除第一个元素，但是不返回
                    cout << "消息为： " << command << endl;
                }
                else
                {
                    cout << "msgRecvQueue消息队列为空...." << i << endl;
                }
            }
            
            cout << "end!" << endl;
        }
        
    private:
        std::list<int> msgRecvQueue;
};

int main()
{
    // // 创建多个线程
    // vector <thread>mythreadPool;
    // // 创建10个线程，线程入口都统一使用myprint
    // for(int i = 0; i < 10; i++)
    // {
    //     mythreadPool.push_back(thread(myprint, i));
    //     // 创建10个线程，并开始执行
    // }
    // for (auto iter = mythreadPool.begin(); iter != mythreadPool.end(); ++iter)
    // {
    //     iter->join(); // 等待10个线程执行完毕
    // }
    
    // myprint开始执行，线程id === 140236394624768编号 === 9

    // myprint开始执行，线程id === 140236403017472编号 === 8
    
    // myprint开始执行，线程id === 140236453373696编号 === 2
    
    // myprint开始执行，线程id === 140236411410176编号 === 7
    
    // myprint开始执行，线程id === 140236470159104编号 === 0
    
    // myprint开始执行，线程id === 140236461766400编号 === 1
    
    // myprint开始执行，线程id === 140236436588288编号 === 4
    
    // myprint开始执行，线程id === 140236428195584编号 === 5
    
    // myprint开始执行，线程id === 140236419802880编号 === 6
    
    // myprint开始执行，线程id === 140236444980992编号 === 3
    
    // main return ... 
    
    // 可以看见多个线程的执行顺序是混乱的，跟操作系统内部对线程的调度机制有关
    // 把thread放到vector容器里，方便进行管理
    
    // 数据共享分析
    // 只读数据,是安全可靠的，不需要什么特别的处理手段，直接读就可以
    // 有读有写，
    // 最简单的处理就是读的时候不写，写的时候不读
    
    // 假定: 网络服务器，两个线程，一个线程用于从玩家收集命令，并将命令写到队列中，
    // 另一个线程负责从命令队列中取出命令并执行命令动作
    
    // list:在频繁的按顺序传入和删除数据时效率高，vector 对于随机的插入和删除数据效率高
    // 这里准备使用成员函数作为线程入口函数
    
    A mymsgOBJ;
    std::thread myoutmsg(&A::outMsgRecvQueue, &mymsgOBJ); // 第二个参数要使用引用，才能保证我们在线程中使用的是mymsgObj这个同一个对象
    std::thread myinmsg(&A::inMsgRecvQueue, &mymsgOBJ);
    
    myoutmsg.join();
    myinmsg.join();
    
    
    
    
    
    cout << "main return ... " << endl;
    

    return 0;
}
