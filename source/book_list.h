//
// Created by crazy_cloud on 2020/1/6.
//

#ifndef CODE_BOOK_LIST_H
#define CODE_BOOK_LIST_H

#include "constraint.h"
#include "hash_table.h"

class book_list
{
private:
	static hash_table name_table , author_table , keyword_table;
	std::string selected;

	class book
	{
		friend std::ostream &operator<<(std::ostream &os , const book &x)
		{
			os << x.ISBN << "\t" << x.name << "\t" << x.author << "\t" << x.keyword << "\t" << convert_double_to_string(x.price) << "\t" << x.quantity << "本" << std::endl;
			return os;
		}
	private:
	public:
		std::string ISBN , name , author , keyword;
		double price;
		int quantity;

		book(std::string , std::string , std::string , std::string , double , int);

		~book(){}
	};

	static void add_to_hash_tables(book);

	static void remove_from_hash_tables(book);
public:
	book_list();

	double buy(std::string , int) const;

	void display() const;

	void find(int , std::string) const;

	void select(std::string);

	bool modify(std::string , std::string , std::string , std::string , double);

	bool import(int) const;

	~book_list(){}
};


#endif //CODE_BOOK_LIST_H
