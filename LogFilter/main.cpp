#include"LogFilter.h"
#include"Tools.hpp"
#include<iostream>

int main()
{
	LogFilter logfilter("log.txt");
	bool ret = logfilter.LogFilterInit();
	if (false == ret)
	{
		cout << "Init Error!!" << endl;
		return 0;
	}

	ret = logfilter.UpdateLogs();

	return 0;
}