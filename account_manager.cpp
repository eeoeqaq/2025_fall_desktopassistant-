#include "account.h"

void account_manager::Start()
{
	//读文件部分
	ifstream accountFile;
	accountFile.open("accountData.txt", ios::in);
	string line,tem;
	
	while (getline(accountFile, line))
	{
		stringstream ss(line);
		ss >> tem;
		int money = stoi(tem);
		ss >> tem;
		int state = stoi(tem);
		ss >> tem;
		int type = stoi(tem);
		account temp(money, state, type);
		this->accounts.push_back(temp);
	}

	//正式运行部分
		this->HomePage();
	
}

void account_manager::HomePage()//首页菜单模块
{
	while (true)
	{
		cout << " ******************************" << endl;
		cout << "添加账目输入1" << endl;
		cout << "删除账目输入2" << endl;
		cout << "打印账单输入3" << endl;
		cout << "清空账本输入4" << endl;
		cout << "分析账单输入5" << endl;
		cout << "退出账本输入6" << endl;
		cout << " ******************************" << endl << endl;


		int num = -1;
		while (num == -1)
		{
			cout << "输入选项";
			cin >> num;
			switch (num)
			{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				break;
			default:
				num = -1;
				// 清除cin的错误状态
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
			cout << endl;
		}
		switch (num)
		{
		case 1:
			this->WriteAccount();
			break;
		case 2:
			this->DeleteAccount();
			break;
		case 3:
			this->showTable();
			break;
		case 4:
			this->clearAccount();
			break;
		case 5:
			this->analysisAccount();
			break;
		case 6:
			return;
		}
	}
}

void account_manager::WriteAccount()
{
	account a;
	a.setAccount();
	accounts.push_back(a);
	this->save();
}

void account_manager::DeleteAccount()
{
	this->showTable();
	int row=-1;
	int max = (int)this->accounts.size();
	while (row==-1)
	{
		cout << "输入需要删除的账目（第几行）" << endl;
		cin >> row;
		row = (row >= 0 && row <= max) ? row : -1;
	}

	this->accounts.erase(accounts.begin()+row-1);
	this->showTable();
}

void account_manager::showTable()
{
	//打印表头
	cout << "账单如下：" << endl;
	cout << setw(10) << "金额" << setw(10) << "收入/支出" << setw(10) << "类别" << endl;
	//打印账目
	for (account a : accounts)
	{
		a.ShowAccount();
	}	
	cout << endl;
}

void account_manager::save()
{
	ofstream accountFile;
	accountFile.open("accountData.txt", ios::out);
	for (account a : accounts)
	{
		accountFile << a.getAccount()<<endl;
	}
	accountFile.close();
}

void account_manager::analysisAccount()
{
	analysis A;
	A.start(account_manager::accounts);
}

void account_manager::clearAccount()
{
	ofstream accountFile;
	accountFile.open("accountData.txt", ios::out);
	accountFile.close();
	this->accounts.clear();
	cout << "账本已清空" << endl << endl;;
}