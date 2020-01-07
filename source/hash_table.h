//
// Created by crazy_cloud on 2020/1/5.
//

#ifndef CODE_HASH_TABLE_H
#define CODE_HASH_TABLE_H

#include "constraint.h"

class hash_table
{
private:
	std::string folder_name;

	static const int ncnt = 15;
	static const int nsize = 4;
	static const int file_cnt = 1 << ncnt;
	static const int file_size = 1 << nsize;
	static const unsigned int mod = 1 << ncnt + nsize;
	static const unsigned int P = 67;

	unsigned int gethash(std::string) const;

public:
	hash_table(){}

	hash_table(std::string);

	std::vector<std::string> find(std::string) const;

	void insert(std::string , std::string) const;

	void erase(std::string , std::string) const;

	std::vector<std::string> display() const;

	~hash_table(){}
};


#endif //CODE_HASH_TABLE_H
