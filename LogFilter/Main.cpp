#include"LogFilter.h"
#include<iostream>

int main()
{
	string log_file_path;
	/*cout << "请输入要进行操作的log文件路径:";
	cin >> log_file_path;*/
	log_file_path = "../log.txt";
	LogFilter logfilter(log_file_path);
	bool ret = logfilter.LogFilterInit();
	if (false == ret)
	{
		cout << "Init Error!! Log File Open Error !!" << endl;
		system("pause");
		return 0;
	}

	while (1)
	{
		logfilter.UpdateLogs();
		string input;
		getline(cin, input);
		cout << endl;
		if (input.size() != 0)
		{
			int ret = logfilter.SearchLF(input + "  ");
			if (ret == -1)
				cout << "命令参数有误，请检查后再试！" << endl << endl;
			else
				cout << endl << "共计 " << ret << " 条结果!!" << endl << endl;
		}
	}
	
	return 0;
}
