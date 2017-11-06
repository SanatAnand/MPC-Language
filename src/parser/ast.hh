#ifndef AST_HH
#define AST_HH

#include<string>
#include<iostream>
#include<iomanip>
#include<typeinfo>
#include<list>
#include<vector>

#define AST_SPACE "         "
#define AST_NODE_SPACE "            "
#define AST_SUB_NODE_SPACE "               "
#define SA_SPACE "      "

using namespace std;
//extern map<string,string> global_strings;
class Ast;

typedef enum {
	session_data_type,
	int_data_type,
	party_data_type, 
	unknown_data_type,
	port_data_type,
	inport_data_type,
	outport_data_type,
	bool_data_type,
	field_data_type,
	other_data_type
} Data_Type;

typedef enum {
	except,
	where,
	no_cond
} condition;

typedef enum {
	conn_to,
	conn_and
} connection_param;

typedef enum {
	before,
	after
} order;

typedef enum {
	less_equalto,
	less_than,
	greater_than,
	greater_equalto,
	equalto,
	not_equalto
} Relational_Op;

typedef enum {
	boolean_not,
	boolean_or,
	boolean_and
} Boolean_Op;

typedef enum {
	environment,
	ideal,
	rand_port, 
	env_port,
	variable,
	wrapper,
	iterator_variable,
	random_sample,
	constant,
	double_dot
} Term_Type;

class Ast {
  protected:
	typedef enum
	{
		zero_arity = 0,
		unary_arity = 1,
		binary_arity = 2,
		ternary_arity = 3
	}Ast_Arity;

	
	static int tab_counter;

  public:
	Ast();
	~Ast();

	int lineno;

	virtual void print(ostream & file_buffer) = 0;
	virtual void print_xml(ostream & file_buffer) = 0;

	virtual void print_comma(ostream & file_buffer){cout<<"Something is horribly wrong";}
	
	virtual void print_tabs(ostream & file_buffer, int tab_counter);
	virtual void print_attribute(ostream & file_buffer, const char* name, string value);
	virtual void print_attribute(ostream & file_buffer, const char* name, int value);
	virtual void print_attribute(ostream & file_buffer, const char* name, Ast* value);
	virtual void print_attribute(ostream & file_buffer, const char* name, list<Ast*>* value);		

};

class Sequence_Ast: public Ast{
	list<Ast *> statement_list;
  public:
	Sequence_Ast(int line);
	~Sequence_Ast();
	
	void ast_push_back(Ast * ast);
	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);
	void print_comma(ostream & file_buffer);

};


class Protocol_Decl_Ast: public Ast {
	string name;
	list<Ast*> * arg_list;
	list<Ast*> * party_list;
	
  public:
	Protocol_Decl_Ast(string name, list<Ast*> * arg_list, list<Ast*> * party_list, int lineno);
	~Protocol_Decl_Ast();

	string get_protocol_name();
	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Protocol_Ast: public Ast {
	Sequence_Ast * sequence_ast;
	list<Ast*> * uses_ast;
	Protocol_Decl_Ast * protocol_decl;
	
  public:
	Protocol_Ast(Protocol_Decl_Ast * protocol_decl, list<Ast*> * uses_ast, Sequence_Ast * sequence_ast,int lineno);
	~Protocol_Ast();

  	void set_sequence_ast(Sequence_Ast & sa);
	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Assignment_Ast:public Ast
{
	Ast * lhs;
	Ast * rhs;

  public:
	Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line);
	~Assignment_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Tying_Ast:public Ast
{
	Ast * lhs;
	Ast * rhs;

  public:
	Tying_Ast(Ast * temp_lhs, Ast * temp_rhs, int line);
	~Tying_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Forward_Ast:public Ast
{
	Ast * lhs;
	Ast * rhs;

  public:
	Forward_Ast(Ast * temp_lhs, Ast * temp_rhs, int line);
	~Forward_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

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
	void print_xml(ostream & file_buffer);

};

///////////////////////////////////////////////

class Term_Ast:public Ast
{
  public:
	Term_Type t;
	list<Ast*> *dim_list;
	Ast* child;

	Term_Ast(Ast *c, list<Ast*> *dim, Term_Type type, int line);
	~Term_Ast();

	void print(ostream & file_buffer);	
	void print_xml(ostream & file_buffer);	

};

///////////////////////////////////////////////
class Decl_Term_Ast:public Ast
{
	bool port_clause_exists;
	Ast* term;
	list<pair<pair<Data_Type, string> *, Ast*> *> * port_clause;
	bool each;
public:
	Decl_Term_Ast(Ast* term, bool port_clause_exists, list<pair<pair<Data_Type, string> *, Ast*> *> * port_clause, bool each, int line);
	~Decl_Term_Ast();

	void print(ostream & file_buffer);	
	void print_xml(ostream & file_buffer);	

};

///////////////////////////////////////////////

class Argument_Ast:public Ast
{
	Ast* term;
	Data_Type type;
	string name;

  public:
	Argument_Ast(Ast *term, Data_Type type, string name, int line);
	~Argument_Ast();

	void print(ostream & file_buffer);	
	void print_xml(ostream & file_buffer);

};

class Prot_Arg_Ast:public Ast
{
	Ast* prot_decl;

  public:
	Prot_Arg_Ast(Ast *prot_decl, int line);
	~Prot_Arg_Ast();

	void print(ostream & file_buffer);	
	void print_xml(ostream & file_buffer);

};

///////////////////////////////////////////////

class Name_Ast:public Ast
{
  public:
	string name;

	Name_Ast(string name, int line);
	~Name_Ast();
	
	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

//////////////  SEND STATEMENT ////////////////////
class Send_Ast:public Ast
{
public:
	Ast * lhs;
	Ast * rhs;

	Send_Ast(Ast * temp_lhs, Ast * temp_rhs, int line);
	~Send_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Decl_Ast:public Ast
{
	Data_Type type;
	list<Ast*> *name_list;
	string type_name;

  public:
	Decl_Ast(Data_Type type,list<Ast*> * name_list,string name,int line);
	~Decl_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);
	void print_comma(ostream & file_buffer);

};

class In_Ast:public Ast {
	Ast * s_list;
	Ast * party;
	bool each;
  public:
	In_Ast(Ast * temp_lhs, Ast * party, bool each, int line);
	~In_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class With_Ast:public Ast {
	list<pair<pair<Data_Type, string> *, Ast*> *> * s_list;
	Ast * party;
	bool each;
  public:
	With_Ast(list<pair<pair<Data_Type, string> *, Ast*> *> * temp_lhs, Ast * party, bool each, int line);
	~With_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Seq_Ast:public Ast {
	Ast * cond;
	Ast * addendum;
	bool each;
	order ord;
  public:
	Seq_Ast(Ast * temp_lhs, Ast * party, bool each, order o, int line);
	~Seq_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Connect_Ast:public Ast {
	Ast * from;
	Ast * to;
	connection_param type;
  public:
	Connect_Ast(Ast * temp_lhs, Ast * party, connection_param each, int line);
	~Connect_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Port_Clause_Ast:public Ast {
	Ast * party;
	bool is_clause_present;
	bool is_each_present;
	list<pair<pair<Data_Type, string> *, Ast*> *> * seq;

public:
	Port_Clause_Ast(Ast * party, bool is_clause_present, bool is_each_present, list<pair<pair<Data_Type, string> *, Ast*> *> * seq, int line);
	~Port_Clause_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

template <class T>
class Number_Ast:public Ast
{
	T constant;
  public:
	Number_Ast(T number, int line);
	~Number_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Arithmetic_Expr_Ast:public Ast
{
  public:
	Ast * lhs;
	Ast * rhs;


	Arithmetic_Expr_Ast() {}
	~Arithmetic_Expr_Ast();

	virtual void print(ostream & file_buffer) = 0;
	void print_xml(ostream & file_buffer) = 0;

};

class Plus_Ast:public Arithmetic_Expr_Ast
{
  public:
	Plus_Ast(Ast * l, Ast * r, int line);
	~Plus_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Minus_Ast:public Arithmetic_Expr_Ast
{
  public:
	Minus_Ast(Ast * l, Ast * r, int line);
	~Minus_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Divide_Ast:public Arithmetic_Expr_Ast
{
  public:
	Divide_Ast(Ast * l, Ast * r, int line);
	~Divide_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);
	
};

class Modulo_Ast:public Arithmetic_Expr_Ast
{
  public:
	Modulo_Ast(Ast * l, Ast * r, int line);
	~Modulo_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Mult_Ast:public Arithmetic_Expr_Ast
{
  public:
	Mult_Ast(Ast * l, Ast * r, int line);
	~Mult_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class UMinus_Ast: public Arithmetic_Expr_Ast
{
  public:
	UMinus_Ast(Ast * l, Ast * r, int line);
	~UMinus_Ast();
	
	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};
///////////////

class Relational_Expr_Ast:public Ast
{
  public:
	Ast * lhs_condition;
	Ast * rhs_condition;
	Relational_Op rel_op;


	Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line);
	~Relational_Expr_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Boolean_Expr_Ast:public Ast
{
  public:
	Ast * lhs_op;
	Ast * rhs_op;
	Boolean_Op bool_op;


	Boolean_Expr_Ast(Ast * lhs, Boolean_Op bop, Ast * rhs, int line);
	~Boolean_Expr_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Port_Expr_Ast:public Ast
{
public:
	Ast * lhs;
	Ast * rhs;


	Port_Expr_Ast(Ast * lhs, Ast * rhs, int line);
	~Port_Expr_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Party_Expr_Ast:public Ast
{
public:
	Ast * lhs;
	Ast * rhs;


	Party_Expr_Ast(Ast * lhs, Ast * rhs, int line);
	~Party_Expr_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class From_Expr_Ast:public Ast
{
public:
	Ast * lhs_op;
	Ast * rhs_op;


	From_Expr_Ast(Ast * lhs, Ast * rhs, int line);
	~From_Expr_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);
	
};

class Expression_List_Ast:public Ast
{
public:
	list<Ast*> *exp_list;


	Expression_List_Ast(list<Ast*> *exp_list, int line);
	~Expression_List_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Session_call_Ast:public Ast
{
	Ast *seq;
	string proc_name;
	Ast* session_name;
	list<Ast*> * arg_list;
	bool partial;
  public:
	Session_call_Ast(Ast *session_name ,Ast *seq, bool partial, string proc_name, list<Ast*> *arg_list, int line);
	~Session_call_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Set_Ast:public Ast
{
	Ast *source;
	Ast *dest;
	string proc_name;
	list<Ast*> * arg_list;
  public:
	Set_Ast(Ast *src, Ast *dst, string name, list<Ast*> * arg_list, int line);
	~Set_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Abort_Ast:public Ast
{

  public:
	Abort_Ast(int line);
	~Abort_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Wrap_Ast:public Ast
{
	Ast * stmt;

public:
	Wrap_Ast(Ast * stmt, int line);
	~Wrap_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Start_Ast:public Ast
{
	Ast * party_exp;
  public:
	Start_Ast(Ast * party_exp, int line);
	~Start_Ast();
	
	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Selection_Statement_Ast: public Ast {
  protected:
	Ast* cond;
	Ast* then_part;
	Ast* else_part;
  public:
	Selection_Statement_Ast(Ast * cond,Ast* then_part, Ast* else_part, int line);
	~Selection_Statement_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Each_Statement_Ast: public Ast {
  public:
	Ast* cond;
	Ast* variable;
	Ast* expression;
	condition cond_type;

	Each_Statement_Ast(Ast * cond, condition cond_type, Ast* variable, Ast* expression, int line);
	~Each_Statement_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Iteration_Statement_Ast: public Ast {
public:
	Ast* each_block;
	Ast* body;

	Iteration_Statement_Ast(Ast* each_block, Ast* body, int line);
	~Iteration_Statement_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Basic_Loop_Ast: public Ast{
	Ast* variable;
	Ast* limit;
	Ast* body;
public:
	Basic_Loop_Ast(Ast* variable, Ast* limit, Ast* body);
	~Basic_Loop_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Array_Limit_Ast: public Ast{
	string array;
	int index;
	
public:
	Array_Limit_Ast( string array, int index);
	~Array_Limit_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Send_Assignment_Ast:public Ast
{
	Ast * lhs;
	Ast * rhs;

  public:
	Send_Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line);
	~Send_Assignment_Ast();

	void print(ostream & file_buffer);
	void print_xml(ostream & file_buffer);

};

class Desugar_Ast{
	static string get_new_variable_name();
	static void find_hash_vars_and_build_assign_expr( Ast* root, std::vector<string>* var_list, std::vector<Ast*>* limits_list );
	static Ast* get_party_port(Ast* root, int flag);
	static Ast* desugar_send_lhs(Ast* root, Ast* rhs_port, int flag);
	static Ast* desugar_send_rhs(Ast* root, Ast* lhs_port, int flag);

public:
	static Ast* desugar_loops( Ast* root );
	static Ast* desugar_send( Ast* root );
	static Ast* desugar_in( Ast* root );
};

#endif