#pragma once
#include<iostream>

int MenuSelect()
{
	int i;
	do {
		system("cls");
		std::cout << "\t\t╭—————-—■□■□—————╮\n";
		std::cout << "\t\t│      日  志  过  滤  器       │\n";
		std::cout << "\t\t╰———■□■□———————-—╯\n";
		std::cout << "\t\t┌————————————————┐\n";
		std::cout << "\t\t│     1. 按照时间筛选过滤        │\n";
		std::cout << "\t\t│                                │\n";
		std::cout << "\t\t│     2. 按照其他信息筛选过滤    │\n";
		std::cout << "\t\t│                                │\n";
		std::cout << "\t\t│     0. 退出系统                │\n";
		std::cout << "\t\t└————————————————┘\n";
		std::cout << "\t\tPlease choose(0-2):";
		std::cin >> i;
	} while (i < 0 || i>2);
	return i;
}
