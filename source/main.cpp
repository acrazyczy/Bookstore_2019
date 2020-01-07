//
// Created by crazy_cloud on 2020/1/3.
//

#include "bookstore.h"
#include <iostream>
#include <fstream>
#include <cstring>

int main()
{
	char *cmd = new char [1000];int argc = 0 , qut = 0;char *argv[100];
	int tot = 0;bool src = 1;std::ifstream cmds;
	bookstore Douban;
	for (Douban.command(-1 , argv , src , cmds);!tot || Douban.command(argc , argv , src , cmds);++ tot)
	{
		if (!src)
		{
			if (cmds.eof()) cmds.close() , src = 1;
			cmds.getline(cmd , 999);
		}
		if (src) std::cin.getline(cmd , 999);
		int len = strlen(cmd);argc = 0;
		cmd[len ++] = ' ' , cmd[len] = '\0';
		bool flag = 1;
		for (char *c = cmd;*c != '\0';++ c)
			if (*c == ' ' && !qut) *c = '\0' , flag = 1;
			else if (*c == '\"') qut ^= 1;
			else if (flag) argv[argc ++] = c , flag = 0;
	}
	if (cmds) cmds.close();
	delete [] cmd;
	return 0;
}