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
        float p_turnaround_time;   // 带权周转时间
        float rr_t;             //响应比
        bool is_finshed;        //是否已结束
    public:
        process(int number,int arrive_time,int burst_time){
            this->number = number;
            this->arrive_time = arrive_time;
            this->burst_time = burst_time;
            is_finshed = false;
        }
        process operator >(const process &p){
            if(arrive_time > p.arrive_time){
                return *this;
            }
            else
            {
                return p;
            }
        }  
        void output_information();  //输出信息
};
void calculate_tt(vector<process> &vec,int num_of_process);     //计算周转时间
void calculate_p_tt(vector<process> &vec,int num_of_process);    // 计算带权周转时间
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
    vector<process> vec;
    vector<process> temp;
    int num_of_process;
    do{
        cout<<"Input number of processes (limit in 10)"<<endl;
        cin >> num_of_process;
        if (num_of_process < 1 || num_of_process > 10 )
        {
            cout<< "please input a valid number: \n"<<endl;
        }
        
    }while (num_of_process > 10 || num_of_process < 1);
    int i = 0;
    for (i = 0; i < num_of_process; i++)
    {
        cout << "please input number " << i + 1 << " process's arrive time and burst time"<<endl;
        int at;
        int bt;
        cin >> at >> bt;
        process p(i+1,at,bt); 
        vec.push_back(p);
        temp.push_back(p);
    }
    int option;
    do{
        cout << "1. FCFS  2.SJF  3.HRRN  4.clear 0.exit"<<endl;
        cin>>option;
        switch (option)
        {
        case 1:{
                sort(vec.begin(),vec.end(),compare);  ///按到达时间排序
                vec[0].finish_time = vec[0].burst_time;  //先到的单独处理
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
        case 2:{
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
            // 全部到达后计算响应比
            // 如果都到达再算的话,等待时间 = 最后一个的提交时间-该作业到达的时刻
            // 响应比为（等待时间+要求服务时间）\要求服务时间=等待时间/要求服务时间+1
            sort(vec.begin(),vec.end(),compare);
            int last_at = vec.back().arrive_time;
            int rr[num_of_process];
            for (int j = 0; j < num_of_process; j++)
            {
                vec[j].waiting_time = last_at - vec[j].arrive_time;
                vec[j].rr_t = vec[i].waiting_time / vec[j].burst_time + 1;
            }
            int left_process = num_of_process;
            int current_time = 0;
            int k = 0;
            while (left_process)
            {
                sort(vec.begin(),vec.end(),compare2);
                current_time = vec.back().finish_time = 0 + vec.back().burst_time;
                vec.back().rr_t = 0;
                left_process--;
                k++;
                for (int i = 0; i < num_of_process - k; i++)
                {
                    if (!vec[i].is_finshed)
                    {
                        vec[i].rr_t = vec[i].waiting_time / vec[i].burst_time + 1;
                    }
                }
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
        case 4:system("clear");
            break;
        default:
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
