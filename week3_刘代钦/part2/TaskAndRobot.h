#ifndef TASK_AND_ROBOT_H
#define TASK_AND_ROBOT_H

#include <bits/stdc++.h>
using namespace std;

class Task{
private:
    int task_id;
    string task_des;
    string task_state;
    int task_priority;
public:
    Task(int id,string des,int priority);
    //void T_initialize(int id,string des);//初始化任务id以及任务描述
    bool operator<(const Task& other)const{
        return task_priority<other.task_priority;
    }
    void execute();//执行任务
    string get_state()const{return task_state;}//获取任务状态
    string get_des()const{return task_des;}//获取任务描述
    int get_id()const{return task_id;}//获取任务id
    int get_priority()const{return task_priority;}//获取任务优先级
};

class Robot{
private:
    string robot_name;
    set<Task>robot_Task;
public:
    Robot(const string name);
    Robot(){};
    //void R_initialize(string name);//初始化机器人编号并且清空任务清单
    ~Robot();
    //void undone_task();//退出程序时输出未完成任务
    void add_task(int id,string des,int priority);//添加任务
    void exeTask_R();//执行队列中优先级最大的一个任务
    void show();//显示任务清单中所有任务的信息
    string get_robName(){return robot_name;}//获取机器人名称
    void revoke();//删除优先级最小的任务
};

class TaskScheduler{
private:
    map<string,Robot>rob_map;
    vector<string>rob_Name;
public:
    void add_rob(Robot rob);
    void delete_rob(const string Name);
    Robot* search_rob(const string Name);
    void show_all();
};
#endif //TASK_AND_ROBOT_H