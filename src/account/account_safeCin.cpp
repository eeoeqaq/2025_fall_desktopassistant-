#include <windows.h>

#include "account.h"
#include "reminder.h"
#include "pomodoro.h"

int safeCin(int& input, int max, int min, int type)
{
	
	while (type == 1)
	{
		if(cin>>input)
		for (int i = min; i <= max; i++)
		{
			if (input == i) return input;
		}
		cout << "输入非法，请重新输入" << endl;
		
		cin.clear();
    	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	
	while (type == 2)
	{
		if ((cin >> input) && input >= min && input <= max)
			return input;
		else  cout << "输入非法，请重新输入" << endl;
		
		cin.clear();
    	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

