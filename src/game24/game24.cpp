#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <stack>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cctype>
#include <map>
#include <set>

using namespace std;

// 题库结构体
struct Problem {
    vector<int> numbers;           // 四个数字
    vector<string> solutions;      // 可能的解决方案
    string example;                // 示例解决方案（用于展示）
};

// 游戏类
class TwentyFourGame {
private:
    vector<Problem> questionBank;  // 题库
    Problem currentProblem;        // 当前题目
    int totalAttempts;             // 总尝试次数
    int correctAttempts;           // 正确次数

    // 初始化题库
    void initializeQuestionBank() {
        // 大约30个不同的24点题目，每个题目有至少一种解法
        questionBank = {
            // 经典题目
            {{1, 2, 3, 4}, {"(1+2+3)*4", "(1+3)*(2+4)", "1*2*3*4"}, "(1+2+3)*4"},
            {{1, 1, 5, 5}, {"(5-1)*(5+1)", "(5*5)-1/1", "(5-1/1)*5"}, "(5-1)*(5+1)"},
            {{1, 3, 4, 6}, {"6/(1-3/4)", "3*4*(1+6/6)"}, "6/(1-3/4)"},
            {{2, 2, 2, 9}, {"(2+9)*2+2", "2*9+2+2", "(9+2/2)*2"}, "(2+9)*2+2"},
            {{3, 3, 7, 7}, {"(3+3/7)*7", "7*(3/7+3)"}, "(3+3/7)*7"},
            {{4, 4, 7, 7}, {"(4-4/7)*7", "7*4-4"}, "(4-4/7)*7"},
            {{5, 5, 5, 1}, {"5*(5-1/5)", "(5-1/5)*5"}, "5*(5-1/5)"},
            {{6, 6, 6, 6}, {"6+6+6+6", "(6*6)-(6+6)", "(6-6/6)*6"}, "6+6+6+6"},
            {{8, 8, 8, 8}, {"8*8/(8+8)", "(8+8)-(8/8)", "(8*8)/(8+8)"}, "8*8/(8+8)"},

            // 中等难度题目
            {{2, 3, 4, 5}, {"(3+4+5)*2", "(5+3-2)*4", "(2*5-3)*4"}, "(3+4+5)*2"},
            {{2, 4, 5, 6}, {"(5-2)*4+6", "6*5/(4-2)", "(2+4)*5-6"}, "(5-2)*4+6"},
            {{2, 6, 6, 7}, {"(6*7-6)/2", "(7-2)*6-6", "6*(7-2)-6"}, "(6*7-6)/2"},
            {{3, 4, 5, 6}, {"(5-3)*4*6", "(3*4)*(6-5)", "(5+3-4)*6"}, "(5-3)*4*6"},
            {{3, 5, 6, 7}, {"(6-5+7)*3", "3*6+7-5", "(7+5)*6/3"}, "(6-5+7)*3"},
            {{4, 5, 6, 7}, {"(7-6)*4*6", "(5+7)*(6-4)", "4*6/(7-5)"}, "(7-6)*4*6"},
            {{4, 6, 8, 8}, {"(8-6)*8+4", "(4*8)-8-6", "8*6/(8-4)"}, "(8-6)*8+4"},
            {{5, 6, 7, 8}, {"(8-7+5)*6", "(5+7-8)*6", "6*8/(7-5)"}, "(8-7+5)*6"},

            // 挑战题目
            {{1, 4, 5, 6}, {"4/(1-5/6)", "6/(5/4-1)", "5*6-4-1"}, "4/(1-5/6)"},
            {{2, 5, 7, 8}, {"(2*5-7)*8", "(8-5+7)*2", "(7*8)/2-5"}, "(2*5-7)*8"},
            {{3, 3, 8, 8}, {"8/(3-8/3)", "(8*3)/(8-3)", "(3+8)*3-8"}, "8/(3-8/3)"},
            {{4, 7, 8, 9}, {"(9-7)*8+4", "(7+9)*8/4", "4*9-7-8"}, "(9-7)*8+4"},
            {{5, 8, 9, 13}, {"(13-9)*5+8", "(8-5)*(13-9)", "(9+13)*8/5"}, "(13-9)*5+8"},
            {{6, 7, 9, 10}, {"(9-7)*10+6", "10*7/6+9", "(10-7)*9-6"}, "(9-7)*10+6"},
            {{7, 8, 10, 11}, {"(10-8)*11+7", "11*8/(10-7)", "(11-7)*10-8"}, "(10-8)*11+7"},
            {{9, 10, 11, 12}, {"(12-10)*11+9", "12*10/(11-9)", "(11-9)*12-10"}, "(12-10)*11+9"},
            {{10, 10, 13, 13}, {"(13*13-10)/10", "(13-10/10)*13"}, "(13*13-10)/10"},

            // 更多题目
            {{1, 6, 8, 12}, {"12*8/(6-1)", "(12-8)*6*1", "8*6/(12/1-8)"}, "12*8/(6-1)"},
            {{2, 7, 9, 11}, {"(11-7)*9-2", "(9-2)*7-11", "11*2+9-7"}, "(11-7)*9-2"},
            {{3, 6, 9, 12}, {"(12-9)*6+3", "12*9/6+3", "(9-3)*6-12"}, "(12-9)*6+3"},
            {{4, 8, 11, 13}, {"(13-11)*8+4", "(11-4)*8-13", "13*4-11-8"}, "(13-11)*8+4"},
            {{5, 9, 12, 13}, {"(13-9)*5+12", "(12-5)*9-13", "13*12/9+5"}, "(13-9)*5+12"}
        };
    }

    // 随机数生成器
    int getRandomNumber(int min, int max) {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    // 检查表达式是否只包含允许的字符
    bool isValidExpression(const string& expr) {
        // 允许的字符：数字0-9、运算符、括号、空格
        for (char c : expr) {
            if (!(isdigit(c) || c == '+' || c == '-' || c == '*' || c == '/' ||
                c == '(' || c == ')' || c == ' ')) {
                return false;
            }
        }
        return true;
    }

    // 从表达式中提取数字
    vector<int> extractNumbersFromExpression(const string& expr) {
        vector<int> numbers;
        string numStr;

        for (char c : expr) {
            if (isdigit(c)) {
                numStr += c;
            }
            else if (!numStr.empty()) {
                // 处理两位数，如10、11、12、13
                int num = stoi(numStr);
                numbers.push_back(num);
                numStr.clear();
            }
        }

        // 处理最后一个数字
        if (!numStr.empty()) {
            int num = stoi(numStr);
            numbers.push_back(num);
        }

        return numbers;
    }

    // 检查数字是否匹配题目
    bool checkNumbersMatch(const vector<int>& exprNumbers) {
        if (exprNumbers.size() != 4) {
            return false;
        }

        // 复制题目数字和表达式数字
        vector<int> probNums = currentProblem.numbers;
        vector<int> exprNums = exprNumbers;

        // 排序后比较
        sort(probNums.begin(), probNums.end());
        sort(exprNums.begin(), exprNums.end());

        return probNums == exprNums;
    }

    // 移除字符串中的空格
    string removeSpaces(const string& str) {
        string result;
        for (char c : str) {
            if (c != ' ') {
                result += c;
            }
        }
        return result;
    }

    // 表达式求值类（简化版）
    class ExpressionEvaluator {
    private:
        // 运算符优先级
        map<char, int> precedence = {
            {'+', 1}, {'-', 1},
            {'*', 2}, {'/', 2}
        };

        // 应用运算符
        double applyOperator(double a, double b, char op) {
            switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/':
                if (fabs(b) < 1e-9) throw runtime_error("除零错误");
                return a / b;
            default: throw runtime_error("无效运算符");
            }
        }

        // 转换为后缀表达式
        vector<string> infixToPostfix(const string& expr) {
            vector<string> postfix;
            stack<char> ops;
            string num;

            for (size_t i = 0; i < expr.length(); i++) {
                char c = expr[i];

                if (isdigit(c)) {
                    num += c;
                    // 处理多位数
                    if (i + 1 < expr.length() && isdigit(expr[i + 1])) {
                        continue;
                    }
                    postfix.push_back(num);
                    num.clear();
                }
                else if (c == '(') {
                    ops.push(c);
                }
                else if (c == ')') {
                    while (!ops.empty() && ops.top() != '(') {
                        postfix.push_back(string(1, ops.top()));
                        ops.pop();
                    }
                    ops.pop(); // 弹出 '('
                }
                else if (c == '+' || c == '-' || c == '*' || c == '/') {
                    while (!ops.empty() && ops.top() != '(' &&
                        precedence[ops.top()] >= precedence[c]) {
                        postfix.push_back(string(1, ops.top()));
                        ops.pop();
                    }
                    ops.push(c);
                }
            }

            while (!ops.empty()) {
                postfix.push_back(string(1, ops.top()));
                ops.pop();
            }

            return postfix;
        }

        // 计算后缀表达式
        double evaluatePostfix(const vector<string>& postfix) {
            stack<double> values;

            for (const string& token : postfix) {
                if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
                    values.push(stod(token));
                }
                else {
                    if (values.size() < 2) throw runtime_error("表达式错误");
                    double b = values.top(); values.pop();
                    double a = values.top(); values.pop();
                    values.push(applyOperator(a, b, token[0]));
                }
            }

            if (values.size() != 1) throw runtime_error("表达式错误");
            return values.top();
        }

    public:
        double evaluate(const string& expr) {
            try {
                vector<string> postfix = infixToPostfix(expr);
                return evaluatePostfix(postfix);
            }
            catch (const exception& e) {
                throw runtime_error("表达式计算错误");
            }
        }
    };

    // 验证表达式结果是否为24
    bool isExpressionValid(const string& expr) {
        ExpressionEvaluator evaluator;

        try {
            double result = evaluator.evaluate(expr);
            // 允许小的浮点误差
            return fabs(result - 24.0) < 1e-6;
        }
        catch (...) {
            return false;
        }
    }

    // 显示抽取数字的动画效果
    void showNumberDrawingAnimation() {
        cout << "\n正在抽取数字..." << endl;

        for (int i = 0; i < 4; i++) {
            // 模拟思考时间
            for (int j = 0; j < 3; j++) {
                cout << ". ";
                cout.flush();
                // 简单延时
                for (int k = 0; k < 10000000; k++) {}
            }

            int num = currentProblem.numbers[i];
            cout << "\n第 " << (i + 1) << " 个数字是: " << num << endl;

            // 显示已抽取的数字
            cout << "当前数字: ";
            for (int j = 0; j <= i; j++) {
                cout << currentProblem.numbers[j] << " ";
            }
            cout << endl;

            // 每次抽取后的小延时
            for (int k = 0; k < 20000000; k++) {}
        }

        cout << "\n════════════════════════════════════════\n";
        cout << "四个数字已全部抽取: ";
        for (int num : currentProblem.numbers) {
            cout << num << " ";
        }
        cout << "\n════════════════════════════════════════\n";
    }

    // 显示游戏规则
    void showRules() {
        cout << "\n========== 24点游戏规则 ==========\n";
        cout << "1. 使用给定的四个数字（1-13）\n";
        cout << "2. 每个数字必须使用且只能使用一次\n";
        cout << "3. 只能使用 + - * / 四种运算符和括号 ( )\n";
        cout << "4. 表达式计算结果必须等于24\n";
        cout << "5. 表达式示例:\n";
        cout << "   数字: 1 2 3 4\n";
        cout << "   表达式: (1+2+3)*4 = 24\n";
        cout << "   或: 1*2*3*4 = 24\n";
        cout << "6. 输入 'quit' 退出游戏\n";
        cout << "7. 输入 'hint' 查看提示\n";
        cout << "==================================\n";
    }

    // 显示提示
    void showHint() {
        cout << "\n💡 提示:\n";
        cout << "当前题目的一个解法是: " << currentProblem.example << endl;
        cout << "但可能有多种解法，试试不同的组合！\n";
    }

    // 格式化表达式
    string formatExpression(const string& expr) {
        string result;
        for (char c : expr) {
            if (c == '+' || c == '-' || c == '*' || c == '/' ||
                c == '(' || c == ')' || isdigit(c)) {
                result += c;
            }
        }
        return result;
    }

public:
    TwentyFourGame() : totalAttempts(0), correctAttempts(0) {
        initializeQuestionBank();
        srand(time(0));
    }

    // 选择新题目
    void selectNewProblem() {
        int index = getRandomNumber(0, questionBank.size() - 1);
        currentProblem = questionBank[index];
    }

    // 显示当前统计
    void showStatistics() {
        cout << "\n📊 当前统计:\n";
        cout << "总尝试次数: " << totalAttempts << endl;
        if (totalAttempts > 0) {
            double accuracy = (double)correctAttempts / totalAttempts * 100;
            cout << "正确次数: " << correctAttempts << endl;
            cout << "准确率: " << fixed << setprecision(1) << accuracy << "%" << endl;
        }
    }

    // 运行游戏
    void run() {
        cout << "欢迎来到24点游戏！\n";
        showRules();

        char playAgain;
        do {
            // 选择新题目
            selectNewProblem();

            // 显示抽取动画
            showNumberDrawingAnimation();

            bool solved = false;
            int attempts = 0;
            const int maxAttempts = 3;

            while (!solved && attempts < maxAttempts) {
                attempts++;
                cout << "\n第 " << attempts << " 次尝试 (还剩 " << (maxAttempts - attempts) << " 次)\n";
                cout << "请输入表达式 (或输入 'hint'/'quit'): ";

                string input;
                getline(cin, input);

                // 检查退出命令
                if (input == "quit" || input == "exit") {
                    cout << "感谢游玩！\n";
                    return;
                }

                // 检查提示命令
                if (input == "hint") {
                    showHint();
                    attempts--; // 提示不算尝试次数
                    continue;
                }

                // 移除空格并格式化
                string expr = removeSpaces(input);
                expr = formatExpression(expr);

                if (expr.empty()) {
                    cout << "表达式不能为空！\n";
                    attempts--;
                    continue;
                }

                // 验证表达式字符
                if (!isValidExpression(expr)) {
                    cout << "❌ 表达式包含非法字符！\n";
                    cout << "只允许数字、+ - * / 和括号 ( )\n";
                    continue;
                }

                // 提取表达式中的数字
                vector<int> exprNumbers = extractNumbersFromExpression(expr);

                // 检查数字是否匹配
                if (!checkNumbersMatch(exprNumbers)) {
                    cout << "❌ 数字使用错误！\n";
                    cout << "必须使用且只能使用这四个数字: ";
                    for (int num : currentProblem.numbers) {
                        cout << num << " ";
                    }
                    cout << "\n你使用了: ";
                    for (int num : exprNumbers) {
                        cout << num << " ";
                    }
                    cout << endl;
                    continue;
                }

                // 检查表达式结果是否为24
                if (isExpressionValid(expr)) {
                    cout << "\n🎉 恭喜！解答正确！\n";
                    cout << "你的表达式: " << expr << " = 24\n";
                    solved = true;
                    correctAttempts++;
                }
                else {
                    cout << "❌ 表达式计算结果不是24！\n";
                    cout << "请检查表达式是否正确\n";
                }

                totalAttempts++;
            }

            // 如果未解决，显示答案
            if (!solved) {
                cout << "\n💡 挑战失败！正确答案示例:\n";
                cout << "题目: ";
                for (int num : currentProblem.numbers) {
                    cout << num << " ";
                }
                cout << "\n";

                // 显示所有解法
                cout << "可能的解法有:\n";
                for (size_t i = 0; i < currentProblem.solutions.size(); i++) {
                    cout << "  " << (i + 1) << ". " << currentProblem.solutions[i] << endl;
                }

                // 显示一题多解的概念
                if (currentProblem.solutions.size() > 1) {
                    cout << "\n✨ 注意：24点游戏经常有多种解法！\n";
                    cout << "不同的数字组合和运算顺序都可能得到24\n";
                }
            }

            // 显示统计
            showStatistics();

            // 询问是否继续
            cout << "\n是否继续挑战新题目？ (y/n): ";
            cin >> playAgain;
            cin.ignore(); // 清除换行符

        } while (playAgain == 'y' || playAgain == 'Y');

        // 最终统计
        cout << "\n========== 最终统计 ==========\n";
        showStatistics();
        cout << "\n感谢游玩24点游戏！再见！\n";
    }
};

// 主函数
int main() {
    // 设置控制台编码为UTF-8（Windows）
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    TwentyFourGame game;
    game.run();

    return 0;
}