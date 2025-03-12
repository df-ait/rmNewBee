#include <bits/stdc++.h>
#include <atomic>
#include <chrono>
#include <thread>
#include "Task.h"
using namespace std;


void Task::start(){
    is_run=true;
    view=thread(&Task::run,this);//类内线程，创建成员函数作为运行对象的线程;
    
}

void Task::stop(){/* */
    is_run=false;
    view.join();
}

void Task1::callback(int msg){
    *p1=msg;
}

void Task1::run() {
    while(is_run){
        //lock_guard<mutex>lock (mtx);
        int buffer=p1->exchange(0);//将p1指向的数据和0交换，并且返回旧值*p1;
        if(buffer!=0){
            *p2=buffer+1;//如果*p1不为0，就对*p1进行运算，把值赋给*p2;
            //cout<<*p2;多线程中使用cout容易导致输出混乱;
            printf("write {kind:Task1}-{key:%d}: {value:%d}\n",key,buffer+1);//*p2是一个atomic<int>对象，不能直接传递给printf,会报错,可以用p2->load()
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Task2::callback(int msg) {
    k=msg;
    *p1=1;
}

void Task2::run() {
    while(is_run){
        //lock_guard<mutex>lock (mtx);
        int buffer=p1->exchange(0);
        if(buffer!=0){
            *p2=buffer*k;
            printf("write {kind:Task2}-{key:%d}: {value:%d}\n",key,buffer*k);
        }
        std::this_thread::sleep_for(chrono::milliseconds(1));
    }
}

void Task3::callback(int msg) {
    *p1=msg;
}

void Task3::run() {
    while(is_run){
        //lock_guard<mutex>lock (mtx);
        int buffer=p1->exchange(0);
        if(buffer!=0){
            *p2=buffer;
            printf("write {kind:Task3}-{key:%d}: {value:%d}\n",key,buffer);
            std::this_thread::sleep_for(chrono::milliseconds(1));//延迟一毫秒将t+1写入*p2
            *p2=buffer+1;
            printf("write {kind:Task3}-{key:%d}: {value:%d}\n",key,buffer+1);
        }
        std::this_thread::sleep_for(chrono::milliseconds(1));
    }
}