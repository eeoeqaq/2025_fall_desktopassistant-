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
	cout << "�����ǻ����˵��ķ���:" << endl<<endl;
	this->stateAnalysis(arrayAccounts);
	this->incomeType(arrayAccounts);
	this->expenseType(arrayAccounts);
}
//����֧������
void analysis::stateAnalysis(vector<vector<int>> arrayAccounts)
{
	int income=0, expense=0;
	for (vector<int> v : arrayAccounts)
	{
		if (v[1] == 1) income += v[0];
		else expense += v[0];
	}
	cout << "������Ϊ " << income << " Ԫ����֧��Ϊ " << expense << " Ԫ" << endl;
	if (income - expense >= 0)
		cout << "������Ϊ " << income - expense << " Ԫ" << endl;
	else
		cout << "��֧��Ϊ " << expense-income << " Ԫ" << endl;

}
//�������ͷֱ����
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
		cout << "��������Ϊ " << income_1 << " Ԫ����������Ϊ " << income_2 << " Ԫ" << endl;
		if (income_1 - income_2 >= 0)
			cout << "������Ҫ������ĿΪ ���� " << endl;
		else
			cout << "������Ҫ������ĿΪ �������� " << endl;
	}
}
//֧�����ͷֱ����
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
		cout << "��ʳ֧��Ϊ " << expense_1 << " Ԫ������֧��Ϊ " << expense_2 << " Ԫ������֧��Ϊ " << expense_3 << " Ԫ" << endl;
		if (max(expense_1, max(expense_2, expense_3)) == expense_1)
			cout << "������Ҫ֧����ĿΪ ��ʳ " << endl;
		else if (max(expense_1, max(expense_2, expense_3)) == expense_2)
			cout << "������Ҫ֧����ĿΪ ���� " << endl;
		else
			cout << "������Ҫ֧����ĿΪ ���� " << endl;
	}
	cout << endl;
}
