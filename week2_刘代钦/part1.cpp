#include <bits/stdc++.h>
#include "TaskAndRobot.h"
// #include "TaskAndRobot.cpp"
using namespace std;

int main(){
    queue<Robot>rob_queue;
    while (1)//外层循环先获取机器人编号
    {   
        cout<<"请输入机器人名称:";
        string rob_name;
        cin>>rob_name;   
        Robot buffer_rob(rob_name);
        while (1)//内层循环对应每个机器人任务清单
        {
            fflush(stdin);
            cout<<"请输入命令(添加/执行/显示/退出): ";
            string user_input;
            cin>>user_input;
            if(user_input=="添加"){
                // cout<<"请输入任务id与任务描述: ";
                int buffer_id;
                string buffer_des;
                cin>>buffer_id>>buffer_des;
                buffer_rob.add_task(buffer_id,buffer_des);
            }
            else if(user_input=="执行"){
                buffer_rob.exeTask_R();
            }
            else if(user_input=="显示"){
                buffer_rob.show();
            }
            else if(user_input=="退出"){
                rob_queue.push(buffer_rob);
                buffer_rob.~Robot();
                break;
            }
            else{
                cout<<"请输入有效命令\n";
            }
        }
    cout<<"已结束对"<<rob_name<<"的命令\n"<<endl;
   
    cout<<"是否退出此系统(输入“是”则退出): ";
    string input;
    cin>>input;
    if(input=="是"){
        break;
    }
    cout<<endl;
    }
    return 0;
}
