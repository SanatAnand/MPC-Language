#include <iostream>
#include <fstream>
#include <typeinfo>
#include <vector>

using namespace std;

#include "ast.hh"

//////////////////////////////////////////////////////////////////////////////////////////

Ast* Protocol_Decl_Ast::desugar () {
	list<Ast*>::iterator pit = arg_list->begin();
	for(;pit!=arg_list->end();pit++)
	{
		list<Ast*>::iterator pit_temp = pit;
		pit_temp++;
		*pit = (*pit) ->desugar();		
	}	
	for(pit = party_list->begin();pit!=party_list->end();pit++)
	{
		list<Ast*>::iterator pit_temp = pit;
		pit_temp++;
		*pit = (*pit) ->desugar();
	}
	return this;
}

//////////////////////////////////////////////////////////////////////////////////////////

Ast* Protocol_Ast::desugar () {
	protocol_decl = (Protocol_Decl_Ast*) protocol_decl ->desugar();
	if(uses_ast!=NULL)
	{
		for(list<Ast*>::iterator pit = uses_ast->begin();pit!=uses_ast->end();pit++)
		{
			(*pit) = (*pit) ->desugar();
		}
	}
	sequence_ast = (Sequence_Ast*) sequence_ast ->desugar();
	return this;
}

//////////////////////////////////////////////////////////////////////////////////////////

Ast* Sequence_Ast::desugar () {
	for (auto ele = this->statement_list.begin(); ele != this->statement_list.end(); ele++)
	    (*ele) = (*ele) ->desugar();
	return this;
}


//////////////////////////////////////////////////////////////////////////////////////////


Ast* Send_Ast::desugar () {
	Ast* res = Desugar_Ast::desugar_send(this);
	res ->desugar();
	return res;
}

///////////////////////////////////////////////////////////////

Ast* Assignment_Ast::desugar () {
	lhs = lhs ->desugar();
	rhs = rhs ->desugar();
	return this;
}

/////////////////////////////////////////////////////

Ast* Tying_Ast::desugar() {
	lhs = lhs ->desugar();
	rhs = rhs ->desugar();	
	return this;
}

///////////////////////////////////////////////////

Ast* Forward_Ast::desugar () {
	lhs = lhs ->desugar();
	rhs = rhs ->desugar();	
	return this;
}

///////////////////////////////////////////////////
Ast* In_Ast::desugar () {
	Ast* res = Desugar_Ast::desugar_in(this);
	res ->desugar();
	return res;
}

////////////////////////////////////////////////////////

Ast* With_Ast::desugar () {
	party = party ->desugar();

	list<pair<pair<Data_Type, string> *, Ast*> *>::iterator pit = s_list->begin();
	for(;pit!=s_list->end();pit++)
	{
		list<pair<pair<Data_Type, string> *, Ast*> *>::iterator pit_temp = pit;
		pit_temp++;
		pair<pair<Data_Type, string> *, Ast*> * decl = (*pit);
		decl->second = decl->second ->desugar();
	}
	return this;
}

////////////////////////////////////////////////////////

Ast* Seq_Ast::desugar () {
	cond = cond ->desugar();
	addendum = addendum ->desugar();
	return this;
}

////////////////////////////////////////////////////////

Ast* Connect_Ast::desugar () {
	from = from ->desugar();
	to = to ->desugar();
	return this;
}

///////////////////////////////////////////////////////////////////////

Ast* Port_Clause_Ast::desugar (){
	party = party ->desugar();
	if(is_clause_present){
		list<pair<pair<Data_Type, string> *, Ast*> *>::iterator pit = seq->begin();
		for(;pit!=seq->end();pit++)
		{
			list<pair<pair<Data_Type, string> *, Ast*> *>::iterator pit_temp = pit;
			pit_temp++;
			pair<pair<Data_Type, string> *, Ast*> * decl = (*pit);
			decl->second = decl->second ->desugar();
		}
	}
	return this;
}

///////////////////////////////////////////////////////////////////////

Ast* Decl_Ast::desugar () {
	list<Ast*>::iterator pit = name_list->begin();
	for(;pit!=name_list->end();pit++)
	{
		list<Ast*>::iterator pit_temp = pit;
		pit_temp++;
		(*pit) = (*pit) ->desugar();
	}
	return this;
}

/////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Ast* Number_Ast<DATA_TYPE>::desugar () {
	return this;
}

///////////////////////////////////////////////////////////////////////////////

Ast* Plus_Ast::desugar() {
	lhs = lhs ->desugar();
	rhs = rhs ->desugar();
	return this;
}

/////////////////////////////////////////////////////////////////

Ast* Minus_Ast::desugar () {
	lhs = lhs ->desugar();
	rhs = rhs ->desugar();	
	return this;
}

//////////////////////////////////////////////////////////////////

Ast* Mult_Ast::desugar () {
	lhs = lhs ->desugar();
	rhs = rhs ->desugar();
	return this;
}

////////////////////////////////////////////////////////////////////

Ast* Divide_Ast::desugar () {
	lhs = lhs ->desugar();
	rhs = rhs ->desugar();
	return this;
}

//////////////////////////////////////////////////////////////////////

Ast* Modulo_Ast::desugar () {
	lhs = lhs ->desugar();
	rhs = rhs ->desugar();
	return this;
}

//////////////////////////////////////////////////////////////////////

Ast* UMinus_Ast::desugar () {
	lhs = lhs ->desugar();	
	return this;
}

///////////////////////////////////////////////////////////////////////

Ast* Relational_Expr_Ast::desugar () {
	lhs_condition = lhs_condition ->desugar();
	rhs_condition = rhs_condition ->desugar();
	return this;
}

///////////////////////////////////////////////////////////////////////

Ast* Boolean_Expr_Ast::desugar () {
	lhs_op = lhs_op ->desugar();
	rhs_op = rhs_op ->desugar();
	return this;
}

///////////////////////////////////////////////////////////////////////

Ast* Port_Expr_Ast::desugar () {
	lhs = lhs ->desugar();
	rhs = rhs ->desugar();
	return this;
}

///////////////////////////////////////////////////////////////////////

Ast* Party_Expr_Ast::desugar () {
	lhs = lhs ->desugar();
	rhs = rhs ->desugar();
	return this;
}

///////////////////////////////////////////////////////////////////////

Ast* From_Expr_Ast::desugar () {
	lhs_op = lhs_op ->desugar();
	rhs_op = rhs_op ->desugar();
	return this;
}

///////////////////////////////////////////////////////////////////////

Ast* Expression_List_Ast::desugar () {
	list<Ast*>::iterator pit = exp_list->begin();
	for(;pit!=exp_list->end();pit++)
	{
		list<Ast*>::iterator pit_temp = pit;
		pit_temp++;
		(*pit) = (*pit) ->desugar();
	}
	return this;
}

///////////////////////////////////////////////////////////////////////

Ast* Session_call_Ast::desugar () {
	session_name = session_name ->desugar();
	list<Ast*>::iterator pit = arg_list->begin();
	for(;pit!=arg_list->end();pit++)
	{
		list<Ast*>::iterator pit_temp = pit;
		pit_temp++;
		(*pit) = (*pit) ->desugar();
	}
	seq = seq ->desugar();	
	return this;
}

///////////////////////////////////////////////////////////////////////

Ast* Set_Ast::desugar () {
	source = source ->desugar();
	list<Ast*>::iterator pit = arg_list->begin();
	for(;pit!=arg_list->end();pit++)
	{
		list<Ast*>::iterator pit_temp = pit;
		pit_temp++;
		(*pit) = (*pit) ->desugar();
	}
	dest = dest ->desugar();
	return this;
}

///////////////////////////////////////////////////////////////////////

Ast* Term_Ast::desugar () {
	if(child != NULL) 
		child = child ->desugar();
	if(dim_list!=NULL){
		list<Ast*>::iterator pit = dim_list->begin();
		for(;pit!=dim_list->end();pit++)
		{
			if((*pit)!=NULL)
				(*pit) = (*pit) ->desugar();
		}
	}
	return this;
}

Ast* Name_Ast::desugar () {
	return this;
}

///////////////////////////////////////////////////////////////////////////

Ast* Decl_Term_Ast::desugar () {
	term = term ->desugar();
	if(port_clause_exists)
	{
		list<pair<pair<Data_Type, string> *, Ast*> *>::iterator pit = port_clause->begin();
		for(;pit!=port_clause->end();pit++)
		{
			list<pair<pair<Data_Type, string> *, Ast*> *>::iterator pit_temp = pit;
			pit_temp++;
			pair<pair<Data_Type, string> *, Ast*> * decl = (*pit);
			decl->second = decl->second ->desugar();
		}
	}
	return this;
}

///////////////////////////////////////////////////////////////////////

Ast* Abort_Ast::desugar () {
	return this;
}

///////////////////////////////////////////////////////////////////////

Ast* Argument_Ast::desugar () {
	term = term ->desugar();
	return this;
}

///////////////////////////////////////////////////////////////////////


Ast* Prot_Arg_Ast::desugar () {
	prot_decl = prot_decl ->desugar();
	return this;
}

///////////////////////////////////////////////////////////////////////

Ast* Wrap_Ast::desugar () {
	stmt = stmt ->desugar();
	return this;
}

/////////////////////////////////////////////////////////////////////////

Ast* Start_Ast::desugar () {
	party_exp = party_exp ->desugar();
	return this;
}

/////////////////////////////////////////////////////////////////////////

Ast* Selection_Statement_Ast::desugar (){
	cond = cond ->desugar();
	then_part = then_part ->desugar();
	if(else_part!=NULL)
	{	
		else_part = else_part ->desugar();
	}
	return this;
}

/////////////////////////////////////////////////////////////////////////

Ast* Each_Statement_Ast::desugar () {
	if(variable!=NULL) {
		variable = variable ->desugar();
	}
	expression = expression ->desugar();
	if(cond!=NULL){
		cond = cond ->desugar();
	}
	return this;
}

/////////////////////////////////////////////////////////////////////////


Ast* Iteration_Statement_Ast::desugar () {
	Ast* res = Desugar_Ast::desugar_loops(this);
	res->desugar();
	return res;
}

Ast* Basic_Loop_Ast::desugar () {
	variable= variable ->desugar();
	limit = limit ->desugar();
	body = body ->desugar();
	return this;
}

Ast* Array_Limit_Ast::desugar () {
	return this;
}

Ast* Send_Assignment_Ast::desugar () {
	lhs = lhs ->desugar();
	rhs = rhs ->desugar();
	return this;
}

Ast* CheckPartyID_Ast::desugar (){
	list<Ast*>::iterator it1 = party_addr->begin();
	it1++;
	for (list<Ast*>::iterator it = party_addr->begin();it!=party_addr->end();it++,it1++){
		(*it) = (*it) ->desugar();
	}
	return this;
}

template class Number_Ast<bool>;
template class Number_Ast<int>;

int dummy_var_counter = 0;

string Desugar_Ast::get_new_iterator_name()
{
	return "__dummy_" + to_string(dummy_var_counter++);
	//TODO
	//return a new iterator name
	//ENSURE THAT ALL THESE VARIABLES ARE DECLARED AS INT SOMEWHERE IN THE PROGRAM AS THEY REFER TO LOOP INDICES
	//lookup in symbol table to get next new name ?	
}

void Desugar_Ast::find_hash_vars_and_build_assign_expr( Ast* root, std::vector<string>* var_list, std::vector<Ast*>* limits_list )
{
	//given a ast "root", find all #vars appropriately inside the dimentions of some var
	//insert the #vars into list "var_list" and its limits to the list "limits_list"
	if(root==NULL)
		return;
	else if(dynamic_cast<Term_Ast*>(root))
	{
		if(((Term_Ast*)root)->t == variable && ((Term_Ast*)root)->dim_list !=NULL)
		{
			int counter = 0;
			for(auto i= ((Term_Ast*)root)->dim_list->begin(); i!= ((Term_Ast*)root)->dim_list->end(); i++, counter++)
			{
				if((*i) == NULL)
				{
					//variable is not specified
					//generate new iterator name and insert into var_list
					string name = get_new_iterator_name();
					(*var_list).push_back(name);
					//insert into limits_list
					(*limits_list).push_back(new Array_Limit_Ast( ((Name_Ast*)(((Term_Ast*)root)->child))->name, counter));
					//construct ast for the new iterator variable and assign appropriately
					// ((Term_Ast*)root)->dim_list->insert(i,new Term_Ast(new Name_Ast(name,root->lineno), NULL, variable, root->lineno));
					(*i) = new Term_Ast(new Name_Ast(name,root->lineno), NULL, variable, root->lineno);
				}
				else if(dynamic_cast<Term_Ast*>((*i)))
				{
					if(((Term_Ast*)(*i))->t == iterator_variable)
					{
						//insert into var_list
						(*var_list).push_back(((Name_Ast*)(((Term_Ast*)(*i))->child))->name);
						//insert into limits_list
						(*limits_list).push_back(new Array_Limit_Ast( ((Name_Ast*)(((Term_Ast*)root)->child))->name, counter));
						//convert to variable type
						((Term_Ast*)(*i))->t = variable;
					}
					else if(((Term_Ast*)(*i))->t == variable)
					{
						find_hash_vars_and_build_assign_expr( (*i), var_list, limits_list);
					}
				}
				else
					find_hash_vars_and_build_assign_expr( (*i), var_list, limits_list);
			}
		}
		else if(((Term_Ast*)root)->t == iterator_variable)
		{
			//throw error? #t not in dimlist of some variable
		}
	}
	else if(dynamic_cast<Boolean_Expr_Ast*>(root))
	{
		//recurse
		find_hash_vars_and_build_assign_expr( ((Boolean_Expr_Ast*)root)->lhs_op, var_list, limits_list);
		if(((Boolean_Expr_Ast*)root)->bool_op != boolean_not)
			find_hash_vars_and_build_assign_expr( ((Boolean_Expr_Ast*)root)->rhs_op, var_list, limits_list);
	}	
	else if(dynamic_cast<Arithmetic_Expr_Ast*>(root))
	{
		//recurse
		find_hash_vars_and_build_assign_expr( ((Arithmetic_Expr_Ast*)root)->lhs, var_list, limits_list);
		find_hash_vars_and_build_assign_expr( ((Arithmetic_Expr_Ast*)root)->rhs, var_list, limits_list);
	}
	else if(dynamic_cast<Relational_Expr_Ast*>(root))
	{
		//recurse
		find_hash_vars_and_build_assign_expr( ((Relational_Expr_Ast*)root)->lhs_condition, var_list, limits_list);
		find_hash_vars_and_build_assign_expr( ((Relational_Expr_Ast*)root)->rhs_condition, var_list, limits_list);
	}
	else if(dynamic_cast<Port_Expr_Ast*>(root))
	{
		//recurse
		find_hash_vars_and_build_assign_expr( ((Port_Expr_Ast*)root)->lhs, var_list, limits_list);
		find_hash_vars_and_build_assign_expr( ((Port_Expr_Ast*)root)->rhs, var_list, limits_list);
	}
	else if(dynamic_cast<Party_Expr_Ast*>(root))
	{
		//recurse
		find_hash_vars_and_build_assign_expr( ((Party_Expr_Ast*)root)->lhs, var_list, limits_list);
		find_hash_vars_and_build_assign_expr( ((Party_Expr_Ast*)root)->rhs, var_list, limits_list);
	}
	else if(dynamic_cast<From_Expr_Ast*>(root))
	{
		//recurse
		find_hash_vars_and_build_assign_expr( ((From_Expr_Ast*)root)->lhs_op, var_list, limits_list);
		find_hash_vars_and_build_assign_expr( ((From_Expr_Ast*)root)->rhs_op, var_list, limits_list);
	}
	else if(dynamic_cast<Expression_List_Ast*>(root))
	{
		//recurse
		//careful with list iterator conversion to list object
		for(auto i= ((Expression_List_Ast*)root)->exp_list->begin(); i!= ((Expression_List_Ast*)root)->exp_list->end(); i++)
			find_hash_vars_and_build_assign_expr( (*i), var_list, limits_list);
	}
	else
	{
		//throw error?
	}
}

Ast* Desugar_Ast::desugar_loops( Ast* root )
{	
	if(root==NULL)
		return NULL;
	else if( typeid(*root) != typeid(Iteration_Statement_Ast))
		return NULL;

	// search for all "# variable" terms in root->each_block->expression and put in an array
	// convert all "#variable" terms to "variable" terms  in root
	std::vector<string> var_list;
	std::vector<Ast*> limits_list;
	find_hash_vars_and_build_assign_expr( ((Each_Statement_Ast*)((Iteration_Statement_Ast*)root)->each_block)->expression, &var_list, &limits_list);
	// construct the basic for loops from the lists
	// TODO, appropriate line numbering...for now 0
	// Innermost loop:
	
	std::vector<string>::reverse_iterator iter_names = var_list.rbegin();
	std::vector<Ast*>::reverse_iterator iter_limits = limits_list.rbegin();
	
	//construct the innermost loop body referenced as iter_body
	Each_Statement_Ast* each_blk_stmt = ((Each_Statement_Ast*)((Iteration_Statement_Ast*)root)->each_block);
	Ast* iter_body = new Sequence_Ast(((Iteration_Statement_Ast*)root)->body->lineno);
	
	if(each_blk_stmt->variable!=NULL)
	{
		//the each block has a variable defined
		//first statement of innermost loop will be to assign value to this variable
		Ast* ass_ast = new Assignment_Ast(each_blk_stmt->variable,each_blk_stmt->expression,iter_body->lineno);
		((Sequence_Ast*)iter_body)->ast_push_back(ass_ast);
	}

	if(each_blk_stmt->cond_type==where)
	{
		//each block has a condition defined, condition type being where
		//construct a new selection statement ast whose then branch is body, else branch is enpty
		Ast* cond_ast = new Selection_Statement_Ast(each_blk_stmt->cond,((Iteration_Statement_Ast*)root)->body,new Sequence_Ast(each_blk_stmt->lineno),iter_body->lineno);
		((Sequence_Ast*)iter_body)->ast_push_back(cond_ast);
	}
	else if(each_blk_stmt->cond_type==except)
	{
		//each block has a condition defined, condition type being except
		//construct a new selection statement ast whose then branch is empty, else branch is body
		Ast* cond_ast = new Selection_Statement_Ast(each_blk_stmt->cond,new Sequence_Ast(each_blk_stmt->lineno),((Iteration_Statement_Ast*)root)->body,iter_body->lineno);
		((Sequence_Ast*)iter_body)->ast_push_back(cond_ast);
	}
	else
	{
		((Sequence_Ast*)iter_body)->ast_push_back(((Iteration_Statement_Ast*)root)->body);
	}

	//construct the innermost loop body
	Ast* d_root;
	if(var_list.empty())
	{
		return iter_body;
	}
	else
	{
		d_root = new Basic_Loop_Ast(new Name_Ast((*iter_names), each_blk_stmt->lineno), *iter_limits, iter_body);
	}
	
	Ast* tmp;
	iter_names++;
	iter_limits++;

	// Build each outer loop whose body is the previous inner loop
	while( iter_names != var_list.rend() && iter_limits != limits_list.rend() )
	{
		tmp = new Basic_Loop_Ast(new Name_Ast((*iter_names), 0), *iter_limits, d_root);
		d_root = tmp;
		iter_names++;
		iter_limits++;
	}

	return d_root;
}

Ast* Desugar_Ast::get_party_port(Ast* root)
{
	if(root==NULL)
		return NULL;

	if(typeid(*root)==typeid(Term_Ast))
	{
		//construct new term_ast and return
		//using remove_hashes to clone
		//TODO create clone_ast and use instead
		Ast *res;		
		res = remove_hashes(root);
		return res;		
	}
	else if(typeid(*root)==typeid(Port_Expr_Ast))
	{
		//recurse 
		return get_party_port(((Port_Expr_Ast*)root)->lhs);
	}
	else if(typeid(*root)==typeid(Party_Expr_Ast))
	{
		//recurse 
		return get_party_port(((Party_Expr_Ast*)root)->lhs);
	}
	else 
		//could be expr...check further or ?
		//TODO clone root
		return remove_hashes(root);
}

Ast* Desugar_Ast::desugar_send_lhs(Ast* root, Ast* rhs_port)
{

	if(typeid(*root)==typeid(Term_Ast))
		//cannot send as not a party
		return NULL;
	else if(typeid(*root)==typeid(Port_Expr_Ast))
	{
		//construct sequence of read and write
		Ast* tmp_seq = new Sequence_Ast(((Party_Expr_Ast*)root)->lineno);
		//read from child and write to rhs_port (will be env if inside another party) 
		//child is a variable, will assign variable to rhs_port 
		Ast* tmp2 = new Send_Assignment_Ast(get_party_port(((Port_Expr_Ast*)root)->rhs), rhs_port, ((Port_Expr_Ast*)root)->lineno);
		//add to sequence
		((Sequence_Ast*)tmp_seq)->ast_push_back(tmp2);
		//put inside in block of party lhs
		Ast* tmp3 = new In_Ast(tmp_seq, ((Port_Expr_Ast*)root)->lhs, false, ((Port_Expr_Ast*)root)->lineno);
		return tmp3;
	}
	else if(typeid(*root)==typeid(Party_Expr_Ast))
	{				
		//construct sequence of recursion, read and write
		Ast* tmp_seq = new Sequence_Ast(((Party_Expr_Ast*)root)->lineno);
		if(typeid(*(((Party_Expr_Ast*)root)->rhs))==(typeid(Party_Expr_Ast)) || typeid(*(((Party_Expr_Ast*)root)->rhs))==(typeid(Port_Expr_Ast))) 
		{
			//recurse on rhs if child(rhs) is a party (not a variable)
			//rhs_port for child will be env
			Ast* term_ast = new Term_Ast(NULL, NULL, env_port, ((Party_Expr_Ast*)root)->lineno);
			Ast* tmp = desugar_send_lhs(((Party_Expr_Ast*)root)->rhs, term_ast);
			//add to sequence
			((Sequence_Ast*)tmp_seq)->ast_push_back(tmp);
		}
		//read from child and write to rhs_port (will be env if inside another party) 
		//if child is a variable, will assign variable to rhs_port 
		Ast* tmp2 = new Send_Assignment_Ast(get_party_port(((Party_Expr_Ast*)root)->rhs), rhs_port, ((Party_Expr_Ast*)root)->lineno);
		//add to sequence
		((Sequence_Ast*)tmp_seq)->ast_push_back(tmp2);
		//put inside in block of party lhs
		Ast* tmp3 = new In_Ast(tmp_seq, ((Party_Expr_Ast*)root)->lhs, false, ((Party_Expr_Ast*)root)->lineno);
		return tmp3;
	}
	else
	{
		//throw error ?
	}
}

Ast* Desugar_Ast::desugar_send_rhs(Ast* root, Ast* lhs_port)
{
	if(typeid(*root)==typeid(Term_Ast))
		//cannot receive as not a party
		return NULL;
	else if(typeid(*root)==typeid(Port_Expr_Ast))
	{
		//construct sequence of read and write
		Ast* tmp_seq = new Sequence_Ast(((Party_Expr_Ast*)root)->lineno);
		//read from lhs_port  and write to child (lhs_port will be env if inside another party) 
		//child is a variable, will assign lhs_port to variable 
		Ast* tmp2 = new Send_Assignment_Ast(lhs_port, get_party_port(((Port_Expr_Ast*)root)->rhs), ((Port_Expr_Ast*)root)->lineno);
		//add to sequence
		((Sequence_Ast*)tmp_seq)->ast_push_back(tmp2);
		//put inside in block of party lhs
		Ast* tmp3 = new In_Ast(tmp_seq, ((Party_Expr_Ast*)root)->lhs, false, ((Party_Expr_Ast*)root)->lineno);
		return tmp3;
	}
	else if(typeid(*root)==typeid(Party_Expr_Ast))
	{	
		//construct sequence of recursion, read and write
		Ast* tmp_seq = new Sequence_Ast(((Party_Expr_Ast*)root)->lineno);
		//read from lhs_port  and write to child (lhs_port will be env if inside another party) 
		//if child is a variable, will assign lhs_port to variable 
		Ast* tmp2 = new Send_Assignment_Ast(lhs_port, get_party_port(((Party_Expr_Ast*)root)->rhs), ((Party_Expr_Ast*)root)->lineno);
		//add to sequence
		((Sequence_Ast*)tmp_seq)->ast_push_back(tmp2);
		if(typeid(*(((Party_Expr_Ast*)root)->rhs))==(typeid(Party_Expr_Ast)) || typeid(*(((Party_Expr_Ast*)root)->rhs))==(typeid(Port_Expr_Ast))) 
		{
			//recurse on rhs if child(rhs) is a party (not a variable)
			//lhs_port for child will be env
			Ast* term_ast = new Term_Ast(NULL, NULL, env_port, ((Party_Expr_Ast*)root)->lineno);
			Ast* tmp = desugar_send_lhs(term_ast, ((Party_Expr_Ast*)root)->rhs);
			//add to sequence
			((Sequence_Ast*)tmp_seq)->ast_push_back(tmp);
		}
		//put inside in block of party lhs
		Ast* tmp3 = new In_Ast(tmp_seq, ((Party_Expr_Ast*)root)->lhs, false, ((Party_Expr_Ast*)root)->lineno);
		return tmp3;
	}
}

Ast* Desugar_Ast::desugar_send( Ast* root )
{
	if(root==NULL)
		return NULL;
	else if(typeid(*root) != typeid(Send_Ast))
		return NULL;
	Ast *lhs_port, *rhs_port;
	Ast *new_lhs, *new_rhs;
	if(typeid(*(((Send_Ast*)root)->lhs))==typeid(Term_Ast) && typeid(*(((Send_Ast*)root)->rhs))==typeid(Term_Ast))
	{
		//port/var to port/var inside current party only
		//port/var remain as it is
		lhs_port = ((Send_Ast*)root)->lhs;
		rhs_port = ((Send_Ast*)root)->rhs;
		//only need to construct one send assignment ast
		return new Send_Assignment_Ast(lhs_port, rhs_port, ((Term_Ast*)root)->lineno);

	}		
	else
	{
		//one of lhs and rhs has sub-parties
		lhs_port = get_party_port(((Send_Ast*)root)->lhs);
		rhs_port = get_party_port(((Send_Ast*)root)->rhs);

		new_lhs = desugar_send_lhs(((Send_Ast*)root)->lhs, rhs_port);
		new_rhs = desugar_send_rhs(((Send_Ast*)root)->rhs, lhs_port);
		//return sequence of above
		Ast* tmp_seq = new Sequence_Ast(root->lineno);
		if(new_lhs!=NULL)
			//null if lhs is a port/var, no need to construct
			((Sequence_Ast*)tmp_seq)->ast_push_back(new_lhs);
		if(new_rhs!=NULL)
			//null if rhs is a port/var, no need to construct
			((Sequence_Ast*)tmp_seq)->ast_push_back(new_rhs);
		return tmp_seq;
	}
}

/*	For Desugaring In Statements */
void Desugar_Ast::add_hashes(Ast* root)
{
	if(dynamic_cast<Term_Ast*>(root))
	{
		if(((Term_Ast*)root)->t == variable && ((Term_Ast*)root)->dim_list !=NULL)
		{
			int counter = 0;
			for(auto i= ((Term_Ast*)root)->dim_list->begin(); i!= ((Term_Ast*)root)->dim_list->end(); i++, counter++)
			{
				if((*i) == NULL)
				{
					string name = get_new_iterator_name();
					// ((Term_Ast*)root)->dim_list->insert(i,new Term_Ast(new Name_Ast(name,root->lineno), NULL, iterator_variable, root->lineno));
					(*i) = new Term_Ast(new Name_Ast(name,root->lineno), NULL, variable, root->lineno);
				}
				else
					add_hashes(*i);
			}
		}
	}
	else if(dynamic_cast<Boolean_Expr_Ast*>(root))
	{
		//recurse
		add_hashes( ((Boolean_Expr_Ast*)root)->lhs_op);
		if(((Boolean_Expr_Ast*)root)->bool_op != boolean_not)
			add_hashes( ((Boolean_Expr_Ast*)root)->rhs_op);
	}	
	else if(dynamic_cast<Arithmetic_Expr_Ast*>(root))
	{
		//recurse
		add_hashes( ((Arithmetic_Expr_Ast*)root)->lhs);
		add_hashes( ((Arithmetic_Expr_Ast*)root)->rhs);
	}
	else if(dynamic_cast<Relational_Expr_Ast*>(root))
	{
		//recurse
		add_hashes( ((Relational_Expr_Ast*)root)->lhs_condition);
		add_hashes( ((Relational_Expr_Ast*)root)->rhs_condition);
	}
	else if(dynamic_cast<Port_Expr_Ast*>(root))
	{
		//recurse
		add_hashes( ((Port_Expr_Ast*)root)->lhs);
		add_hashes( ((Port_Expr_Ast*)root)->rhs);
	}
	else if(dynamic_cast<Party_Expr_Ast*>(root))
	{
		//recurse
		add_hashes( ((Party_Expr_Ast*)root)->lhs);
		add_hashes( ((Party_Expr_Ast*)root)->rhs);
	}
	else if(dynamic_cast<From_Expr_Ast*>(root))
	{
		//recurse
		add_hashes( ((From_Expr_Ast*)root)->lhs_op);
		add_hashes( ((From_Expr_Ast*)root)->rhs_op);
	}
	else if(dynamic_cast<Expression_List_Ast*>(root))
	{
		//recurse
		//careful with list iterator conversion to list object
		for(auto i= ((Expression_List_Ast*)root)->exp_list->begin(); i!= ((Expression_List_Ast*)root)->exp_list->end(); i++)
			add_hashes(*i);
	}
	else
	{
		//throw error?
	}
}

Ast* Desugar_Ast::remove_hashes(Ast* root)
{
	if(dynamic_cast<Term_Ast*>(root))
	{
		Term_Type temp_type = ((Term_Ast*)root)->t;
		list<Ast*> *temp_dim_list = NULL;
		Ast* temp_child;

		if(dynamic_cast<Name_Ast*>((((Term_Ast*)root)->child)))
			temp_child = new Name_Ast(((Name_Ast*)(((Term_Ast*)root)->child))->name, ((Name_Ast*)(((Term_Ast*)root)->child))->lineno);
		else if(dynamic_cast<Number_Ast<int>*>((((Term_Ast*)root)->child)))
			temp_child = new Number_Ast<int>(((Number_Ast<int>*)(((Term_Ast*)root)->child))->constant, (((Term_Ast*)root)->child)->lineno);
		else if(dynamic_cast<Number_Ast<bool>*>((((Term_Ast*)root)->child)))
			temp_child = new Number_Ast<bool>(((Number_Ast<bool>*)(((Term_Ast*)root)->child))->constant, (((Term_Ast*)root)->child)->lineno);
		else if((((Term_Ast*)root)->child) == NULL) 
			temp_child = NULL;
		else
		{
			//error ?
		}
		

		if(((Term_Ast*)root)->t == variable && ((Term_Ast*)root)->dim_list !=NULL)
		{			
			// Root is an array
			
			temp_dim_list = new list<Ast*>();
			Ast* temp_elem;
			int counter = 0;
			

			for(auto i= ((Term_Ast*)root)->dim_list->begin(); i!= ((Term_Ast*)root)->dim_list->end(); i++, counter++)
			{
				
				if(dynamic_cast<Term_Ast*>((*i)))
				{

					//Array index
					list<Ast*> *elem_dim_list = NULL;
					Ast* elem_child;
					if(dynamic_cast<Name_Ast*>((((Term_Ast*)(*i))->child)))
					{
						elem_child = new Name_Ast(((Name_Ast*)(((Term_Ast*)(*i))->child))->name, (((Term_Ast*)(*i))->child)->lineno);
					}
					else if(dynamic_cast<Number_Ast<int>*>((((Term_Ast*)(*i))->child)))
					{
						elem_child = new Number_Ast<int>(((Number_Ast<int>*)(((Term_Ast*)(*i))->child))->constant, (((Term_Ast*)(*i))->child)->lineno);
					}
					else if(dynamic_cast<Number_Ast<bool>*>((((Term_Ast*)(*i))->child)))
					{
						elem_child = new Number_Ast<bool>(((Number_Ast<bool>*)(((Term_Ast*)(*i))->child))->constant, (((Term_Ast*)(*i))->child)->lineno);
					}
					

					Term_Type elem_type = ((Term_Ast*)(*i))->t;
					if(((Term_Ast*)(*i))->t == iterator_variable){
						elem_type = variable;
						temp_elem = new Term_Ast(elem_child,elem_dim_list,elem_type,(*i)->lineno);
					}
					else{
						temp_elem = remove_hashes(*i);
					}

				}
				else
					temp_elem = remove_hashes(*i);

				temp_dim_list->push_back(temp_elem);
			}

		}
		return new Term_Ast(temp_child,temp_dim_list,temp_type,root->lineno);
	}
	else if(dynamic_cast<Boolean_Expr_Ast*>(root))
	{
		//recurse
		Ast* lhs_ast = remove_hashes( ((Boolean_Expr_Ast*)root)->lhs_op);
		Ast* rhs_ast = NULL;
		if(((Boolean_Expr_Ast*)root)->bool_op != boolean_not)
			rhs_ast = remove_hashes( ((Boolean_Expr_Ast*)root)->rhs_op);
		return new Boolean_Expr_Ast(lhs_ast, ((Boolean_Expr_Ast*)root)->bool_op, rhs_ast, root->lineno);
	}
	else if(dynamic_cast<Arithmetic_Expr_Ast*>(root))
	{
		//recurse
		Ast* lhs_ast = remove_hashes( ((Arithmetic_Expr_Ast*)root)->lhs);
		Ast* rhs_ast = remove_hashes( ((Arithmetic_Expr_Ast*)root)->rhs);
		if(dynamic_cast<Plus_Ast*>(root))
			return new Plus_Ast(lhs_ast, rhs_ast, root->lineno);
		else if(dynamic_cast<Minus_Ast*>(root))
			return new Minus_Ast(lhs_ast, rhs_ast, root->lineno);
		else if(dynamic_cast<Mult_Ast*>(root))
			return new Mult_Ast(lhs_ast, rhs_ast, root->lineno);
		else if(dynamic_cast<Divide_Ast*>(root))
			return new Divide_Ast(lhs_ast, rhs_ast, root->lineno);
		else if(dynamic_cast<Modulo_Ast*>(root))
			return new Modulo_Ast(lhs_ast, rhs_ast, root->lineno);
		else if(dynamic_cast<UMinus_Ast*>(root))
			return new UMinus_Ast(lhs_ast, rhs_ast, root->lineno);
		else{
			//error
			return NULL;
		}
	}
	else if(dynamic_cast<Relational_Expr_Ast*>(root))
	{
		//recurse
		Ast* lhs_ast = remove_hashes( ((Relational_Expr_Ast*)root)->lhs_condition);
		Ast* rhs_ast = remove_hashes( ((Relational_Expr_Ast*)root)->rhs_condition);
		return new Relational_Expr_Ast(lhs_ast, ((Relational_Expr_Ast*)root)->rel_op, rhs_ast, root->lineno);
	}
	else if(dynamic_cast<Port_Expr_Ast*>(root))
	{
		//recurse
		Ast* lhs_ast = remove_hashes( ((Port_Expr_Ast*)root)->lhs);
		Ast* rhs_ast = remove_hashes( ((Port_Expr_Ast*)root)->rhs);
		return new Port_Expr_Ast(lhs_ast, rhs_ast, root->lineno);
	}
	else if(dynamic_cast<Party_Expr_Ast*>(root))
	{
		//recurse
		Ast* lhs_ast = remove_hashes( ((Party_Expr_Ast*)root)->lhs);
		Ast* rhs_ast = remove_hashes( ((Party_Expr_Ast*)root)->rhs);
		return new Party_Expr_Ast(lhs_ast, rhs_ast, root->lineno);
	}
	else if(dynamic_cast<From_Expr_Ast*>(root))
	{
		//recurse
		Ast* lhs_ast = remove_hashes( ((From_Expr_Ast*)root)->lhs_op);
		Ast* rhs_ast = remove_hashes( ((From_Expr_Ast*)root)->rhs_op);
		return new From_Expr_Ast(lhs_ast, rhs_ast, root->lineno);
	}
	else if(dynamic_cast<Expression_List_Ast*>(root))
	{
		//recurse
		//careful with list iterator conversion to list object
		list<Ast*> *temp_list = new list<Ast*>();
		for(auto i= ((Expression_List_Ast*)root)->exp_list->begin(); i!= ((Expression_List_Ast*)root)->exp_list->end(); i++)
			temp_list->push_back(remove_hashes(*i));
		return new Expression_List_Ast(temp_list, root->lineno);
	}
	else
	{
		//throw error?
	}
}

void Desugar_Ast::resolve_addr(Ast* party, list<Ast*>* res)
{
	if(dynamic_cast<Party_Expr_Ast*>(party))
	{
		resolve_addr(((Party_Expr_Ast*)party)->lhs,res);
		resolve_addr(((Party_Expr_Ast*)party)->rhs,res);
	}
	else if(dynamic_cast<Term_Ast*>(party))
	{
		res->push_back(party);
	}
	else
	{
		//Syntax error: Report it - Not a valid party
		return;
	}
}

Ast* Desugar_Ast::desugar_in(Ast* root)
{
	if(root==NULL)
		return NULL;
	else if(typeid(*root) != typeid(In_Ast))
		return NULL;

	Ast* body = ((In_Ast*)root)->s_list;
	Ast* party = ((In_Ast*)root)->party;

	if(!(((In_Ast*)root)->each))
	{
		if(!(dynamic_cast<Party_Expr_Ast*>(party) || dynamic_cast<Term_Ast*>(party) )|| !dynamic_cast<Sequence_Ast*>(body))
		{
			//error
			return NULL;
		}

		list<Ast*>* res = new list<Ast*>();
		resolve_addr(party,res);
		Ast * cond = new CheckPartyID_Ast(res);
		Ast * if_stmt = new Selection_Statement_Ast(cond,body,NULL,root->lineno);
		return if_stmt;
	}
	else
	{
		if(!dynamic_cast<Each_Statement_Ast*>(party) || !dynamic_cast<Sequence_Ast*>(body))
		{
			//error
			return NULL;
		}

		Ast * new_body = new Sequence_Ast(body->lineno);
		add_hashes(((Each_Statement_Ast*)party)->expression);
		Ast* party_in_body = remove_hashes(((Each_Statement_Ast*)party)->expression);
		
		list<Ast*>* res = new list<Ast*>();
		resolve_addr(party_in_body,res);
		Ast * cond = new CheckPartyID_Ast(res);
		Ast * if_stmt = new Selection_Statement_Ast(cond,body,NULL,root->lineno);
		((Sequence_Ast*)new_body)->ast_push_back(if_stmt);
		Ast * iter_stmt = new Iteration_Statement_Ast(party, new_body, root->lineno);
		return iter_stmt;
	}
}
