//客户端
/*
用于接受客户数目
将客户订单传递给server服务器端进行判断
每个客户采用独立的线程
*/
#define WIN32_LEAN_AND_MEAN //减少window头文件冲突
#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

struct Food{
    int id;//存放菜品编号
    string name;//存放菜品名称
    vector<string>ingredients;//存放菜品要用的食材
};

vector<Food>menu;

void load_menu(const string& filename){
    //ifstream是只读文件的类，ofstream是只写文件的类，fstream是总的文件操作类(读写都可以进行)
    ifstream file(filename);//打开文件
    if(!file.is_open()){
        cerr<<"The menu file failed to open."<<endl;
        return;
    }
    string buf;//用作一个缓存区，一行一行读取文件里的信息
    while (getline(file,buf))
    {
        istringstream buffer(buf);//将一行内容转换为字符串流，使得其可以像用户输入一样调用>>来进行读取
        Food food;
        buffer>>food.id;
        buffer>>ws;//跳过空白字符
        getline(buffer,food.name,'\t');//读取菜品的名称，遇到tab键就停止读入

        string ingrediens;
        while (buffer>>ingrediens){
            food.ingredients.push_back(ingrediens);
        }
        menu.push_back(food);//完全读入一样菜品以后，把它存放到meun当中
    }
}

//用于网络连接的函数
void connect_server(int cus_id,const vector<int>&ithems){
    //初始化网络库
    WSADATA net;
    //Windows专用网络初始化
    if(WSAStartup(MAKEWORD(2,2),&net)!=0){
        cerr<<"...Failed to initialize the network...."<<endl;
        return;
    }

    //创建socket
    SOCKET client_socket=socket(AF_INET,SOCK_STREAM,0);//协议参数填写0是让系统根据套接字(第二个参数)类型自动选择默认协议
    if(client_socket==INVALID_SOCKET){
        cerr<<"...Failed to create socket...."<<endl;
        WSACleanup();
        return;
    }

    //链接服务器
    sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(8080);
    //inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(connect(client_socket,(sockaddr*)&server_addr,sizeof(server_addr))==SOCKET_ERROR){
        cerr<<"An error occurred on the linked server side."<<endl;
        //cerr<<"...Incorrect link to the server..."<<endl;
        closesocket(client_socket);
        WSACleanup();
        return;
    }

    //发送订单
    ostringstream order;
    for(int item:ithems){
        order<<item<<" ";
    }
    send(client_socket,order.str().c_str(),order.str().size(),0);

    //接收响应
    char buffer[1024]={0};
    int respond_server=recv(client_socket,buffer,sizeof(buffer),0);
    if(respond_server==0){
        cerr<<"The server side is not responding."<<WSAGetLastError()<<endl;
        
        //cerr<<"...Serverdid not respond..."<<endl;
    }
    else{
        istringstream respondse(buffer);//将接收到的数据流buffer转换为字符串流并且储存在respondse中
        int result;
        bool all_deal=true;
        while(respondse>>result){
            if(result==-1){
                all_deal=false;
            }
        }
        cout<<"client:"<<cus_id<<(all_deal?"   Order fulfilled":"Out-of-stock failure")<<endl;
        //cout<<"client:"<<cus_id<<(all_deal?"order completion":"Lack of inventory")<<endl;
    }
    closesocket(client_socket);
    WSACleanup();
}

int main(){
    //读取文件，主要是获取菜品数量
    load_menu("menu.txt");
    int dish_num=menu.size();
    if(dish_num<=0){
        cout<<"The current menu is empty"<<endl;
        //cout<<"The current menu is empty."<<endl;
        return 0;
    }
    int num_cus;
    cout<<"Please enter the number of customers:";
    //cout<<"Pls enter the number of customer:";
    cin>>num_cus;
    vector<thread>customers;
    //随机数生成器
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<>item_dish(1,static_cast<int>(menu.size()));//根据菜单大小生成随机数代表菜品
    //这里将顾客可以点的菜品限定为一道了，要是要点多道就改一下这一行随机数的范围值即可
    uniform_int_distribution<>count_dish(1,1);//每个顾客最多能点和菜单上数量一样的菜品
    //********** */

    //创建线程处理每个顾客的订单，每个线程处理一个顾客
    for(int i=0;i<num_cus;++i){
        customers.emplace_back([i,&item_dish,&count_dish,&gen](){
            this_thread::sleep_for(chrono::milliseconds(100*i));
            int item_count=count_dish(gen);
            vector<int>items;
            for(int j=0;j<item_count;++j){
                items.push_back(item_dish(gen));
            }
            connect_server(i+1,items);
        });
    }
    for(auto& th:customers){
        th.join();
    }

    return 0;
}