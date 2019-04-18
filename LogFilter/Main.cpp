#include"LogFilter.h"
#include<iostream>

int main()
{
	string log_file_path;
	/*cout << "������Ҫ���в�����log�ļ�·��:";
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
				cout << "�������������������ԣ�" << endl << endl;
			else
				cout << endl << "���� " << ret << " �����!!" << endl << endl;
		}
	}
	
	return 0;
}
