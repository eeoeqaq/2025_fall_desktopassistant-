#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <ctime>
#include <algorithm>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

using namespace std;

// 音效类型枚举
enum SoundType {
    SOUND_COMPLETION = 1,    // 完成番茄钟
    SOUND_ACHIEVEMENT = 2    // 解锁成就
};

// 番茄钟记录结构体
struct PomodoroRecord {
    int id;                     // 记录ID
    string taskName;            // 任务名称
    string startTime;           // 开始时间
    string endTime;             // 结束时间
    int duration;               // 持续时长(分钟)
    int interruptions;          // 干扰次数
    bool completed;             // 是否完成
    string date;                // 日期 (YYYY-MM-DD)
    
    PomodoroRecord() : id(0), duration(0), interruptions(0), completed(false) {}
};

// 成就结构体
struct Achievement {
    string name;                // 成就名称
    string description;         // 成就描述
    int requiredCount;          // 所需番茄钟数量
    bool unlocked;              // 是否已解锁
    
    Achievement(const string& n, const string& d, int req) 
        : name(n), description(d), requiredCount(req), unlocked(false) {}
};

// 番茄钟管理器类
class PomodoroManager {
private:
    vector<PomodoroRecord> records;     // 所有番茄钟记录
    vector<Achievement> achievements;   // 成就列表
    int nextId;                         // 下一个可用ID
    
    // 用户设置
    int focusDuration;          // 专注时长(分钟) 默认25
    int shortBreakDuration;     // 短休息时长(分钟) 默认5
    int longBreakDuration;      // 长休息时长(分钟) 默认15
    int pomodorosUntilLongBreak; // 长休息前的番茄钟数 默认4
    
    const string dataFile = "pomodoro_data.txt";
    const string achievementFile = "achievements_data.txt";
    
    // 数据管理
    void saveToFile();
    void loadFromFile();
    void saveAchievements();
    void loadAchievements();
    
    // 核心功能
    void startPomodoro();
    void viewStatistics();
    void viewAchievements();
    void viewHistory();
    void settings();
    void exportReport();
    
    // 辅助功能
    void checkAndUnlockAchievements();
    void displayProgressBar(int current, int total);
    void clearScreen();
    string formatTime(int seconds);
    string getCurrentDate();
    string getCurrentTime();
    void playSound(SoundType soundType); // 使用枚举类型
    int getTodayPomodoroCount();
    int getTotalPomodoroCount();
    void display7DaysTrend();
    
    // 初始化成就列表
    void initializeAchievements();

public:
    PomodoroManager();
    void start();  // 主菜单入口
};
