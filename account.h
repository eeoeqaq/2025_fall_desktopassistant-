//宏定义保护，防止重复依赖
#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<limits>

using namespace std;

//声明单次账目类
class account
{

	//金额
	int money;
	//区分支出收入的状态，1为收入，2为支出
	int state;
	//12为收入类型：工资，其他收入；345为支出类型：吃，用，玩
	int type;
	
public:
	
	//填写单条账目
	void setAccount();
	//展示单条账目
	void ShowAccount();
	//单条账目转为字符串，便于写文件
	string getAccount();
	//从文件中读字符串转入一条账目,借用构造函数实现
	account(int money,int state,int type);
	//补默认构造函数
	account()
	{
		this->money = -1;
		this->state = -1;
		this->type = -1;
	}
	

};

//提供分析报告类
class analysis
{

public:
	void start(vector<account> accounts);

	//总结收入支出
	void stateAnalysis(vector<vector<int>> arrayAccounts);
	//总结收入下子类型
	void incomeType(vector<vector<int>> arrayAccounts);
	//总结支出下子类型
	void expenseType(vector<vector<int>> arrayAccounts);
	
};

//账单管理类
class account_manager
{//储存所有账目

	vector<account> accounts;
	
public:
	//开启程序
	void Start();
	//首页菜单模块
	void HomePage();
	//写入一条账目
	void WriteAccount();
	//删除一条账目
	void DeleteAccount();
	//展示账目表格
	void showTable();
	//保存所有账目
	void save();
	//提供分析报告
	void analysisAccount();
	//清空账本
	void clearAccount();
};

//安全输入函数
//type=1,在max和min之间的整数是合法的
//type=2，在max和min之间的实数是合法的
int safeCin(int& input, int max, int min, int type);



