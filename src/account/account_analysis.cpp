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
Expenses are summarized by type
void analysis::expenseType(vector<vector<int>> arrayAccounts)
{
	int expense_1 = 0, expense_2 = 0, expense_3 = 0;
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
		cout << "Food expenses are " << expense_1 << " yuan, daily expenses are " << expense_2 << " yuan, and daily expenses are " << expense_3 << " yuan" << endl;
if (max(expense_1, max(expense_2, expense_3)) == expense_1)
			cout << "Your main expense item is Food " << endl;
		else if (max(expense_1, max(expense_2, expense_3)) == expense_2)
			cout << "Your main expense item is Daily Necessities " << endl;
else
			cout << "Your main expenditure item is Entertainment " << endl;
	}
	cout << endl;
}     
