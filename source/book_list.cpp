//
// Created by crazy_cloud on 2020/1/6.
//

#include "book_list.h"

hash_table book_list::name_table = hash_table("name_table");
hash_table book_list::author_table = hash_table("author_table");
hash_table book_list::keyword_table = hash_table("keyword_table");

book_list::book_list()
{
	selected = "";
	std::ifstream in;std::ofstream out;
	in.open("book_list_tmp.dat" , ios::binary);
	if (!in) out.open("book_list_tmp.dat" , ios::binary) , out.close();
	else in.close();
}

void book_list::add_to_hash_tables(const book &bk , const int &state)
{
	if (!(state & 1)) name_table.insert(bk.name , bk.ISBN);
	if (!((state >> 1) & 1)) author_table.insert(bk.author , bk.ISBN);
	if (state >> 2) return;
	char *cstr = new char [max_len];
	int len = 0;std::set<std::string> bin;std::string tmp;
	for (std::string::const_iterator it = bk.keyword.begin();it != bk.keyword.end();++ it)
		if (*it == '|')
		{
			cstr[len ++] = '\0' , tmp = std::string(cstr);
			if (bin.find(tmp) == bin.end()) keyword_table.insert(tmp , bk.ISBN) , len = 0 , bin.insert(tmp);
		}
		else cstr[len ++] = *it;
	cstr[len ++] = '\0' , tmp = std::string(cstr);
	if (bin.find(tmp) == bin.end()) keyword_table.insert(tmp , bk.ISBN) , len = 0 , bin.insert(tmp);
	delete [] cstr;
}

void book_list::remove_from_hash_tables(const book &bk , const int &state)
{
	if (!(state & 1)) name_table.erase(bk.name , bk.ISBN);
	if (!((state >> 1) & 1)) author_table.erase(bk.author , bk.ISBN);
	if (state >> 2) return;
	char *cstr = new char [max_len];
	int len = 0;std::set<std::string> bin;std::string tmp;
	for (std::string::const_iterator it = bk.keyword.begin();it != bk.keyword.end();++ it)
		if (*it == '|')
		{
			cstr[len ++] = '\0' , tmp = std::string(cstr);
			if (bin.find(tmp) == bin.end()) keyword_table.erase(std::string(cstr) , bk.ISBN) , len = 0 , bin.insert(tmp);
		}
		else cstr[len ++] = *it;
	cstr[len ++] = '\0' , tmp = std::string(cstr);
	if (bin.find(tmp) == bin.end()) keyword_table.erase(std::string(cstr) , bk.ISBN) , len = 0 , bin.insert(tmp);
	delete [] cstr;
}

book_list::book::book(std::string ISBN_ , std::string name_ , std::string author_ , std::string keyword_ , double price_ , int quantity_ , int state)
{
	ISBN = ISBN_;
	std::string file_path = "book_list_" + ISBN + ".dat";
	std::ifstream in;std::ofstream out;
	in.open(file_path , ios::binary);
	char cstr[max_len];memset(cstr , 0 , sizeof cstr);
	if (!in)
	{
		out.open(file_path , ios::binary);
		name = name_ , strcpy(cstr , name.c_str()) , out.write(reinterpret_cast<char *> (&cstr) , sizeof cstr);
		author = author_ , strcpy(cstr , author.c_str()) , out.write(reinterpret_cast<char *> (&cstr) , sizeof cstr);
		keyword = keyword_ , strcpy(cstr , keyword.c_str()) , out.write(reinterpret_cast<char *> (&cstr) , sizeof cstr);
		price = price_ , out.write(reinterpret_cast<char *> (&price) , sizeof (double));
		quantity = quantity_ , out.write(reinterpret_cast<char *> (&quantity) , sizeof (int));
		out.close();
		add_to_hash_tables(*this , state);
	}
	else
	{
		in.read(reinterpret_cast<char *> (&cstr) , sizeof cstr) , name = std::string(cstr);
		in.read(reinterpret_cast<char *> (&cstr) , sizeof cstr) , author = std::string(cstr);
		in.read(reinterpret_cast<char *> (&cstr) , sizeof cstr) , keyword = std::string(cstr);
		in.read(reinterpret_cast<char *> (&price) , sizeof (double));
		in.read(reinterpret_cast<char *> (&quantity) , sizeof (int));
	}
}


double book_list::buy(const std::string &ISBN_ , const int &quantity_) const
{
	std::string file_path = "book_list_" + ISBN_ + ".dat";
	std::ifstream in(file_path , ios::binary);
	if (!in) return -1;
	in.close();
	book target(ISBN_);
	if (target.quantity < quantity_) return -1;
	target.quantity -= quantity_;
	std::ofstream out(file_path , ios::binary);
	char cstr[max_len];memset(cstr , 0 , sizeof cstr);
	strcpy(cstr , target.name.c_str()) , out.write(reinterpret_cast<char *> (&cstr) , sizeof cstr);
	strcpy(cstr , target.author.c_str()) , out.write(reinterpret_cast<char *> (&cstr) , sizeof cstr);
	strcpy(cstr , target.keyword.c_str()) , out.write(reinterpret_cast<char *> (&cstr) , sizeof cstr);
	out.write(reinterpret_cast<char *> (&target.price) , sizeof (double));
	out.write(reinterpret_cast<char *> (&target.quantity) , sizeof (int));
	out.close();
	return quantity_ * target.price;
}

void book_list::display() const
{
	std::vector<std::string> ret = name_table.display();
	for (std::string ISBN : ret)
	{
		std::ifstream in("book_list_" + ISBN + ".dat" , ios::binary);
		if (in) in.close() , std::cout << book(ISBN);
	}
}

void book_list::find(const int &key_type , const std::string &key) const
{
	std::ifstream in;
	if (key_type == 1)
	{
		in.open("book_list_" + key + ".dat" , ios::binary);
		if (in) in.close() , std::cout << book(key);
		return;
	}
	std::vector<std::string> ret;
	ret = key_type == 2 ? name_table.find(key) : (key_type == 3 ? author_table.find(key) : keyword_table.find(key));
	for (std::string ISBN : ret)
	{
		std::ifstream in("book_list_" + ISBN + ".dat" , ios::binary);
		if (in) in.close() , std::cout << book(ISBN);
	}
}

void book_list::select(const std::string &ISBN_) {book(selected = ISBN_);}

bool book_list::modify(const std::string &ISBN_ , const std::string &name_ , const std::string &author_ , const std::string &keyword_ , const double &price_)
{
	if (selected.empty()) return 0;
	if (!ISBN_.empty())
	{
		std::ifstream in("book_list_" + ISBN_ + ".dat" , ios::binary);
		if (in)
		{
			in.close();
			return 0;
		}
	}
	book bk(selected);
	int state = (ISBN_ == bk.ISBN) * (((name_ == bk.name) << 0) | ((author_ == bk.author) << 1) | ((keyword_ == bk.keyword) << 2));
	remove(("book_list_" + selected + ".dat").c_str()) , remove_from_hash_tables(bk , state);
	book nbk(selected = ISBN_.empty() ? bk.ISBN : ISBN_ , name_.empty() ? bk.name : name_ , author_.empty() ? bk.author : author_ , keyword_.empty() ? bk.keyword : keyword_ , price_ == -1 ? bk.price : price_ , bk.quantity , state);
	return 1;
}

bool book_list::import(const int &quantity_) const
{
	if (selected.empty()) return 0;
	std::string file_path = "book_list_" + selected + ".dat";
	book target(selected);
	target.quantity += quantity_;
	std::ofstream out(file_path , ios::binary);
	char cstr[max_len];memset(cstr , 0 , sizeof cstr);
	strcpy(cstr , target.name.c_str()) , out.write(reinterpret_cast<char *> (&cstr) , sizeof cstr);
	strcpy(cstr , target.author.c_str()) , out.write(reinterpret_cast<char *> (&cstr) , sizeof cstr);
	strcpy(cstr , target.keyword.c_str()) , out.write(reinterpret_cast<char *> (&cstr) , sizeof cstr);
	out.write(reinterpret_cast<char *> (&target.price) , sizeof (double));
	out.write(reinterpret_cast<char *> (&target.quantity) , sizeof (int));
	out.close();
	return 1;
}