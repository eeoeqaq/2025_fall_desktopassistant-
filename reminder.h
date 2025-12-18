#pragma once
#include<map>
#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<sstream>
#include<vector>
#include<limits>
using namespace std;
#define PASSWORD_FILE "password_dic.txt"
#define DIARY_FILE "diary_dic.txt"
#define NOTEPAD_FILE "notepad_dic.txt"

// 说明：
// 本头文件声明了备忘录系统的所有核心类型与接口，包括：
// - 交互式 UI（`reminder_ui`）
// - 模块：记事本（`notepads`/`notepad`）、密码箱（`passwords`/`password`）、日记本（`diaries`/`diary`）
// - 日期类型 `date`（可作为 map 的键）与输入辅助函数。
// 数据持久化：
// - 记事本：`NOTEPAD_FILE`，按行保存“标题 内容”
// - 密码箱：`PASSWORD_FILE`，首行保存 `checkword`，其余按行保存“名称 密码”
// - 日记本：`DIARY_FILE`，按块保存 `date y m d` 与紧随的多行 `content 文本`
// 约定：
// - 交互中用户输入 -1 表示终止当前流程（由 `checkinput` 统一处理）。
// - `passwords` 与 `notepads` 构造时自动从文件加载（`load`），`save` 覆盖写入当前全部数据。

//在本部分实现备忘录功能
//本部分预计有三个功能；密码保险箱，日记本，记事本
//各部分声明在本文件完成，具体函数实现在其它cpp文件
class password;
class passwords;
class notepad;
class notepads;
class diary;
class diaries;
//给出用户打开备忘录的方法
// enum create_options{
//     read_only,
//     read_and_write,
//     administrator,
//     blank
// };

//帮助检测用户输入的辅助函数，待拓展。
// 行为约定：若传入值等于 -1（字符串将尝试转换为 int），返回 -1；否则返回 1。
// 该约定用于在交互流程中快捷地终止当前步骤或返回上一层。
int checkinput(string a);
int checkinput(int a);
//diaries部分用到的string.split辅助函数。
// 将字符串按分隔符拆分为 tokens，默认以空格分隔。
vector<string> split(const string& s, char delimiter = ' ');

//一个用于模拟备忘录部分的交互、UI、顶层设计的类
class reminder_ui{
    //create_options my_option=blank;
    //passwords current_passwords;
    public:
    //创建一个备忘录进程
    // 调用唯一的成员函数时进入主 UI 循环（阻塞式），通过菜单选择进入各子模块或退出。
    //打印ui，读入用户操作并进入下一级ui
    void reminder_main_ui();
    //直接搜索某个文件是否存在
    //void global_search();
    //passwords的二级ui
    // 记事本子菜单（写/列/读/改/清空/退出）
    void reminder_notepads_ui();
    // 密码箱子菜单（新增/更改/检索/更改 checkword/退出）
    void reminder_passwords_ui();
    // 日记本子菜单（写/读/改/退出）
    void reminder_diaries_ui();
};

//记事本的集合类
class notepads {
    map <string,notepad> notepad_dic={};
    public:
    // 构造时从 `NOTEPAD_FILE` 读取所有条目到 `notepad_dic`
    notepads(){this->load();}
    // 新增一篇记事本（标题唯一），若存在同名则提示并不覆盖
    void store_notepad();
    // 根据标题覆盖写入新内容
    void change_notepad();
    // 根据标题检索并输出内容
    void fetch_notepad();
    // 列出所有已有标题（不输出内容）
    void list_title();
    // 两次确认后清空所有条目
    void delete_all();
    //保存整个notepad_dic到txt文件
    // 覆盖写入：每行“标题 空格 内容\n”
    void save();
    //读取整个notepad_dic
    // 按行读取填充 `notepad_dic`，遇到重复标题将覆盖为最后读到的内容
    void load();
};

//一个用于模拟单个记事本的类
class notepad {
    string title;
    string notepad_content;
    public:
    notepad()=default;
    notepad(string a,string b):title(a),notepad_content(b){};
    inline string get_name () const{return title;};
    inline string get_pw_content() const{return notepad_content;};
};

//一个用于模拟所有已储存密码的集合类
class passwords {
    static string checkword;
    map <string,password> password_dic={};
    public:
    // 构造时从 `PASSWORD_FILE` 读取：首行 checkword，后续为 `<name> <password>`
    passwords(){this->load();}
    // 修改校验词（需旧校验词验证与两次新输入一致）
    void set_checkword();
    // 新增密码条目（键为名称/网站），若存在同名则提示
    void store_password();
    // 更改已有条目密码（双重确认）
    void change_password();
    // 输入名称与正确的 checkword 后输出密码内容
    void fetch_password();
    //保存整个password_dic到txt文件
    // 第一行写入 checkword，随后逐行写入“名称 空格 密码\n”，覆盖写入
    void save();
    //读取整个password_dic
    // 先读取 checkword，再逐行读取键值对到 `password_dic`
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
    // 按 year、month、day 的字典序比较；用于 `map<date, ...>` 的键排序
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
    // 获取日期与内容（同时提供 const 版本）
    date get_date(){return someday;}
    string get_content(){return content;}
    string get_content()const{return content;}
};

//一个用于模拟日记本的类
class diaries{
    public:
    map <date,vector<diary>> dairy_lib;
    // 写入指定日期的日记（同一天可多篇，按 vector 追加）
    void write_diary();
    // 读取指定日期的日记：无/单篇/多篇（需用户选择序号）
    void fetch_diary();
    // 修改指定日期的日记：单篇直接替换，多篇按序号选择后替换
    void change_diary();
    // 保存到 `DIARY_FILE`：文本格式，`date y m d` + 多行 `content 文本`
    void save();
    // 从 `DIARY_FILE` 加载：遇到 `date` 切换当前日期，随后每行 `content` 归属于最近日期
    void load();
};



