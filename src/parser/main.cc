#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "ast.hh"
#include "procedure.hh"
#include "program.hh"
#include "parser.h"

#include "dirent.h" 

int main(int argc, char * argv[]) 
{
	string input_file_name = command_options.process_user_command_options(argc, argv);
	ofstream spim_file;
	spim_file.open(input_file_name+".asm");
	Parser sclp_parser(input_file_name);

	CHECK_INPUT((!sclp_parser.parse()), "Cannot parse the input program", NO_FILE_LINE);

	if (command_options.not_only_parse)
	{
		if ((error_status() == false) && (command_options.is_show_ast_selected()))
			program_object.print(std::cout);
		
		spim_file.close();
	}

	return 0;
}
