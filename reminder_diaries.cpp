#include"reminder.h"
using namespace std;

void diaries::save(){
    fstream fs;
    fs.open(DIARY_FILE,ios::binary|ios::out);
    for (const auto &p : dairy_lib) {
        fs <<to_string(p.first.get_year()) << ' ' <<to_string(p.first.get_month())<<' '
        <<to_string(p.first.get_day())<<"\n";
        for (diary x :p.second){
            fs<<x.get_content()<<'\n';
        }
    }
    fs.close();
}

void diaries::load(){
    fstream fs;
    fs.open(DIARY_FILE,ios::binary|ios::in);
    while()
    fs.close();
}