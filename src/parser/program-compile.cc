#include "common-classes.hh"
#include "symbol-table.hh"
#include "program.hh"
#include "procedure.hh"
#include "icode.hh"

#include "reg-alloc.hh"

// compile
void Program::compile(){
	machine_desc_object.initialize_instruction_table();
	machine_desc_object.initialize_register_table();
	machine_desc_object.clear_local_register_mappings();
	// cout<<"into program compile"<<endl;
	map<string, Procedure*>::iterator it = local_procedures->begin();
	for(;it!=local_procedures->end();it++)
	{
		((*it).second)->compile();
		machine_desc_object.clear_local_register_mappings();
	}
	// cout<<"out of program compile"<<endl;
}

