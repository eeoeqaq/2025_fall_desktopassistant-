//宏定义保护，防止重复依赖
#pragma once

#include<iostream>
#include<string>

using namespace std;

//声明单次账目类
class account
{

	//金额
	int money;
	//区分支出收入的状态，1为收入，2为支出
	bool state;


};
//声明收入支出两个子类；
class income :public account
{//收入类型(结构体)
	int type;
};
class coutcome :public account
{
	//支出类型(结构体)
	int type;
};

//声明中枢管理类
class manager
{
public:
	//展示首页菜单
	void showHomePage();
	//保存单次写入的账目(把账目类的对象作为参数传入保存函数中)
	void save(const account& A);
	//退出程序
	void exitProgram();
	//展示账目表格
	void showTable();

};
