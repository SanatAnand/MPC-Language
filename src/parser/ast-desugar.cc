#include <iostream>
#include <fstream>
#include <typeinfo>
#include <vector>

using namespace std;

#include "ast.hh"

int Desugar_Ast::get_limits( Ast* root, int index )
{
	//given a variable ast "root" and a index "index", return the limit of the dimention "index" of array "root"
	if(dynamic_cast<Term_Ast*>(root))
	{
		if(((Term_Ast*)root)->t == variable && ((Term_Ast*)root)->dim_list !=NULL)
		{
			//TODO
			//get the limits of dim "index" from symbol table?
			return -1;
		}
		else
		{
			//throw error?
		}
	}
	else
	{
		//throw error?
	}
}

string Desugar_Ast::get_new_iterator_name()
{
	return "sanat";
	//TODO
	//return a new iterator name
	//lookup in symbol table to get next new name ?
}

void Desugar_Ast::find_hash_vars_and_build_assign_expr( Ast* root, std::vector<string>* var_list, std::vector<int>* limits_list )
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
						(*limits_list).push_back(get_limits(root, counter));
						//construct ast for the new iterator variable and assign appropriately
						((Term_Ast*)root)->dim_list->insert(i,new Term_Ast(new Name_Ast(name,root->lineno), NULL, variable, root->lineno));
					}
					else if(dynamic_cast<Term_Ast*>((*i)))
					{
						if(((Term_Ast*)(*i))->t == iterator_variable)
						{
							//insert into var_list
							(*var_list).push_back(((Name_Ast*)(((Term_Ast*)(*i))->child))->name);
							//insert into limits_list
							(*limits_list).push_back(get_limits(root, counter));
							//convert to variable type
							((Term_Ast*)(*i))->t = variable;
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
	std::vector<int> limits_list;
	find_hash_vars_and_build_assign_expr( ((Each_Statement_Ast*)((Iteration_Statement_Ast*)root)->each_block)->expression, &var_list, &limits_list);
	// construct the basic for loops from the lists
	// TODO, appropriate line numbering...for now 0
	// Innermost loop:
	std::vector<string>::reverse_iterator iter_names = var_list.rbegin();
	std::vector<int>::reverse_iterator iter_limits = limits_list.rbegin();
	
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

	//construct the innermost loop body
	Ast* d_root = new Basic_Loop_Ast(new Name_Ast((*iter_names), each_blk_stmt->lineno), *iter_limits, iter_body);
	Ast* tmp;
	iter_names++;
	iter_limits++;

	// Build each outer loop whose body is the previous inner loop
	while( iter_names != var_list.rend() && iter_limits != limits_list.rend() )
	{
		tmp = new Basic_Loop_Ast(new Name_Ast((*iter_names), 0), *iter_limits, d_root);
		d_root = tmp;
	}

	//Temporary
	root = d_root;

	return d_root;
}
