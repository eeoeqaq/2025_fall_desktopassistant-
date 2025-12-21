#include "pomodoro.h"
#include <map>

// 构造函数
PomodoroManager::PomodoroManager() 
    : nextId(1), focusDuration(25), shortBreakDuration(5), 
      longBreakDuration(15), pomodorosUntilLongBreak(4) {
    loadFromFile();
    initializeAchievements();
    loadAchievements();
}

// 清屏函数
void PomodoroManager::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 格式化时间显示
string PomodoroManager::formatTime(int seconds) {
    int mins = seconds / 60;
    int secs = seconds % 60;
    ostringstream oss;
    oss << setfill('0') << setw(2) << mins << ":" << setw(2) << secs;
    return oss.str();
}

// 获取当前日期
string PomodoroManager::getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    ostringstream oss;
    oss << (1900 + ltm->tm_year) << "-" 
        << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "-" 
        << setfill('0') << setw(2) << ltm->tm_mday;
    return oss.str();
}

// 获取当前时间
string PomodoroManager::getCurrentTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    ostringstream oss;
    oss << setfill('0') << setw(2) << ltm->tm_hour << ":" 
        << setfill('0') << setw(2) << ltm->tm_min << ":" 
        << setfill('0') << setw(2) << ltm->tm_sec;
    return oss.str();
}

// 播放音效
void PomodoroManager::playSound(int soundType) {
#ifdef _WIN32
    if (soundType == 1) {  // 完成番茄钟
        Beep(800, 200);
        this_thread::sleep_for(chrono::milliseconds(50));
        Beep(1000, 200);
        this_thread::sleep_for(chrono::milliseconds(50));
        Beep(1200, 300);
    } else if (soundType == 2) {  // 解锁成就
        Beep(800, 150);
        this_thread::sleep_for(chrono::milliseconds(50));
        Beep(1000, 150);
        this_thread::sleep_for(chrono::milliseconds(50));
        Beep(1200, 200);
    }
#endif
}

// 显示进度条
void PomodoroManager::displayProgressBar(int current, int total) {
    int barWidth = 50;
    float progress = (float)current / total;
    int pos = barWidth * progress;
    
    cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) cout << "█";
        else cout << "░";
    }
    cout << "] " << int(progress * 100.0) << "%\r";
    cout.flush();
}

// 初始化成就列表
void PomodoroManager::initializeAchievements() {
    achievements.clear();
    achievements.push_back(Achievement("🌱 初出茅庐", "完成第1个番茄钟", 1));
    achievements.push_back(Achievement("🔥 初见成效", "完成5个番茄钟", 5));
    achievements.push_back(Achievement("⭐ 专注新星", "完成10个番茄钟", 10));
    achievements.push_back(Achievement("💎 专注达人", "完成25个番茄钟", 25));
    achievements.push_back(Achievement("🏆 专注大师", "完成50个番茄钟", 50));
    achievements.push_back(Achievement("👑 专注王者", "完成100个番茄钟", 100));
    achievements.push_back(Achievement("🚀 专注传说", "完成200个番茄钟", 200));
}

// 保存记录到文件
void PomodoroManager::saveToFile() {
    ofstream file(dataFile);
    if (!file.is_open()) {
        cerr << "无法打开文件保存数据！" << endl;
        return;
    }
    
    for (const auto& record : records) {
        file << record.id << "|" 
             << record.taskName << "|"
             << record.startTime << "|"
             << record.endTime << "|"
             << record.duration << "|"
             << record.interruptions << "|"
             << (record.completed ? "1" : "0") << "|"
             << record.date << "\n";
    }
    
    file.close();
}

// 从文件加载记录
void PomodoroManager::loadFromFile() {
    ifstream file(dataFile);
    if (!file.is_open()) {
        return;  // 文件不存在，首次运行
    }
    
    records.clear();
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        istringstream iss(line);
        PomodoroRecord record;
        string completed;
        
        char delimiter;
        iss >> record.id >> delimiter;
        getline(iss, record.taskName, '|');
        getline(iss, record.startTime, '|');
        getline(iss, record.endTime, '|');
        iss >> record.duration >> delimiter 
            >> record.interruptions >> delimiter;
        getline(iss, completed, '|');
        record.completed = (completed == "1");
        getline(iss, record.date);
        
        records.push_back(record);
        
        if (record.id >= nextId) {
            nextId = record.id + 1;
        }
    }
    
    file.close();
}

// 保存成就到文件
void PomodoroManager::saveAchievements() {
    ofstream file(achievementFile);
    if (!file.is_open()) {
        cerr << "无法打开文件保存成就数据！" << endl;
        return;
    }
    
    for (const auto& achievement : achievements) {
        file << achievement.name << "|"
             << achievement.description << "|"
             << achievement.requiredCount << "|"
             << (achievement.unlocked ? "1" : "0") << "\n";
    }
    
    file.close();
}

// 从文件加载成就
void PomodoroManager::loadAchievements() {
    ifstream file(achievementFile);
    if (!file.is_open()) {
        return;  // 文件不存在，使用默认状态
    }
    
    map<string, bool> unlockedStatus;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        istringstream iss(line);
        string name, description, unlocked;
        int requiredCount;
        
        getline(iss, name, '|');
        getline(iss, description, '|');
        iss >> requiredCount;
        iss.ignore(1);  // 忽略分隔符
        getline(iss, unlocked);
        
        unlockedStatus[name] = (unlocked == "1");
    }
    
    // 更新成就解锁状态
    for (auto& achievement : achievements) {
        if (unlockedStatus.find(achievement.name) != unlockedStatus.end()) {
            achievement.unlocked = unlockedStatus[achievement.name];
        }
    }
    
    file.close();
}

// 获取今日番茄钟数量
int PomodoroManager::getTodayPomodoroCount() {
    string today = getCurrentDate();
    int count = 0;
    for (const auto& record : records) {
        if (record.date == today && record.completed) {
            count++;
        }
    }
    return count;
}

// 获取总番茄钟数量
int PomodoroManager::getTotalPomodoroCount() {
    int count = 0;
    for (const auto& record : records) {
        if (record.completed) {
            count++;
        }
    }
    return count;
}

// 检查并解锁成就
void PomodoroManager::checkAndUnlockAchievements() {
    int totalCompleted = getTotalPomodoroCount();
    
    for (auto& achievement : achievements) {
        if (!achievement.unlocked && totalCompleted >= achievement.requiredCount) {
            achievement.unlocked = true;
            
            cout << "\n";
            cout << "════════════════════════════════════════" << endl;
            cout << "         🎉 恭喜解锁新成就！" << endl;
            cout << "════════════════════════════════════════" << endl;
            cout << "\n  " << achievement.name << endl;
            cout << "  " << achievement.description << endl;
            cout << "\n════════════════════════════════════════" << endl;
            
            playSound(2);
            saveAchievements();
            
            this_thread::sleep_for(chrono::seconds(3));
        }
    }
}

// 开始番茄钟
void PomodoroManager::startPomodoro() {
    clearScreen();
    cout << "════════════════════════════════════════" << endl;
    cout << "       🍅 开始新番茄钟" << endl;
    cout << "════════════════════════════════════════" << endl;
    
    cout << "\n请输入任务名称 (直接回车跳过): ";
    string taskName;
    getline(cin, taskName);
    if (taskName.empty()) {
        taskName = "未命名任务";
    }
    
    clearScreen();
    cout << "════════════════════════════════════════" << endl;
    cout << "       🍅 专注模式 - " << taskName << endl;
    cout << "════════════════════════════════════════" << endl;
    cout << "\n专注时长: " << focusDuration << " 分钟" << endl;
    cout << "今日已完成: " << getTodayPomodoroCount() << " 个番茄钟" << endl;
    cout << "\n提示: 按 'p' 暂停 | 按 'i' 记录干扰 | 按 'q' 放弃\n" << endl;
    
    PomodoroRecord record;
    record.id = nextId++;
    record.taskName = taskName;
    record.startTime = getCurrentTime();
    record.date = getCurrentDate();
    record.interruptions = 0;
    record.duration = focusDuration;
    
    int totalSeconds = focusDuration * 60;
    int elapsedSeconds = 0;
    bool paused = false;
    bool abandoned = false;
    
    auto startTime = chrono::steady_clock::now();
    auto pauseStartTime = chrono::steady_clock::now();
    int totalPausedSeconds = 0;
    
    while (elapsedSeconds < totalSeconds && !abandoned) {
        if (!paused) {
            auto currentTime = chrono::steady_clock::now();
            auto elapsed = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
            elapsedSeconds = elapsed - totalPausedSeconds;
            
            if (elapsedSeconds > totalSeconds) {
                elapsedSeconds = totalSeconds;
            }
        }
        
        int remainingSeconds = totalSeconds - elapsedSeconds;
        
        // 显示倒计时和进度条
        cout << "⏱️ 剩余时间: " << formatTime(remainingSeconds) << "  ";
        displayProgressBar(elapsedSeconds, totalSeconds);
        
        if (paused) {
            cout << "  [已暂停]";
        }
        
        // 检测按键
#ifdef _WIN32
        if (_kbhit()) {
            char key = _getch();
            if (key == 'p' || key == 'P') {
                if (paused) {
                    // 继续
                    auto currentTime = chrono::steady_clock::now();
                    totalPausedSeconds += chrono::duration_cast<chrono::seconds>(
                        currentTime - pauseStartTime).count();
                    paused = false;
                } else {
                    // 暂停
                    pauseStartTime = chrono::steady_clock::now();
                    paused = true;
                }
            } else if (key == 'i' || key == 'I') {
                if (!paused) {
                    record.interruptions++;
                    cout << "\n  ✓ 已记录干扰 (共 " << record.interruptions << " 次)\n";
                    this_thread::sleep_for(chrono::milliseconds(500));
                }
            } else if (key == 'q' || key == 'Q') {
                cout << "\n\n确认放弃此番茄钟？(y/n): ";
                char confirm;
                cin >> confirm;
                cin.ignore();
                if (confirm == 'y' || confirm == 'Y') {
                    abandoned = true;
                }
            }
        }
#endif
        
        if (!abandoned) {
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }
    
    record.endTime = getCurrentTime();
    record.completed = !abandoned;
    
    if (abandoned) {
        clearScreen();
        cout << "════════════════════════════════════════" << endl;
        cout << "         ❌ 番茄钟已放弃" << endl;
        cout << "════════════════════════════════════════" << endl;
        cout << "\n任务: " << taskName << endl;
        cout << "已专注: " << elapsedSeconds / 60 << " 分钟" << endl;
        cout << "干扰次数: " << record.interruptions << " 次" << endl;
    } else {
        clearScreen();
        cout << "════════════════════════════════════════" << endl;
        cout << "         ✅ 番茄钟完成！" << endl;
        cout << "════════════════════════════════════════" << endl;
        cout << "\n任务: " << taskName << endl;
        cout << "专注时长: " << focusDuration << " 分钟" << endl;
        cout << "干扰次数: " << record.interruptions << " 次" << endl;
        cout << "\n今日已完成: " << (getTodayPomodoroCount() + 1) << " 个番茄钟" << endl;
        
        playSound(1);
    }
    
    records.push_back(record);
    saveToFile();
    
    if (record.completed) {
        checkAndUnlockAchievements();
        
        // 计算应该休息的类型
        int completedToday = getTodayPomodoroCount();
        bool isLongBreak = (completedToday % pomodorosUntilLongBreak == 0);
        int breakDuration = isLongBreak ? longBreakDuration : shortBreakDuration;
        
        cout << "\n⏰ 进入" << (isLongBreak ? "长" : "短") 
             << "休息 (" << breakDuration << "分钟)" << endl;
        cout << "按任意键跳过休息...";
        
        // 简单的休息倒计时
        int breakSeconds = breakDuration * 60;
        for (int i = 0; i < breakSeconds; i++) {
#ifdef _WIN32
            if (_kbhit()) {
                _getch();
                break;
            }
#endif
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
    
    cout << "\n\n按任意键返回主菜单...";
    cin.get();
}

// 显示7天趋势
void PomodoroManager::display7DaysTrend() {
    map<string, int> dailyCount;
    
    // 统计每天的番茄钟数量
    for (const auto& record : records) {
        if (record.completed) {
            dailyCount[record.date]++;
        }
    }
    
    // 获取最近7天的日期
    vector<string> last7Days;
    time_t now = time(0);
    for (int i = 6; i >= 0; i--) {
        time_t dayTime = now - (i * 24 * 60 * 60);
        tm* ltm = localtime(&dayTime);
        ostringstream oss;
        oss << (1900 + ltm->tm_year) << "-" 
            << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "-" 
            << setfill('0') << setw(2) << ltm->tm_mday;
        last7Days.push_back(oss.str());
    }
    
    // 找出最大值以调整显示
    int maxCount = 1;
    for (const auto& day : last7Days) {
        if (dailyCount[day] > maxCount) {
            maxCount = dailyCount[day];
        }
    }
    
    cout << "\n📈 最近7天趋势:\n" << endl;
    for (const auto& day : last7Days) {
        int count = dailyCount[day];
        cout << "  " << day << " ";
        
        int barLength = (maxCount > 0) ? (count * 30 / maxCount) : 0;
        for (int i = 0; i < barLength; i++) {
            cout << "█";
        }
        cout << " " << count << "个" << endl;
    }
}

// 查看统计
void PomodoroManager::viewStatistics() {
    clearScreen();
    cout << "════════════════════════════════════════" << endl;
    cout << "       📊 专注统计" << endl;
    cout << "════════════════════════════════════════" << endl;
    
    int totalCompleted = getTotalPomodoroCount();
    int todayCompleted = getTodayPomodoroCount();
    
    // 计算总专注时长
    int totalMinutes = 0;
    int totalInterruptions = 0;
    for (const auto& record : records) {
        if (record.completed) {
            totalMinutes += record.duration;
            totalInterruptions += record.interruptions;
        }
    }
    
    cout << "\n📌 总体统计:" << endl;
    cout << "  总番茄钟数: " << totalCompleted << " 个" << endl;
    cout << "  总专注时长: " << totalMinutes << " 分钟 (" 
         << (totalMinutes / 60) << " 小时 " << (totalMinutes % 60) << " 分钟)" << endl;
    cout << "  总干扰次数: " << totalInterruptions << " 次" << endl;
    if (totalCompleted > 0) {
        cout << "  平均干扰: " << fixed << setprecision(1) 
             << (double)totalInterruptions / totalCompleted << " 次/番茄钟" << endl;
    }
    
    cout << "\n📅 今日统计:" << endl;
    cout << "  今日番茄钟: " << todayCompleted << " 个" << endl;
    
    int todayMinutes = 0;
    string today = getCurrentDate();
    for (const auto& record : records) {
        if (record.date == today && record.completed) {
            todayMinutes += record.duration;
        }
    }
    cout << "  今日专注: " << todayMinutes << " 分钟" << endl;
    
    display7DaysTrend();
    
    cout << "\n════════════════════════════════════════" << endl;
    cout << "\n按任意键返回...";
    cin.get();
}

// 查看成就
void PomodoroManager::viewAchievements() {
    clearScreen();
    cout << "════════════════════════════════════════" << endl;
    cout << "       🏆 成就系统" << endl;
    cout << "════════════════════════════════════════" << endl;
    
    int unlockedCount = 0;
    for (const auto& achievement : achievements) {
        if (achievement.unlocked) {
            unlockedCount++;
        }
    }
    
    cout << "\n已解锁: " << unlockedCount << " / " << achievements.size() << endl;
    cout << "\n────────────────────────────────────────" << endl;
    
    int totalCompleted = getTotalPomodoroCount();
    
    for (const auto& achievement : achievements) {
        if (achievement.unlocked) {
            cout << "✅ ";
        } else {
            cout << "🔒 ";
        }
        cout << achievement.name << " - " << achievement.description << endl;
        cout << "   进度: " << min(totalCompleted, achievement.requiredCount) 
             << " / " << achievement.requiredCount << endl;
        cout << endl;
    }
    
    cout << "════════════════════════════════════════" << endl;
    cout << "\n按任意键返回...";
    cin.get();
}

// 查看历史记录
void PomodoroManager::viewHistory() {
    clearScreen();
    cout << "════════════════════════════════════════" << endl;
    cout << "       📜 历史记录" << endl;
    cout << "════════════════════════════════════════" << endl;
    
    if (records.empty()) {
        cout << "\n暂无历史记录" << endl;
    } else {
        // 倒序显示最近20条
        int count = 0;
        for (auto it = records.rbegin(); it != records.rend() && count < 20; ++it, ++count) {
            cout << "\n────────────────────────────────────────" << endl;
            cout << "ID: " << it->id << " | " << it->date << " " << it->startTime << endl;
            cout << "任务: " << it->taskName << endl;
            cout << "时长: " << it->duration << " 分钟 | 干扰: " << it->interruptions << " 次" << endl;
            cout << "状态: " << (it->completed ? "✅ 已完成" : "❌ 未完成") << endl;
        }
    }
    
    cout << "\n════════════════════════════════════════" << endl;
    cout << "\n按任意键返回...";
    cin.get();
}

// 设置
void PomodoroManager::settings() {
    clearScreen();
    cout << "════════════════════════════════════════" << endl;
    cout << "       ⚙️ 设置" << endl;
    cout << "════════════════════════════════════════" << endl;
    
    cout << "\n当前设置:" << endl;
    cout << "  1. 专注时长: " << focusDuration << " 分钟" << endl;
    cout << "  2. 短休息时长: " << shortBreakDuration << " 分钟" << endl;
    cout << "  3. 长休息时长: " << longBreakDuration << " 分钟" << endl;
    cout << "  4. 长休息前番茄钟数: " << pomodorosUntilLongBreak << " 个" << endl;
    cout << "  0. 返回" << endl;
    
    cout << "\n请选择要修改的设置: ";
    int choice = safeCin("", 0, 4);
    
    switch (choice) {
        case 1:
            focusDuration = safeCin("\n请输入专注时长 (1-60分钟)", 1, 60);
            cout << "✓ 已更新专注时长为 " << focusDuration << " 分钟" << endl;
            break;
        case 2:
            shortBreakDuration = safeCin("\n请输入短休息时长 (1-30分钟)", 1, 30);
            cout << "✓ 已更新短休息时长为 " << shortBreakDuration << " 分钟" << endl;
            break;
        case 3:
            longBreakDuration = safeCin("\n请输入长休息时长 (1-60分钟)", 1, 60);
            cout << "✓ 已更新长休息时长为 " << longBreakDuration << " 分钟" << endl;
            break;
        case 4:
            pomodorosUntilLongBreak = safeCin("\n请输入长休息前番茄钟数 (2-10个)", 2, 10);
            cout << "✓ 已更新长休息前番茄钟数为 " << pomodorosUntilLongBreak << " 个" << endl;
            break;
    }
    
    if (choice != 0) {
        this_thread::sleep_for(chrono::seconds(2));
    }
}

// 导出报告
void PomodoroManager::exportReport() {
    clearScreen();
    cout << "════════════════════════════════════════" << endl;
    cout << "       📄 导出报告" << endl;
    cout << "════════════════════════════════════════" << endl;
    
    string filename = "pomodoro_report_" + getCurrentDate() + ".txt";
    ofstream file(filename);
    
    if (!file.is_open()) {
        cout << "\n❌ 无法创建报告文件！" << endl;
        cout << "\n按任意键返回...";
        cin.get();
        return;
    }
    
    file << "════════════════════════════════════════\n";
    file << "       🍅 番茄钟专注系统报告\n";
    file << "════════════════════════════════════════\n";
    file << "\n导出日期: " << getCurrentDate() << " " << getCurrentTime() << "\n";
    
    // 总体统计
    int totalCompleted = getTotalPomodoroCount();
    int totalMinutes = 0;
    int totalInterruptions = 0;
    for (const auto& record : records) {
        if (record.completed) {
            totalMinutes += record.duration;
            totalInterruptions += record.interruptions;
        }
    }
    
    file << "\n📌 总体统计:\n";
    file << "  总番茄钟数: " << totalCompleted << " 个\n";
    file << "  总专注时长: " << totalMinutes << " 分钟 (" 
         << (totalMinutes / 60) << " 小时 " << (totalMinutes % 60) << " 分钟)\n";
    file << "  总干扰次数: " << totalInterruptions << " 次\n";
    if (totalCompleted > 0) {
        file << "  平均干扰: " << fixed << setprecision(1) 
             << (double)totalInterruptions / totalCompleted << " 次/番茄钟\n";
    }
    
    // 详细记录
    file << "\n📜 详细记录 (最近50条):\n";
    file << "────────────────────────────────────────\n";
    
    int count = 0;
    for (auto it = records.rbegin(); it != records.rend() && count < 50; ++it, ++count) {
        file << "\nID: " << it->id << " | " << it->date << " " << it->startTime << "\n";
        file << "任务: " << it->taskName << "\n";
        file << "时长: " << it->duration << " 分钟 | 干扰: " << it->interruptions << " 次\n";
        file << "状态: " << (it->completed ? "已完成" : "未完成") << "\n";
    }
    
    file << "\n════════════════════════════════════════\n";
    file.close();
    
    cout << "\n✓ 报告已导出: " << filename << endl;
    cout << "\n按任意键返回...";
    cin.get();
}

// 主菜单
void PomodoroManager::start() {
    while (true) {
        clearScreen();
        cout << "════════════════════════════════════════" << endl;
        cout << "       🍅 番茄钟专注系统" << endl;
        cout << "════════════════════════════════════════" << endl;
        
        cout << "\n📊 快速概览:" << endl;
        cout << "  今日: " << getTodayPomodoroCount() << " 个 | 总计: " 
             << getTotalPomodoroCount() << " 个" << endl;
        
        cout << "\n────────────────────────────────────────" << endl;
        cout << "  1. 🍅 开始番茄钟" << endl;
        cout << "  2. 📊 查看统计" << endl;
        cout << "  3. 🏆 查看成就" << endl;
        cout << "  4. 📜 历史记录" << endl;
        cout << "  5. ⚙️ 设置" << endl;
        cout << "  6. 📄 导出报告" << endl;
        cout << "  0. 🚪 返回主菜单" << endl;
        cout << "────────────────────────────────────────" << endl;
        
        int choice = safeCin("\n请输入选项", 0, 6);
        
        // 清空输入缓冲区
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                startPomodoro();
                break;
            case 2:
                viewStatistics();
                break;
            case 3:
                viewAchievements();
                break;
            case 4:
                viewHistory();
                break;
            case 5:
                settings();
                break;
            case 6:
                exportReport();
                break;
            case 0:
                return;
        }
    }
}
