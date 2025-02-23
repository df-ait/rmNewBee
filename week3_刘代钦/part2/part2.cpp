#include <bits/stdc++.h>
#include "TaskAndRobot.h"
// #include "TaskAndRobot.cpp"
using namespace std;

int main(){
    TaskScheduler task_schedule;
    cout<<"本程序有以下可用命令:"<<endl<<
    "-添加机器人"<<endl<<"-添加任务"<<endl<<"-撤销任务(将删除优先级最小的任务)"<<
    endl<<"-执行任务"<<endl<<"-删除机器人"<<endl<<"-查询任务"<<
    endl<<"-获取机器人名称列表"<<endl<<"-退出程序"<<endl;
    cout<<"请输入你想要执行的命令:";

    while (1)//先获取用户指令
    {   
        cout<<"请输入你想要执行的命令:";
        string order;/*用户输入的命令*/
        cin>>order;   
        // Robot* buffer_rob=new Robot(rob_name);
        if(order=="添加机器人"){
            cout<<"--机器人名称:";
            string rob_name;
            cin>>rob_name;
            Robot buffer_rob(rob_name);
            task_schedule.add_rob(buffer_rob);
            // Robot* buffer_rob=new Robot(rob_name);
            // task_schedule.add_rob(*buffer_rob);
            // delete buffer_rob;
            // buffer_rob==nullptr;
        }
        else if(order=="添加任务"){
            cout<<"--输入机器人名称 任务ID 任务描述 任务优先级: "<<endl;
            string addTask_name;
            int task_id,task_priority;
            string task_des;
            cin>>addTask_name>>task_id>>task_des>>task_priority;
            Robot* Rob_addTask=task_schedule.search_rob(addTask_name);
            if(Rob_addTask!=nullptr)
            {
                Rob_addTask->add_task(task_id,task_des,task_priority);
            }
            else 
                continue;
        }
        else if(order=="撤销任务"){
            cout<<"--机器人名称:";
            string Name;
            cin>>Name;
            Robot* Rob_revoke=task_schedule.search_rob(Name);
            if(Rob_revoke!=nullptr)
            {
                Rob_revoke->revoke();
                Rob_revoke->show();
            }
            else 
                continue;
        }
        else if(order=="执行任务"){
            cout<<"--机器人名称:";
            string exe_robName;
            cin>>exe_robName;
            Robot* Rob_exe=task_schedule.search_rob(exe_robName);
            if(Rob_exe!=nullptr)
            {
                Rob_exe->exeTask_R();
                Rob_exe->show();
            }
            else 
                continue;
        }
        else if(order=="删除机器人"){
            cout<<"--机器人名称:";
            string delete_robName;
            cin>>delete_robName;
            Robot* Rob_delete=task_schedule.search_rob(delete_robName);
            if(Rob_delete!=nullptr)
            {
                task_schedule.delete_rob(delete_robName);
                Rob_delete->show();
            }
            else 
                continue;
        }
        else if(order=="查询任务"){
            cout<<"--机器人名称:";
            string search_robName;
            cin>>search_robName;
            Robot* Rob_search=task_schedule.search_rob(search_robName);
            if(Rob_search!=nullptr)
            {
                Rob_search->show();
            }
            else 
                continue;
        }
        else if(order=="获取机器人名称列表"){
            task_schedule.show_all();
        }
        else if(order=="退出程序"){
            break;
        }
        else
            cerr<<"*请输入可使用的命令*"<<endl;
    }
    
    return 0;
}
