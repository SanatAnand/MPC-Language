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
	Symbol_Table* child_table = new Symbol_Table(symbol_table);
	bool b1 = this->protocol_decl->check_semantics(child_table, "protocol");
	for(auto ele : *uses_ast)
		b1 = b1 && ele->check_semantics(child_table, "protocol");
	b1 = b1 && this->sequence_ast->check_semantics(child_table, "protocol");
	return b1;
}

bool Protocol_Decl_Ast::check_semantics(Symbol_Table* symbol_table, string tag)
{
	bool b1 = true;
	for(auto ele : *arg_list)
		b1 = b1 && ele->check_semantics(symbol_table);
	for(auto ele : *party_list)
		b1 = b1 && ele->check_semantics(symbol_table);
	return b1;
}

bool Sequence_Ast::check_semantics(Symbol_Table* symbol_table, string tag)
{
	bool b1 = true;
	for(auto ele: statement_list)
		b1 = b1 && ele->check_semantics(symbol_table);
	return b1;
}

bool Send_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Assignment_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Party_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Tying_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
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
}

bool Name_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
	//ADD CODE HERE
}

bool Plus_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Minus_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Mult_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Divide_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Modulo_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool UMinus_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Relational_Expr_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
}

bool Boolean_Expr_Ast::check_semantics(Symbol_Table* symbol_table, string tag) {
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

template class Number_Ast<double>;
template class Number_Ast<int>;
