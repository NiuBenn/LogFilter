#pragma once
#include<iostream>

int MenuSelect()
{
	int i;
	do {
		system("cls");
		std::cout << "\t\t�q����������-���������������������r\n";
		std::cout << "\t\t��      ��  ־  ��  ��  ��       ��\n";
		std::cout << "\t\t�t����������������������������-���s\n";
		std::cout << "\t\t������������������������������������\n";
		std::cout << "\t\t��     1. ����ʱ��ɸѡ����        ��\n";
		std::cout << "\t\t��                                ��\n";
		std::cout << "\t\t��     2. ����������Ϣɸѡ����    ��\n";
		std::cout << "\t\t��                                ��\n";
		std::cout << "\t\t��     0. �˳�ϵͳ                ��\n";
		std::cout << "\t\t������������������������������������\n";
		std::cout << "\t\tPlease choose(0-2):";
		std::cin >> i;
	} while (i < 0 || i>2);
	return i;
}
