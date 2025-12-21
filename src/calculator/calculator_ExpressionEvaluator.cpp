#include"calculator.h"



// 检查字符是否为运算符
bool ExpressionEvaluator::isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
}

// 检查字符是否为数字或小数点
bool ExpressionEvaluator::isDigitOrDot(char c) {
    return isdigit(c) || c == '.' || c == 'e' || c == 'E';
}

// 获取运算符优先级
int ExpressionEvaluator::getPrecedence(char op) {
    if (precedence.find(op) != precedence.end()) {
        return precedence[op];
    }
    return 0;
}

// 应用运算符到两个操作数
double ExpressionEvaluator::applyOperator(double a, double b, char op) {
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
string ExpressionEvaluator::processAbsFunction(const string& expr) {
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
vector<string> ExpressionEvaluator::infixToPostfix(const string& expression) {
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
double ExpressionEvaluator::evaluatePostfix(const vector<string>& postfix) {
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