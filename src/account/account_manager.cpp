#include "account.h"

void account_manager::Start()
{
	//读文件部分
	ifstream accountFile;
	accountFile.open(ACCOUNTFILE, ios::in);
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
		system("cls");
		cout << " ******************************" << endl;
		cout << "添加账目输入1" << endl;
		cout << "删除账目输入2" << endl;
		cout << "打印账单输入3" << endl;
		cout << "清空账本输入4" << endl;
		cout << "分析账单输入5" << endl;
		cout << "退出账本输入0" << endl;
		cout << " ******************************" << endl << endl;


		int choice;
		cout << "输入选项";
		safeCin(choice, 5, 0, 1);
		cout << endl;
		switch (choice)
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
		case 0:
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
	int max = (int)this->accounts.size();
	if (max == 0){
		cout<<"您目前还未存入任何账目！\n";
		return;
	}
	int row;
	cout << "输入需要删除的账目（第几行） " << endl;
	safeCin(row, max, 1, 1);

	this->accounts.erase(accounts.begin()+row-1);
	this->showTable();
}

void account_manager::showTable()
{
	//打印表头
	cout << "账单如下： " << endl;
	cout << setw(12) << "金额" << setw(15) << "收入/支出" << setw(15) << "类别" << endl;
	//打印账目
	for (account a : accounts)
	{
		a.ShowAccount();
	}	
	cout << endl;
	system("pause");
}

void account_manager::save()
{
	ofstream accountFile;
	accountFile.open(ACCOUNTFILE, ios::out);
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
	system("pause");
}

void account_manager::clearAccount()
{
	ofstream accountFile;
	accountFile.open(ACCOUNTFILE, ios::out);
	accountFile.close();
	this->accounts.clear();
	cout << "账本已清空 " << endl << endl;;
	system("pause");
}