//
// Created by crazy_cloud on 2020/1/5.
//

#include "bookstore.h"

bookstore::Visitor *bookstore::current_user;
book_list bookstore::books;

bookstore::bookstore()
{
	std::ifstream in;std::ofstream out;
	in.open("report_finance.dat" , ios::binary);
	if (!in)
	{
		out.open("report_finance.dat" , ios::binary);
		double x = 0;
		out.write(reinterpret_cast<char *> (&x) , sizeof (double));
		out.write(reinterpret_cast<char *> (&x) , sizeof (double));
		int y = 0;
		out.write(reinterpret_cast<char *> (&y) , sizeof (int));
		out.close();
	}
	else in.close();
	in.open("report_system_log.dat" , ios::binary);
	if (!in) out.open("report_system_log.dat" , ios::binary) , out.close();
	else in.close();
	root = Root("root" , "sjtu" , "root") , current_user = &root;
}

bookstore::Visitor::Visitor(std::string user_id_ , std::string passwd_ , std::string name_ , int authority_)
{
	user_id = user_id_;
	std::ifstream in;std::ofstream out;
	in.open(std::string("user_" + user_id + ".dat") , ios::binary);
	char cstr[max_len];memset(cstr , 0 , sizeof cstr);
	if (!in)
	{
		passwd = passwd_ , name = name_ , authority = authority_;
		out.open(std::string("user_" + user_id + ".dat") , ios::binary);
		strcpy(cstr , passwd.c_str()) , out.write(reinterpret_cast<char *> (&cstr) , sizeof (cstr));
		strcpy(cstr , name.c_str()) , out.write(reinterpret_cast<char *> (&cstr) , sizeof (cstr));
		out.write(reinterpret_cast<char *> (&authority) , sizeof (int));
		out.close();
	}
	else
	{
		in.read(reinterpret_cast<char *> (&cstr) , sizeof (cstr)) , passwd = std::string(cstr);
		in.read(reinterpret_cast<char *> (&cstr) , sizeof (cstr)) , name = std::string(cstr);
		in.read(reinterpret_cast<char *> (&authority) , sizeof (int));
		in.close();
	}
}

void bookstore::Visitor::add_to_system_log() const
{
	//to do
}

void bookstore::Visitor::reg(const std::string &user_id_ , const std::string &passwd_ , const std::string &name_) const
{
	std::ifstream in("user_" + user_id_ + ".dat" , ios::binary);
	if (in) invalid() , in.close();
	else Visitor(user_id_ , passwd_ , name_ , 1);
	//to do : add to system log
}

bookstore::User::User(std::string user_id_ , std::string passwd_ , std::string name_ , int authority_) : Visitor(user_id_ , passwd_ , name_ , authority_) {}

void bookstore::User::add_to_shopping_list(const std::string&) const
{
	//to do
}

void bookstore::User::buy(const std::string &ISBN , const int &quantity) const
{
	double ret = books.buy(ISBN , quantity);
	if (ret == -1.) invalid();
	else add_to_finance(1 , ret);
	//to do : add to system log
	//to do : add to shopping list
	//to do : add to finance
}

void bookstore::User::show(int key_type, std::string key) const
{
	if (!key_type) books.display();
	else books.find(key_type , key);
}

void bookstore::User::_passwd(const std::string &user_id_ , const std::string &old_passwd_ , const std::string &passwd_) const
{
	std::string file_path = "user_" + user_id_ + ".dat";
	std::ifstream in(file_path , ios::binary);
	if (!in) invalid();
	else
	{
		in.close();
		Visitor tmp(user_id_);
		if (current_user -> authority == 7 || tmp.passwd == old_passwd_)
		{
			std::ofstream out(file_path , ios::binary);char cstr[max_len];memset(cstr , 0 , sizeof cstr);
			tmp.passwd = passwd_;
			strcpy(cstr , tmp.passwd.c_str()) , out.write(reinterpret_cast<char *> (&cstr) , sizeof (cstr));
			strcpy(cstr , tmp.name.c_str()) , out.write(reinterpret_cast<char *> (&cstr) , sizeof (cstr));
			out.write(reinterpret_cast<char *> (&tmp.authority) , sizeof (int));
			out.close();
			//to do : add to system_log
		}else invalid();
	}
}

bookstore::Admin::Admin(std::string user_id_ , std::string passwd_ , std::string name_ , int authority_) : User(user_id_ , passwd_ , name_ , authority_) {}

void bookstore::Admin::add_to_operating_list(const std::string&) const
{
	//to do
}

void bookstore::Admin::report_work() const
{
	//to do
}

void bookstore::Admin::useradd(const std::string &user_id_ , const std::string &passwd_ , const std::string &name_ , const int &authority_) const
{
	if (authority_ < authority)
	{
		std::ifstream in("user_" + user_id_ + ".dat" , ios::binary);
		if (in) invalid() , in.close();
		else Visitor(user_id_ , passwd_ , name_ , authority_);
	}
	else invalid();
	//to do : add to system_log
	//to do : add to add_to_operating_list
}

void bookstore::Admin::select(const std::string &ISBN) const
{
	books.select(ISBN);
	//to do : add to system_log
	//to do : add to operating_list
}

void bookstore::Admin::modify(const std::string &ISBN_ , const std::string &name_ , const std::string &author_ , const std::string &keyword_ , const double &price_) const
{
	bool flag = books.modify(ISBN_ , name_ , author_ , keyword_ , price_);
	if (!flag) invalid();
	//to do : add to system_log
	//to do : add to operating_list
}

void bookstore::Admin::import(const int &quantity , const double &cost_price) const
{
	bool flag = books.import(quantity);
	//to do : add to system_log
	//to do : add to operating_list
	if (flag) add_to_finance(0 , cost_price);
	else invalid();
}

void bookstore::Admin::report(const int &rep_type) const
{
	if (rep_type && authority != 7) invalid();
	else switch (rep_type)
	{
		case 0:
			//to do
			break;
		case 1:
			//to do
			break;
		case 2:
			//to do
			break;
	}
}

bookstore::Root::Root(std::string user_id_ , std::string passwd_ , std::string name_ , int authority_) : Admin(user_id_ , passwd_ , name_ , authority_) {}

void bookstore::Root::show_finance(int time) const
{
	std::ifstream in("report_finance.dat" , ios::binary);
	double val[2] = {0 , 0};
	if (~time)
	{
		in.seekg(2 * sizeof (double));
		int tot;in.read(reinterpret_cast<char *> (&tot) , sizeof (int));
		if (tot < time) invalid();
		else
		{
			in.seekg(- time * (sizeof (double) + sizeof (bool)) , ios::end);
			double tmp;bool sig;
			for (int i = 0;i < time;++ i)
			{
				in.read(reinterpret_cast<char *> (&sig) , sizeof (bool)) , in.read(reinterpret_cast<char *> (&tmp) , sizeof (double));
				val[sig] += tmp;
			}
		}
	}
	else in.read(reinterpret_cast<char *> (&val[0]) , sizeof (double)) , in.read(reinterpret_cast<char *> (&val[1]) , sizeof (double));
	std::cout << "+ " << convert_double_to_string(val[1]) << " - " << convert_double_to_string(val[0]) << std::endl;
	in.close();
}

void bookstore::Root::del(const std::string &user_id_) const
{
	if (user_id_ == std::string("root")) invalid();
	else
	{
		std::ifstream in("user_" + user_id_ + ".dat" , ios::binary);
		if (!in) invalid();
		else in.close() , remove(("user_" + user_id_ + ".dat").c_str());
		//to do : remove work report
		//to do : add to system_log
		//to do : add to operating_list
	}
}

void bookstore::Root::log() const
{
	//to do
}

void bookstore::add_to_finance(const bool &flag , const double &money)
{
	std::fstream file("report_finance.dat" , ios::binary | ios::in | ios::out);
	file.seekg(0);
	double val[2];
	file.read(reinterpret_cast<char *> (&val[0]) , sizeof (double));
	file.read(reinterpret_cast<char *> (&val[1]) , sizeof (double));
	val[flag] += money;
	int tot;
	file.read(reinterpret_cast<char *> (&tot) , sizeof (int));
	++ tot;
	file.seekp(0);
	file.write(reinterpret_cast<char *> (&val[0]) , sizeof (double));
	file.write(reinterpret_cast<char *> (&val[1]) , sizeof (double));
	file.write(reinterpret_cast<char *> (&tot) , sizeof (int));
	file.seekp(0 , ios::end);
	file.write(reinterpret_cast<const char *> (&flag) , sizeof (bool));
	file.write(reinterpret_cast<const char *> (&money) , sizeof (double));
	file.close();
}

void bookstore::su(const std::string &user_id_ , const std::string &passwd_)
{
	std::ifstream in(std::string("user_" + user_id_ + ".dat") , ios::binary);
	if (!in) invalid();
	else
	{
		in.close();
		Visitor tmp(user_id_);
		if (current_user -> authority > tmp.authority || tmp.passwd == passwd_)
		{
			if (tmp.authority == 1) user = User(user_id_) , current_user = &user;
			else if (tmp.authority == 3) admin = Admin(user_id_) , current_user = &admin;
			else root = Root(user_id_) , current_user = &root;
			//to do : add to system_log
		}else invalid();
	}
}

bool bookstore::command(const int &argc , char *argv[] , bool &src , std::ifstream &ist)
{
	if (!argc)
	{
		invalid();
		return 1;
	}
	if (argc == 1 && std::string(argv[0]) == std::string("exit")) return 0;
	if (!~argc || argc == 2 && std::string(argv[0]) == std::string("load"))
	{
		if (argc != 2 && ~argc) invalid();
		else if (~argc && current_user -> authority != 7) invalid();
		else
		{
			if (~argc) ist.open(argv[1]);
			else ist.open("command.txt");
			if (ist) src = 0 , root = Root("root") , current_user = &root;
			else if (~argc) invalid();
		}
		return 1;
	}
	if (std::string(argv[0]) == std::string("su"))
	{
		if (argc == 2) su(argv[1] , "");
		else if (argc == 3) su(argv[1] , argv[2]);
		else invalid();
		return 1;
	}
	if (std::string(argv[0]) == std::string("logout"))
	{
		if (argc != 1 || !current_user -> authority) invalid();
		else current_user = &visitor;
		return 1;
	}
	if (std::string(argv[0]) == std::string("useradd"))
	{
		if (argc != 5) invalid();
		else current_user -> useradd(argv[1] , argv[2] , argv[4] , atoi(argv[3]));
		return 1;
	}
	if (std::string(argv[0]) == std::string("register"))
	{
		if (argc != 4) invalid();
		else current_user -> reg(argv[1] , argv[2] , argv[3]);
		return 1;
	}
	if (std::string(argv[0]) == std::string("delete"))
	{
		if (argc != 2) invalid();
		else current_user -> del(argv[1]);
		return 1;
	}
	if (std::string(argv[0]) == std::string("passwd"))
	{
		if (argc == 3) current_user -> _passwd(argv[1] , "" , argv[2]);
		else if (argc == 4) current_user -> _passwd(argv[1] , argv[2] , argv[3]);
		else invalid();
		return 1;
	}
	if (std::string(argv[0]) == std::string("select"))
	{
		if (argc != 2) invalid();
		else current_user -> select(argv[1]);
		return 1;
	}
	if (std::string(argv[0]) == std::string("modify"))
	{
		bool valid = 1;
		std::string ISBN_  , name_ , author_ , keyword_;double price_ = -1;
		for (int i = 1;valid && i < argc;++ i)
		{
			std::string str(argv[i]);int len = str.length();
			if (str.rfind("-ISBN=") == 0) ISBN_ = str.substr(6 , len - 6);
			else if (str.rfind("-name=\"") == 0 && str.back() == '\"') name_ = str.substr(7 , len - 8);
			else if (str.rfind("-author=\"") == 0 && str.back() == '\"') author_ = str.substr(9 , len - 10);
			else if (str.rfind("-keyword=\"") == 0 && str.back() == '\"') keyword_ = str.substr(10 , len - 11);
			else if (str.rfind("-price=") == 0) {std::stringstream sst(str.substr(7 , len - 7));sst >> price_;}
			else valid = 0;
		}
		if (!valid) invalid();
		else current_user -> modify(ISBN_ , name_ , author_ , keyword_ , price_);
		return 1;
	}
	if (std::string(argv[0]) == std::string("import"))
	{
		if (argc != 3) invalid();
		else
		{
			std::stringstream sst( static_cast<std::string>(argv[2]) );
			double cost_price_;sst >> cost_price_;
			current_user -> import(atoi(argv[1]) , cost_price_);
		}
		return 1;
	}
	if (std::string(argv[0]) == std::string("show"))
	{
		if (argc == 1) current_user -> show();
		else if (argc == 2)
		{
			std::string str(argv[1]);int len = str.length();
			if (str.rfind("-ISBN=") == 0) current_user -> show(1 , str.substr(6 , len - 6));
			else if (str.rfind("-name=\"") == 0 && str.back() == '\"') current_user -> show(2 , str.substr(7 , len - 8));
			else if (str.rfind("-author=\"") == 0 && str.back() == '\"') current_user -> show(3 , str.substr(9 , len - 10));
			else if (str.rfind("-keyword=\"") == 0 && str.back() == '\"') current_user -> show(4 , str.substr(10 , len - 11));
			else if (str == std::string("finance")) current_user -> show_finance(-1);
			else invalid();
		}
		else if (argc == 3 && std::string(argv[1]) == std::string("finance")) current_user -> show_finance(atoi(argv[2]));
		else invalid();
		return 1;
	}
	if (std::string(argv[0]) == std::string("buy"))
	{
		if (argc != 3) invalid();
		else current_user -> buy(argv[1] , atoi(argv[2]));
		return 1;
	}
	if (std::string(argv[0]) == std::string("report"))
	{
		//to do
		return 1;
	}
	if (std::string(argv[0]) == std::string("log"))
	{
		//to do
		return 1;
	}
	invalid();
	return 1;
}