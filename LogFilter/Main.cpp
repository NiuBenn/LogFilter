#include"LogFilter.h"
#include<iostream>
#include<string.h>

int main(int argc, const char* argv[])
{
	list<Filter*> filter_list = Tools::CreatFilterList(argc, argv);

	bool ret = Tools::IsFilePathOK(argv[1]);
	if (ret == false)
	{
		Tools::ReadLogAndParse(std::cin, filter_list);
	}
	else
	{
		std::ifstream log_file(argv[1]);
		Tools::ReadLogAndParse(log_file, filter_list);
	}

	for (auto& filter : filter_list)
	{
		delete filter;
	}

	return 0;
}
