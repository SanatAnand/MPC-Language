
#include <string>
#include <iostream>

using namespace std;

#include "common-classes.hh"
#include "user-options.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "symbol-table.hh"
#include "ast.hh"

Symbol_Table::Symbol_Table(scope_type sc, Symbol_Table* parent_table)
{
	this->scope = sc;
	this->parent_table = parent_table;
	this->table = new map<string, Symbol_Table_Entry*>();
}

Symbol_Table::~Symbol_Table()
{
	for(auto i = this->table->begin(); i != this->table->end(); i++)
		delete (*i).second;
}

Symbol_Table_Entry* Symbol_Table::find(string name)
{
	map<string, Symbol_Table_Entry*>::iterator res = this->table->find(name);
	if(res!=this->table->end())
	{
		return (*res).second;
	}
	else
	{
		//TODO : Goto parent
	}
}

void Symbol_Table::insert(string variable_name, Data_Type variable_data_type, Symbol_Table* table)
{}

Symbol_Table_Entry::Symbol_Table_Entry(string variable_name, Data_Type variable_data_type, Symbol_Table* table)
{
	this->variable_name = variable_name;
	this->variable_data_type = variable_data_type;
	this->table = table;
}

Symbol_Table_Entry::~Symbol_Table_Entry()
{}