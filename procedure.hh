
#ifndef PROCEDURE_HH
#define PROCEDURE_HH

#include <string>
#include <map>
#include <list>
#include "ast.hh"

#define PROC_SPACE "   "
#define LOC_VAR_SPACE "      "

using namespace std;

class Procedure;

class Procedure
{
	string name;
	
	Sequence_Ast * sequence_ast;
	list<pair<Data_Type, string> *> * varlist;
	list<string> * party_list;

	int lineno;

public:
	Procedure(string proc_name, list<pair<Data_Type, string> *> * varlist, list<string> * party_list,int line);
	~Procedure();

	string get_proc_name();
  	void set_sequence_ast(Sequence_Ast & sa);

	void print(ostream & file_buffer);
};

#endif
