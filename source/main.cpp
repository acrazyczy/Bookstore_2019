//
// Created by crazy_cloud on 2020/1/3.
//

#include "bookstore.h"
#include <iostream>
#include <fstream>
#include <cstring>

int main()
{
	std::ifstream in("/mnt/d/Academy/OIACM/Code/Homework/Bookstore_2019/bookstore/test_data/testcase6.txt");
	std::cin.rdbuf(in.rdbuf());
	char *cmd = new char [1000];int argc = 0 , qut = 0;char *argv[100];
	int tot = 0;
	for (bookstore Douban;!tot || Douban.command(argc , argv);++ tot)
	{
		in.getline(cmd , 999 , '\r') , in.get();
		int len = strlen(cmd);argc = 0;
		cmd[len ++] = ' ' , cmd[len] = '\0';
		bool flag = 1;
		for (char *c = cmd;*c != '\0';++ c)
			if (*c == ' ' && !qut) *c = '\0' , flag = 1;
			else if (*c == '\"') qut ^= 1;
			else if (flag) argv[argc ++] = c , flag = 0;
	}
	delete [] cmd;
	return 0;
}