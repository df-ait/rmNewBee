#ifndef TASK_MANAGE_H
#define TASK_MANAGE_H
#include <bits/stdc++.h>
#include <atomic>
#include <thread>
#include "Task.h"
using namespace std;

class TaskManage{
    protected:
        atomic<int> out;
        vector<atomic<int>*>monitor_task;//用指针在vector容器中进行存储，确保其指向的变量的地址不会改变
        vector<Task* >task_list;//存放每个任务
        int kind;
    public:
        /*当任务管理类中有n个任务时，也对应存有n个int类型的变量a1,a2,...,ana1​,a2​,...,an​。对于第i个任务来说，
        它应当监视aiai​(对应上面的p1)并操作ai−1ai−1​(对应上面的p2)。特别的，第1个任务应当监视a1​并操作out变量*/
        map<int,Task*>task_map;//需要用到key做回调，所以需要键值对来让key和任务产生映射关系
        thread monitor_out;//线程一:监视out值;
        // thread signal;//接收信号执行操作;
        void check_out();
        void run();
        void add_task(int key,int kind);//add {key} {kind}
        void pop_task();
        void callback_task(int key,int msg);//callback {key} {msg}
};


#endif //TASK_MAGAGE_H
