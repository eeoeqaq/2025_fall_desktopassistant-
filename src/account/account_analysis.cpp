#include"account.h"

void analysis::start(vector<account> accounts)
{
	vector<vector<int>> arrayAccounts;
	for (account a : accounts)
	{
		string data = a.getAccount(), tem;
		
		stringstream ss(data);
		ss >> tem;
		int money = stoi(tem);
		ss >> tem;
		int state = stoi(tem);
		ss >> tem;
		int type = stoi(tem);
		vector<int> temp = { money,state,type };
		arrayAccounts.push_back(temp);
	}
	cout << "以下是基于账单的分析:" << endl<<endl;
	this->stateAnalysis(arrayAccounts);
	this->incomeType(arrayAccounts);
	this->expenseType(arrayAccounts);
}
//收入支出汇总
void analysis::stateAnalysis(vector<vector<int>> arrayAccounts)
{
	int income=0, expense=0;
	for (vector<int> v : arrayAccounts)
	{
		if (v[1] == 1) income += v[0];
		else expense += v[0];
	}
	cout << "总收入为 " << income << " 元，总支出为 " << expense << " 元" << endl;
	if (income - expense >= 0)
		cout << "净收入为 " << income - expense << " 元" << endl;
	else
		cout << "净支出为 " << expense-income << " 元" << endl;

}
//收入类型分别汇总
void analysis::incomeType(vector<vector<int>> arrayAccounts)
{
	int income_1 = 0, income_2 = 0;
	for (vector<int> v : arrayAccounts)
	{
		if (v[1] == 1)
		{
			if (v[2] == 1) income_1 += v[0];
			else income_2 += v[0];
		}
	}
	if (income_1 || income_2)
	{
		cout << "工资收入为 " << income_1 << " 元，其他收入为 " << income_2 << " 元" << endl;
		if (income_1 - income_2 >= 0)
			cout << "您的主要收入项目为 工资 " << endl;
		else
			cout << "您的主要收入项目为 其他收入 " << endl;
	}
}
//支出类型分别汇总
void analysis::expenseType(vector<vector<int>> arrayAccounts)
{
	int expense_1 = 0, expense_2 = 0,expense_3=0;
	for (vector<int> v : arrayAccounts)
	{
		if (v[1] == 2)
		{
			if (v[2] == 1) expense_1 += v[0];
			else if (v[2] == 2) expense_2 += v[0];
			else expense_3 += v[0];
		}
	}
	if (expense_1 || expense_2 || expense_3)
	{
		cout << "饮食支出为 " << expense_1 << " 元，日用支出为 " << expense_2 << " 元，日用支出为 " << expense_3 << " 元" << endl;
		if (max(expense_1, max(expense_2, expense_3)) == expense_1)
			cout << "您的主要支出项目为 饮食 " << endl;
		else if (max(expense_1, max(expense_2, expense_3)) == expense_2)
			cout << "您的主要支出项目为 日用 " << endl;
		else
			cout << "您的主要支出项目为 娱乐 " << endl;
	}
	cout << endl;
}