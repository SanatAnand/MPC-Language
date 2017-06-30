#include<iostream>
#include<fstream>
#include<typeinfo>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"ast.hh"
#include"procedure.hh"
#include"program.hh"

Ast::Ast()
{}

Ast::~Ast()
{}


////////////////////////////////////////////////////////////////

Sequence_Ast::Sequence_Ast(int line)
{
	this->lineno = line;
}

void Sequence_Ast::ast_push_back(Ast* a)
{
	statement_list.push_back(a);
}

Sequence_Ast::~Sequence_Ast()
{
	std::list<Ast*>::iterator it;
	for (it = statement_list.begin(); it != statement_list.end(); ++it) 
    	delete(*it);
    statement_list.clear();
}


void Sequence_Ast::print(ostream & file_buffer)
{
	for (list<Ast*>::iterator it=statement_list.begin(); it != statement_list.end(); ++it)
	    (*it)->print(file_buffer);
}


///////////////////////////////////////////////////////////////

Send_Ast::Send_Ast(Ast *sender, Ast *receiver, Ast * sent_var, Ast *rec_var, int line, int type)
{
	this->sender = sender;
	this->receiver = receiver;
	this->sent_var = sent_var;
	this->rec_var = rec_var;
	this->lineno = line;
	this->type = type;
}

Send_Ast::~Send_Ast()
{
	delete sender;
	delete receiver;
	delete sent_var;
	delete rec_var;
}

void Send_Ast::print(ostream & file_buffer)
{
	sender->print(file_buffer);
	file_buffer<<" => ";
	receiver->print(file_buffer);
	file_buffer<<";\n";
}

///////////////////////////////////////////////////////////////
Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line)
{
	//ADD CODE HERE
	this->lhs = temp_lhs;
	this->rhs = temp_rhs;
	this->lineno = line;
}

Assignment_Ast::~Assignment_Ast()
{
	//ADD CODE HERE
	delete this->lhs;
	delete this->rhs;

}

void Assignment_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	lhs->print(file_buffer);
	file_buffer<<" = ";
	rhs->print(file_buffer);
	file_buffer<<";\n";
}
/////////////////////////////////////////////////////

Party_Ast::Party_Ast(string party_name, int party_type, int line)
{
	//ADD CODE HERE
	this->party_name = party_name;
	this->party_type = party_type;
	this->lineno = line;
}

Party_Ast::~Party_Ast()
{
	//ADD CODE HERE

}

void Party_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<party_name;
	
}

///////////////////////////////////////////////////////

Random_Ast::Random_Ast(int line)
{
	this->lineno = line;
}

Random_Ast::~Random_Ast()
{

}

void Random_Ast::print(ostream & file_buffer)
{
	file_buffer<<"$";
}

////////////////////////////////////////////////////

Tying_Ast::Tying_Ast(Ast * temp_lhs, string temp_rhs, int line)
{
	//ADD CODE HERE
	this->lhs = temp_lhs;
	this->rhs = temp_rhs;
	this->lineno = line;
}

Tying_Ast::~Tying_Ast()
{
	//ADD CODE HERE
}

void Tying_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	lhs->print(file_buffer);
	file_buffer<<" : ";
	file_buffer<<rhs;
	file_buffer<<";\n";
}

///////////////////////////////////////////////////
In_Ast::In_Ast(Ast * temp_lhs, Ast * party, int line)
{
	//ADD CODE HERE
	this->s_list = temp_lhs;
	this->party = party;
	this->lineno = line;
}

In_Ast::~In_Ast()
{
	//ADD CODE HERE
	delete this->s_list;
	delete this->party;
}

void In_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	//WRITE THIS
	file_buffer<<"in ";
	party->print(file_buffer);
	file_buffer<<" : {\n";
	s_list->print(file_buffer);
	file_buffer<<"}\n";
}

///////////////////////////////////////////////////////////////////////

Decl_Ast::Decl_Ast(Data_Type type,list<string> * name_list,string name,int line)
{
	this->lineno = line;
	this->type = type;
	this->type_name = name;
	this->name_list = name_list;
}

Decl_Ast::~Decl_Ast()
{

}

void Decl_Ast::print(ostream & file_buffer)
{
	file_buffer<<type_name<<" ";
	list<string>::iterator pit = name_list->begin();
	for(;pit!=name_list->end();pit++)
	{
		file_buffer<<*pit<<", ";
		list<string>::iterator pit_temp = pit;
		pit_temp++;
		file_buffer<<(*pit);
		if(pit_temp!=name_list->end())
			file_buffer<<", ";
	}
	file_buffer<<";\n";
}


/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string name, int line)
{
	lineno = line;
	this->name = name;
	//ADD CODE HERE
}

Name_Ast::~Name_Ast()
{}

void Name_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<this->name;

}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, int line)
{
	// use Ast_arity from ast.hh
	//ADD CODE HERE
	constant = number;
	lineno = line;
}

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast()
{}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<constant;
}

///////////////////////////////////////////////////////////////////////////////

Arithmetic_Expr_Ast::~Arithmetic_Expr_Ast(){
	delete lhs;
	delete rhs;
}

/////////////////////////////////////////////////////////////////////

Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line)
{
	// set arity and data type
	//ADD CODE HERE
	lhs = l;
	rhs = r;
	lineno = line;

}

void Plus_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	lhs->print(file_buffer);
	file_buffer<<" + ";
	rhs->print(file_buffer);
}

/////////////////////////////////////////////////////////////////

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	lhs = l;
	rhs = r;
	lineno = line;
}

void Minus_Ast::print(ostream & file_buffer)
{
	lhs->print(file_buffer);
	file_buffer<<" - ";
	rhs->print(file_buffer);
}

//////////////////////////////////////////////////////////////////

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	lhs = l;
	rhs = r;
	lineno = line;
	
}

void Mult_Ast::print(ostream & file_buffer)
{
	lhs->print(file_buffer);
	file_buffer<<" * ";
	rhs->print(file_buffer);
}

////////////////////////////////////////////////////////////////////

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	lhs = l;
	rhs = r;
	lineno = line;
	
}

void Divide_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	lhs->print(file_buffer);
	file_buffer<<" / ";
	rhs->print(file_buffer);
}

//////////////////////////////////////////////////////////////////////

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	lhs = l;
	rhs = r;
	lineno = line;
}

void UMinus_Ast::print(ostream & file_buffer)
{
	file_buffer<<" - ";
	lhs->print(file_buffer);
}

///////////////////////////////////////////////////////////////////////
//updated


Session_call_Ast::Session_call_Ast(Ast *seq, string proc_name, string session_name, int line)
{
	this->seq = seq;
	this->proc_name = proc_name;
	this->lineno = line;
	this->session_name = session_name;
}

Session_call_Ast::~Session_call_Ast()
{
	delete seq;
}


void Session_call_Ast::print(ostream & file_buffer)
{
	
}

//////////

///////////////////////////////////////////////////////////////////////


template class Number_Ast<double>;
template class Number_Ast<int>;
