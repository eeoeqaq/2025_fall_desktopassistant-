#include "todolist.h"


using namespace std;

// 构造函数
Todo::Todo() : id(0), type(1), completed(false) {}

Todo::Todo(int id, const string& content, int type)
    : id(id), content(content), type(type), completed(false) {

}

// 获取成员变量
int Todo::getId() const {
    return id;
}

string Todo::getContent() const {
    return content;
}

int Todo::getType() const {
    return type;
}

bool Todo::isCompleted() const {
    return completed;
}

// 获取类型字符串
string Todo::getTypeString() const {
    switch (type) {
    case 1:
        return "重要紧急";
    case 2:
        return "重要不紧急";
    case 3:
        return "紧急不重要";
    case 4:
        return "不紧急不重要";
    }
}

// 设置成员变量
void Todo::setContent(const string& content) {
    this->content = content;
}

void Todo::setType(int type) {
        this->type = type;
    
}

void Todo::setCompleted(bool completed) {
    this->completed = completed;
}

// 显示事项信息
void Todo::display() const {
    cout << setw(6) << id
        << setw(30) << left
        << (content.length() > 28 ? content.substr(0, 28) + ".." : content)
        << setw(12) << getTypeString()
        << setw(4) << (completed ? "完成" : " ")
        << endl;
}