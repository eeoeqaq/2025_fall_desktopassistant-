#include"reminder.h"
using namespace std;

void diaries::write_diary(){
    int y,m,d;string new_content;
    cout<<"请输入日期（格式：yyyy mm dd）："<<endl;
    cin>>y;if(checkinput(y)==-1){return;}
    cin>>m;if(checkinput(m)==-1){return;}
    cin>>d;if(checkinput(d)==-1){return;}
    cout<<"请输入日记内容："<<endl;
    cin>>new_content;
    dairy_lib[date(y,m,d)].push_back(diary(date(y,m,d),new_content));
    cout<<"写入成功！"<<endl;
    save();
}

void diaries::fetch_diary(){
    string got_content;int y,m,d;
    cout<<"请输入日期以查看日记！格式：yyyy mm dd"<<endl;
    cin>>y;if(checkinput(y)==-1){return;}
    cin>>m;if(checkinput(m)==-1){return;}
    cin>>d;if(checkinput(d)==-1){return;}
    if(dairy_lib[date(y,m,d)].size()==0){
        cout<<"你这天没有写日记哦"<<endl;
    }else if (dairy_lib[date(y,m,d)].size()==1){
        cout<<"你这天写了 1 篇日记。正在为你取出："<<endl;
        cout<<dairy_lib[date(y,m,d)][0].get_content()<<endl;
    }else{
        cout<<"你在此天写了 "<<dairy_lib[date(y,m,d)].size()<<" 篇日记。请输入序号以查看某篇日记："<<endl;
        int order;cin>>order;if(checkinput(to_string(order))==-1){return;}
        while(order>=dairy_lib[date(y,m,d)].size()||order<-1){
            cout<<"没有这个编号的日记！"<<endl;
            cin>>order;if(checkinput(to_string(order))==-1){return;}
        }
        cout<<dairy_lib[date(y,m,d)][order].get_content()<<endl;
    }    
    save();
}

void diaries::change_diary(){
    string got_content;int y,m,d;
    cout<<"请输入日期以修改日记！格式：yyyy mm dd"<<endl;
    cin>>y;if(checkinput(y)==-1){return;}
    cin>>m;if(checkinput(m)==-1){return;}
    cin>>d;if(checkinput(d)==-1){return;}
    if(dairy_lib[date(y,m,d)].size()==0){
        cout<<"你这天没有写日记哦"<<endl;
    }else if (dairy_lib[date(y,m,d)].size()==1){
        cout<<"你这天写了 1 篇日记。正在为你取出："<<endl;
        cout<<dairy_lib[date(y,m,d)][0].get_content()<<endl;
        cout<<"请输入修改后的内容："<<endl;
        cin>>got_content;
        dairy_lib[date(y,m,d)][0]=diary(date(y,m,d),got_content);
        cout<<"修改完成！！"<<endl;
    }else{
        cout<<"你在此天写了 "<<dairy_lib[date(y,m,d)].size()<<" 篇日记。请输入序号以修改某篇日记："<<endl;
        int order;cin>>order;if(checkinput(to_string(order))==-1){return;}
        while(order>=dairy_lib[date(y,m,d)].size()||order<-1){
            cout<<"没有这个编号的日记！"<<endl;
            cin>>order;if(checkinput(to_string(order))==-1){return;}
        }
        cout<<dairy_lib[date(y,m,d)][order].get_content()<<endl;
        cout<<"请输入修改后的内容："<<endl;
        cin>>got_content;
        dairy_lib[date(y,m,d)][0]=diary(date(y,m,d),got_content);
    }    
    save();
}

void diaries::save(){
    fstream fs;
    fs.open(DIARY_FILE,ios::out);
    for (const auto &p : dairy_lib) {
        fs <<"date "<<to_string(p.first.get_year()) << ' ' <<to_string(p.first.get_month())<<' '
        <<to_string(p.first.get_day())<<"\n";
        for (diary x :p.second){
            fs<<"content "<<x.get_content()<<'\n';
        }
    }
    fs.close();
}

void diaries::load(){
    fstream fs;
    fs.open(DIARY_FILE,ios::in);
    string bf_str,bf1,bf2,bf3,state;
    stringstream bf_ss;
    date bf_day;
    //一次读一行
    while(getline(fs,bf_str)){
        bf_ss.str(bf_str);
        bf_ss.clear();  // 重置流状态标志
        bf_ss>>state;
        //用每行的第一个词,即标志词做区分
        if(state=="date"){
            bf_ss>>bf1>>bf2>>bf3;
            bf_day=date(stoi(bf1),stoi(bf2),stoi(bf3));
        }else if(state=="content"){
            bf_ss.ignore();
            getline(bf_ss, bf1);  
            dairy_lib[bf_day].push_back(diary(bf_day,bf1));       
        }else{
            cout<<"-1"<<endl;
            return;
        }
    }
    fs.close();
}


bool date::operator<(const date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }