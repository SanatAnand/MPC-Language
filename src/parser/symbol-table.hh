#ifndef SYMBOL_TABLE_HH
#define SYMBOL_TABLE_HH

#include <string>
#include <list>

using namespace std;

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

class Symbol_Table;
class Symbol_Table_Entry;

//////////////////////////////////////////////////////////////


class Symbol_Table
{	
public:
	std::map<string, Symbol_Table_Entry*>* table;
	Symbol_Table* parent_table;
	
	Symbol_Table(Symbol_Table* parent_table);
	~Symbol_Table();
	static string build_tag(string tag1, string tag2);	
	Symbol_Table_Entry* find(string name, string var_tag);
	void insert(string variable_name, Data_Type variable_data_type, string var_tag);
};

class Symbol_Table_Entry
{
public:
	string variable_name;
	Data_Type variable_data_type;
	string var_tag;
	
	Symbol_Table_Entry(string variable_name, Data_Type variable_data_type, string var_tag);
	~Symbol_Table_Entry();
};

#endif
