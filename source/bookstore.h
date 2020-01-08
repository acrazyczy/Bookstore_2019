//
// Created by crazy_cloud on 2020/1/5.
//

#ifndef CODE_BOOKSTORE_H
#define CODE_BOOKSTORE_H

#include "constraint.h"
#include "book_list.h"

class bookstore
{
private:
	static void invalid(){std::cout << "Invalid" << std::endl;};

	class Visitor
	{
	private:
	public:
		std::string user_id , passwd , name;
		int authority;

		Visitor (){authority = 0;};

		Visitor (std::string , std::string = "" , std::string = "" , int = 0);

		virtual void buy(const std::string& , const int&) const {invalid();}

		virtual void modify(const std::string& , const std::string& , const std::string& , const std::string& , const double&) const {invalid();}

		virtual void show(int = 0 , std::string = "") const {invalid();}

		virtual void show_finance(int = -1) const {invalid();}

		virtual void import(const int& , const double&) const {invalid();}

		virtual void select(const std::string&) const {invalid();}

		virtual void useradd(const std::string& , const std::string& , const std::string& , const int&) const {invalid();}

		virtual void _passwd(const std::string& , const std::string& , const std::string&) const {invalid();}

		virtual void del(const std::string&) const {invalid();}

		virtual void report(const int&) const {invalid();}

		virtual void log() const {invalid();}

		virtual void reg(const std::string& , const std::string& , const std::string&) const;

		~Visitor (){}
	}visitor;

	class User : public Visitor
	{
	public:
		User(){authority = 1;}

		User (std::string , std::string = "" , std::string = "" , int = 1);

		virtual void buy(const std::string& , const int&) const override;

		virtual void show(int = 0 , std::string = "") const override;

		virtual void _passwd(const std::string& , const std::string& , const std::string&) const override;

		~User (){}
	}user;

	class Admin : public User
	{
	private:

		virtual void report_work() const;
	public:
		Admin(){authority = 3;}

		Admin (std::string , std::string = "" , std::string = "" , int = 3);

		virtual void useradd(const std::string& , const std::string& , const std::string& , const int&) const override;

		virtual void select(const std::string&) const override;

		virtual void modify(const std::string& , const std::string& , const std::string& , const std::string& , const double&) const override;

		virtual void import(const int& , const double&) const override;

		virtual void report(const int&) const override;

		~Admin (){}
	}admin;

	class Root : public Admin
	{
	private:
	public:
		Root(){authority = 7;}

		Root (std::string , std::string = "" , std::string = "" , int = 7);

		virtual void show_finance(int = -1) const override;

		virtual void del(const std::string&) const override;

		virtual void log() const override;

		~Root (){}
	}root;

	static void add_to_finance(const bool& , const double&);

	static void add_to_operating_list(const std::string&);

	static void add_to_system_log(const std::string&);

	static void add_to_shopping_list(const std::string&);

	void su(const std::string& , const std::string&);

	static Visitor *current_user;

	static book_list books;

public:
	bookstore();

	bool command(const int& , char *[] , bool& , std::ifstream&);

	~bookstore(){}
};


#endif //CODE_BOOKSTORE_H
