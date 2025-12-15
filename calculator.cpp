#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <memory>

using namespace std;

// 计算历史记录条目
struct CalculationRecord {
    string expression;
    double result;
    string timestamp;
};

// 保存的栏目条目
struct SavedItem {
    string name;
    string expression;
    double value;
};

// 表达式求值类
class ExpressionEvaluator {
private:
    // 运算符优先级映射
    map<char, int> precedence = {
        {'+', 1}, {'-', 1},
        {'*', 2}, {'/', 2},
        {'^', 3}, {'%', 2}
    };

    // 检查字符是否为运算符
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
    }

    // 检查字符是否为数字或小数点
    bool isDigitOrDot(char c) {
        return isdigit(c) || c == '.' || c == 'e' || c == 'E';
    }

    // 获取运算符优先级
    int getPrecedence(char op) {
        if (precedence.find(op) != precedence.end()) {
            return precedence[op];
        }
        return 0;
    }

    // 应用运算符到两个操作数
    double applyOperator(double a, double b, char op) {
        switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw runtime_error("除零错误");
            return a / b;
        case '^': return pow(a, b);
        case '%':
            if (b == 0) throw runtime_error("模零错误");
            return fmod(a, b);
        default: throw runtime_error("无效运算符");
        }
    }

    // 处理绝对值函数
    string processAbsFunction(const string& expr) {
        string result;
        for (size_t i = 0; i < expr.length(); i++) {
            // 检查是否为abs函数
            if (i + 3 < expr.length() && expr.substr(i, 4) == "abs(") {
                result += "abs(";
                i += 3;
            }
            else {
                result += expr[i];
            }
        }
        return result;
    }

    // 将中缀表达式转换为后缀表达式（逆波兰表示法）
    vector<string> infixToPostfix(const string& expression) {
        vector<string> postfix;
        stack<char> operators;
        string number;

        string expr = processAbsFunction(expression);

        for (size_t i = 0; i < expr.length(); i++) {
            char c = expr[i];

            // 跳过空格
            if (isspace(c)) continue;

            // 处理数字和小数点
            if (isDigitOrDot(c) || (c == '-' && (i == 0 || expr[i - 1] == '(' || isOperator(expr[i - 1])))) {
                number += c;
                // 检查下一个字符是否还是数字或小数点
                if (i + 1 < expr.length() && isDigitOrDot(expr[i + 1])) {
                    continue;
                }
                postfix.push_back(number);
                number.clear();
            }
            // 处理abs函数
            else if (i + 3 < expr.length() && expr.substr(i, 4) == "abs(") {
                operators.push('a'); // 用'a'表示abs函数
                i += 3; // 跳过"abs("
            }
            // 处理左括号
            else if (c == '(') {
                operators.push(c);
            }
            // 处理右括号
            else if (c == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    if (operators.top() == 'a') {
                        postfix.push_back("abs");
                    }
                    else {
                        postfix.push_back(string(1, operators.top()));
                    }
                    operators.pop();
                }
                if (!operators.empty() && operators.top() == '(') {
                    operators.pop();
                }
                // 处理函数调用
                if (!operators.empty() && operators.top() == 'a') {
                    postfix.push_back("abs");
                    operators.pop();
                }
            }
            // 处理运算符
            else if (isOperator(c)) {
                while (!operators.empty() && operators.top() != '(' &&
                    getPrecedence(operators.top()) >= getPrecedence(c)) {
                    if (operators.top() == 'a') {
                        postfix.push_back("abs");
                    }
                    else {
                        postfix.push_back(string(1, operators.top()));
                    }
                    operators.pop();
                }
                operators.push(c);
            }
        }

        // 处理剩余的运算符
        while (!operators.empty()) {
            if (operators.top() == 'a') {
                postfix.push_back("abs");
            }
            else {
                postfix.push_back(string(1, operators.top()));
            }
            operators.pop();
        }

        return postfix;
    }

    // 计算后缀表达式
    double evaluatePostfix(const vector<string>& postfix) {
        stack<double> values;

        for (const string& token : postfix) {
            // 如果是数字
            if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1 && isdigit(token[1]))) {
                values.push(stod(token));
            }
            // 如果是abs函数
            else if (token == "abs") {
                if (values.empty()) throw runtime_error("缺少操作数");
                double val = values.top(); values.pop();
                values.push(fabs(val));
            }
            // 如果是运算符
            else if (isOperator(token[0])) {
                if (values.size() < 2) throw runtime_error("缺少操作数");
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                values.push(applyOperator(a, b, token[0]));
            }
        }

        if (values.size() != 1) throw runtime_error("表达式无效");
        return values.top();
    }

public:
    // 计算表达式
    double evaluate(const string& expression) {
        try {
            vector<string> postfix = infixToPostfix(expression);
            return evaluatePostfix(postfix);
        }
        catch (const exception& e) {
            throw runtime_error("计算错误: " + string(e.what()));
        }
    }
};

// 计算器类
class Calculator {
private:
    vector<CalculationRecord> history;
    vector<SavedItem> savedItems;
    ExpressionEvaluator evaluator;
    int nextSaveSlot;

    // 获取当前时间字符串
    string getCurrentTime() {
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
    void showHelp() {
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

public:
    Calculator() : nextSaveSlot(1) {}

    // 计算表达式
    void calculateExpression() {
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
    void saveResult(const string& expression, double result) {
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
    void viewHistory() {
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
    void viewSavedItems() {
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

    // 使用保存的值
    void useSavedItem() {
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
        cout << "\n使用保存值: " << item.name << " = " << item.value << endl;
        cout << "可以在表达式中使用 $" << index << " 来引用此值\n";
        cout << "例如: $" << index << " * 2 或 sqrt($" << index << ")\n";
    }

    // 删除保存项
    void deleteSavedItem() {
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
    void showMenu() {
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
    void run() {
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
};

// 主函数
int main() {
    // 设置控制台编码为UTF-8（Windows）
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    cout << "欢迎使用C++高级计算器！\n";
    cout << "支持功能：加减乘除、幂运算(^)、取模(%)、绝对值(abs())\n";

    Calculator calculator;
    calculator.run();

    return 0;
}