#include <iostream>
#include <fstream>
#include <typeinfo>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "ast.hh"
#include "program.hh"


int Ast::tab_counter=0;

string DataTypesArray[]={"session", "int", "party", "unknown", "port", "inport", "outport", "bool", "field", "Other"};
string ConditionsArray[]={"except", "where", "NoCondition"};
string ConnectionParamsArray[]={"to", "and"};
string SeqOrdersArray[]={"before", "after"};
string RelationalOpsArray[]={"LessThanEqualTo", "LessThan", "GreaterThan", "GreaterThanEqualTo", "EqualTo", "NotEqualTo"};
string RelationalPrintArray[]={"<=", "<", ">", ">=", "==", "!="};
string BooleanOpsArray[]={ "BooleanNot", "BooleanOr", "BooleanAnd"};
string BooleanPrintArray[]={ "!", "||", "&&"};
string BoolSymbolsArray[]={"false", "true"};
string TermTypesArray[]={"Environment", "Ideal", "RandPort",  "EnvPort", "Variable", "Wrapper", "IteratorVariable", "RandomSample", "Constant", "DoubleDot"};
string TermPrintArray[]={"env", "ideal", "random",  "environ", "Variable", "@", "#", "$", "Constant", ".."};

void Ast::print_tabs(ostream & file_buffer, int tab_counter) {
	while(tab_counter-->0)
		file_buffer<<'\t';
}


void Ast::print_attribute(ostream & file_buffer, const char* name, string value) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<"<<name<<">";
	file_buffer<<value;
	file_buffer<<"</"<<name<<">\n";
}

void Ast::print_attribute(ostream & file_buffer, const char* name, int value) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<"<<name<<">";
	file_buffer<<value;
	file_buffer<<"</"<<name<<">\n";
}

void Ast::print_attribute(ostream & file_buffer, const char* name, Ast* value) {
	if(value==NULL) return;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<"<<name<<">\n";
	tab_counter++;
	value->print_xml(file_buffer);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</"<<name<<">\n";
}

void Ast::print_attribute(ostream & file_buffer, const char* name, list<Ast*>* value) {
	if(value==NULL) return;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<"<<name<<">\n";
	tab_counter++;
	for(auto ele: *value) {
		if(ele!=NULL)
			ele->print_xml(file_buffer);	
	}
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</"<<name<<">\n";
}

//////////////////////////////////////////////////////////////////////////////////////////

void Protocol_Decl_Ast::print(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"protocol "<<name<<" (";
	list<Ast*>::iterator pit = arg_list->begin();
	for(;pit!=arg_list->end();pit++)
	{
		list<Ast*>::iterator pit_temp = pit;
		pit_temp++;
		(*pit)->print(file_buffer);
		if(pit_temp!=arg_list->end())
			file_buffer<<", ";
	}
	file_buffer<<"): party ";
	pit = party_list->begin();
	for(;pit!=party_list->end();pit++)
	{
		list<Ast*>::iterator pit_temp = pit;
		pit_temp++;
		(*pit)->print(file_buffer);
		if(pit_temp!=party_list->end())
			file_buffer<<", ";
	}
}

void Protocol_Decl_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<ProtocolDeclaration>\n";
	tab_counter++;
	print_attribute(file_buffer, "Name", this->name);
	print_attribute(file_buffer, "Arguments", this->arg_list);
	print_attribute(file_buffer, "Parties", this->party_list);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</ProtocolDeclaration>\n";
}

//////////////////////////////////////////////////////////////////////////////////////////

void Protocol_Ast::print(ostream & file_buffer) {	
	protocol_decl->print(file_buffer);
	file_buffer<<"{\n";
	tab_counter++;
	if(uses_ast!=NULL)
	{
		print_tabs(file_buffer, tab_counter);
		file_buffer<<"uses \n";
		for(list<Ast*>::iterator pit = uses_ast->begin();pit!=uses_ast->end();pit++)
		{
			print_tabs(file_buffer, tab_counter);
			(*pit)->print(file_buffer);
			file_buffer<<"\n";
		}
		print_tabs(file_buffer, tab_counter);
		file_buffer<<";\n";
	}
	sequence_ast->print(file_buffer);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"}\n";
}

void Protocol_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Protocol>\n";
	tab_counter++;
	if(this->protocol_decl!=NULL)
		this->protocol_decl->print_xml(file_buffer);
	print_attribute(file_buffer, "Uses", this->uses_ast);
	print_attribute(file_buffer, "Body", this->sequence_ast);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Protocol>\n";
}

//////////////////////////////////////////////////////////////////////////////////////////

void Sequence_Ast::print(ostream & file_buffer) {
	for (auto ele: this->statement_list)
	    ele->print(file_buffer);
}

void Sequence_Ast::print_comma(ostream & file_buffer) {
	for (auto ele: this->statement_list)
	    ele->print_comma(file_buffer);
}

void Sequence_Ast::print_xml(ostream & file_buffer) {
	for (auto ele: this->statement_list)
	    ele->print_xml(file_buffer);
}

//////////////////////////////////////////////////////////////////////////////////////////


void Send_Ast::print(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	lhs->print(file_buffer);
	file_buffer<<" => ";
	rhs->print(file_buffer);
	file_buffer<<";\n";
}

void Send_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Send>\n";
	tab_counter++;
	print_attribute(file_buffer, "Sender", this->lhs);
	print_attribute(file_buffer, "Receiver", this->rhs);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Send>\n";
}

///////////////////////////////////////////////////////////////

void Assignment_Ast::print(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	lhs->print(file_buffer);
	file_buffer<<" = ";
	rhs->print(file_buffer);
	file_buffer<<";\n";
}

void Assignment_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Assignment>\n";
	tab_counter++;
	print_attribute(file_buffer, "Lvalue", this->lhs);
	print_attribute(file_buffer, "Rvalue", this->rhs);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Assignment>\n";
}

/////////////////////////////////////////////////////

void Party_Ast::print(ostream &file_buffer) {
	file_buffer<<party_name;
}

void Party_Ast::print_xml(ostream &file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Party>\n";
	tab_counter++;
	print_attribute(file_buffer, "Name", this->party_name);
	print_attribute(file_buffer, "Type", this->party_type);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Party>\n";
}

////////////////////////////////////////////////////

void Tying_Ast::print(ostream& file_buffer) {
	print_tabs(file_buffer, tab_counter);
	lhs->print(file_buffer);
	file_buffer<<" : ";
	rhs->print(file_buffer);
	file_buffer<<";\n";
}

void Tying_Ast::print_xml(ostream& file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Tying>\n";
	tab_counter++;
	print_attribute(file_buffer, "SessionParty", this->lhs);
	print_attribute(file_buffer, "RealParty", this->rhs);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Tying>\n";
}

///////////////////////////////////////////////////

void Forward_Ast::print(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"forward ";
	lhs->print(file_buffer);
	file_buffer<<" to ";
	rhs->print(file_buffer);
	file_buffer<<";\n";
}


void Forward_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Forward>\n";
	tab_counter++;
	print_attribute(file_buffer, "Sender", this->lhs);
	print_attribute(file_buffer, "Receiver", this->rhs);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Forward>\n";
}

///////////////////////////////////////////////////
void In_Ast::print(ostream & file_buffer) {
	//WRITE THIS
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"in ";
	party->print(file_buffer);
	file_buffer<<"{\n";
	tab_counter++;
	s_list->print(file_buffer);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"}\n";
}

void In_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<In>\n";
	tab_counter++;
	print_attribute(file_buffer, "Party", this->party);
	print_attribute(file_buffer, "Body", this->s_list);
	print_attribute(file_buffer, "IsEachPresent", BoolSymbolsArray[this->each]);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</In>\n";
}

////////////////////////////////////////////////////////

void With_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE
	//WRITE THIS
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"with ";
	party->print(file_buffer);
	file_buffer<<"{\n";
	tab_counter++;

	print_tabs(file_buffer, tab_counter);
	list<pair<pair<Data_Type, string> *, Ast*> *>::iterator pit = s_list->begin();
	for(;pit!=s_list->end();pit++)
	{
		list<pair<pair<Data_Type, string> *, Ast*> *>::iterator pit_temp = pit;
		pit_temp++;
		pair<pair<Data_Type, string> *, Ast*> * decl = (*pit);
		if(decl->first->first == other_data_type)
			file_buffer<<decl->first->second<<" ";
		else file_buffer<<DataTypesArray[decl->first->first]<<" ";
		decl->second->print(file_buffer);
		if(pit_temp!=s_list->end())
			file_buffer<<", ";
		else file_buffer<<"\n";
	}

	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"};\n";
}

void With_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<With>\n";
	tab_counter++;
	if(this->party)
		this->party->print_xml(file_buffer);

	if(s_list!=NULL)
	{
		print_tabs(file_buffer, tab_counter);
		file_buffer<<"<"<<"PortDeclarations"<<">\n";
		tab_counter++;
		for(auto ele: *s_list) {
			if(ele!=NULL)
			{
				print_tabs(file_buffer, tab_counter);
				file_buffer<<"<Declaration>\n";
				tab_counter++;
				print_attribute(file_buffer, "TypeName", ele->first->second);
				print_attribute(file_buffer, "Type", DataTypesArray[ele->first->first]);
				print_attribute(file_buffer, "VariableList", ele->second);
				// print_attribute(file_buffer, "LineNo", this->lineno);
				tab_counter--;
				print_tabs(file_buffer, tab_counter);
				file_buffer<<"</Declaration>\n";
			}
		}
		tab_counter--;
		print_tabs(file_buffer, tab_counter);
		file_buffer<<"</"<<"PortDeclarations"<<">\n";
	}

	print_attribute(file_buffer, "IsEachPresent", BoolSymbolsArray[this->each]);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</With>\n";
}

////////////////////////////////////////////////////////

void Seq_Ast::print(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<SeqOrdersArray[this->ord]<<" ";
	cond->print(file_buffer);
	file_buffer<<"{\n";
	tab_counter++;
	addendum->print(file_buffer);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"}\n";
}

void Seq_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<"<<SeqOrdersArray[this->ord]<<"Statement>\n";
	tab_counter++;
	print_attribute(file_buffer, "Condition", this->cond);
	print_attribute(file_buffer, "Body", this->addendum);
	print_attribute(file_buffer, "IsEachPresent", BoolSymbolsArray[this->each]);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</"<<SeqOrdersArray[this->ord]<<"Statement>\n";
}

////////////////////////////////////////////////////////

void Connect_Ast::print(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"connect ";
	from->print(file_buffer);
	file_buffer<<" "<<ConnectionParamsArray[this->type]<<" ";
	to->print(file_buffer);
	file_buffer<<";\n";
}

void Connect_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Connect>\n";
	tab_counter++;
	print_attribute(file_buffer, "From", this->from);
	print_attribute(file_buffer, "To", this->to);
	print_attribute(file_buffer, "Type", ConnectionParamsArray[this->type]);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Connect>\n";
}

///////////////////////////////////////////////////////////////////////

void Port_Clause_Ast::print(ostream & file_buffer){
	// ADD CODE HERE
	party->print(file_buffer);
	if(is_clause_present){
		if(is_each_present)file_buffer<<" each";
		file_buffer<<" with {\n";
		tab_counter++;

		print_tabs(file_buffer, tab_counter);
		list<pair<pair<Data_Type, string> *, Ast*> *>::iterator pit = seq->begin();
		for(;pit!=seq->end();pit++)
		{
			list<pair<pair<Data_Type, string> *, Ast*> *>::iterator pit_temp = pit;
			pit_temp++;
			pair<pair<Data_Type, string> *, Ast*> * decl = (*pit);
			if(decl->first->first == other_data_type)
				file_buffer<<decl->first->second<<" ";
			else file_buffer<<DataTypesArray[decl->first->first]<<" ";
			decl->second->print(file_buffer);
			if(pit_temp!=seq->end())
				file_buffer<<", ";
			else file_buffer<<"\n";
		}
		
		tab_counter--;
		print_tabs(file_buffer, tab_counter);
		file_buffer<<"}";
	}

}

void Port_Clause_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<PartyDeclaration>\n";
	tab_counter++;
	print_attribute(file_buffer, "Party", this->party);
	if(this->is_clause_present) {
		print_attribute(file_buffer, "IsEachPresent", BoolSymbolsArray[this->is_each_present]);
		if(seq!=NULL)
		{
			print_tabs(file_buffer, tab_counter);
			file_buffer<<"<"<<"PortDeclarations"<<">\n";
			tab_counter++;
			for(auto ele: *seq) {
				if(ele!=NULL)
				{
					print_tabs(file_buffer, tab_counter);
					file_buffer<<"<Declaration>\n";
					tab_counter++;
					print_attribute(file_buffer, "TypeName", ele->first->second);
					print_attribute(file_buffer, "Type", DataTypesArray[ele->first->first]);
					print_attribute(file_buffer, "VariableList", ele->second);
					// print_attribute(file_buffer, "LineNo", this->lineno);
					tab_counter--;
					print_tabs(file_buffer, tab_counter);
					file_buffer<<"</Declaration>\n";
				}
			}
			tab_counter--;
			print_tabs(file_buffer, tab_counter);
			file_buffer<<"</"<<"PortDeclarations"<<">\n";
		}

	}
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</PartyDeclaration>\n";	
}

///////////////////////////////////////////////////////////////////////

void Decl_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE
	print_tabs(file_buffer, tab_counter);
	if(this->type == other_data_type)
		file_buffer<<type_name<<" ";
	else file_buffer<<DataTypesArray[this->type]<<" ";
	list<Ast*>::iterator pit = name_list->begin();
	for(;pit!=name_list->end();pit++)
	{
		list<Ast*>::iterator pit_temp = pit;
		pit_temp++;
		(*pit)->print(file_buffer);
		if(pit_temp!=name_list->end())
			file_buffer<<", ";
	}
	file_buffer<<";\n";
}

void Decl_Ast::print_comma(ostream & file_buffer) {
	//ADD CODE HERE
	print_tabs(file_buffer, tab_counter);
	if(this->type == other_data_type)
		file_buffer<<type_name<<" ";
	else file_buffer<<DataTypesArray[this->type]<<" ";
	list<Ast*>::iterator pit = name_list->begin();
	for(;pit!=name_list->end();pit++)
	{
		list<Ast*>::iterator pit_temp = pit;
		pit_temp++;
		(*pit)->print(file_buffer);
		if(pit_temp!=name_list->end())
			file_buffer<<", ";
	}
	file_buffer<<",\n";
}

void Decl_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Declaration>\n";
	tab_counter++;
	print_attribute(file_buffer, "TypeName", this->type_name);
	print_attribute(file_buffer, "Type", DataTypesArray[this->type]);
	print_attribute(file_buffer, "VariableList", this->name_list);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Declaration>\n";
}

/////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print(ostream & file_buffer) {
	//ADD CODE HERE
	file_buffer<<constant;
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<constant;
}

///////////////////////////////////////////////////////////////////////////////

void Plus_Ast::print(ostream& file_buffer) {
	//ADD CODE HERE
	file_buffer<<"(";
	lhs->print(file_buffer);
	file_buffer<<" + ";
	rhs->print(file_buffer);
	file_buffer<<")";
}

void Plus_Ast::print_xml(ostream& file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Plus>\n";
	tab_counter++;
	print_attribute(file_buffer, "LOperand", this->lhs);
	print_attribute(file_buffer, "ROperand", this->rhs);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Plus>\n";
}

/////////////////////////////////////////////////////////////////

void Minus_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE
	file_buffer<<"(";
	lhs->print(file_buffer);
	file_buffer<<" - ";
	rhs->print(file_buffer);
	file_buffer<<")";
}

void Minus_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Minus>\n";
	tab_counter++;
	print_attribute(file_buffer, "LOperand", this->lhs);
	print_attribute(file_buffer, "ROperand", this->rhs);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Minus>\n";
}

//////////////////////////////////////////////////////////////////

void Mult_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE
	file_buffer<<"(";
	lhs->print(file_buffer);
	file_buffer<<" * ";
	rhs->print(file_buffer);
	file_buffer<<")";
}

void Mult_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Mult>\n";
	tab_counter++;
	print_attribute(file_buffer, "LOperand", this->lhs);
	print_attribute(file_buffer, "ROperand", this->rhs);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Mult>\n";
}

////////////////////////////////////////////////////////////////////

void Divide_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE
	file_buffer<<"(";
	lhs->print(file_buffer);
	file_buffer<<" / ";
	rhs->print(file_buffer);
	file_buffer<<")";
}

void Divide_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Divide>\n";
	tab_counter++;
	print_attribute(file_buffer, "LOperand", this->lhs);
	print_attribute(file_buffer, "ROperand", this->rhs);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Divide>\n";
}

//////////////////////////////////////////////////////////////////////

void Modulo_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE
	file_buffer<<"(";
	lhs->print(file_buffer);
	file_buffer<<" % ";
	rhs->print(file_buffer);
	file_buffer<<")";
}

void Modulo_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Modulo>\n";
	tab_counter++;
	print_attribute(file_buffer, "LOperand", this->lhs);
	print_attribute(file_buffer, "ROperand", this->rhs);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Modulo>\n";
}

//////////////////////////////////////////////////////////////////////

void UMinus_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE
	file_buffer<<"(";
	file_buffer<<" - ";
	lhs->print(file_buffer);
	file_buffer<<")";
}

void UMinus_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<UMinus>\n";
	tab_counter++;
	print_attribute(file_buffer, "Operand", this->lhs);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</UMinus>\n";
}

///////////////////////////////////////////////////////////////////////

void Relational_Expr_Ast::print(ostream & file_buffer) {
	file_buffer<<"(";
	lhs_condition->print(file_buffer);
	file_buffer<<" "<<RelationalPrintArray[this->rel_op]<<" ";
	rhs_condition->print(file_buffer);
	file_buffer<<")";
}

void Relational_Expr_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Relational>\n";
	tab_counter++;
	print_attribute(file_buffer, "LOperand", this->lhs_condition);
	print_attribute(file_buffer, "ROperand", this->rhs_condition);
	print_attribute(file_buffer, "Type", RelationalOpsArray[this->rel_op]);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Relational>\n";
}

///////////////////////////////////////////////////////////////////////

void Boolean_Expr_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE
	file_buffer<<"(";
	lhs_op->print(file_buffer);
	file_buffer<<" "<<BooleanPrintArray[this->bool_op]<<" ";
	rhs_op->print(file_buffer);
	file_buffer<<")";
}

void Boolean_Expr_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<BooleanExpr>\n";
	tab_counter++;
	print_attribute(file_buffer, "LOperand", this->lhs_op);
	print_attribute(file_buffer, "ROperand", this->rhs_op);
	print_attribute(file_buffer, "Type", BooleanOpsArray[this->bool_op]);
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</BooleanExpr>\n";
}

///////////////////////////////////////////////////////////////////////

void Port_Expr_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE
	lhs->print(file_buffer);
	file_buffer<<"|";
	rhs->print(file_buffer);
}

void Port_Expr_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<PortExpr>\n";
	tab_counter++;
	print_attribute(file_buffer, "Party", this->lhs);
	print_attribute(file_buffer, "Port", this->rhs);
	// print_attribute(file_buffer, "LineNo", this->linenoe);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</PortExpr>\n";
}

///////////////////////////////////////////////////////////////////////

void Party_Expr_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE
	lhs->print(file_buffer);
	file_buffer<<".";
	rhs->print(file_buffer);
}

void Party_Expr_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<PartyExpr>\n";
	tab_counter++;
	print_attribute(file_buffer, "ParentParty", this->lhs);
	print_attribute(file_buffer, "ChildExpression", this->rhs);
	// print_attribute(file_buffer, "LineNo", this->linenoe);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</PartyExpr>\n";
}

///////////////////////////////////////////////////////////////////////

void From_Expr_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE
	lhs_op->print(file_buffer);
	file_buffer<<" from ";
	rhs_op->print(file_buffer);
}

void From_Expr_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<FromList>\n";
	tab_counter++;
	print_attribute(file_buffer, "Variable", this->lhs_op);
	print_attribute(file_buffer, "List", this->rhs_op);
	// print_attribute(file_buffer, "LineNo", this->linenoe);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</FromList>\n";
}

///////////////////////////////////////////////////////////////////////

void Expression_List_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE
	file_buffer<<"{";
	list<Ast*>::iterator pit = exp_list->begin();
	for(;pit!=exp_list->end();pit++)
	{
		list<Ast*>::iterator pit_temp = pit;
		pit_temp++;
		(*pit)->print(file_buffer);
		if(pit_temp!=exp_list->end())
			file_buffer<<", ";
	}
	file_buffer<<"}";
}

void Expression_List_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<ExpressionList>\n";
	tab_counter++;
	if(this->exp_list!=NULL)
		for(auto ele : *this->exp_list)
			ele->print_xml(file_buffer);
	// print_attribute(file_buffer, "LineNo", this->linenoe);

	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</ExpressionList>\n";
}

///////////////////////////////////////////////////////////////////////

void Session_call_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE	
	print_tabs(file_buffer, tab_counter);
	if(partial)file_buffer<<"partial ";
	file_buffer<<"open ";
	session_name->print(file_buffer);
	file_buffer<<" as "<<proc_name;

	file_buffer<<"(";
	list<Ast*>::iterator pit = arg_list->begin();
	for(;pit!=arg_list->end();pit++)
	{
		list<Ast*>::iterator pit_temp = pit;
		pit_temp++;
		(*pit)->print(file_buffer);
		if(pit_temp!=arg_list->end())
			file_buffer<<", ";
	}
	file_buffer<<"){\n";
	tab_counter++;
	seq->print(file_buffer);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"}\n";
}

void Session_call_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<SessionCall>\n";
	tab_counter++;
	print_attribute(file_buffer, "ProtocolName", this->proc_name);
	print_attribute(file_buffer, "SessionName", this->session_name);
	print_attribute(file_buffer, "IsPartial", BoolSymbolsArray[this->partial]);
	print_attribute(file_buffer, "ArgumentList", this->arg_list);
	print_attribute(file_buffer, "Body", this->seq);
	// print_attribute(file_buffer, "LineNo", this->linenoe);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</SessionCall>\n";
}

///////////////////////////////////////////////////////////////////////

void Set_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE	
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"set ";
	source->print(file_buffer);
	file_buffer<<" as "<<proc_name;

	file_buffer<<"(";
	list<Ast*>::iterator pit = arg_list->begin();
	for(;pit!=arg_list->end();pit++)
	{
		list<Ast*>::iterator pit_temp = pit;
		pit_temp++;
		(*pit)->print(file_buffer);
		if(pit_temp!=arg_list->end())
			file_buffer<<", ";
	}
	file_buffer<<")::";
	dest->print(file_buffer);
	file_buffer<<";\n";
}

void Set_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Set>\n";
	tab_counter++;
	print_attribute(file_buffer, "TargetParty", this->dest);
	print_attribute(file_buffer, "SourceProtocol", this->proc_name);
	print_attribute(file_buffer, "SourceParty", this->source);
	print_attribute(file_buffer, "ArgumentList", this->arg_list);
	// print_attribute(file_buffer, "LineNo", this->linenoe);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Set>\n";
}

///////////////////////////////////////////////////////////////////////

void Term_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE	
	if(t!=variable && t!= constant)file_buffer<<TermPrintArray[t];
	if(child != NULL) child->print(file_buffer);
	if(dim_list!=NULL){
		list<Ast*>::iterator pit = dim_list->begin();
		for(;pit!=dim_list->end();pit++)
		{
			file_buffer<<"[";
			if((*pit)!=NULL)(*pit)->print(file_buffer);
			file_buffer<<"]";
		}
	}
}

void Term_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Set>\n";
	tab_counter++;
	if(this->child!=NULL)
		this->child->print_xml(file_buffer);
	print_attribute(file_buffer, "DimensionList", this->dim_list);
	print_attribute(file_buffer, "TermType", TermTypesArray[this->t]);
	// print_attribute(file_buffer, "LineNo", this->linenoe);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Set>\n";
}

///////////////////////////////////////////////////////////////////////////

void Decl_Term_Ast::print(ostream & file_buffer) {
	term->print(file_buffer);
	if(port_clause_exists)
	{
		if(each)file_buffer<<" each";
		file_buffer<<" with {\n";
		tab_counter++;
		
		print_tabs(file_buffer, tab_counter);
		list<pair<pair<Data_Type, string> *, Ast*> *>::iterator pit = port_clause->begin();
		for(;pit!=port_clause->end();pit++)
		{
			list<pair<pair<Data_Type, string> *, Ast*> *>::iterator pit_temp = pit;
			pit_temp++;
			pair<pair<Data_Type, string> *, Ast*> * decl = (*pit);
			if(decl->first->first == other_data_type)
				file_buffer<<decl->first->second<<" ";
			else file_buffer<<DataTypesArray[decl->first->first]<<" ";
			decl->second->print(file_buffer);
			if(pit_temp!=port_clause->end())
				file_buffer<<", ";
			else file_buffer<<"\n";
		}
		tab_counter--;
		print_tabs(file_buffer, tab_counter);
		file_buffer<<"}";
	}
}

void Decl_Term_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Declaration>\n";
	tab_counter++;
	if(this->term){
		this->term->print_xml(file_buffer);
	}
	if(this->port_clause_exists) {
		print_attribute(file_buffer, "IsEachPresent", BoolSymbolsArray[this->each]);
		if(port_clause!=NULL)
		{
			print_tabs(file_buffer, tab_counter);
			file_buffer<<"<"<<"PortDeclarations"<<">\n";
			tab_counter++;
			for(auto ele: *port_clause) {
				if(ele!=NULL)
				{
					print_tabs(file_buffer, tab_counter);
					file_buffer<<"<Declaration>\n";
					tab_counter++;
					print_attribute(file_buffer, "TypeName", ele->first->second);
					print_attribute(file_buffer, "Type", DataTypesArray[ele->first->first]);
					print_attribute(file_buffer, "VariableList", ele->second);
					// print_attribute(file_buffer, "LineNo", this->lineno);
					tab_counter--;
					print_tabs(file_buffer, tab_counter);
					file_buffer<<"</Declaration>\n";
				}
			}
			tab_counter--;
			print_tabs(file_buffer, tab_counter);
			file_buffer<<"</"<<"PortDeclarations"<<">\n";
		}
	}
	// print_attribute(file_buffer, "LineNo", this->lineno);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Declaration>\n";
}

///////////////////////////////////////////////////////////////////////

void Abort_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE	
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"abort;\n";
}

void Abort_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Abort/>\n";
	// print_attribute(file_buffer, "LineNo", this->linenoe);
}

///////////////////////////////////////////////////////////////////////

void Argument_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE	
	if(this->type == other_data_type)
		file_buffer<<name<<" ";
	else file_buffer<<DataTypesArray[this->type]<<" ";
	term->print(file_buffer);
}

void Argument_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Argument>\n";
	tab_counter++;
	print_attribute(file_buffer, "Term", this->term);
	print_attribute(file_buffer, "Type", DataTypesArray[this->type]);
	print_attribute(file_buffer, "Name", this->name);
	// print_attribute(file_buffer, "LineNo", this->linenoe);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Argument>\n";		
}

///////////////////////////////////////////////////////////////////////


void Prot_Arg_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE	
	file_buffer<<"(";
	prot_decl->print(file_buffer);
	file_buffer<<")";
}

void Prot_Arg_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Argument>\n";
	tab_counter++;
	print_attribute(file_buffer, "Protocol", this->prot_decl);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Argument>\n";		
}

///////////////////////////////////////////////////////////////////////

void Wrap_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE	
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"wrap {\n";
	tab_counter++;
	stmt->print(file_buffer);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"}\n";
}

void Wrap_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Wrap>\n";
	tab_counter++;
	print_attribute(file_buffer, "Statement", this->stmt);
	// print_attribute(file_buffer, "LineNo", this->linenoe);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Wrap>\n";		
}

/////////////////////////////////////////////////////////////////////////

void Start_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE	
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"start ";
	party_exp->print(file_buffer);
	file_buffer<<";\n";
}

void Start_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Start>\n";
	tab_counter++;
	print_attribute(file_buffer, "Party", this->party_exp);
	// print_attribute(file_buffer, "LineNo", this->linenoe);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</Start>\n";		
}

/////////////////////////////////////////////////////////////////////////

void Selection_Statement_Ast::print(ostream & file_buffer){
	//ADD CODE HERE	
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"if(";
	cond->print(file_buffer);
	file_buffer<<"){\n";
	tab_counter++;
	then_part->print(file_buffer);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"}\n";
	if(else_part!=NULL)
	{	
		print_tabs(file_buffer, tab_counter);
		file_buffer<<"else{\n";
		tab_counter++;
		else_part->print(file_buffer);
		tab_counter--;
		file_buffer<<"}\n";
	}
}

void Selection_Statement_Ast::print_xml(ostream & file_buffer){
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<SelectionStatement>\n";
	tab_counter++;
	print_attribute(file_buffer, "Condition", this->cond);
	print_attribute(file_buffer, "Then", this->then_part);
	print_attribute(file_buffer, "Else", this->else_part);
	// print_attribute(file_buffer, "LineNo", this->linenoe);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</SelectionStatement>\n";
}

/////////////////////////////////////////////////////////////////////////

void Each_Statement_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE	
	file_buffer<<"each ";
	if(variable!=NULL) {
		variable->print(file_buffer);
		file_buffer<<" of ";
	}
	expression->print(file_buffer);
	if(cond!=NULL){
		file_buffer<<" "<<ConditionsArray[cond_type]<<" ";
		cond->print(file_buffer);
	}
	file_buffer<<" ";
}

void Each_Statement_Ast::print_xml(ostream & file_buffer){
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<EachClause>\n";
	tab_counter++;
	print_attribute(file_buffer, "Condition", this->cond);
	print_attribute(file_buffer, "ConditionType", this->cond_type);
	if(this->variable!=NULL)
		this->variable->print_xml(file_buffer);
	print_attribute(file_buffer, "Expression", this->expression);
	// print_attribute(file_buffer, "LineNo", this->linenoe);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</EachClause>\n";
}

/////////////////////////////////////////////////////////////////////////


void Iteration_Statement_Ast::print(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"for ";
	each_block->print(file_buffer);
	file_buffer<<"{\n";
	tab_counter++;
	body->print(file_buffer);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"}\n";
}

void Iteration_Statement_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<IterationStatement>\n";
	tab_counter++;
	if(this->each_block!=NULL)
		this->each_block->print_xml(file_buffer);
	print_attribute(file_buffer, "Body", this->body);
	// print_attribute(file_buffer, "LineNo", this->linenoe);
	tab_counter--;
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"</IterationStatement>\n";
}

template class Number_Ast<double>;
template class Number_Ast<int>;



