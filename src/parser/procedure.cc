#include "common-classes.hh"
#include "ast.hh"
#include "procedure.hh"


Procedure::Procedure(string proc_name, list<pair<Data_Type, string> *> * varlist, list<string> * party_list,int line){
	name = proc_name;
	this->varlist = varlist;
	this->party_list = party_list;
	lineno = line;
}
Procedure::~Procedure(){}

string Procedure::get_proc_name(){return this->name;}
void Procedure::set_sequence_ast(Sequence_Ast & sa){
	this->sequence_ast = &sa;
}

void Procedure::print(ostream & file_buffer){
	cout<<"PROTOCOL "<<name<<"( ";
	list<pair<Data_Type, string> *>::iterator it = varlist->begin();
	for(;it!=varlist->end();it++)
	{
		if((*it)->first == int_data_type)
			file_buffer<<"int ";
		else if((*it)->first == field_data_type) file_buffer<<"field ";
		list<pair<Data_Type, string> *>::iterator it_temp = it;
		it_temp++;
		file_buffer<<(*it)->second;
		if(it_temp!=varlist->end())
			file_buffer<<", ";
	}
	cout<<") : PARTY ";
	list<string>::iterator pit = party_list->begin();
	for(;pit!=party_list->end();pit++)
	{
		file_buffer<<*pit<<", ";
		list<string>::iterator pit_temp = pit;
		pit_temp++;
		file_buffer<<(*pit);
		if(pit_temp!=party_list->end())
			file_buffer<<", ";
	}
	cout<<"{"<<endl;
	sequence_ast->print(file_buffer);
	file_buffer<<"}\n";
}