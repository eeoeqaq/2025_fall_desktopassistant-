#include "account.h"
#include "reminder.h"
#include <climits>
int safeCin(int& input, int max=INT_MAX, int min=INT_MIN, int type=2)
{
	
	while (type == 1)
	{
		if(cin>>input)
		for (int i = min; i <= max; i++)
		{
			if (input == i) return input;
		}
		cout << "输入非法，请重新输入" << endl;
	}
	
	while (type == 2)
	{
		
		if ((cin >> input) && input >= min && input <= max)
			return input;
		else  cout << "输入非法，请重新输入" << endl;
	}

}
