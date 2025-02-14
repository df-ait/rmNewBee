#ifndef TASK_AND_ROBOT_H
#define TASK_AND_ROBOT_H

#include <bits/stdc++.h>
using namespace std;

class Task{
private:
    int task_id;
    string task_des;
    string task_state;
public:
    Task(int id,string des);
    //void T_initialize(int id,string des);//初始化任务id以及任务描述
    void execute();//执行任务
    string get_state()const{return task_state;}//获取任务状态
    string get_des()const{return task_des;}//获取任务描述
    int get_id()const{return task_id;}//获取任务id
};

class Robot{
private:
    string robot_name;
    vector<Task>robot_Task;
public:
    Robot(string name);
    //void R_initialize(string name);//初始化机器人编号并且清空任务清单
    ~Robot();
    //void undone_task();//退出程序时输出未完成任务
    void add_task(int id,string des);//添加任务
    void exeTask_R();//执行队列中所有任务
    void show();//显示任务清单中所有任务的信息
    string get_robName(){return robot_name;}
};

#endif //TASK_AND_ROBOT_H