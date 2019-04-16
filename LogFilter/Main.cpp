#include"LogFilter.h"
#include"Menu.h"
#include<iostream>

int main()
{
	string log_file_path;
	/*cout << "请输入要进行操作的log文件路径:";
	cin >> log_file_path;*/
	log_file_path = "log.txt";
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
		string input;
		getline(cin, input);

	}
	
	return 0;
}