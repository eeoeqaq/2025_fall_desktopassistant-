#include "reminder.h"
using namespace std;

// 说明：
// 本文件负责终端交互式 UI，包括主菜单、记事本、密码箱、日记本三个子菜单。
// 同时提供两个重载的输入检查函数 `checkinput`，用于统一处理用户输入中用 -1 终止流程的约定。
// 注意：UI 循环使用 `while(1)` 的无限循环，通过选择“退出”或在输入处输入 -1 来跳出。
// Windows 环境使用 `system("cls")` 清屏、`system("pause")` 暂停，若在其他平台请替换相应命令。

int checkinput(string a){
    try{
        if(stoi(a)==-1){
        return -1;
    }else{
        return 1;
    }
    }catch(invalid_argument){
        // 非数字字符串，无法转为 int，按普通输入处理（不是 -1）
        return 1;
    }catch(out_of_range){
    //输入了太长的int。肯定不是-1。
        return 1;
    }
}
int checkinput(int a){
    try{
            if(a==-1){
            return -1;
        }else{
            return 1;
        }
    }catch(...){
        // 兜底：任何异常都按有效输入处理
        return 1;
    }
}

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    istringstream iss(s);
    string item;
    while (getline(iss, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}

// 主菜单：提供三大模块入口与退出
// 交互路径：输入 1/2/3 进入子菜单，输入 4 退出程序；任意输入位置输入 -1 立即返回上一层。
void reminder_ui::reminder_main_ui(){
    while(1){
        system("cls");
    cout<<"########################"<<endl
        <<"#    欢迎使用备忘录！    #"<<endl
        <<"#    1.记事本           #"<<endl
        <<"#    2.日记本           #"<<endl
        <<"#    3.密码保险箱       #"<<endl
        <<"#    4.退出             #"<<endl
        <<"########################"<<endl
        <<"tips:您可以在任何输入部分输入“-1”以终止进程"<<endl;
    int x;cin>>x;if (checkinput(to_string(x))==-1){return;}
    // 根据用户选择分派到不同子模块
    switch(x){
        case 1:
        reminder_notepads_ui();
        break;
        case 2:
        reminder_diaries_ui();
        break;
        case 3:
        reminder_passwords_ui();   
        break;
        case 4:
        // 退出主循环
        return;
    }
    }
}

// 记事本子菜单：写、列、读、改、清空、退出
// 子菜单均为阻塞式流程：完成一次操作后通过 `system("pause")` 等待用户继续。
void reminder_ui::reminder_notepads_ui(){
    notepads a;
    while(1){
        system("cls");
    cout<<"########################"<<endl
        <<"#    欢迎使用记事本！    #"<<endl
        <<"#    1.写文章           #"<<endl
        <<"#    2.列出文章标题     #"<<endl
        <<"#    3.检索文章         #"<<endl
        <<"#    4.更改文章内容     #"<<endl
        <<"#    5.清空记事本       #"<<endl
        <<"#    6.退出             #"<<endl
        <<"########################"<<endl
        <<"tips:您可以在大部分输入部分输入“-1”以终止进程"<<endl;
        int x;cin>>x;if (checkinput(to_string(x))==-1){return;}
        switch(x){
            case 1:
            // 写文章：标题唯一，若重复则提示
            a.store_notepad();
            system("pause");
            break;
            case 2:
            // 列出所有现有标题
            a.list_title();
            system("pause");
            break;
            case 3:
            // 根据标题检索内容
            a.fetch_notepad();
            system("pause");
            break;
            case 4:
            // 根据标题覆盖原有内容
            a.change_notepad();
            system("pause");
            break;
            case 5:
            // 两次确认后清空所有条目
            a.delete_all();
            break;
            case 6:   
            // 返回主菜单
            return;        
        }
    }
}

// 密码箱子菜单：新增、更改、检索密码、修改 checkword、退出
// 注意：检索密码需输入正确的 `checkword`；可通过菜单 4 修改 `checkword`。
void reminder_ui::reminder_passwords_ui(){
    passwords a;
    while(1){
        system("cls");
    cout<<"########################"<<endl
        <<"#    欢迎使用密码箱！    #"<<endl
        <<"#    1.输入新密码       #"<<endl
        <<"#    2.更改密码         #"<<endl
        <<"#    3.检索密码         #"<<endl
        <<"#    4.更改checkword    #"<<endl
        <<"#    5.退出             #"<<endl
        <<"########################"<<endl
        <<"tips:您可以在大部分输入部分输入“-1”以终止进程"<<endl;
        int x;cin>>x;if (checkinput(to_string(x))==-1){return;}
        switch(x){
            case 1:
            // 新增密码条目（键为名称/网站）
            a.store_password();
            system("pause");
            break;
            case 2:
            // 更改已有条目密码，双重确认
            a.change_password();
            system("pause");
            break;
            case 3:
            // 输入名称与 checkword 后展示密码
            a.fetch_password();
            system("pause");
            break;
            case 4:
            // 修改校验词 checkword
            a.set_checkword();
            system("pause");
            break;
            case 5:   
            // 返回主菜单
            return;        
            break;
        }
    }
}

// 日记本子菜单：写、读、改、退出
// 写/读/改都按日期维度组织，多篇日记以序号区分。
void reminder_ui::reminder_diaries_ui(){
    diaries a;
    while (1){
        system("cls");
    cout<<"########################"<<endl
        <<"#    欢迎使用日记本！    #"<<endl
        <<"#    1.写日记           #"<<endl
        <<"#    2.读日记           #"<<endl
        <<"#    3.修改日记         #"<<endl
        <<"#    4.退出             #"<<endl
        <<"########################"<<endl
        <<"tips:您可以在大部分输入部分输入“-1”以终止进程"<<endl;
        int x;cin>>x;if (checkinput(to_string(x))==-1){return;}
        switch(x){
            case 1:
            // 写入指定日期的日记（可多篇）
            a.write_diary();
            system("pause");
            break;
            case 2:
            // 读取日记：无/单篇/多篇时提示不同逻辑
            a.fetch_diary();
            system("pause");
            break;
            case 3:
            // 修改日记：单篇直接替换，多篇按序号选择后替换
            a.change_diary();
            system("pause");
            break;
            case 4:   
            // 返回主菜单
            return;        
            break;
        }
    }
}
    