//
// Created by crazy_cloud on 2020/1/5.
//

#include "hash_table.h"

unsigned int hash_table::gethash(const std::string &str) const
{
	char *cstr = new char [str.length() + 1];
  	std::strcpy(cstr, str.c_str());
	unsigned int ret = 0;
	for (char *c = cstr;*c != '\0';++ c) ((ret *= P) += ((unsigned int)(*c) + 1)) %= mod;
	delete [] cstr;
	return ret;
}

hash_table::hash_table(std::string name)
{
	folder_name = name;
	std::ifstream in;std::ofstream out;
	in.open("" + folder_name + "_tmp.dat" , ios::binary);
	if (in) in.close();
	else out.open("" + folder_name + "_tmp.dat" , ios::binary),out.close();
}

std::vector<std::string> hash_table::find(const std::string &target) const
{
	std::vector<std::string> ret;
	unsigned int hash_value = gethash(target);
	std::string file_path = "" + folder_name + "_hash_table_" + std::to_string(hash_value >> nsize) + ".dat";
	std::ifstream in(file_path , ios::binary);
	if (in) for (int i = 0 , tot;i < file_size;++ i)
	{
		in.read(reinterpret_cast<char *> (&tot) , sizeof (int));
		for (int j = 0;j < tot;++ j)
		{
			if ((hash_value & (file_size - 1)) != i)
			{
				in.seekg(max_len << 1, ios::cur);
				continue;
			}
			char key[max_len] , value[max_len];memset(key , 0 , max_len) , memset(value , 0 , max_len);
			in.read(reinterpret_cast<char *> (&key) , max_len) , in.read(reinterpret_cast<char *> (&value) , max_len);
			if (std::string(key) == target) ret.push_back(std::string(value));
		}
	}
	std::sort(ret.begin() , ret.end());
	return ret;
}

void hash_table::insert(const std::string &t_key , const std::string &t_value) const
{
	unsigned int hash_value = gethash(t_key);
	std::string file_path = "" + folder_name + "_hash_table_" + std::to_string(hash_value >> nsize) + ".dat";
	std::ifstream in;std::ofstream out;
	in.open(file_path , ios::binary);
	if (!in)
	{
		out.open(file_path , ios::binary);
		for (int i = 0 , x = 0;i < file_size;++ i) out.write(reinterpret_cast<char *> (&x) , sizeof (int));
		out.close();
	}else in.close();
	std::fstream file(file_path , ios::binary | ios::in | ios::out);int tot;
	file.seekg(0) , file.read(reinterpret_cast <char *> (&tot) , sizeof (int)) , ++ tot , file.seekp(0) , file.write(reinterpret_cast <char *> (&tot) , sizeof (int));
	char key[max_len] , value[max_len];memset(key , 0 , max_len) , memset(value , 0 , max_len);
	strcpy(key , t_key.c_str()) , strcpy(value , t_value.c_str());
	file.seekp(0 , ios::end) , file.write(reinterpret_cast<char *> (&key) , max_len) , file.write(reinterpret_cast<char *> (&value) , max_len);
	file.close();
}

void hash_table::erase(const std::string &t_key , const std::string &t_value) const
{
	unsigned int hash_value = gethash(t_key);
	std::string file_path = "" + folder_name + "_hash_table_" + std::to_string(hash_value >> nsize) + ".dat";
	std::ifstream in , tmp_;std::ofstream out , tmp;
	in.open(file_path , ios::binary) , tmp.open("" + folder_name + "_tmp.dat" , ios::binary) , assert(in);
	char key[max_len] , value[max_len];memset(key , 0 , max_len) , memset(value , 0 , max_len);
	for (int i = 0 , tot;i < file_size;++ i)
	{
		in.read(reinterpret_cast<char *> (&tot) , sizeof (int)) , tmp.write(reinterpret_cast<char *> (&tot) , sizeof (int));
		for (int j = 0;j < tot;++ j)
		{
			in.read(reinterpret_cast<char *> (&key) , max_len) , in.read(reinterpret_cast<char *> (&value) , max_len);
			tmp.write(reinterpret_cast<char *> (&key) , max_len) , tmp.write(reinterpret_cast<char *> (&value) , max_len);
		}
	}
	in.close() , tmp.close();
	out.open(file_path , ios::binary) , tmp_.open("" + folder_name + "_tmp.dat" , ios::binary);
	strcpy(key , t_key.c_str()) , strcpy(value , t_value.c_str());
	for (int i = 0 , tot , flag = 1;i < file_size;++ i)
	{
		tmp_.read(reinterpret_cast<char *> (&tot) , sizeof (int));
		tot -= i == (hash_value & (file_size - 1));
		out.write(reinterpret_cast<char *> (&tot) , sizeof (int));
		tot += i == (hash_value & (file_size - 1));
		for (int j = 0;j < tot;++ j)
		{
			tmp_.read(reinterpret_cast<char *> (&key) , max_len) , tmp_.read(reinterpret_cast<char *> (&value) , max_len);
			if (i == (hash_value & (file_size - 1)) && std::string(value) == t_value && flag)
			{
				flag = 0;
				continue;
			}
			out.write(reinterpret_cast<char *> (&key) , max_len) , out.write(reinterpret_cast<char *> (&value) , max_len);
		}
	}
	out.close() , tmp_.close();
}

std::vector<std::string> hash_table::display() const
{
	std::vector<std::string> ret;
	char key[max_len] , value[max_len];memset(key , 0 , max_len) , memset(value , 0 , max_len);
	for (int i = 0 , tot;i < file_cnt;++ i)
	{
		std::ifstream in("" + folder_name + "_hash_table_" + std::to_string(i) + ".dat" , ios::binary);
		if (in)
		{
			for (int j = 0;j < file_size;++ j)
			{
				in.read(reinterpret_cast<char *> (&tot) , sizeof (int));
				for (int k = 0;k < tot;++ k)
				{
					in.read(reinterpret_cast<char *> (&key) , max_len) , in.read(reinterpret_cast<char *> (&value) , max_len);
					ret.push_back(std::string(value));
				}
			}
			in.close();
		}
	}
	sort(ret.begin() , ret.end());
	return ret;
}