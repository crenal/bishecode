#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#define a 2          //每个vm的大小
#define s_mount 3    //物理机s的数量
#define r_mount 40   //请求r的初始数量
#define Cs 10        //每个物理机s的大小
#define r_min 0.05    //每个请求r的最小值
#define r_max 0.55      //每个请求r的最大值
#define T 2          //系统容忍的最大延迟时间
#define beta 0.5    //随机增加请求概率
#define gama 0.5    //增加请求比例
#define deta 0.1     //随机减少请求概率
using namespace std;
//生产随机小数
float getrandom(float min,float max,default_random_engine::result_type seed = 0){
    static default_random_engine e(seed);
    static default_random_engine::result_type last_seed = seed;
    if (seed != last_seed){
        e = default_random_engine(seed);
        last_seed = seed;
    }
    uniform_real_distribution<float> u(min,max);
    return(u(e));
}
//生成随机整数
int getrandomint(int min,int max,default_random_engine::result_type seed = 0){
    static default_random_engine e(seed);
    static default_random_engine::result_type last_seed = seed;
    if (seed != last_seed){
        e = default_random_engine(seed);
        last_seed = seed;
    }
    uniform_int_distribution<int> u(min,max);
    return(u(e));
}
//物理机
class S{       
private:
    int C;     //物理机容量
    int n;     //最大容纳VM数量
    vector<bool> x; //x数组
public:
    S():C(Cs){
        n=C/a;
        x.resize(n);
    };
};
//请求
class R{ 
    friend class problemsovler; 
private:
    float f_r;//请求的大小
    float t_r;//请求更新需要的时间
    int ys;   //更新后分配的物理机号
    int yn;   //更新后分配的物理机上的vm号
    int bs;   //更新前上一个分配的物理机号
    int bn;   //更新前上一个分配的物理机上的vm号
public:
    R():t_r(0.3){
        //t_r需要考虑实际情况
        f_r=0;
    };

    //更新请求的大小（实际情况中是有数据采集的过程，监控请求的大小）
    void update(){
        //如何更新需要考虑实际情况
        f_r=getrandom(r_min,r_max,time(0));
    }
};
class problemsovler{
private:
    int num; //迭代次数
    vector<S> s; //物理机集合
    vector<R> r; //请求集合
    int m_vm=Cs/a; //vm数量
    int m_s=s_mount;//s数量
    int m_r=r_mount;//r数量
public:
    problemsovler(){
        num=0;
        s.resize(m_s);//初始化s数组
        r.resize(m_r);//初始化r数组
    }
    void show(){
        cout<<"第"<<num<<"次迭代："<<endl;
        cout<<"请求总数为"<<m_r<<endl;
        for(int i=0;i<m_r;i++){
            cout<<"请求"<<i<<"的大小为:"<<r[i].f_r<<",分配到"<<r[i].bs<<","<<r[i].bn<<endl;
        }
    }
    //为所有r随机分配vm
    void random_assign(){
        for(auto i=r.begin();i!=r.end();i++){
            i->bs=getrandomint(0,s_mount-1,time(0));
            i->bn=getrandomint(0,Cs/a-1,time(0));
            cout<<i->bs<<','<<i->bn<<endl;
        }
    }
    //更新请求的状态
    void update_all(){
        //先随机删除一些请求
        for(auto i=r.begin();i!=r.end();){
            if(getrandom(0,1,time(0))<deta){
                i=r.erase(i);
                m_r--;
            }
            else
                i++;
        }
        //随机增加一些请求
        if(getrandom(0,1,time(0))<beta){
            int m_join=r.size()*gama;
            r.resize(r.size()+m_join);
            //新的请求随机分配vm
            for(int i=m_r;i<r.size();i++){
                r[i].bs=getrandomint(0,s_mount-1,time(0));
                r[i].bn=getrandomint(0,Cs/a-1,time(0));
            }
        }
        //更新所有请求的状态
        for(auto i=r.begin();i!=r.end();i++){
            i->update();
        }
        m_r=r.size();
        num++; //迭代次数++
    }
    void solve(){ //主要的算法函数
        vector<vector<vector<bool>>> y;//y[1][2][3]表示请求1 物理机2 vm3
        vector<vector<vector<bool>>> b;
        vector<vector<bool>> x; //x[1][2]表示 物理机1 vm2
        //初始化y数组
        y.resize(m_r);
        for(int i=0;i<m_r;i++){
            y[i].resize(m_s);
            for(int j=0;j<m_s;j++){
                y[i][j].resize(m_vm);
            }
        }
        //初始化b数组
        b.resize(m_r);
        for(int i=0;i<m_r;i++){
            b[i].resize(m_s);
            for(int j=0;j<m_s;j++){
                b[i][j].resize(m_vm);
            }
        }
        //b数组赋值
        for(int i=0;i<m_r;i++){
            b[i][r[i].bs][r[i].bn]=1;
        }
        //初始化x数组
        x.resize(m_s);
        for(int j=0;j<m_s;j++){
            x[j].resize(m_vm);
        }
        //利用线性规划求解器求解
        
        //利用求解器的结果得到x与y  y的结果赋值到r数组中的ys与yn，x的结果赋值到s数组中的x

    }
};
int main(){
    problemsovler P;
    P.random_assign();
    P.show();
    P.update_all();
    P.show();
    P.update_all();
    P.show();
    P.update_all();
    P.show();
}