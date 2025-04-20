//服务器端
/*维护菜单和库存
接受订单，检查库存
扣减库存并同步记录到文件中或者返回失败
记录详细日志(时间戳，菜品名称，是否完成，食材名，剩余数量)
使用多线程并发处理多个客户的点单需求*/

#include <bits/stdc++.h>
#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <iomanip>
#include <fstream>
#include <WinSock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
    
struct Food{
    int id;//存放菜品编号
    string name;//存放菜品名称
    vector<string>ingredients;//存放菜品要用的食材
};

vector<Food>meun;
unordered_map<string,int>ingreds;//用哈希表的方法来映射库存，方便检查是否还有食材，因为数据无规则所以用unordered_map而不是数组
mutex log_m,kc;//对库存和日志写入要进行加锁

//读取有哪些菜品
void load_menu(const string& Filename){
    //ifstream是只读文件的类，ofstream是只写文件的类，fstream是总的文件操作类(读写都可以进行)
    ifstream file(Filename);//打开文件
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
        getline(buffer,food.name,' ');//读取菜品的名称，遇到空格就停止读入

        string ingrediens;
        // while (buffer>>ingrediens){
        //     food.ingredients.push_back(ingrediens);
        // }
        while(getline(buffer,ingrediens,' ')){
            food.ingredients.push_back(ingrediens);
        }
        meun.push_back(food);//完全读入一样菜品以后，把它存放到meun当中
        // cout<<food.name;
        // for(auto &i:food.ingredients){
        //     std::cout<<i<<"ooooo"<<" "<<food.ingredients.size();
        // }
        // cout<<"wwwwwwwwwwwwwwwwwwwwwwww"<<endl;
        
    }
    
}

//读取库存，存放到unordered_map中
void load_ingredient(const string& Filename){
    ifstream file(Filename);
    if(!file.is_open()){
        cerr<<"The food_inventory file failed to open.."<<endl;
        return;
    }
    string buf;
    int quantity;
    while(file>>buf>>quantity){//自动按照空格分割
        ingreds[buf]=quantity;
        //cout<<buf<<" "<<quantity<<endl;读取文件正确
        //cout<<ingreds[buf]<<endl;
    }
}

//用于写入日志的函数，打开日志采用追加模式
void write_log(const string& log_msg){
    lock_guard<mutex>lock(log_m);//对写入日志操作上锁，避免多个线程同时更新日志造成混乱
    //cout<<log_msg;//这里log_msg里的库存就一直都是一个数字
    ofstream log_file("Food.log",ios::app/*|ios::binary*/);//以追加方式打开文件
    if(log_file.is_open()){
        //获取当前时间(时间戳)
        auto now=chrono::system_clock::now();
        auto now_time=chrono::system_clock::to_time_t(now);
        //进行时间格式初始化,并将新内容写入log文件之中
        log_file<<put_time(localtime(&now_time),"\n%Y-%m-%d %H:%M:%S")<<"|"<<log_msg<<"\n";
    }
}

//进行订单处理
string order_del(const string& order){
    //这里的处理思路是:只有满足订单中所有菜品都能做出来才会输出order finish认为这个订单可以完成
    //只要多个订单中有一个无法完成，也判定为failed
    unordered_map<string,int>test=ingreds;
    cout<<"The order function has been entered."<<endl;
    //istringstream是字符串转换为可读取的流
    istringstream buffer(order);
    vector<int>food_ids;
    int id;
    while(buffer>>id){
        food_ids.push_back(id);
    }
    //自动加锁保护库存操作，避免多个线程同时操作造成库存记录失误
    lock_guard<mutex>lock(kc);
    //ostringstream支持将所有类型的数据格式化为字符串
    ostringstream log_msg,respond;//将log_msg和respend强制转换为输出流字符串
    log_msg.str("");//确保为空
    log_msg<<"Task:";//流式写入，比多次拼接字符串方便，还可以写入多种类型
    //因为日志文件里这个词老是乱码所以用英文了

    vector<string>ordered_food;//储存客户订单中的菜品名称
    vector<bool>is_finish(food_ids.size(),true);

    bool all_order=true;
    
    for(size_t i=0;i<food_ids.size();i++){
        int id=food_ids[i];
        //首先检查菜品id是否有效
        if(id<0||id>meun.size())
        {
            all_order=false;
            break;
        }

        Food& index_dish=meun[id-1];//获取对应数字的菜品,因为meun是从下标0开始存储菜品的
        ordered_food.push_back(index_dish.name);

        //log_msg<<" "<<index_dish.name;
        //检查库存是否足够
        //这里循环遍历的是index_dish中的原材料，再通过哈希表查看是否还有剩余食材
        for(auto &inv:index_dish.ingredients){
            //只要有一项库存不够，那都做不了当前菜品
            cout<<inv<<endl;
            if(test[inv]<=0){
                //cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
                all_order=false;
                is_finish[i]=false;
                break;
            }
            test[inv]--;
        }  
    }
    respond<<(all_order==true?1:-1)<<" ";
    //cout<<respond.str()<<endl;
    //只有所有id合理，并且菜 品能做出来才能完成所有order
    //要是所有食材都还有剩余，那就进入扣除库存做菜的阶段
    if(all_order==true){
        ingreds=test;
    }else{
        test=ingreds;
    }

    log_msg<<"[";
    for(size_t i=0;i<ordered_food.size();i++){
        if(i!=0) log_msg<<" ";
        log_msg<<ordered_food[i]<<" "<<(is_finish[i]?" dish Finished":"dish Failed")<<endl<<"\t\t\t\t\t\t\t";
    }
    log_msg<<"]"<<(all_order?" order Finished ":" order Failed ")<<"[";
    for(auto it=ingreds.begin();it!=ingreds.end();it++){
        //cout<<it.first<<" "<<it.second;
        log_msg<<it->first<<" "<<it->second<<"; ";
    }
    log_msg<<"]";
    write_log(log_msg.str());//提取ostringstream流中的内容，转换为字符串，便于日志文件的写入
    return respond.str();
}

//线程函数，处理客户订单
DWORD WINAPI client_deal(LPVOID lpParam){
    SOCKET client_socket=*(SOCKET*)lpParam;
    char buffer[1024]={0};

    //接受客户端传入的订单数据
    int recevied=recv(client_socket,buffer,sizeof(buffer),0);
    if(recevied<=0){
        closesocket(client_socket);
        return 1;
    }

    //处理订单并进行回复响应
    string order(buffer,recevied);
    string respendse=order_del(order);
    //cout<<respendse<<endl;
    send(client_socket,respendse.c_str(),respendse.size(),0);

    closesocket(client_socket);
    delete (SOCKET*)lpParam;//释放内存
    return 0;
}

int main(){
    load_menu("menu.txt");
    load_ingredient("food_inventory.txt");
    //cout<<meun[0].name;
    //初始化网络库
    WSADATA net;
    //Windows专用网络初始化
    if(WSAStartup(MAKEWORD(2,2),&net)!=0){
        cerr<<"...Failed to initialize the network..."<<endl;
        return -1;
    }

    //创建socket
    SOCKET server_socket=socket(AF_INET,SOCK_STREAM,0);//协议参数填写0是让系统根据套接字(第二个参数)类型自动选择默认协议
    if(server_socket==INVALID_SOCKET){
        cerr<<"...Failed to create socket..."<<endl;
        WSACleanup();
        return -2;
    }

    //绑定ip和端口
    sockaddr_in server_addr;//sockaddr_in是用于存储ipv4地址和端口号的结构体
    server_addr.sin_family=AF_INET;//地址簇
    server_addr.sin_port=htons(8080);//绑定端口8080
    server_addr.sin_addr.s_addr = INADDR_ANY;//监听所有端口
    if(bind(server_socket,(sockaddr*)&server_addr,sizeof(server_addr))==SOCKET_ERROR){
        cerr<<"Failed to bind IP and port."<<endl;
        WSACleanup();
        return -3;
    }
    //开始监听
    if(listen(server_socket,SOMAXCONN)==SOCKET_ERROR){
        cerr<<"Bugging failed."<<endl;
        WSACleanup();
        return -4;
    }

    cout<<"The server side is connected to port 8080..."<<endl;    
    //循环，作用是接受客户端的连接，并且每次用一个线程来处理一个客户的订单
    while (1)
    {
        sockaddr_in client_addr;//存储客户端的ipV4地址
        int client_addr_len=sizeof(client_addr);
        SOCKET client_socket = accept(server_socket,(sockaddr*)&client_addr,&client_addr_len);
        /*
        //
        SOCKET* client_socket_ptr = new SOCKET(client_socket);
        HANDLE work_thread = CreateThread(NULL, 0, client_deal, client_socket_ptr, 0, NULL);*/
        if(client_socket==INVALID_SOCKET){
            cerr<<"Error connecting to client."<<WSAGetLastError()<<endl;
            continue;
        }
        cout<<"The client is connected"<<endl;
        //创建线程,这里使用了windows API函数
        HANDLE work_thread=CreateThread(NULL,0,client_deal,&client_socket,0,NULL);
        if(work_thread==NULL){
            cerr<<"The thread creation failed."<<endl;
            continue;//继续等待下一个链接
        }
        else{
            CloseHandle(work_thread);//释放线程句柄(线程继续运行)
        }
    }
    //清理资源
    closesocket(server_socket);
    WSACleanup();
    return 0;
}

