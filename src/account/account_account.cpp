#include "account.h"

void account::setAccount()
{
	int money;
	cout << "请输入金额(单笔不超过1000000元的整数)" << endl<<endl;
	safeCin(money, 1000000, 1, 2);	
	int state;
	cout << "请输入收支类型(1为收入/2为支出)" << endl << endl;
	safeCin(state, 2, 1, 1);

	int type ;
	if (state == 1)//收入类型
	{
		cout << "请输入收入类型（1为工资，2为其他收入）" << endl << endl;
		safeCin(type, 2, 1, 1);
	}
	else //支出类型
	{
		cout << "请输入支出类型（1为饮食，2为日用，3为娱乐）" << endl<<endl;
		safeCin(type, 3, 1, 1);
			
	}
	

	this->money = money;
	this->state = state;
	this->type = type;
}

void account::ShowAccount()//用表格形式展示单条账目
{
	cout << setw(12) << this->money;
	string showState = (this->state - 1) ? "支出" : "收入";
	cout << setw(15) << showState;
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
	cout << setw(15) << showType<<endl;
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
