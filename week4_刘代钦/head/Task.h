#ifndef TASK_H
#define TASK_H
#include <bits/stdc++.h>
#include <atomic>
#include <thread>
using namespace std;

class Task{
    protected:
        atomic<int>* p1=nullptr,*p2=nullptr;
    public:
        int key;
        atomic<bool> is_run;
        thread view;//监视p1指向变量的线程
        Task(int key_,atomic<int>* p1_,atomic<int>* p2_): key(key_), p1(p1_), p2(p2_){}//初始化列表
        virtual void callback(int msg) = 0;//回调函数
        virtual void run() = 0;//运行函数
        void start();//开始函数，里面运行view线程
        void stop();//停止函数
        atomic<int>* Getp1(){return p1;}//获取指针p1;
        atomic<int>* Getp2(){return p2;}//获取指针p2;
        int GetKey(){return key;}//获取标识符key;
        virtual ~Task() = default;//有虚函数就带上虚析构
};

class Task1:public Task{
    public:
        Task1(int key_,atomic<int>* p1_,atomic<int>* p2_):Task(key_,p1_,p2_){}//调用基类构造函数实现初始化列表
        void callback(int msg) override;//重写基类纯虚函数callback;
        void run() override;//重写基类纯虚函数run
};

class Task2:public Task{
    private:
        int k=1;
    public:
        Task2(int key_,atomic<int>* p1_,atomic<int>* p2_):Task(key_,p1_,p2_){}
        void callback(int msg) override;
        void run() override;
};

class Task3:public Task{
    public:
        Task3(int key_,atomic<int>* p1_,atomic<int>* p2_):Task(key_,p1_,p2_){}
        void callback(int msg) override;
        void run() override;
};
#endif //TASK_H
