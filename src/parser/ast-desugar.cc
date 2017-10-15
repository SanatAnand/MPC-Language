#include <iostream>
#include <fstream>
#include <typeinfo>
#include <vector>

using namespace std;

#include "ast.hh"

class Desugar_Ast
{
		static void find_hash_vars_and_build_assign_expr( Ast* root, std::vector<Ast*>* var_list )
		{
			if(root==NULL || var_list==NULL)
				return;
			else if(dynamic_cast<Term_Ast*>(root))
			{
					if(((Term_Ast*)root)->t == iterator_variable)
					{
						//insert into array of var_list
						(*var_list).insert((*var_list).end(), root);
						//convert to variable type
						((Term_Ast*)root)->t = variable;
					}
			}
			else if(dynamic_cast<Boolean_Expr_Ast*>(root))
			{
				//recurse
				find_hash_vars_and_build_assign_expr( ((Boolean_Expr_Ast*)root)->lhs_op, var_list);
				if(((Boolean_Expr_Ast*)root)->bool_op != boolean_not)
					find_hash_vars_and_build_assign_expr( ((Boolean_Expr_Ast*)root)->rhs_op, var_list);
			}	
			else if(dynamic_cast<Arithmetic_Expr_Ast*>(root))
			{
				//recurse
				find_hash_vars_and_build_assign_expr( ((Arithmetic_Expr_Ast*)root)->lhs, var_list);
				find_hash_vars_and_build_assign_expr( ((Arithmetic_Expr_Ast*)root)->rhs, var_list);
			}
			else if(dynamic_cast<Relational_Expr_Ast*>(root))
			{
				//recurse
				find_hash_vars_and_build_assign_expr( ((Relational_Expr_Ast*)root)->lhs_condition, var_list);
				find_hash_vars_and_build_assign_expr( ((Relational_Expr_Ast*)root)->rhs_condition, var_list);
			}
			else if(dynamic_cast<Port_Expr_Ast*>(root))
			{
				//recurse
				find_hash_vars_and_build_assign_expr( ((Port_Expr_Ast*)root)->lhs, var_list);
				find_hash_vars_and_build_assign_expr( ((Port_Expr_Ast*)root)->rhs, var_list);
			}
			else if(dynamic_cast<Party_Expr_Ast*>(root))
			{
				//recurse
				find_hash_vars_and_build_assign_expr( ((Party_Expr_Ast*)root)->lhs, var_list);
				find_hash_vars_and_build_assign_expr( ((Party_Expr_Ast*)root)->rhs, var_list);
			}
			else if(dynamic_cast<From_Expr_Ast*>(root))
			{
				//recurse
				find_hash_vars_and_build_assign_expr( ((From_Expr_Ast*)root)->lhs_op, var_list);
				find_hash_vars_and_build_assign_expr( ((From_Expr_Ast*)root)->rhs_op, var_list);
			}
			else if(dynamic_cast<Expression_List_Ast*>(root))
			{
				//recurse
				//careful with list iterator conversion to list object
				for(auto i= ((Expression_List_Ast*)root)->exp_list->begin(); i!= ((Expression_List_Ast*)root)->exp_list->end(); i++)
					find_hash_vars_and_build_assign_expr( (*i), var_list);
			}
			else
			{
				//throw error?
			}
		}

	public:
		static Ast* desugar_loops( Ast* root )
		{	
			if(root==NULL)
				return NULL;
			else if( typeid(*root) != typeid(Iteration_Statement_Ast))
				return NULL;
	
			//search for all "# variable" terms in root->each_block->expression and put in an array
			//convert all "#variable" terms to "variable" terms  in root
			std::vector<Ast*> var_list;
			var_list.begin();
			find_hash_vars_and_build_assign_expr( ((Each_Statement_Ast*)((Iteration_Statement_Ast*)root)->each_block)->expression, &var_list);

		}

};
