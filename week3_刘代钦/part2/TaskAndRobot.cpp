#include "TaskAndRobot.h"
#include <bits/stdc++.h>
using namespace std;

Task::Task(int id,string des,int priority){
    task_id=id;
    task_des=des;
    task_priority=priority;
    task_state="待执行";
}

void Task::execute(){
    task_state="已执行";
}

Robot::Robot(const string name){
    robot_name=name;
    robot_Task.clear();
}

Robot::~Robot(){
    int judge=-1;
    if(robot_Task.size()==0){
        return;
    }
    for (auto i : robot_Task)
    {
        if(i.get_state()!="已执行"){
            cout<<"任务未全部执行机器人名称: "<<robot_name<<" 未执行任务ID: "<<i.get_id()<<", 描述: "<<i.get_des()<<", 任务优先级: "<<i.get_priority()<<endl;
            judge=0;
        }
        else if(i.get_state()=="已执行"){
            judge=1;
        }
    }
    // if(judge){
    //     cout<<"机器人"<<robot_name<<"任务已全部执行";
    // }
    cout<<endl;
}

void Robot::add_task(int id,string des,int priority){
    Task index_rob(id,des,priority);
    robot_Task.insert(index_rob);
}

void Robot::exeTask_R(){
    // for (size_t k=0;k<robot_Task.size();k++)
    // {
    //     robot_Task[k].execute();
    // }
    if(robot_Task.empty()){
        return;
        cerr<<"任务列表中没有任务\n";
    }
    bool found=false;
    while(!robot_Task.empty()&&!found){
        auto i=robot_Task.end();
        while (i!=robot_Task.begin())
        {
            --i;
            if(i->get_state()!="已执行"){
                Task exe_Task=*i;
                exe_Task.execute();
                auto j=i;
                j--;
                robot_Task.erase(i);
                robot_Task.insert(exe_Task);
                found=true;
                break;
            }
        }
    }
    
    
    
    return;
}

void Robot::show(){
    // for (auto j : robot_Task)
    // {
    //     cout<<"任务ID: "<<j.get_id()<<", 描述: "<<j.get_des()<<", 状态: "<<j.get_state()<<endl;
    // }
    for (auto j = robot_Task.rbegin(); j !=robot_Task.rend(); ++j)
    {
        cout<<"任务ID: "<<j->get_id()<<", 描述: "<<j->get_des()<<", 状态: "<<j->get_state()<<", 任务优先级: "<<j->get_priority()<<endl;
    }
    cout<<endl;
    return;
}

void Robot::revoke(){
    if(!robot_Task.empty()){
        robot_Task.erase(robot_Task.begin());//set从小到大排序的，删掉它第一个元素即可
    }
    return;
}

void TaskScheduler::delete_rob(string Name){
    auto p=rob_map.find(Name);
    if(p!=rob_map.end()){
        rob_map.erase(p);
        cout<<"已删除机器人名称为  "<<Name<<"  的机器人"<<endl;
    }
    else{
        cerr<<"未查找到该机器人\n"<<endl;
    }
}

Robot* TaskScheduler::search_rob(string Name){
    auto p=rob_map.find(Name);
    if(p!=rob_map.end()){
        // (p->second).show();
        cout<<endl;
        return &(p->second);
    }
    else{
        cerr<<"未查找到该机器人\n"<<endl;
        return nullptr;
    }
}

void TaskScheduler::show_all(){
    for(auto p=rob_map.begin();p!=rob_map.end();p++){
        rob_Name.push_back(p->first);
    }
    cout<<endl<<"机器人名称列表:"<<endl;
    for (auto i = rob_Name.begin(); i!=rob_Name.end(); i++)
    {
        cout<<*i<<endl;
    }
    cout<<endl;
}

void TaskScheduler::add_rob(Robot rob){
    if(rob_map.count(rob.get_robName())){
        cout<<"已有机器人: "<<rob.get_robName()<<endl;
    }
    else{
        rob_map[rob.get_robName()]=rob;
    }
}