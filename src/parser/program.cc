#include "ast.hh"
#include "common-classes.hh"
#include "program.hh"
// #include "procedure.hh"
//extern map<string,string> global_strings;
Program::Program(){
	protocols = new map<string, Protocol_Ast*>();
}
Program::~Program(){ 
	delete protocols;
}

void Program::add_protocol(Protocol_Ast * proc, string protocol_name, int line){
	(*protocols)[protocol_name] = proc;
}

void Program::print(ostream & file_buffer){
	for(auto ele : *protocols)
		ele.second->print(file_buffer);
}

void Program::print_xml(ostream & file_buffer){
	for(auto ele : *protocols)
		ele.second->print_xml(file_buffer);
}

Program program_object;