#include "TwentyFourGame.h"

// 随机数生成器
int GameInner::getRandomNumber(int min, int max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

// 检查表达式是否只包含允许的字符
bool GameInner::isValidExpression(const string& expr) {
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
vector<int> GameInner::extractNumbersFromExpression(const string& expr) {
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

// 格式化表达式
string GameInner::formatExpression(const string& expr) {
    string result;
    for (char c : expr) {
        if (c == '+' || c == '-' || c == '*' || c == '/' ||
            c == '(' || c == ')' || isdigit(c)) {
            result += c;
        }
    }
    return result;
}

// 移除字符串中的空格
string GameInner::removeSpaces(const string& str) {
    string result;
    for (char c : str) {
        if (c != ' ') {
            result += c;
        }
    }
    return result;
}