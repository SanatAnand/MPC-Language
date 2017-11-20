#include <iostream>
#include <fstream>
#include <typeinfo>
#include <map>

using namespace std;

#include "ast.hh"
#include "error-display.hh"
#include "symbol-table.hh"

bool Protocol_Ast::check_semantics(Symbol_Table* symbol_table, string tag)
{
	//create new symbol table for the protocol
	Symbol_Table* child_table = new Symbol_Table(symbol_table);
	//check semantics of protocol_decl
	bool b1 = this->protocol_decl->check_semantics(child_table, "protocol");
	//check semantics of uses_ast
	for(auto ele : *(this->uses_ast))
		b1 = b1 && ele->check_semantics(child_table, "protocol");
	//check semantics of body (sequence_ast)
	b1 = b1 && this->sequence_ast->check_semantics(child_table, "protocol");
	delete (child_table);
	return b1;
}

bool Protocol_Decl_Ast::check_semantics(Symbol_Table* symbol_table, string tag)
{
	bool b1 = true;
	//insert name of protocol into the symbol table
	symbol_table->insert(this->name, 0, protocol_data_type, "PROTOCOL", tag);
	//check semantics of each element in arg_list
	for(auto ele : *(this->arg_list))
		b1 = b1 && ele->check_semantics(symbol_table, tag);
	//check semantics of each element in party_list
	for(auto ele : *(this->party_list))
		b1 = b1 && ele->check_semantics(symbol_table, tag);
	return b1;
}

bool Sequence_Ast::check_semantics(Symbol_Table* symbol_table, string tag)
{
	bool b1 = true;
	//create new symbol table corresponding to new block
	Symbol_Table* child_table = new Symbol_Table(symbol_table);
	//check semantics of each statement in statement_list
	for(auto ele: this->statement_list)
		b1 = b1 && ele->check_semantics(child_table, tag);
	delete (child_table);
	return b1;
}

bool Send_Ast::check_semantics(Symbol_Table* symbol_table, string tag)
{
	//desugared to Send_Assignment_Ast
	//no need to handle
}

bool Assignment_Ast::check_semantics(Symbol_Table* symbol_table, string tag)
{
	bool b1 = true;
	//lhs must be a mutable
	CHECK_INPUT(((Term_Ast*)(this->lhs))->t == variable, "LHS not mutable", this->lineno);
	if(((Term_Ast*)(this->lhs))->t != variable)
		return false;
	//check semantics of lhs and set data_type in its term_ast
	//TODO: lhs is an indexed array variable (out of bounds check)
	b1 = b1 && this->lhs->check_semantics(symbol_table, tag);
	
	if(typeid(*(this->rhs))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->rhs))->t != random_sample || ((Term_Ast*)(this->lhs))->data_type == other_data_type,
			"Sampling must be done from a field", this->lineno);
		if(!(((Term_Ast*)(this->rhs))->t != random_sample || ((Term_Ast*)(this->lhs))->data_type == other_data_type))
			return false;
		if(((Term_Ast*)(this->rhs))->t == random_sample)
		{
			return b1;
		}
	}
	//check semantics of rhs and set data_type in its expr_ast
	b1 = b1 && this->rhs->check_semantics(symbol_table, tag);

	//check data_types of lhs and rhs are equal and appropriate
	Data_Type temp1 = ((Term_Ast*)(this->lhs))->data_type;
	Data_Type temp2 = ((Expr_Ast*)(this->rhs))->data_type;

	string temp1_name = ((Term_Ast*)(this->lhs))->data_type_name;
	string temp2_name = ((Expr_Ast*)(this->rhs))->data_type_name;

	CHECK_INPUT((temp1 == int_data_type || temp1 == bool_data_type || temp1 == other_data_type) && temp1 == temp2,
		"LHS and RHS data type mismatch", this->lineno);
	if(!((temp1 == int_data_type || temp1 == bool_data_type || temp1 == other_data_type) && temp1 == temp2))
	{
		return false;
	}
	CHECK_INPUT( !(temp1==other_data_type && temp2==other_data_type && !temp1_name.compare(temp2_name)), 
		"LHS and RHS field mismatch",this->lineno);
	if(!(!(temp1==other_data_type && temp2==other_data_type && !temp1_name.compare(temp2_name))))
	{
		return false;
	}
	return b1;
}

// bool Party_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
// }

bool Tying_Ast::check_semantics(Symbol_Table* symbol_table, string tag){
}

bool Forward_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool In_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool With_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Seq_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Connect_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Port_Clause_Ast::check_semantics(Symbol_Table* symbol_table, string tag){
}

bool Decl_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
	//if field declaration, scope should be protocol_scope
	//if field element declaration, 
}

template <class DATA_TYPE>
bool Number_Ast<DATA_TYPE>::check_semantics(Symbol_Table* symbol_table, string tag) {
	return true;
}

bool Name_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
	return true;
}

bool Plus_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
	bool b1 = true;
	//lhs must be a mutable
		
	if(typeid(*(this->lhs))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->lhs))->t == variable || ((Term_Ast*)(this->lhs))->t == constant,
			"Invalid term used in lhs of add", this->lineno);
		if(!(((Term_Ast*)(this->lhs))->t == variable || ((Term_Ast*)(this->lhs))->t == constant))
			return false;
	}

	//check semantics of lhs and set data_type in its term_ast
	//TODO: lhs is an indexed array variable (out of bounds check)
	b1 = b1 && this->lhs->check_semantics(symbol_table, tag);
	
	if(typeid(*(this->rhs))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->rhs))->t == variable || ((Term_Ast*)(this->rhs))->t == constant,
			"Invalid term used in rhs of add", this->lineno);
		if(!(((Term_Ast*)(this->rhs))->t == variable || ((Term_Ast*)(this->rhs))->t == constant))
			return false;
	}
	
	//check semantics of rhs and set data_type in its expr_ast
	b1 = b1 && this->rhs->check_semantics(symbol_table, tag);

	//check data_types of lhs and rhs are equal and appropriate
	Data_Type temp1 = ((Expr_Ast*)(this->lhs))->data_type;
	Data_Type temp2 = ((Expr_Ast*)(this->rhs))->data_type;

	string temp1_name = ((Expr_Ast*)(this->lhs))->data_type_name;
	string temp2_name = ((Expr_Ast*)(this->rhs))->data_type_name;

	CHECK_INPUT((temp1 == int_data_type || temp1 == other_data_type) && temp1 == temp2,
		"LHS and RHS data type mismatch", this->lineno);
	if(!((temp1 == int_data_type || temp1 == other_data_type) && temp1 == temp2))
		return false;
	CHECK_INPUT( !(temp1==other_data_type && temp2==other_data_type && !temp1_name.compare(temp2_name)),
		"LHS and RHS field mismatch",this->lineno);
	if((temp1==other_data_type && temp2==other_data_type && !temp1_name.compare(temp2_name)))
		return false;
	this->data_type = temp1;
	this->data_type_name = temp1_name;

	return b1;
}

bool Minus_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
	bool b1 = true;
	//lhs must be a mutable
		
	if(typeid(*(this->lhs))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->lhs))->t == variable || ((Term_Ast*)(this->lhs))->t == constant,
			"Invalid term used in lhs of subtraction", this->lineno);
		if(!(((Term_Ast*)(this->lhs))->t == variable || ((Term_Ast*)(this->lhs))->t == constant))
			return false;
	}

	//check semantics of lhs and set data_type in its term_ast
	//TODO: lhs is an indexed array variable (out of bounds check)
	b1 = b1 && this->lhs->check_semantics(symbol_table, tag);
	
	if(typeid(*(this->rhs))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->rhs))->t == variable || ((Term_Ast*)(this->rhs))->t == constant,
			"Invalid term used in rhs of subtraction", this->lineno);
		if(!(((Term_Ast*)(this->rhs))->t == variable || ((Term_Ast*)(this->rhs))->t == constant))
			return false;
	}
	
	//check semantics of rhs and set data_type in its expr_ast
	b1 = b1 && this->rhs->check_semantics(symbol_table, tag);

	//check data_types of lhs and rhs are equal and appropriate
	Data_Type temp1 = ((Expr_Ast*)(this->lhs))->data_type;
	Data_Type temp2 = ((Expr_Ast*)(this->rhs))->data_type;

	string temp1_name = ((Expr_Ast*)(this->lhs))->data_type_name;
	string temp2_name = ((Expr_Ast*)(this->rhs))->data_type_name;

	CHECK_INPUT((temp1 == int_data_type || temp1 == other_data_type) && temp1 == temp2,
		"LHS and RHS data type mismatch", this->lineno);
	if(!((temp1 == int_data_type || temp1 == other_data_type) && temp1 == temp2))
		return false;
	CHECK_INPUT( !(temp1==other_data_type && temp2==other_data_type && !temp1_name.compare(temp2_name)),
		"LHS and RHS field mismatch",this->lineno);
	if((temp1==other_data_type && temp2==other_data_type && !temp1_name.compare(temp2_name)))
		return false;
	this->data_type = temp1;
	this->data_type_name = temp1_name;

	return b1;
}

bool Mult_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
	bool b1 = true;
	//lhs must be a mutable
		
	if(typeid(*(this->lhs))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->lhs))->t == variable || ((Term_Ast*)(this->lhs))->t == constant,
			"Invalid term used in lhs of multiplication", this->lineno);
		if(!(((Term_Ast*)(this->lhs))->t == variable || ((Term_Ast*)(this->lhs))->t == constant))
			return false;
	}

	//check semantics of lhs and set data_type in its term_ast
	//TODO: lhs is an indexed array variable (out of bounds check)
	b1 = b1 && this->lhs->check_semantics(symbol_table, tag);
	
	if(typeid(*(this->rhs))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->rhs))->t == variable || ((Term_Ast*)(this->rhs))->t == constant,
			"Invalid term used in rhs of multiplication", this->lineno);
		if(!(((Term_Ast*)(this->rhs))->t == variable || ((Term_Ast*)(this->rhs))->t == constant))
			return false;
	}
	
	//check semantics of rhs and set data_type in its expr_ast
	b1 = b1 && this->rhs->check_semantics(symbol_table, tag);

	//check data_types of lhs and rhs are equal and appropriate
	Data_Type temp1 = ((Expr_Ast*)(this->lhs))->data_type;
	Data_Type temp2 = ((Expr_Ast*)(this->rhs))->data_type;

	string temp1_name = ((Expr_Ast*)(this->lhs))->data_type_name;
	string temp2_name = ((Expr_Ast*)(this->rhs))->data_type_name;

	CHECK_INPUT((temp1 == int_data_type || temp1 == other_data_type) && temp1 == temp2,
		"LHS and RHS data type mismatch", this->lineno);
	if(!((temp1 == int_data_type || temp1 == other_data_type) && temp1 == temp2))
		return false;
	CHECK_INPUT( !(temp1==other_data_type && temp2==other_data_type && !temp1_name.compare(temp2_name)),
		"LHS and RHS field mismatch",this->lineno);
	if((temp1==other_data_type && temp2==other_data_type && !temp1_name.compare(temp2_name)))
		return false;
	this->data_type = temp1;
	this->data_type_name = temp1_name;

	return b1;
}

bool Divide_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
	bool b1 = true;
	//lhs must be a mutable
		
	if(typeid(*(this->lhs))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->lhs))->t == variable || ((Term_Ast*)(this->lhs))->t == constant,
			"Invalid term used in lhs of divide", this->lineno);
		if(!(((Term_Ast*)(this->lhs))->t == variable || ((Term_Ast*)(this->lhs))->t == constant))
			return false;
	}

	//check semantics of lhs and set data_type in its term_ast
	//TODO: lhs is an indexed array variable (out of bounds check)
	b1 = b1 && this->lhs->check_semantics(symbol_table, tag);
	
	if(typeid(*(this->rhs))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->rhs))->t == variable || ((Term_Ast*)(this->rhs))->t == constant,
			"Invalid term used in rhs of divide", this->lineno);
		if(!(((Term_Ast*)(this->rhs))->t == variable || ((Term_Ast*)(this->rhs))->t == constant))
			return false;
	}
	
	//check semantics of rhs and set data_type in its expr_ast
	b1 = b1 && this->rhs->check_semantics(symbol_table, tag);

	//check data_types of lhs and rhs are equal and appropriate
	Data_Type temp1 = ((Expr_Ast*)(this->lhs))->data_type;
	Data_Type temp2 = ((Expr_Ast*)(this->rhs))->data_type;

	string temp1_name = ((Expr_Ast*)(this->lhs))->data_type_name;
	string temp2_name = ((Expr_Ast*)(this->rhs))->data_type_name;

	CHECK_INPUT((temp1 == int_data_type || temp1 == other_data_type) && temp1 == temp2,
		"LHS and RHS data type mismatch", this->lineno);
	if(!((temp1 == int_data_type || temp1 == other_data_type) && temp1 == temp2))
		return false;
	CHECK_INPUT( !(temp1==other_data_type && temp2==other_data_type && !temp1_name.compare(temp2_name)),
		"LHS and RHS field mismatch",this->lineno);
	if((temp1==other_data_type && temp2==other_data_type && !temp1_name.compare(temp2_name)))
		return false;
	this->data_type = temp1;
	this->data_type_name = temp1_name;

	return b1;
}

bool Modulo_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
	bool b1 = true;
	//lhs must be a mutable
		
	if(typeid(*(this->lhs))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->lhs))->t == variable || ((Term_Ast*)(this->lhs))->t == constant,
			"Invalid term used in lhs of modulo", this->lineno);
		if(!(((Term_Ast*)(this->lhs))->t == variable || ((Term_Ast*)(this->lhs))->t == constant))
			return false;
	}

	//check semantics of lhs and set data_type in its term_ast
	//TODO: lhs is an indexed array variable (out of bounds check)
	b1 = b1 && this->lhs->check_semantics(symbol_table, tag);
	
	if(typeid(*(this->rhs))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->rhs))->t == variable || ((Term_Ast*)(this->rhs))->t == constant,
			"Invalid term used in rhs of modulo", this->lineno);
		if(!(((Term_Ast*)(this->rhs))->t == variable || ((Term_Ast*)(this->rhs))->t == constant))
			return false;
	}
	
	//check semantics of rhs and set data_type in its expr_ast
	b1 = b1 && this->rhs->check_semantics(symbol_table, tag);

	//check data_types of lhs and rhs are equal and appropriate
	Data_Type temp1 = ((Expr_Ast*)(this->lhs))->data_type;
	Data_Type temp2 = ((Expr_Ast*)(this->rhs))->data_type;

	string temp1_name = ((Expr_Ast*)(this->lhs))->data_type_name;
	string temp2_name = ((Expr_Ast*)(this->rhs))->data_type_name;

	CHECK_INPUT((temp1 == int_data_type || temp1 == other_data_type) && temp1 == temp2,
		"LHS and RHS data type mismatch", this->lineno);
	if(!((temp1 == int_data_type || temp1 == other_data_type) && temp1 == temp2))
		return false;
	CHECK_INPUT( !(temp1==other_data_type && temp2==other_data_type && !temp1_name.compare(temp2_name)),
		"LHS and RHS field mismatch",this->lineno);
	if((temp1==other_data_type && temp2==other_data_type && !temp1_name.compare(temp2_name)))
		return false;
	this->data_type = temp1;
	this->data_type_name = temp1_name;

	return b1;
}

bool UMinus_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
	bool b1 = true;
	//lhs must be a mutable
		
	if(typeid(*(this->lhs))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->lhs))->t == variable || ((Term_Ast*)(this->lhs))->t == constant,
			"Invalid term used in lhs of uminus", this->lineno);
		if(!(((Term_Ast*)(this->lhs))->t == variable || ((Term_Ast*)(this->lhs))->t == constant))
			return false;
	}

	//check semantics of lhs and set data_type in its term_ast
	//TODO: lhs is an indexed array variable (out of bounds check)
	b1 = b1 && this->lhs->check_semantics(symbol_table, tag);

	//check data_types of lhs and rhs are equal and appropriate
	Data_Type temp1 = ((Expr_Ast*)(this->lhs))->data_type;

	string temp1_name = ((Expr_Ast*)(this->lhs))->data_type_name;

	CHECK_INPUT((temp1 == int_data_type || temp1 == other_data_type),
		"Invalid data type for unary minus", this->lineno);
	if(!(temp1 == int_data_type || temp1 == other_data_type))
		return false;
	this->data_type = temp1;
	this->data_type_name = temp1_name;

	return b1;
}

bool Relational_Expr_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
	bool b1 = true;
	//lhs must be a mutable
		
	if(typeid(*(this->lhs_condition))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->lhs_condition))->t == variable || ((Term_Ast*)(this->lhs_condition))->t == constant,
			"Invalid term used in lhs of relation", this->lineno);
		if(!(((Term_Ast*)(this->lhs_condition))->t == variable || ((Term_Ast*)(this->lhs_condition))->t == constant))
			return false;
	}

	//check semantics of lhs and set data_type in its term_ast
	//TODO: lhs is an indexed array variable (out of bounds check)
	b1 = b1 && this->lhs_condition->check_semantics(symbol_table, tag);
	
	if(typeid(*(this->rhs_condition))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->rhs_condition))->t == variable || ((Term_Ast*)(this->rhs_condition))->t == constant,
			"Invalid term used in rhs of relation", this->lineno);
		if(!(((Term_Ast*)(this->rhs_condition))->t == variable || ((Term_Ast*)(this->rhs_condition))->t == constant))
			return false;
	}
	
	//check semantics of rhs and set data_type in its expr_ast
	b1 = b1 && this->rhs_condition->check_semantics(symbol_table, tag);

	//check data_types of lhs and rhs are equal and appropriate
	Data_Type temp1 = ((Expr_Ast*)(this->lhs_condition))->data_type;
	Data_Type temp2 = ((Expr_Ast*)(this->rhs_condition))->data_type;

	string temp1_name = ((Expr_Ast*)(this->lhs_condition))->data_type_name;
	string temp2_name = ((Expr_Ast*)(this->rhs_condition))->data_type_name;

	CHECK_INPUT((temp1 == int_data_type || temp1 == other_data_type) && temp1 == temp2,
		"LHS and RHS data type mismatch", this->lineno);
	if(!((temp1 == int_data_type || temp1 == other_data_type) && temp1 == temp2))
		return false;
	CHECK_INPUT( !(temp1==other_data_type && temp2==other_data_type && !temp1_name.compare(temp2_name)),
		"LHS and RHS field mismatch",this->lineno);
	if((temp1==other_data_type && temp2==other_data_type && !temp1_name.compare(temp2_name)))
		return false;

	return b1;
}

bool Boolean_Expr_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
	bool b1 = true;
	//lhs must be a mutable
		
	if(typeid(*(this->lhs_op))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->lhs_op))->t == variable || ((Term_Ast*)(this->lhs_op))->t == constant,
			"Invalid term used in lhs of relation", this->lineno);
		if(!(((Term_Ast*)(this->lhs_op))->t == variable || ((Term_Ast*)(this->lhs_op))->t == constant))
			return false;
	}

	//check semantics of lhs and set data_type in its term_ast
	//TODO: lhs is an indexed array variable (out of bounds check)
	b1 = b1 && this->lhs_op->check_semantics(symbol_table, tag);
	
	if(typeid(*(this->rhs_op))==typeid(Term_Ast))
	{
		//if rhs is a random sample, lhs must be other_data_type
		CHECK_INPUT(((Term_Ast*)(this->rhs_op))->t == variable || ((Term_Ast*)(this->rhs_op))->t == constant,
			"Invalid term used in rhs of relation", this->lineno);
		if(!(((Term_Ast*)(this->rhs_op))->t == variable || ((Term_Ast*)(this->rhs_op))->t == constant))
			return false;
	}
	
	//check semantics of rhs and set data_type in its expr_ast
	b1 = b1 && this->rhs_op->check_semantics(symbol_table, tag);

	//check data_types of lhs and rhs are equal and appropriate
	Data_Type temp1 = ((Expr_Ast*)(this->lhs_op))->data_type;
	Data_Type temp2 = ((Expr_Ast*)(this->rhs_op))->data_type;

	CHECK_INPUT((temp1 == bool_data_type) && temp1 == temp2,
		"LHS and RHS data type mismatch", this->lineno);
	if(!((temp1 == bool_data_type) && temp1 == temp2))
		return false;

	return b1;
}

bool Port_Expr_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Party_Expr_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool From_Expr_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Expression_List_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Session_call_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Set_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Term_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
	if(this->t == variable)
	{
		Symbol_Table_Entry* res = symbol_table->find(((Name_Ast*)(this->child))->name, tag);
		CHECK_INPUT(res!=NULL, "Variable not declared", this->lineno);
		if(res==NULL)
			return false;
		CHECK_INPUT(this->dim_list->size() == res->no_of_dim, "Dimension mismatch", this->lineno);
		if(!(this->dim_list->size() == res->no_of_dim))
			return false;
		this->data_type = res->variable_data_type;
		this->data_type_name = res->field_name;
	}
	else if(this->t == iterator_variable) 
		;
	else if(this->t == random_sample) 
		;
	else if(this->t == constant) 
	{
		this->data_type = ((Expr_Ast*)(this->child))->data_type;
		this->data_type_name = ((Expr_Ast*)(this->child))->data_type_name;
	}
	else if(this->t == ideal)
	{
		this->data_type = party_data_type;
		this->data_type_name = "PARTY";
	}
	else if(this->t == rand_port) 
	{
		this->data_type = port_data_type;
		this->data_type_name = "PORT";
	}
	else if(this->t == env_port)
	{	
		this->data_type = port_data_type;
		this->data_type_name = "PORT"; 
	}
	else if(this->t == double_dot)
		;
	
	return true;
}

bool Decl_Term_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Abort_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Argument_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Prot_Arg_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Wrap_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Start_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Selection_Statement_Ast::check_semantics(Symbol_Table* symbol_table, string tag){
}

bool Each_Statement_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Iteration_Statement_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Basic_Loop_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Array_Limit_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Send_Assignment_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool CheckPartyID_Ast::check_semantics(Symbol_Table* symbol_table, string tag){
}

template class Number_Ast<bool>;
template class Number_Ast<int>;
