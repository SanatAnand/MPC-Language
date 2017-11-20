
#include <string>
#include <iostream>

using namespace std;

#include "common-classes.hh"
#include "user-options.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "symbol-table.hh"
#include "ast.hh"

string Symbol_Table::build_tag(string tag1, string tag2)
{
	return tag1 + "." + tag2;	
}

Symbol_Table::Symbol_Table(Symbol_Table* parent_table)
{
	this->parent_table = parent_table;
	this->table = new map<string, Symbol_Table_Entry*>();
}

Symbol_Table::~Symbol_Table()
{
	for(auto i = this->table->begin(); i != this->table->end(); i++)
		delete (*i).second;
}

Symbol_Table_Entry* Symbol_Table::find(string name, string var_tag)
{
	map<string, Symbol_Table_Entry*>::iterator res = this->table->find(build_tag(var_tag, name));
	if(res != this->table->end())
		return (*res).second;	
	else
	{
		str::size_t found = var_tag.find(".");
		found = var_tag.find(".");
		if(found == std::string::npos)
		{
			//var_tag of the form "protocol.<party>"
			//var of tag "protocol" can match this
			//find appropriately and return if found
			map<string, Symbol_Table_Entry*>::iterator res2 = this->table->find(build_tag("protocol", name));
			if(res2 != this->table->end())
				return (*res).second;
		}
	}
	if(this->parent_table==NULL)
		//throw error? symbol not found
		return NULL;
	else
		return this->parent_table->find(name, var_tag);
}

void Symbol_Table::insert(string variable_name, Data_Type variable_data_type, string var_tag)
{
	Symbol_Table_Entry* ste = new Symbol_Table_Entry( variable_name, variable_data_type, var_tag );
	this->table->insert( std::pair<string, Symbol_Table_Entry*> (build_tag(var_tag, variable_name), ste ));
}

Symbol_Table_Entry::Symbol_Table_Entry(string variable_name, Data_Type variable_data_type, string var_tag)
{
	this->variable_name = variable_name;
	this->variable_data_type = variable_data_type;
	this->var_tag = var_tag;
}

Symbol_Table_Entry::~Symbol_Table_Entry()
{}
