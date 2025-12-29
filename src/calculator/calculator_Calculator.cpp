#include"calculator.h"
// 获取当前时间字符串
string Calculator::getCurrentTime() {
    time_t now = time(0);
    char buf[80];
    // 使用 localtime_s 替代 localtime 以避免 C4996 警告
    struct tm timeinfo;
#ifdef _WIN32
    localtime_s(&timeinfo, &now);
#else
    localtime_r(&now, &timeinfo);
#endif
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return string(buf);
}

// 显示帮助信息
void Calculator::showHelp() {
    cout << "\n========== 计算器使用教程 ==========\n";
    cout << "支持的运算符和函数:\n";
    cout << "  +  加法       例: 5 + 3\n";
    cout << "  -  减法       例: 10 - 4\n";
    cout << "  *  乘法       例: 6 * 7\n";
    cout << "  /  除法       例: 15 / 3\n";
    cout << "  ^  幂运算     例: 2 ^ 3 (2的3次方)\n";
    cout << "  %  取模       例: 10 % 3\n";
    cout << "  abs() 绝对值  例: abs(-5) 或 abs(3-10)\n";

    cout << "\n复杂表达式示例:\n";
    cout << "  1. 混合运算: (3 + 5) * 2 - 8 / 4\n";
    cout << "  2. 幂运算: 2 ^ 3 ^ 2 (注意: 从右向左计算)\n";
    cout << "  3. 带函数: abs(-5 * 2) + 3 ^ 2\n";
    cout << "  4. 小数运算: 3.14 * 2.5 + 1.2\n";
    cout << "  5. 科学计数法: 1.2e3 + 3.5e2 (1200 + 350)\n";

    cout << "\n注意事项:\n";
    cout << "  1. 使用括号改变运算优先级\n";
    cout << "  2. 幂运算是右结合的: 2^3^2 = 2^(3^2) = 2^9 = 512\n";
    cout << "  3. abs()函数可以嵌套: abs(abs(-5) - 10)\n";
    cout << "  4. 空格是可选的，但建议使用以提高可读性\n";
    cout << "====================================\n";
}




//// 计算表达式
//void Calculator::calculateExpression() {
//    string expression;
//    cout << "\n请输入表达式 (输入 'help' 查看教程，'back' 返回):\n> ";
//    getline(cin, expression);
//
//    if (expression == "back") return;
//    if (expression == "help") {
//        showHelp();
//        return calculateExpression();
//    }
//
//    if (expression.empty()) {
//        cout << "表达式不能为空！\n";
//        return;
//    }
//
//    try {
//        double result = evaluator.evaluate(expression);
//
//        cout << fixed << setprecision(6);
//        cout << "\n结果: " << expression << " = " << result << endl;
//
//        // 添加到历史记录
//        CalculationRecord record;
//        record.expression = expression;
//        record.result = result;
//        record.timestamp = getCurrentTime();
//        history.push_back(record);
//
//        // 询问是否保存
//        cout << "\n是否保存此结果？ (y/n): ";
//        char choice;
//        cin >> choice;
//        cin.ignore(); // 清除换行符
//
//        if (tolower(choice) == 'y') {
//            saveResult(expression, result);
//        }
//
//    }
//    catch (const exception& e) {
//        cout << "错误: " << e.what() << endl;
//    }
//}
// 计算表达式
double ExpressionEvaluator::evaluate(const string& expression) {
    try {
        vector<string> postfix = infixToPostfix(expression);
        return evaluatePostfix(postfix);
    }
    catch (const exception& e) {
        throw runtime_error("计算错误: " + string(e.what()));
    }
}

//计算表达式（修改版，支持 $index 语法）
void Calculator::calculateExpression() {
    string expression;
    cout << "\n请输入表达式 (输入 'help' 查看教程，'back' 返回):\n> ";
    getline(cin, expression);

    if (expression == "back") return;
    if (expression == "help") {
        showHelp();
        return calculateExpression();
    }

    if (expression.empty()) {
        cout << "表达式不能为空！\n";
        return;
    }

    try {
        // 检查并替换所有 $index 占位符
        for (size_t i = 0; i < savedItems.size(); i++) {
            string placeholder = "$" + to_string(i + 1);
            size_t pos = expression.find(placeholder);
            while (pos != string::npos) {
                expression.replace(pos, placeholder.length(), to_string(savedItems[i].value));
                pos = expression.find(placeholder, pos + 1);
            }
        }

        double result = evaluator.evaluate(expression);

        cout << fixed << setprecision(6);
        cout << "\n结果: " << expression << " = " << result << endl;

        // 添加到历史记录
        CalculationRecord record;
        record.expression = expression;
        record.result = result;
        record.timestamp = getCurrentTime();
        history.push_back(record);

        // 询问是否保存
        cout << "\n是否保存此结果？ (y/n): ";
        char choice;
        cin >> choice;
        cin.ignore(); // 清除换行符

        if (tolower(choice) == 'y') {
            saveResult(expression, result);
        }

    }
    catch (const exception& e) {
        cout << "错误: " << e.what() << endl;
    }
}

// 保存结果
void Calculator::saveResult(const string& expression, double result) {
    string name;
    cout << "请输入保存名称 (或按回车使用默认名称): ";
    getline(cin, name);

    if (name.empty()) {
        name = "保存项" + to_string(nextSaveSlot++);
    }

    SavedItem item;
    item.name = name;
    item.expression = expression;
    item.value = result;
    savedItems.push_back(item);

    cout << "已保存: " << name << " = " << expression << " = " << result << endl;
}

// 查看历史记录
void Calculator::viewHistory() {
    if (history.empty()) {
        cout << "\n暂无历史记录。\n";



        return;
    }

    cout << "\n========== 计算历史记录 ==========\n";
    cout << left << setw(5) << "序号" << setw(30) << "表达式"
        << setw(15) << "结果" << setw(20) << "时间" << endl;
    cout << string(70, '-') << endl;

    for (size_t i = 0; i < history.size(); i++) {
        cout << left << setw(5) << i + 1
            << setw(30) << (history[i].expression.length() > 28 ?
                history[i].expression.substr(0, 25) + "..." :
                history[i].expression)
            << setw(15) << fixed << setprecision(6) << history[i].result
            << setw(20) << history[i].timestamp << endl;
    }
    cout << "===================================\n";
}

// 查看保存的栏目
void Calculator::viewSavedItems() {
    if (savedItems.empty()) {
        cout << "\n暂无保存的栏目。\n";
        return;
    }

    cout << "\n========== 保存的栏目 ==========\n";
    cout << left << setw(5) << "序号" << setw(15) << "名称"
        << setw(30) << "表达式" << setw(15) << "值" << endl;
    cout << string(65, '-') << endl;

    for (size_t i = 0; i < savedItems.size(); i++) {
        cout << left << setw(5) << i + 1
            << setw(15) << savedItems[i].name
            << setw(30) << (savedItems[i].expression.length() > 28 ?
                savedItems[i].expression.substr(0, 25) + "..." :
                savedItems[i].expression)
            << setw(15) << fixed << setprecision(6) << savedItems[i].value << endl;
    }
    cout << "================================\n";

    // 提供操作选项
    cout << "\n操作选项:\n";
    cout << "1. 使用保存的值\n";
    cout << "2. 删除保存项\n";
    cout << "3. 返回主菜单\n";
    cout << "请选择: ";

    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        useSavedItem();
    }
    else if (choice == 2) {
        deleteSavedItem();
    }
}

//// 使用保存的值
//void Calculator::useSavedItem() {
//    if (savedItems.empty()) return;
//
//    cout << "请输入要使用的保存项序号: ";
//    int index;
//    cin >> index;
//    cin.ignore();
//
//    if (index < 1 || index > static_cast<int>(savedItems.size())) {
//        cout << "无效的序号！\n";
//        return;
//    }
//
//    SavedItem& item = savedItems[index - 1];
//    cout << "\n使用保存值: " << item.name << " = " << item.value << endl;
//    cout << "可以在表达式中使用 $" << index << " 来引用此值\n";
//    cout << "例如: $" << index << " * 2 或 sqrt($" << index << ")\n";
//}
// 使用保存的值
void Calculator::useSavedItem() {
    if (savedItems.empty()) return;

    cout << "请输入要使用的保存项序号: ";
    int index;
    cin >> index;
    cin.ignore();

    if (index < 1 || index > static_cast<int>(savedItems.size())) {
        cout << "无效的序号！\n";
        return;
    }

    SavedItem& item = savedItems[index - 1];
    cout << "\n已选择保存值: " << item.name << " = " << item.value << endl;

    // 询问用户如何使用这个值
    cout << "请选择操作:\n";
    cout << "1. 直接查看值\n";
    cout << "2. 在表达式中使用 (使用 $" << index << " 表示)\n";
    cout << "3. 返回\n";
    cout << "请选择: ";

    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        cout << item.name << " = " << item.expression << " = " << item.value << endl;
    }
    else if (choice == 2) {
        // 允许用户输入包含 $index 的表达式
        cout << "请输入表达式 (使用 $" << index << " 表示保存的值):\n> ";
        string expression;
        getline(cin, expression);

        // 将 $index 替换为实际值
        string placeholder = "$" + to_string(index);
        size_t pos = expression.find(placeholder);
        while (pos != string::npos) {
            expression.replace(pos, placeholder.length(), to_string(item.value));
            pos = expression.find(placeholder, pos + 1);
        }

        try {
            double result = evaluator.evaluate(expression);
            cout << fixed << setprecision(6);
            cout << "\n结果: " << expression << " = " << result << endl;

            // 添加到历史记录
            CalculationRecord record;
            record.expression = expression;
            record.result = result;
            record.timestamp = getCurrentTime();
            history.push_back(record);
        }
        catch (const exception& e) {
            cout << "错误: " << e.what() << endl;
        }
    }
}

// 删除保存项
void Calculator::deleteSavedItem() {
    if (savedItems.empty()) return;

    cout << "请输入要删除的保存项序号: ";
    int index;
    cin >> index;
    cin.ignore();

    if (index < 1 || index > static_cast<int>(savedItems.size())) {
        cout << "无效的序号！\n";
        return;
    }

    cout << "确定要删除 '" << savedItems[index - 1].name << "' 吗？ (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore();

    if (tolower(confirm) == 'y') {
        savedItems.erase(savedItems.begin() + index - 1);
        cout << "已删除。\n";
    }
}

// 显示主菜单
void Calculator::showMenu() {
    cout << "\n========== C++ 高级计算器 ==========\n";
    cout << "1. 计算表达式\n";
    cout << "2. 查看计算历史记录\n";
    cout << "3. 查看保存的栏目\n";
    cout << "4. 查看使用教程\n";
    cout << "5. 退出程序\n";
    cout << "===================================\n";
    cout << "请选择操作 (1-5): ";
}

// 运行计算器
void Calculator::run() {
    int choice;
    do {
        showMenu();
        cin >> choice;
        cin.ignore(); // 清除换行符

        switch (choice) {
        case 1:
            calculateExpression();
            break;
        case 2:
            viewHistory();
            break;
        case 3:
            viewSavedItems();
            break;
        case 4:
            showHelp();
            break;
        case 5:
            cout << "\n感谢使用C++计算器！再见！\n";
            break;
        default:
            cout << "无效选择，请重新输入！\n";
        }
    } while (choice != 5);
}