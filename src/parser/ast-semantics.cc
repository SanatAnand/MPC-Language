#include <iostream>
#include <fstream>
#include <typeinfo>
#include <map>

using namespace std;

#include "ast.hh"
#include "error-display.hh"
#include "symbol-table.hh"

bool Protocol_Ast::check_semantics(Symbol_Table* symbol_table)
{
	CHECK_INVARIANT(symbol_table!=NULL, "Symbol table null");
	bool b1 = this->protocol_decl->check_semantics(symbol_table);
	for(auto ele : *uses_ast)
		b1 = b1 && ele->check_semantics(symbol_table);
	b1 = b1 && this->sequence_ast->check_semantics(symbol_table);
	return b1;
}

bool Protocol_Decl_Ast::check_semantics(Symbol_Table* symbol_table) {
	bool b1 = true;
	for(auto ele : *arg_list)
		b1 = b1 && ele->check_semantics(symbol_table);
	for(auto ele : *party_list)
		b1 = b1 && ele->check_semantics(symbol_table);
	return b1;
}

bool Sequence_Ast::check_semantics(Symbol_Table* symbol_table) {
	bool b1 = true;
	for(auto ele: statement_list)
		b1 = b1 && ele->check_semantics(symbol_table);
	return b1;
}

bool Send_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Assignment_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Party_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Tying_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Forward_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool In_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool With_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Seq_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Connect_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Port_Clause_Ast::check_semantics(Symbol_Table* symbol_table){
}

bool Decl_Ast::check_semantics(Symbol_Table* symbol_table) {
}

template <class DATA_TYPE>
bool Number_Ast<DATA_TYPE>::check_semantics(Symbol_Table* symbol_table) {
}

bool Name_Ast::check_semantics(Symbol_Table* symbol_table) {
	//ADD CODE HERE
}

bool Plus_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Minus_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Mult_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Divide_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Modulo_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool UMinus_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Relational_Expr_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Boolean_Expr_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Port_Expr_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Party_Expr_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool From_Expr_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Expression_List_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Session_call_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Set_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Term_Ast::check_semantics(Symbol_Table* symbol_table) {
	switch(t){
		case variable:
			break;
		case iterator_variable: 
			break;
		case wrapper: 
			break;
		case random_sample: 
			break;
		case constant: 
			break;
		case environment: 
			break;
		case ideal: 
			break;
		case rand_port: 
			break;
		case env_port: 
			break;
		case double_dot: 
			break;
	}
}

bool Decl_Term_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Abort_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Argument_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Prot_Arg_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Wrap_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Start_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Selection_Statement_Ast::check_semantics(Symbol_Table* symbol_table){
}

bool Each_Statement_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Iteration_Statement_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Basic_Loop_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Array_Limit_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool Send_Assignment_Ast::check_semantics(Symbol_Table* symbol_table) {
}

bool CheckPartyID_Ast::check_semantics(Symbol_Table* symbol_table){
}

template class Number_Ast<double>;
template class Number_Ast<int>;
