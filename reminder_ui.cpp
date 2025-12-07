#include "reminder.h"
using namespace std;

int checkinput(string a){
    if(stoi(a)==-1){
        return -1;
    }else{
        return 1;
    }
}

void reminder_ui::reminder_main_ui(){
    while(1){
        system("cls");
        cout<<"########################"<<endl
        <<"#    欢迎使用备忘录！    #"<<endl
        <<"#    1."<<endl
        <<"#    2."<<endl
        <<"#    3.密码保险箱"<<endl
        <<"#    4.退出             #"<<endl
        <<"########################"<<endl
        <<"tips:您可以在任何输入部分输入“-1”以终止进程"<<endl;
    int x;cin>>x;if (checkinput(to_string(x))==-1){return;}
    switch(x){
        case 3:
        reminder_passwords_ui();   
        break;
        case 4:
        return;
    }
    }
}

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
        <<"tips:您可以在任何输入部分输入“-1”以终止进程"<<endl;
    int x;cin>>x;if (checkinput(to_string(x))==-1){return;}
    switch(x){
        case 1:
        a.store_password();
        system("pause");
        break;
        case 2:
        a.change_password();
        system("pause");
        break;
        case 3:
        a.fetch_password();
        system("pause");
        break;
        case 4:
        a.set_checkword();
        system("pause");
        break;
        case 5:   
        return;        
        break;
    }
    }
}
