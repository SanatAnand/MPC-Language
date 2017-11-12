#include<iostream>
#include<fstream>
#include<typeinfo>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"ast.hh"
#include"program.hh"

Ast::Ast(){}

Ast::~Ast(){}

//////////////////////////////////////////////////////////////////////////////////////////

Protocol_Decl_Ast::Protocol_Decl_Ast(string name, list<Ast*>* arg_list, list<Ast*>* party_list, int lineno) {
	this->name = name;
	this->arg_list = arg_list;
	this->party_list = party_list;
	this->lineno = lineno;
}

string Protocol_Decl_Ast::get_protocol_name() {
	return name;
}

Protocol_Decl_Ast::~Protocol_Decl_Ast(){
	if(arg_list!=NULL)
		for(auto ele : *arg_list)
			delete ele;
}

//////////////////////////////////////////////////////////////////////////////////////////

Protocol_Ast::Protocol_Ast(Protocol_Decl_Ast* protocol_decl, list<Ast*>* uses_ast, Sequence_Ast* sequence_ast, int lineno) {
	this->protocol_decl = protocol_decl;
	this->uses_ast = uses_ast;
	this->sequence_ast = sequence_ast;
	this->lineno = lineno;
}

Protocol_Ast::~Protocol_Ast(){
	delete protocol_decl;
	delete sequence_ast;
	if(uses_ast!=NULL)
		for(auto ele : *uses_ast)
			delete ele;
}

void Protocol_Ast::set_sequence_ast(Sequence_Ast & sa) {
	this->sequence_ast = &sa;
}

////////////////////////////////////////////////////////////////

Sequence_Ast::Sequence_Ast(int line) {
	this->lineno = line;
}

void Sequence_Ast::ast_push_back(Ast* a) {
	statement_list.push_back(a);
}

Sequence_Ast::~Sequence_Ast() {
	for(auto ele : statement_list)
		delete(ele);
	statement_list.clear();
}

///////////////////////////////////////////////////////////////

Send_Ast::Send_Ast(Ast* lhs, Ast* rhs, int line) {
	this->lhs = lhs;
	this->rhs = rhs;
	this->lineno = line;
}

Send_Ast::~Send_Ast() {
	delete lhs;
	delete rhs;
}

///////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast* lhs, Ast* rhs, int line) {
	this->lhs = lhs;
	this->rhs = rhs;
	this->lineno = line;
}

Assignment_Ast::~Assignment_Ast() {
	delete lhs;
	delete rhs;
}

/////////////////////////////////////////////////////

Party_Ast::Party_Ast(string party_name, int party_type, int line) {
	this->party_name = party_name;
	this->party_type = party_type;
	this->lineno = line;
}

Party_Ast::~Party_Ast() {}

////////////////////////////////////////////////////

Tying_Ast::Tying_Ast(Ast* lhs, Ast* rhs, int line) {
	this->lhs = lhs;
	this->rhs = rhs;
	this->lineno = line;
}

Tying_Ast::~Tying_Ast() {
	delete lhs;
	delete rhs;
}

///////////////////////////////////////////////////

Forward_Ast::Forward_Ast(Ast* lhs, Ast* rhs, int line) {
	this->lhs = lhs;
	this->rhs = rhs;
	this->lineno = line;
}

Forward_Ast::~Forward_Ast() {
	delete lhs;
	delete rhs;
}

///////////////////////////////////////////////////

In_Ast::In_Ast(Ast * lhs, Ast * party, bool each, int line) {
	this->s_list = lhs;
	this->party = party;
	this->each = each;
	this->lineno = line;
}

In_Ast::~In_Ast() {
	delete s_list;
	delete party;
}

////////////////////////////////////////////////////////

With_Ast::With_Ast(list<pair<pair<Data_Type, string> *, Ast*> *> * temp_lhs, Ast * party, bool each, int line) {
	//ADD CODE HERE
	this->s_list = temp_lhs;
	this->party = party;
	this->each = each;
	this->lineno = line;
}

With_Ast::~With_Ast() {
	//ADD CODE HERE
	delete this->s_list;
	delete this->party;
}

////////////////////////////////////////////////////////

Seq_Ast::Seq_Ast(Ast * temp_lhs, Ast * party, bool each, order o, int line) {
	//ADD CODE HERE
	this->cond = temp_lhs;
	this->addendum = party;
	this->each = each;
	this->ord = o;
	this->lineno = line;
}

Seq_Ast::~Seq_Ast() {
	//ADD CODE HERE	
	delete cond;
	delete addendum;
}

////////////////////////////////////////////////////////

Connect_Ast::Connect_Ast(Ast* lhs, Ast* rhs, connection_param type, int line) {
	this->from = lhs;
	this->to = rhs;
	this->type = type;
	this->lineno = line;	
}

Connect_Ast::~Connect_Ast() {
	delete from;
	delete to;
}

///////////////////////////////////////////////////////////////////////

Port_Clause_Ast::Port_Clause_Ast(Ast * exp, bool is_clause_present, bool is_each_present, list<pair<pair<Data_Type, string> *, Ast*> *> * seq, int line) {
	this->party = exp;
	this->is_clause_present = is_clause_present;
	this->is_each_present = is_each_present;
	this->seq = seq;
	this->lineno = line;
}

Port_Clause_Ast::~Port_Clause_Ast(){
	delete party;
	delete seq;
}

///////////////////////////////////////////////////////////////////////

Decl_Ast::Decl_Ast(Data_Type type, list<Ast*>* name_list, string type_name, int line) {
	this->type = type;
	this->type_name = type_name;
	this->name_list = name_list;
	this->lineno = line;
}

Decl_Ast::~Decl_Ast() {
	if(name_list!=NULL)
		for(auto ele : *(name_list))
			delete ele;
}

/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string name, int line) {
	this->name = name;
	this->lineno = line;
}

Name_Ast::~Name_Ast() {}

void Name_Ast::print(ostream & file_buffer) {
	//ADD CODE HERE
	file_buffer<<this->name;
}

void Name_Ast::print_xml(ostream & file_buffer) {
	print_tabs(file_buffer, tab_counter);
	file_buffer<<"<Variable name=\""<<this->name<<"\"/>\n";
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, int line) {
	// use Ast_arity from ast.hh
	//ADD CODE HERE
	constant = number;
	lineno = line;
}

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast() {}

///////////////////////////////////////////////////////////////////////////////

Arithmetic_Expr_Ast::~Arithmetic_Expr_Ast() {
	delete lhs;
	delete rhs;
}

/////////////////////////////////////////////////////////////////////

Plus_Ast::Plus_Ast(Ast* l, Ast* r, int line) {
	// set arity and data type
	//ADD CODE HERE
	this->lhs = l;
	this->rhs = r;
	this->lineno = line;
}

/////////////////////////////////////////////////////////////////

Minus_Ast::Minus_Ast(Ast* l, Ast* r, int line) {
	lhs = l;
	rhs = r;
	lineno = line;
}

//////////////////////////////////////////////////////////////////

Mult_Ast::Mult_Ast(Ast* l, Ast* r, int line) {
	lhs = l;
	rhs = r;
	lineno = line;	
}

////////////////////////////////////////////////////////////////////

Divide_Ast::Divide_Ast(Ast* l, Ast* r, int line) {
	lhs = l;
	rhs = r;
	lineno = line;	
}

//////////////////////////////////////////////////////////////////////

Modulo_Ast::Modulo_Ast(Ast* l, Ast* r, int line) {
	lhs = l;
	rhs = r;
	lineno = line;	
}

//////////////////////////////////////////////////////////////////////

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line) {
	lhs = l;
	rhs = r;
	lineno = line;
}

///////////////////////////////////////////////////////////////////////

Relational_Expr_Ast::Relational_Expr_Ast(Ast* l, Relational_Op rop, Ast* r, int line) {
	this->lhs_condition = l;
	this->rhs_condition = r;
	this->rel_op = rop;
	this->lineno = line;
}

Relational_Expr_Ast::~Relational_Expr_Ast(){
	delete lhs_condition;
	delete rhs_condition;
}

///////////////////////////////////////////////////////////////////////

Boolean_Expr_Ast::Boolean_Expr_Ast(Ast * l, Boolean_Op bop, Ast * r, int line) {
	lhs_op = l;
	rhs_op = r;
	bool_op = bop;
	lineno = line;
}

Boolean_Expr_Ast::~Boolean_Expr_Ast() {
	delete lhs_op;
	delete rhs_op;
}

///////////////////////////////////////////////////////////////////////

Port_Expr_Ast::Port_Expr_Ast(Ast* l, Ast* r, int line) {
	lhs = l;
	rhs = r;
	lineno = line;
}

Port_Expr_Ast::~Port_Expr_Ast() {
	delete lhs;
	delete rhs;
}

///////////////////////////////////////////////////////////////////////

Party_Expr_Ast::Party_Expr_Ast(Ast* l, Ast* r, int line) {
	lhs = l;
	rhs = r;
	lineno = line;
}

Party_Expr_Ast::~Party_Expr_Ast() {
	delete lhs;
	delete rhs;
}

///////////////////////////////////////////////////////////////////////

From_Expr_Ast::From_Expr_Ast(Ast * l, Ast * r, int line) {
	this->lhs_op = l;
	this->rhs_op = r;
	this->lineno = line;
}

From_Expr_Ast::~From_Expr_Ast() {
	delete lhs_op;
	delete rhs_op;
}

///////////////////////////////////////////////////////////////////////

Expression_List_Ast::Expression_List_Ast(list<Ast*>* exp_list, int line) {
	this->exp_list = exp_list;
	this->lineno = line;
}

Expression_List_Ast::~Expression_List_Ast() {
	if(exp_list!=NULL)
		for(auto ele : *exp_list)
			delete ele;
}

///////////////////////////////////////////////////////////////////////
//updated

Session_call_Ast::Session_call_Ast(Ast* session_name, Ast* seq, bool partial, string proc_name, list<Ast*>* arg_list, int line) {
	this->seq = seq;
	this->proc_name = proc_name;
	this->session_name = session_name;
	this->partial = partial;
	this->arg_list = arg_list;
	this->lineno = line;
}

Session_call_Ast::~Session_call_Ast() {
	delete session_name;
	delete seq;
	if(arg_list!=NULL)
		for (auto ele : *arg_list)
			delete ele;
}

///////////////////////////////////////////////////////////////////////

Set_Ast::Set_Ast(Ast* src, Ast* dst, string name, list<Ast*>* arg_list, int line) {
	this->source = src;
	this->proc_name = name;
	this->dest = dst;
	this->arg_list = arg_list;
	this->lineno = line;
}

Set_Ast::~Set_Ast() {
	delete this->source;
	delete this->dest;
	if(arg_list!=NULL)
		for(auto ele : *arg_list)
			delete ele;
}

///////////////////////////////////////////////////////////////////////

Term_Ast::Term_Ast(Ast* c, list<Ast*>* dim, Term_Type type, int line) {
	this->child = c;
	this->dim_list = dim;
	this->t = type;
	this->lineno = line;
}

Term_Ast::~Term_Ast(){
	delete child;
	if(dim_list!=NULL)
		for(auto ele : *dim_list)
			delete ele;
}

///////////////////////////////////////////////////////////////////////////

Decl_Term_Ast::Decl_Term_Ast(Ast* term, bool port_clause_exists, list<pair<pair<Data_Type, string> *, Ast*> *> * port_clause, bool each, int line) {
	this->term = term;
	this->port_clause_exists = port_clause_exists;
	this->port_clause = port_clause;
	this->each = each;
	this->lineno = line;
}

Decl_Term_Ast::~Decl_Term_Ast() {
	delete term;
	delete port_clause;
}

///////////////////////////////////////////////////////////////////////

Prot_Arg_Ast::Prot_Arg_Ast(Ast *prot_decl, int line)
{
	this->prot_decl = prot_decl;
	this->lineno = line;
}

Prot_Arg_Ast::~Prot_Arg_Ast(){
	delete prot_decl;
}

///////////////////////////////////////////////////////////////////////

Abort_Ast::Abort_Ast(int line) {
	this->lineno = line;
}

Abort_Ast::~Abort_Ast() {}

///////////////////////////////////////////////////////////////////////

Argument_Ast::Argument_Ast(Ast *term, Data_Type type, string name, int line) {
	this->term = term;
	this->type = type;
	this->name = name;
	this->lineno = line;
}

Argument_Ast::~Argument_Ast() {
	delete term;
}

///////////////////////////////////////////////////////////////////////

Wrap_Ast::Wrap_Ast(Ast * stmt, int line) {
	this->stmt = stmt;
	this->lineno = line;
}

Wrap_Ast::~Wrap_Ast(){
	delete stmt;
}

/////////////////////////////////////////////////////////////////////////

Start_Ast::Start_Ast(Ast * party_exp, int line) {
	this->party_exp = party_exp;
	this->lineno = line;
}

Start_Ast::~Start_Ast(){
	delete party_exp;
}

/////////////////////////////////////////////////////////////////////////

Selection_Statement_Ast::Selection_Statement_Ast(Ast* cond, Ast* then_part, Ast* else_part, int line) {
	this->cond = cond;
	this->then_part = then_part;
	this->else_part = else_part;
	this->lineno = line;
}

Selection_Statement_Ast::~Selection_Statement_Ast() {
	delete cond;
	delete then_part;
	delete else_part;
}

/////////////////////////////////////////////////////////////////////////

Each_Statement_Ast::Each_Statement_Ast(Ast * cond, condition cond_type, Ast* variable, Ast* expression, int line) {
	this->cond = cond;
	this->cond_type = cond_type;
	this->variable = variable;
	this->expression = expression;
	this->lineno = line;
}

Each_Statement_Ast::~Each_Statement_Ast() {
	delete cond;
	delete variable;
	delete expression;
}

/////////////////////////////////////////////////////////////////////////

Iteration_Statement_Ast::Iteration_Statement_Ast(Ast* each_block, Ast* body, int line) {
	this->each_block = each_block;
	this->body = body;
	this->lineno = line;
}

Iteration_Statement_Ast::~Iteration_Statement_Ast() {
	delete each_block;
	delete body;
}

Basic_Loop_Ast::Basic_Loop_Ast( Ast* variable, Ast* limit, Ast* body)
{
	this->variable = variable;
	this->limit = limit;
	this->body = body;
}

Basic_Loop_Ast::~Basic_Loop_Ast()
{
	delete this->variable;
	delete this->body;
	delete this->limit;
}

Array_Limit_Ast::Array_Limit_Ast( string array, int index)
{
	this->array = array;
	this->index = index;
}

Array_Limit_Ast::~Array_Limit_Ast(){}

<<<<<<< HEAD
Send_Assignment_Ast::Send_Assignment_Ast(Ast* lhs, Ast* rhs, int line) {
	this->lhs = lhs;
	this->rhs = rhs;
	this->lineno = line;
}

Send_Assignment_Ast::~Send_Assignment_Ast() {
	delete lhs;
	delete rhs;
}
=======
CheckPartyID_Ast::CheckPartyID_Ast(list<Ast*>* party_addr)
{
	this->party_addr = party_addr;
}

CheckPartyID_Ast::~CheckPartyID_Ast(){}
>>>>>>> desugar_in

template class Number_Ast<double>;
template class Number_Ast<int>;
