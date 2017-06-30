#include "common-classes.hh"
#include "program.hh"
#include "procedure.hh"
//extern map<string,string> global_strings;
Program::Program(){
	local_procedures = new map<string, Procedure*>();
}
Program::~Program(){ 
	delete local_procedures;
}

void Program::add_proc(Procedure * proc, string proc_name, int line){
	(*local_procedures)[proc_name] = proc;
}

void Program::print(ostream & file_buffer){
	map<string, Procedure*>::iterator it = local_procedures->begin();
	for(;it!=local_procedures->end();it++)
	{
		it->second->print(std::cout);
	}
}


Program program_object;