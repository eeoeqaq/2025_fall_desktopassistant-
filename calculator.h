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



// 表达式求值类
class ExpressionEvaluator {
    // 运算符优先级映射
    map<char, int> precedence = {
        {'+', 1}, {'-', 1},
        {'*', 2}, {'/', 2},
        {'^', 3}, {'%', 2}
    };
public:
	bool isOperator(char c);
    bool isDigitOrDot(char c);
    int getPrecedence(char op);
    double applyOperator(double a, double b, char op);
    string processAbsFunction(const string& expr);
    vector<string> infixToPostfix(const string& expression);
    double evaluatePostfix(const vector<string>& postfix);
	double evaluate(const string& expression);
};


// 计算器类
class Calculator {
private:

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

    vector<CalculationRecord> history;
    vector<SavedItem> savedItems;
    ExpressionEvaluator evaluator;
    int nextSaveSlot;
public:
    Calculator() : nextSaveSlot(1) {}
	string getCurrentTime();
    void showHelp();
    void calculateExpression();
    void saveResult(const string& expression, double result);
    void viewHistory();
    void viewSavedItems();
    void useSavedItem();
    void deleteSavedItem();
    void showMenu();
	void run();
};