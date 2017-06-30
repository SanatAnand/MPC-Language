#ifndef AST_HH
#define AST_HH

#include<string>
#include<iostream>
#include<iomanip>
#include<typeinfo>
#include<list>

#define AST_SPACE "         "
#define AST_NODE_SPACE "            "
#define AST_SUB_NODE_SPACE "               "
#define SA_SPACE "      "

using namespace std;
//extern map<string,string> global_strings;
class Ast;

typedef enum
{
	void_data_type,
	int_data_type,
	double_data_type, 
	string_data_type,
	field_data_type,
	other_data_type
} Data_Type;

class Ast
{
protected:
	typedef enum
	{
		zero_arity = 0,
		unary_arity = 1,
		binary_arity = 2,
		ternary_arity = 3
	}Ast_Arity;

	int lineno;

public:
	Ast();
	~Ast();

	
	virtual void print(ostream & file_buffer) = 0;
	
};

class Assignment_Ast:public Ast
{
	Ast * lhs;
	Ast * rhs;

public:
	Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line);
	~Assignment_Ast();

	void print(ostream & file_buffer);

};

class Tying_Ast:public Ast
{
	Ast * lhs;
	string rhs;

public:
	Tying_Ast(Ast * temp_lhs, string temp_rhs, int line);
	~Tying_Ast();

	void print(ostream & file_buffer);

};

class In_Ast:public Ast
{
	Ast * s_list;
	Ast * party;

public:
	In_Ast(Ast * s_list, Ast * party, int line);
	~In_Ast();

	void print(ostream & file_buffer);
};


/////////////// DONE WITH SEND ////////////////////

class Party_Ast:public Ast
{
	string party_name;
	int party_type;

public:
	Party_Ast(string party_name, int party_type, int line);
	~Party_Ast();

	void print(ostream & file_buffer);
};

///////////////////////////////////////////////

class Name_Ast:public Ast
{
	string name;

public:
	Name_Ast(string name, int line);
	~Name_Ast();

	void print(ostream & file_buffer);
};

//////////////  SEND STATEMENT ////////////////////
class Send_Ast:public Ast
{
	Ast *sender, *receiver;
	Ast * sent_var, *rec_var;
	int type;

public:
	Send_Ast( Ast *sender, Ast *receiver, Ast * sent_var, Ast *rec_var, int line, int type);
	~Send_Ast();

	void print(ostream & file_buffer);
};

class Random_Ast:public Ast
{

public:
	Random_Ast(int line);
	~Random_Ast();

	void print(ostream & file_buffer);
};

class Decl_Ast:public Ast
{
	Data_Type type;
	list<string> *name_list;
	string type_name;

public:
	Decl_Ast(Data_Type type,list<string> * name_list,string name,int line);
	~Decl_Ast();

	void print(ostream & file_buffer);
};

template <class T>
class Number_Ast:public Ast
{
	T constant;

public:
	Number_Ast(T number, int line);
	~Number_Ast();

	void print(ostream & file_buffer);

};

class Arithmetic_Expr_Ast:public Ast
{
protected:
	Ast * lhs;
	Ast * rhs;

public:
	Arithmetic_Expr_Ast() {}
	~Arithmetic_Expr_Ast();

	virtual void print(ostream & file_buffer) = 0;
};

class Plus_Ast:public Arithmetic_Expr_Ast
{
public:
	Plus_Ast(Ast * l, Ast * r, int line);
	~Plus_Ast();

	void print(ostream & file_buffer);

};

class Minus_Ast:public Arithmetic_Expr_Ast
{
public:
	Minus_Ast(Ast * l, Ast * r, int line);
	~Minus_Ast();

	void print(ostream & file_buffer);

};

class Divide_Ast:public Arithmetic_Expr_Ast
{
public:
	Divide_Ast(Ast * l, Ast * r, int line);
	~Divide_Ast();

	void print(ostream & file_buffer);
};

class Mult_Ast:public Arithmetic_Expr_Ast
{
public:
	Mult_Ast(Ast * l, Ast * r, int line);
	~Mult_Ast();

	void print(ostream & file_buffer);

};

class UMinus_Ast: public Arithmetic_Expr_Ast
{
public:
	UMinus_Ast(Ast * l, Ast * r, int line);
	~UMinus_Ast();
	
	void print(ostream & file_buffer);
};

class Sequence_Ast: public Ast{
	list<Ast *> statement_list;
public:
	Sequence_Ast(int line);
	~Sequence_Ast();
	
	void ast_push_back(Ast * ast);
	void print(ostream & file_buffer);
	
};

class Session_call_Ast:public Ast
{
	Ast *seq;
	string proc_name;
	string session_name;
public:
	Session_call_Ast(Ast *seq, string proc_name, string session_name, int line);
	~Session_call_Ast();

	void print(ostream & file_buffer);
};

#endif
