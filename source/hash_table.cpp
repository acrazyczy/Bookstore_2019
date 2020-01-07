//
// Created by crazy_cloud on 2020/1/5.
//

#include "hash_table.h"

unsigned int hash_table::gethash(std::string str) const
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

std::vector<std::string> hash_table::find(std::string target) const
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
			char key[max_len] , value[max_len];memset(key , 0 , sizeof key) , memset(value , 0 , sizeof value);
			in.read(reinterpret_cast<char *> (&key) , sizeof key) , in.read(reinterpret_cast<char *> (&value) , sizeof value);
			if (std::string(key) == target) ret.push_back(std::string(value));
		}
	}
	std::sort(ret.begin() , ret.end());
	return ret;
}

void hash_table::insert(std::string t_key , std::string t_value) const
{
	unsigned int hash_value = gethash(t_key);
	std::string file_path = "" + folder_name + "_hash_table_" + std::to_string(hash_value >> nsize) + ".dat";
	std::ifstream in , tmp_;std::ofstream out , tmp;
	in.open(file_path , ios::binary) , tmp.open("" + folder_name + "_tmp.dat" , ios::binary);
	if (!in)
	{
		out.open(file_path , ios::binary);
		for (int i = 0 , x = 0;i < file_size;++ i) out.write(reinterpret_cast<char *> (&x) , sizeof (int));
		out.close() , in.open(file_path , ios::binary);
	}
	char key[max_len] , value[max_len];memset(key , 0 , sizeof key) , memset(value , 0 , sizeof value);
	for (int i = 0 , tot;i < file_size;++ i)
	{
		in.read(reinterpret_cast<char *> (&tot) , sizeof (int)) , tmp.write(reinterpret_cast<char *> (&tot) , sizeof (int));
		for (int j = 0;j < tot;++ j)
		{
			in.read(reinterpret_cast<char *> (&key) , sizeof key) , in.read(reinterpret_cast<char *> (&value) , sizeof key);
			tmp.write(reinterpret_cast<char *> (&key) , sizeof key) , tmp.write(reinterpret_cast<char *> (&value) , sizeof value);
		}
	}
	in.close() , tmp.close();
	out.open(file_path , ios::binary) , tmp_.open("" + folder_name + "_tmp.dat" , ios::binary);
	for (int i = 0 , tot;i < file_size;++ i)
	{
		tmp_.read(reinterpret_cast<char *> (&tot) , sizeof (int));
		tot += i == (hash_value & (file_size - 1));
		out.write(reinterpret_cast<char *> (&tot) , sizeof (int));
		tot -= i == (hash_value & (file_size - 1));
		for (int j = 0;j < tot;++ j)
		{
			tmp_.read(reinterpret_cast<char *> (&key) , sizeof key) , tmp_.read(reinterpret_cast<char *> (&value) , sizeof key);
			out.write(reinterpret_cast<char *> (&key) , sizeof value) , out.write(reinterpret_cast<char *> (&value) , sizeof value);
		}
		if (i == (hash_value & (file_size - 1)))
		{
			strcpy(key , t_key.c_str()) , strcpy(value , t_value.c_str());
			out.write(reinterpret_cast<char *> (&key) , sizeof key) , out.write(reinterpret_cast<char *> (&value) , sizeof value);
		}
	}
	out.close() , tmp_.close();
}

void hash_table::erase(std::string t_key , std::string t_value) const
{
	unsigned int hash_value = gethash(t_key);
	std::string file_path = "" + folder_name + "_hash_table_" + std::to_string(hash_value >> nsize) + ".dat";
	std::ifstream in , tmp_;std::ofstream out , tmp;
	in.open(file_path , ios::binary) , tmp.open("" + folder_name + "_tmp.dat" , ios::binary) , assert(in);
	char key[max_len] , value[max_len];memset(key , 0 , sizeof key) , memset(value , 0 , sizeof value);
	for (int i = 0 , tot;i < file_size;++ i)
	{
		in.read(reinterpret_cast<char *> (&tot) , sizeof (int)) , tmp.write(reinterpret_cast<char *> (&tot) , sizeof (int));
		for (int j = 0;j < tot;++ j)
		{
			in.read(reinterpret_cast<char *> (&key) , sizeof key) , in.read(reinterpret_cast<char *> (&value) , sizeof value);
			tmp.write(reinterpret_cast<char *> (&key) , sizeof key) , tmp.write(reinterpret_cast<char *> (&value) , sizeof value);
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
			tmp_.read(reinterpret_cast<char *> (&key) , sizeof key) , tmp_.read(reinterpret_cast<char *> (&value) , sizeof value);
			if (i == (hash_value & (file_size - 1)) && std::string(value) == t_value && flag)
			{
				flag = 0;
				continue;
			}
			out.write(reinterpret_cast<char *> (&key) , sizeof key) , out.write(reinterpret_cast<char *> (&value) , sizeof value);
		}
	}
	out.close() , tmp_.close();
}

std::vector<std::string> hash_table::display() const
{
	std::vector<std::string> ret;
	char key[max_len] , value[max_len];memset(key , 0 , sizeof key) , memset(value , 0 , sizeof value);
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
					in.read(reinterpret_cast<char *> (&key) , sizeof key) , in.read(reinterpret_cast<char *> (&value) , sizeof value);
					ret.push_back(std::string(value));
				}
			}
			in.close();
		}
	}
	sort(ret.begin() , ret.end());
	return ret;
}