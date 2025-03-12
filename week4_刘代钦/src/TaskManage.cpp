#include "TaskManage.h"
#include <bits/stdc++.h>
#include <atomic>
#include <thread>
using namespace std;

void TaskManage::check_out(){
    while(true){
        //lock_guard<mutex>lock (mtx);
        int buffer=out.exchange(0);
        if(buffer!=0){
            printf("Index out : %d\n",buffer);
            printf("The modified value(out): %d\n\n",out.load());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void TaskManage::run(){
    monitor_out=thread(&TaskManage::check_out,this);
    while(true){
        string order;
        cin>>order;
        if(order=="add"){
            int key;
            cin>>key>>kind;
            if(kind>=1&&kind<=3&&!task_map.count(key)){//确保键不重复
                add_task(key,kind);
            }else if(kind<1&&kind>3){
                printf("No such kind.\n\n");
            }else if(task_map.count(key)){
                printf("There is already the same key.\n\n");
            }
             
        }else if(order=="pop"){
            pop_task();
        }else if(order=="callback"){
            int key,msg;
            cin>>key>>msg;
            callback_task(key,msg);
        }
        else {
            printf("Please enter effective order.\n\n");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void TaskManage::add_task(int key,int kind){
    Task* p=nullptr;
    auto p2=monitor_task.empty() ? &out : monitor_task.back();//如果没有任务，即monitor_task为空，out要被操作，则p2监视out
    monitor_task.push_back(new atomic<int>(0));
    auto p1=monitor_task.back();
    if(kind==1){
        p=new Task1(key,p1,p2);
    }else if(kind==2){
        p=new Task2(key,p1,p2);
    }else if(kind==3){
        p=new Task3(key,p1,p2);
    }
    task_map[key]=p;
    task_list.push_back(p);
    p->start();//p是一个Task的基指针
    printf("Add a task complete.\n\n");
}

void TaskManage::pop_task(){
    if(!task_list.empty()){
        Task* index_task=task_list.back();
        task_list.pop_back();
        index_task->stop();
        task_map.erase(index_task->key);
        delete monitor_task.back();//monitor_task里管理的是指针，是手动new进去的，所以先释放再弹出
        monitor_task.pop_back();
        delete index_task;
        printf("Delete the last task complete.\n\n");
    }
    else{
        printf("Already empty!\n\n");
    }
}

void TaskManage::callback_task(int key,int msg){
    // lock_guard<mutex>lock  (mtx);
    auto it=task_map.find(key);
    if(it!=task_map.end()){
        if(it->first==key){
            (it->second)->callback(msg);
        }
    }
    // if(task_map.count(key)){
    //     task_map[key]->callback(msg);
    // }
    else{
        printf("No such key: %d\n\n",key);
    }
}