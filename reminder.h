#pragma once
#include<map>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
using namespace std;
#define PASSWORD_FILE "password_dic.txt"
#define DIARY_FILE "diary_dic.txt"

//在本部分实现备忘录功能
//我们希望：按标题区分不同备忘录，在这些二进制文件中写内容
//为用户列出所有文章标题，用户选择后读取
class password;
class passwords;
//给出用户打开备忘录的方法
// enum create_options{
//     read_only,
//     read_and_write,
//     administrator,
//     blank
// };

//帮助检测用户输入的辅助函数，待拓展。
int checkinput(string a);
int checkinput(int a);
//diaries部分用到的string.split。
vector<string> split(const string& s, char delimiter = ' ');

//一个用于模拟备忘录部分的交互、UI、顶层设计
class reminder_ui{
    //create_options my_option=blank;
    //passwords current_passwords;
    public:
    //创建一个备忘录进程
    reminder_ui(){this->reminder_main_ui();}
    //打印ui，读入用户操作并进入下一级ui
    void reminder_main_ui();
    //直接搜索某个文件是否存在
    //void global_search();
    //passwords的二级ui
    void reminder_passwords_ui();
    void reminder_diaries_ui();
};


//一个用于模拟所有已储存密码的集合类
class passwords {
    static string checkword;
    map <string,password> password_dic={};
    public:
    passwords(){this->load();}
    void set_checkword();
    void store_password();
    void change_password();
    void fetch_password();
    void save();
    void load();
};


//一个用于模拟单个密码的类
class password {
    string web_or_name;
    string password_content;
    public:
    password()=default;
    password(string a,string b):web_or_name(a),password_content(b){};
    inline string get_name () const{return web_or_name;};
    inline string get_pw_content() const{return password_content;};
};

//一个用于模拟日期的类
class date{
    int year,month,day;
    public:
    date()=default;
    date(int ny,int nm, int nd):year(ny),month(nm),day(nd){};
    //为了作为map的key，需要重载'<'
    bool operator<(const date& other) const ;
    //一些用于取值的函数
    int get_year()const {return year;}
    int get_month()const {return month;}
    int get_day()const {return day;}

};

//一个用于模拟单篇日记的类
class diary{
    date someday;
    string content;
    public:
    diary()=default;
    diary(date a,string b):someday(a),content(b){};
    date get_date(){return someday;}
    string get_content(){return content;}
    string get_content()const{return content;}
};

//一个用于模拟日记本的类
class diaries{
    public:
    map <date,vector<diary>> dairy_lib;
    void write_diary();
    void fetch_diary();
    void change_diary();
    void save();
    void load();
};


