
#ifndef PROGRAM_HH
#define PROGRAM_HH

#include <string>
#include <map>

#define GLOB_SPACE "   "

using namespace std;

class Program;

extern Program program_object;
//map<string,string> global_strings;
class Program
{
	// Symbol_Table global_symbol_table;
	

public:
	Program();
	~Program();
	
	map<string, Procedure*> *local_procedures;

	void add_proc(Procedure * proc, string proc_name, int line);
	void print(ostream & file_buffer);
};

#endif
