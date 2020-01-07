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
		void add_to_system_log() const;
	public:
		std::string user_id , passwd , name;
		int authority;

		Visitor (){authority = 0;};

		Visitor (std::string , std::string = "" , std::string = "" , int = 0);

		virtual void buy(std::string , int) const {invalid();}

		virtual void modify(std::string , std::string , std::string , std::string , double) const {invalid();}

		virtual void show(int = 0 , std::string = "") const {invalid();}

		virtual void show_finance(int) const {invalid();}

		virtual void import(int , double) const {invalid();}

		virtual void select(std::string) const {invalid();}

		virtual void useradd(std::string , std::string , std::string , int) const {invalid();}

		virtual void _passwd(std::string , std::string , std::string) const {invalid();}

		virtual void del(std::string) const {invalid();}

		virtual void report(int) const {invalid();}

		virtual void log() const {invalid();}

		virtual void reg(std::string , std::string , std::string) const;

		~Visitor (){}
	}visitor;

	class User : public Visitor
	{
	private:
		void add_to_shopping_list(std::string) const;
	public:
		User(){authority = 1;}

		User (std::string , std::string = "" , std::string = "" , int = 1);

		virtual void buy(std::string , int) const override;

		virtual void show(int = 0 , std::string = "") const override;

		virtual void _passwd(std::string , std::string , std::string) const override;

		~User (){}
	}user;

	class Admin : public User
	{
	private:
		virtual void add_to_operating_list(std::string) const;

		virtual void report_work() const;
	public:
		Admin(){authority = 3;}

		Admin (std::string , std::string = "" , std::string = "" , int = 3);

		virtual void useradd(std::string , std::string , std::string , int) const override;

		virtual void select(std::string) const override;

		virtual void modify(std::string , std::string , std::string , std::string , double) const override;

		virtual void import(int , double) const override;

		virtual void report(int) const override;

		~Admin (){}
	}admin;

	class Root : public Admin
	{
	private:
	public:
		Root(){authority = 7;}

		Root (std::string , std::string = "" , std::string = "" , int = 7);

		virtual void show_finance(int) const override;

		virtual void del(std::string) const override;

		virtual void log() const override;

		~Root (){}
	}root;

	static void add_to_finance(bool , double);

	void su(std::string , std::string);

	static Visitor *current_user;

	static book_list books;

public:
	bookstore();

	bool command(int argc, char *argv[]);

	~bookstore(){}
};


#endif //CODE_BOOKSTORE_H
