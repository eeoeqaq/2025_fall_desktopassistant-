#pragma once
//宏定义保护，防止重复依赖

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

// 1重要紧急 2重要不紧急 3紧急不重要 4不紧急不重要
     
// 安全输入函数声明
int safeCin(const string& prompt = "", int min=0, int max = numeric_limits<int>::max());

//单次事项类
class Todo {
private:
    int id;          // 序号
    string content;  // 事项内容
    int type;        // 类型:遵循四象限分类法(1-4)
    bool completed;  // 是否已完成

public:
    // 构造函数
    Todo();
    Todo(int id, const string& content, int type);

    // 获取和设置成员变量
    int getId() const;
    string getContent() const;
    int getType() const;
    bool isCompleted() const;
    string getTypeString() const;

    void setContent(const string& content);
    void setType(int type);
    void setCompleted(bool completed);

    // 显示事项信息
    void display() const;
};



//管理类
class TodolistManager {
private:
    vector<Todo> todos;           // 储存所有事项
    int nextId;                   // 下一个可用的ID
    const string dataFile = "todolist_data.txt"; // 数据文件

    // 私有辅助函数
    void saveToFile();            // 保存数据到文件
    void loadFromFile();          // 从文件加载数据
    void displayHeader() const;   // 显示表头
   
    int getChoice(int min, int max) const; // 安全获取选择

public:
    TodolistManager();

    // 核心功能
    void start();                 // 程序主循环
    void addTodo();               // 添加事项
    void deleteTodo();            // 删除事项
    void modifyTodo();            // 修改事项
    void markComplete();          // 标记完成/未完成
    void displayAll() const;      // 显示所有事项
    void displayByType(int type) const; // 按类型显示
    void displayQuadrants() const; // 显示四象限分类
    void displayStatistics() const; // 显示统计信息
    int getTodoCount() const;     // 获取事项总数
};

