#include<windows.h>
#include "todolist.h"
#include"account.h"
#include"reminder.h"
#include"calculator.h"
#include"TwentyFourGame.h"
#include"pomodoro.h"

int main()
{
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        TodolistManager todolistManager;
        account_manager accountManager;  
        reminder_ui reminderManager;
        Calculator calculatorManager;
        TwentyFourGame twentyfourgameManager;
        PomodoroManager pomodoroManager;

        while (true) {
            system("cls");
            cout << "=================================" << endl;
            cout << "      桌面助手" << endl;
            cout << "=================================" << endl;
            cout << "1. 账本" << endl;
            cout << "2. 备忘录" << endl;
            cout << "3. todolist" << endl;
            cout << "4. 多功能计算器" << endl;
            cout << "5. 24点游戏" << endl;
            cout << "6. 🍅 番茄钟专注系统" << endl;
            cout << "0. 退出程序" << endl;
            cout << "=================================" << endl;

            int choice=safeCin("\n请输入选项", 0, 6);

            switch (choice) {
            case 1:
                accountManager.Start();
                break;
            case 2:
                reminderManager.reminder_main_ui();
                break;
            case 3:
                todolistManager.start();
                break;
            case 4:
                calculatorManager.run();
                break;
            case 5:
                twentyfourgameManager.run();
                break;
            case 6:
                pomodoroManager.start();
                break;
            case 0:
                return 0;
            }


        }
    return 0;

}
