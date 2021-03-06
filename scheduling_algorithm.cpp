#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
/*
    进程类
*/
class process
{
    public:
        int number;            // 进程序号
        int arrive_time;       //到达时间 
        int burst_time;        // 执行时间
        int waiting_time;      // 等待时间
        int finish_time;       //完成时间
        int turnaround_time;   // 周转时间
        double p_turnaround_time;   // 带权周转时间
        double rr_t;             //响应比
        bool isfinished;        //  进程是否已结束
    public:
        process(){};
        process(int number,int arrive_time,int burst_time){
            this->number = number;
            this->arrive_time = arrive_time;
            this->burst_time = burst_time;
            this->waiting_time = 0;
            this->finish_time = 0;
            this->turnaround_time = 0;
            this->p_turnaround_time = 0;
            this->rr_t = 0;
            isfinished = false;
        } 
        void output_information();  //输出信息
};
void calculate_tt(vector<process> &vec,int num_of_process);     //计算周转时间
void calculate_p_tt(vector<process> &vec,int num_of_process);    // 计算带权周转时间
int calculate_rrt(process v[],int num_of_process);    // 计算最大响应比
float calculate_avg_tt(vector<process> &vec,int num_of_process);    // 计算平均周转时间
float calculate_avg_p_tt(vector<process> &vec,int num_of_process);    // 计算平均带权周转时间
// 定义排序法则-- 按到达时间升序排序
bool compare(process a,process b){
	return a.arrive_time < b.arrive_time;
};
// 定义排序法则-- 按服务时间升序排序
bool compare1(process a,process b){
	return a.burst_time < b.burst_time;
};

// 定义排序法则-- 按响应比升序排序
bool compare2(process a,process b){
	return a.rr_t < b.rr_t;
};
int main(int argc, char const *argv[])
{   
    vector<process> temp;
    int num_of_process;
    cout<<"Input number of processes (limit in 10)"<<endl;
    cin >> num_of_process;
    while (1)
    {  
        if(num_of_process > 1 && num_of_process <= 10)
            break;
        else
        {
            cout<< " please input a valid number:"<<endl; 
            cin >> num_of_process;
        }
    }
    int i = 0;
    for (i = 0; i < num_of_process; i++)
    {
        cout << "please input number " << i + 1 << " process's arrive time and burst time"<<endl;
        int at;
        int bt;
        cin >> at >> bt;
        process p(i+1,at,bt); 
        temp.push_back(p);
    }
    int option;
    do{
        cout << "1. FCFS  2.SJF  3.HRRN  4.clear 0.exit"<<endl;
        cin>>option;
        switch (option)
        {
        case 1:{
                vector<process> vec(temp);      // 拷贝数组  为了重复利用用户输入的数据 将进程信息先存起来 
                sort(vec.begin(),vec.end(),compare);  ///按到达时间排序
                vec[0].finish_time = vec[0].burst_time;  //先到的单独处理
                vec[0].waiting_time = 0;                 // 第一个等待时间为 0 

                for (int i = 1; i < num_of_process; i++)
                {
                    vec[i].finish_time = vec[i-1].finish_time + vec[i].burst_time;
                    vec[i].waiting_time = vec[i-1].finish_time - vec[i].arrive_time;   
                }

                calculate_tt(vec,num_of_process);   // 计算周转时间
                calculate_p_tt(vec,num_of_process); // 计算带权周转时间
                for (int i = 0; i < num_of_process; i++)
                {
                    vec[i].output_information();        // 输出信息
                }
                cout<<calculate_avg_tt(vec,num_of_process)<<"\t"<<calculate_avg_p_tt(vec,num_of_process)<<endl;
                cout<<endl;
        }
            break;
        case 2:{
            vector<process> vec(temp);      // 拷贝数组
            sort(vec.begin(),vec.end(),compare);
            vec[0].finish_time = vec[0].burst_time;  //最短作业单独处理
            vec[0].waiting_time = 0;
            for (int i = 1; i < num_of_process; i++)
            {
                vec[i].finish_time = vec[i-1].finish_time + vec[i].burst_time;
                vec[i].waiting_time = vec[i-1].finish_time - vec[i].arrive_time;   
            }

            calculate_tt(vec,num_of_process);
            calculate_p_tt(vec,num_of_process);
            
            for (int i = 0; i < num_of_process; i++)
            {
                vec[i].output_information();
            }
            cout<<calculate_avg_tt(vec,num_of_process)<<"\t"<<calculate_avg_p_tt(vec,num_of_process)<<endl;
            cout<<endl;
        }
            break;
        case 3:{
            vector<process> vec(temp);      // 拷贝数组
            int v[num_of_process];          // 保存某一个时刻都已到达，等待调度的作业 再由响应度优先调度算法进行调度
            int current_time = 0;           // 记录当前时间
            int num = num_of_process;
            do{
                int count = 0;              // 计数器   记录当前时刻已到达的进程个数
                for (int i = 0; i < num_of_process; i++)
                {
                    if (vec[i].arrive_time <= current_time && vec[i].isfinished == false)       
                    {
                        v[count++] = i;
                    }
                }
                if (count == 1)
                {
                    int index = v[0];               
                    current_time  += vec[index].burst_time;
                    vec[index].finish_time = current_time;
                    vec[index].isfinished = true;
                    num --;
                }
                else
                {
                    process r[count];
                    for (int i = 0; i < count; i++)
                    {
                        int index = v[i];
                        vec[index].waiting_time =  current_time - vec[index].arrive_time;
                        // rrt 是小数 将 int 强转为double
                        vec[index].rr_t = 1 + double(vec[index].waiting_time)/vec[index].burst_time; 
                        r[i] = vec[index];
                    }
                    int max_index = calculate_rrt(r,count);         // 计算最高响应比  返回进程下标
                    current_time  += vec[max_index].burst_time;     
                    vec[max_index].finish_time = current_time;
                    vec[max_index].isfinished = true;
                    num--;
                }
            }while (num);                   

            calculate_tt(vec,num_of_process);
            calculate_p_tt(vec,num_of_process);

            for (int i = 0; i < num_of_process; i++)
            {
                vec[i].output_information();
            }
            cout<<calculate_avg_tt(vec,num_of_process)<<"\t"<<calculate_avg_p_tt(vec,num_of_process)<<endl;
            cout<<endl;

        }
            break;
        case 4:system("clear");
            break;
        case 0:exit(0);
        default:cout<<"invalid number!"<<endl;
            break;
        }
    }while(option);
}

void calculate_tt(vector<process> &vec,int num_of_process){
    for (int i = 0; i < num_of_process; i++)
    {
        vec[i].turnaround_time = vec[i].finish_time - vec[i].arrive_time;
    }
}

void calculate_p_tt(vector<process> &vec,int num_of_process){
    
    for (int i = 0; i < num_of_process; i++)
    {      
        vec[i].p_turnaround_time = vec[i].turnaround_time / vec[i].burst_time;
    }
}
///  计算响应比最高的进程  并返回下标
int calculate_rrt(process v[],int count){
    int max_index = 0 ;
    for (int i = 0; i < count; i++)
    {
        if (v[i].rr_t > v[max_index].rr_t)
        {
            max_index = i;
        }
    }

    return max_index;
    
}

float calculate_avg_tt(vector <process> &vec,int num_of_process){
    int sum = 0;
    for (int i = 0; i < num_of_process; i++)
    {   
         sum += vec[i].turnaround_time;
    }
    return sum / num_of_process;
}

float calculate_avg_p_tt(vector <process> &vec,int num_of_process){
    int sum = 0;
    for (int i = 0; i < num_of_process; i++)
    {   
         sum += vec[i].p_turnaround_time;
    }
    return  sum / num_of_process;
}

void process::output_information(){
    cout<<"No\tAT\tFT\tBT\tWT\tTT\tPTT" <<endl;;
    cout << this->number<<"\t"<<this->arrive_time<<"\t"<<this->finish_time<<"\t"<<this->burst_time<<"\t"<<this->waiting_time<<"\t"<<this->turnaround_time<<"\t"<<this->p_turnaround_time<<endl; 
    cout<<endl;
}
