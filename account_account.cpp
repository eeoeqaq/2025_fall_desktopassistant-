#include "account.h"

void account::setAccount()
{
	int money;
	cout << "请输入金额(单笔不超过1000000元)" << endl<<endl;
	cin >> money;

	int state=-1;
	while (state == -1)
	{
		cout << "请输入收入/支出（1为收入，2为支出）" << endl<<endl;
		cin >> state;
		switch (state)
		{
		case 1:
		case 2:
			break;
		default:
			state = -1;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		}
	}

	int type = -1;
	while (type == -1)
	{
		if (state == 1)//收入类型
		{
			cout << "请输入收入类型（1为工资，2为其他收入）" << endl<<endl;
			cin >> type;
			switch (type)
			{
			case 1:
			case 2:
				break;
			default:
				type = -1;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
		}
		else //支出类型
		{
			cout << "请输入支出类型（1为饮食，2为日用，3为娱乐）" << endl<<endl;
			cin >> type;
			switch (type)
			{
			case 1:
			case 2:
			case 3:
				break;
			default:
				type = -1;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
		}
	}

	this->money = money;
	this->state = state;
	this->type = type;
}

void account::ShowAccount()//用表格形式展示单条账目
{
	cout << setw(10) << this->money;
	string showState = (this->state - 1) ? "支出" : "收入";
	cout << setw(10) << showState;
	string showType;
	if(state==1)
	switch (this->type)
	{
	case 1:
		showType = "工资";
		break;
	case 2:
		showType = "其他收入";
		break;
	}
	if(state==2)
	switch (this->type)
	{
	case 1:
		showType = "饮食";
		break;
	case 2:
		showType = "日用";
		break;
	case 3:
		showType = "娱乐";
		break;
	}
	cout << setw(10) << showType<<endl;
}
string account::getAccount()
{
	string res;
	res = to_string(this->money);
	res += " ";
	res += to_string(this->state);
	res += " ";
	res += to_string(this->type);
	return res;
}

account::account(int money,int state,int type)
{
	this->money = money;
	this->state = state;
	this->type = type;
}
