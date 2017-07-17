#include "common-classes.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "procedure.hh"


// compile
void Procedure::compile(){
	// cout<<"into compile of "<<get_proc_name()<<endl;
	sequence_ast->compile();
	//cout<<"step 1"<<endl;
	// cout<<return_ast<<endl;
	if(return_ast)
	{
		return_ast->compile();
	}
	
	// cout<<"out of compile of "<<get_proc_name()<<endl;

}