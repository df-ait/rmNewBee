#include "TaskAndRobot.h"
#include <bits/stdc++.h>
using namespace std;

Task::Task(int id,string des){
    task_id=id;
    task_des=des;
    task_state="待执行";
}

void Task::execute(){
    task_state="已执行";
}

Robot::Robot(string name){
    robot_name=name;
    robot_Task.clear();
}

Robot::~Robot(){
    int judge=1;
    for (auto i : robot_Task)
    {
        if(i.get_state()!="已执行"){
            cout<<"未执行任务ID: "<<i.get_id()<<", 描述: "<<i.get_des()<<endl;
            judge=0;
        }
    }
    if(judge){
        cout<<"任务已全部执行";
    }
    cout<<endl;
}

void Robot::add_task(int id,string des){
    Task index_rob(id,des);
    robot_Task.push_back(index_rob);
}

void Robot::exeTask_R(){
    for (size_t k=0;k<robot_Task.size();k++)
    {
        robot_Task[k].execute();
    }
}

void Robot::show(){
    for (auto j : robot_Task)
    {
        cout<<"任务ID: "<<j.get_id()<<", 描述: "<<j.get_des()<<", 状态: "<<j.get_state()<<endl;
    }
    cout<<endl;
}