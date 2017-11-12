#include <iostream>
#include <fstream>
#include <typeinfo>
#include <vector>

using namespace std;

#include "ast.hh"

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
					((Term_Ast*)root)->dim_list->insert(i,new Term_Ast(new Name_Ast(name,root->lineno), NULL, variable, root->lineno));
				}
				else if(dynamic_cast<Term_Ast*>((*i)))
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
			Ast* name_ast = new Name_Ast(((Name_Ast*)(((Term_Ast*)root)->child))->name, ((Term_Ast*)root)->lineno);
			return new Term_Ast(name_ast, ((Term_Ast*)root)->dim_list, ((Term_Ast*)root)->t, ((Term_Ast*)root)->lineno);
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
}

Ast* Desugar_Ast::desugar_send_lhs(Ast* root, Ast* rhs_port)
{

	if(typeid(*root)==typeid(Term_Ast))
		//cannot send as not a party
			return NULL;
	else if(typeid(*root)==typeid(Port_Expr_Ast))
	{
		//read from child and write to rhs_port (will be env if inside another party) 
		//child is a variable, will assign variable to rhs_port 
		Ast* tmp2 = new Send_Assignment_Ast(get_party_port(((Party_Expr_Ast*)root)->rhs), rhs_port, ((Party_Expr_Ast*)root)->lineno);
		//put inside in block of party lhs
		Ast* tmp3 = new In_Ast(tmp2, ((Party_Expr_Ast*)root)->lhs, false, ((Party_Expr_Ast*)root)->lineno);
		return tmp3;
	}
	else if(typeid(*root)==typeid(Party_Expr_Ast))
	{				
		//construct sequence of recursion, read and write
		Ast* tmp_seq = new Sequence_Ast(((Party_Expr_Ast*)root)->lineno);
		if(typeid(*(((Party_Expr_Ast*)root)->rhs))!=(typeid(Term_Ast)))
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
		//read from lhs_port  and write to child (lhs_port will be env if inside another party) 
		//child is a variable, will assign lhs_port to variable 
		Ast* tmp2 = new Send_Assignment_Ast(lhs_port, get_party_port(((Party_Expr_Ast*)root)->rhs), ((Party_Expr_Ast*)root)->lineno);
		//put inside in block of party lhs
		Ast* tmp3 = new In_Ast(tmp2, ((Party_Expr_Ast*)root)->lhs, false, ((Party_Expr_Ast*)root)->lineno);
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
		if(typeid(*(((Party_Expr_Ast*)root)->rhs))!=(typeid(Term_Ast)))
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
		Ast* temp_child = new Name_Ast(((Name_Ast*)(((Term_Ast*)root)->child))->name, ((Name_Ast*)(((Term_Ast*)root)->child))->lineno);
		
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
					Ast* elem_child = new Name_Ast(((Name_Ast*)(((Term_Ast*)(*i))->child))->name, ((Name_Ast*)(((Term_Ast*)(*i))->child))->lineno);
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
