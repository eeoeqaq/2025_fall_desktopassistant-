#include"reminder.h"
using namespace std;

string passwords::checkword = "000000";

// 说明：
// 本文件实现密码模块，包括设置/校验 checkword、存储/更改/检索密码，
// 以及对密码字典与 checkword 的持久化保存与加载。
// 安全提醒：
// - 当前实现以明文存储密码与 checkword，仅用于练习/本地演示；生产环境需改为加密存储（如散列加盐、对称加密等）。
// - `checkword` 相当于全局“查看口令”，务必妥善保管并避免与密码相同。
// 文件保存格式（文本示例）：
//   000000
//   github myStrongPass
//   mail anotherPass
// 第一行是 checkword，后续每行是 `<name> <password>`。

void passwords::set_checkword(){
    cout<<"请输入旧checkword:"<<endl;
    string input;
    cin>>input;
    while(input!=checkword){
        if (checkinput(input)==-1){return;}
        cout<<"checkword错误！"<<endl;
        cin>>input;
    }
    cout<<"请输入新checkword："<<endl;
    string new_checkword1,new_checkword2;
    cin>>new_checkword1;if (checkinput(new_checkword1)==-1){return;}
    cout<<"请再次输入新checkword："<<endl;
    cin>>new_checkword2;if (checkinput(new_checkword2)==-1){return;}
    while(new_checkword1!=new_checkword2){
        cout<<"两次输入不符！！请再次输入："<<endl;
        cin>>new_checkword1;if (checkinput(new_checkword1)==-1){return;}
        cout<<"请再次输入新checkword："<<endl;
        cin>>new_checkword2;if (checkinput(new_checkword2)==-1){return;}
    }
    // 成功后更新并持久化
    checkword=new_checkword1;
    save();
}

void passwords::store_password(){
    cout<<"请输入您要注册的密码名/对应网站/"<<endl;
    string name;
    cin>>name;if (checkinput(name)==-1){return;}
    if(password_dic.count(name)){
        cout<<"您已注册该密码！请使用更改功能以更改密码。"<<endl;
   }else{
    cout<<"请输入密码。"<<endl;
    string new_password;
    cin>>new_password;if (checkinput(new_password)==-1){return;}
    // 将密码以键值对形式保存：name -> password 对象
    password_dic[name]=password(name,new_password);
    save();
    }
}

void passwords::change_password(){
    cout<<"请输入要更改的密码名称。"<<endl;
    string name;
    cin>>name;if (checkinput(name)==-1){return;}
    if(!password_dic.count(name)){
        cout<<"该密码不存在！"<<endl;
    }else{
        cout<<"请输入新密码："<<endl;
        string renewed_password1,renewed_password2;bool check=false;
        while(!check){
            cin>>renewed_password1;if (checkinput(renewed_password1)==-1){return;}
            cout<<"请再次输入新密码以确认："<<endl;
            cin>>renewed_password2;if (checkinput(renewed_password2)==-1){return;}
            if(renewed_password1==renewed_password2){
                check=true;
            }else{
                cout<<"两次输入密码不同！请再次输入："<<endl;
            }
        }
        // 双重确认后替换旧密码
        password_dic[name]=password(name,renewed_password1);
        cout<<"修改成功！"<<endl;
        save();
    }
}

void passwords::fetch_password(){
    cout<<"请输入你要读取的密码名称："<<endl;
    string name,checkword1;
    cin>>name;if (checkinput(name)==-1){return;}
    if(!password_dic.count(name)){cout<<"无此密码！";return;}
    cout<<"请输入checkword："<<endl;
    cin>>checkword1;if (checkinput(checkword1)==-1){return;}
    while(checkword1!=checkword){
        cout<<"checkword错误！！请重新输入："<<endl;
        cin>>checkword1;if (checkinput(checkword1)==-1){return;}
    }
    // 校验通过后显示密码内容
    cout<<"您所检索的密码是："<<password_dic[name].get_pw_content()<<endl;

}

void passwords::save(){
    fstream fs;
    fs.open(PASSWORD_FILE,ios::binary|ios::out);
    // 第一行保存 checkword，随后按行保存所有密码的键值对
    fs << checkword <<"\n";
    for (const auto &p : password_dic) {
        fs << p.first << " " << p.second.get_pw_content()<< "\n";
    }
    // 注意：此处为覆盖写入，不做追加；每次保存都会重写全部条目。
    fs.close();
}

void passwords::load(){
    fstream fs;
    fs.open(PASSWORD_FILE,ios::binary|ios::in);
    string nm,pw;
    // 先读出 checkword，再读密码字典
    fs>>checkword;
    while (fs>>nm>>pw){
        password_dic[nm]=password(nm,pw);
    }
    fs.close();
}