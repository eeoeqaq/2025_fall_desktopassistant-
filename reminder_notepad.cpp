#include"reminder.h"
using namespace std;



void notepads::store_notepad(){
    cout<<"请输入您的本篇记事本标题："<<endl;
    string name;
    cin>>name;if (checkinput(name)==-1){return;}
    if(notepad_dic.count(name)){
        cout<<"已有同名记事本。请换一个名字！"<<endl;
   }else{
    cout<<"在此输入记事本内容："<<endl;
    string new_notepad;
    cin>>new_notepad;
    notepad_dic[name]=notepad(name,new_notepad);
    save();
    }
}

void notepads::change_notepad(){
    cout<<"请输入要更改的记事本的标题。"<<endl;
    string name;
    cin>>name;if (checkinput(name)==-1){return;}
    if(!notepad_dic.count(name)){
        cout<<"该文章不存在！"<<endl;
    }else{
        cout<<"请输入新内容："<<endl;
        string renewed_notepad1;
        cin>>renewed_notepad1;
        notepad_dic[name]=notepad(name,renewed_notepad1);
        cout<<"修改成功！"<<endl;
        save();
    }
}

void notepads::fetch_notepad(){
    cout<<"请输入你要读取的文章名称："<<endl;
    string name;
    cin>>name;if (checkinput(name)==-1){return;}
    if(!notepad_dic.count(name)){cout<<"没有这个标题的文章"<<endl;return;}
    cout<<"您所检索记事本内容："<<notepad_dic[name].get_pw_content()<<endl;

}

void notepads::list_title(){
    cout<<"以下为现有的所有记事本标题："<<endl;
    for(auto key:notepad_dic){
        cout<<left<<setw(20)<<key.first<<endl;
    }
}

void notepads::delete_all(){
    string ans;
    cout<<"你真的要清空记事本吗？输入-1以退出！"<<endl;
    cin>>ans;
    if (checkinput(ans)==-1){return;}
    cout<<"你真的真的真的要清空记事本吗？输入-1以退出！"<<endl;
    cin>>ans;
    if (checkinput(ans)==-1){return;}
    notepad_dic.clear();
    cout<<"清除完成！"<<endl;
}

void notepads::save(){
    fstream fs;
    fs.open(NOTEPAD_FILE,ios::binary|ios::out);
    for (const auto &p : notepad_dic) {
        fs << p.first << " " << p.second.get_pw_content()<< "\n";
    }
    fs.close();
}

void notepads::load(){
    fstream fs;
    fs.open(NOTEPAD_FILE,ios::binary|ios::in);
    string nm,pw;
    while (fs>>nm>>pw){
        notepad_dic[nm]=notepad(nm,pw);
    }
    fs.close();
}