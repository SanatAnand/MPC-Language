%scanner scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

%union 
{
	//ADD CODE HERE

	int integer_value;
	float float_value;
	std::string * string_value;
	list<string> * n_list;
	Sequence_Ast * sequence_ast;
	Ast * ast;
	Procedure * procedure;
	Arithmetic_Expr_Ast * arith;
	list<Ast *> *ast_list;
	pair<Data_Type, string> * declar;
	list<pair<Data_Type, string> *> * decl_entry_list;
};

//ADD TOKENS HERE

%token <integer_value> INTEGER_NUMBER
%token <string_value> NAME
%token INTEGER FIELD PROTOCOL
%token ASSIGN PARTY SEND
%token IDEAL ENV IN
%token OPEN AS RAND

%left '+' '-'
%left '*' '/'
%right UMINUS
%nonassoc '('

//ADD CODE HERE


%type <sequence_ast> statement_list
%type <ast> assignment_statement
%type <ast> variable
%type <ast> constant
%type <ast> arith_expression
%type <ast> expression_term
%type <ast> operand
%type <ast> session_call
%type <ast> statement
%type <ast> tying_statement
%type <ast> pvt_statement
%type <ast> send_statement
%type <ast> mult_statement
%type <ast> party_id
%type <ast> random
%type <ast> declaration_statement
%type <ast> variable_declaration
%type <ast> declaration
%type <n_list> party_list
%type <declar> var_decl
%type <decl_entry_list> varlist
%type <decl_entry_list> opt_varlist
%type <string_value> type_decl
%type <n_list> n_list

%start program

%%

program:
	protocol_list
	{
	if (NOT_ONLY_PARSE)
	{
		// CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

		// program_object.set_procedure(current_procedure, get_line_number());

		// CHECK_INVARIANT((program_object.is_GMW_defined()), "No GMW protocol");
	}
	}
;

protocol_list:
	protocol
	{
	if (NOT_ONLY_PARSE)
	{
		// Symbol_Table * global_table = new Symbol_Table();
		// program_object.set_global_table(*global_table);
		// global_table->set_table_scope(global);
	}	
	}
|
	protocol
	protocol_list
	{
	if (NOT_ONLY_PARSE)
	{
		// Symbol_Table * global_table = $1;

		// CHECK_INVARIANT((global_table != NULL), "Global declarations cannot be null");

		// global_table->set_table_scope(global);
		// program_object.set_global_table(*global_table);
	}
	}
;


protocol:
	PROTOCOL NAME '(' opt_varlist ')' ':' PARTY party_list '{'
	{
	if(NOT_ONLY_PARSE)
	{

		CHECK_INVARIANT(($8 != NULL), "party_list cannot be null");

		Procedure *proc = new Procedure(*$2, $4, $8, get_line_number());

		program_object.add_proc(proc, *$2, get_line_number());

		current_procedure = proc;
	}
	}
	statement_list '}'
	{
	if(NOT_ONLY_PARSE)
	{
		Sequence_Ast* seq = $11;

		current_procedure->set_sequence_ast(*seq);
	}
	}
;

party_list:
	NAME
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Name cannot be null");
		list<string> *name_list = new list<string>();
		name_list->push_back(*$1);

		$$ = name_list;
	}
	}
|
	NAME ',' party_list
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Name cannot be null");
		$3->push_back(*$1);
		
		$$ = $3;
	}
	}
;


//------------------------------------------------------------------------//
//updated
opt_varlist:
	{
	if(NOT_ONLY_PARSE)
	{
		list<pair<Data_Type, string> *> * decl_entry_list = new list<pair<Data_Type, string> *>();
		$$ = decl_entry_list;
	}
	}
|
	varlist
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Variable list cannot be null in function arguments");
		$$ = $1;
	}
	}
;

varlist:
	var_decl
	{
	if(NOT_ONLY_PARSE)
	{
		pair<Data_Type, string> * decl = $1;
		
		CHECK_INVARIANT((decl != NULL), "Var Decl cannot be null in function arguments");
		list<pair<Data_Type, string> *> * decl_entry_list = new list<pair<Data_Type, string> *>();
		
		decl_entry_list->push_back(decl);

		$$ = decl_entry_list;
	}
	}
|
	varlist ',' var_decl
	{
	if(NOT_ONLY_PARSE)
	{
		pair<Data_Type, string> * decl = $3;
		list<pair<Data_Type, string> *> * decl_entry_list = $1;

		CHECK_INVARIANT((decl != NULL), "Var Decl cannot be null in function arguments");
		CHECK_INVARIANT((decl_entry_list != NULL), "Variable list cannot be null in function arguments");
		
		decl_entry_list->push_back(decl);

		$$ = decl_entry_list;
	}
	}
;

var_decl:
	INTEGER NAME
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Name cannot be null");
		string name = *$2;
		Data_Type type = int_data_type;
		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);
		$$ = declar;
	}
	}
|
	FIELD NAME
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Name cannot be null");
		string name = *$2;
		Data_Type type = field_data_type;
		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);
		$$ = declar;
	}
	}
;

//--------------------------------------------------------------------------//
type_decl:
	NAME
	{
	if(NOT_ONLY_PARSE)
	{
		//Fieldname
		$$ = $1;
	}
	}
;

declaration_statement:
	variable_declaration
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = $1;
	}
	}
;

variable_declaration:
	declaration ';'
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = $1;
	}
	}
;

declaration:
	INTEGER n_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Name cannot be null");
		Data_Type type = int_data_type;
		string dat_name = "int";
		Ast * number_ast = new Decl_Ast(type,$2,dat_name,get_line_number());
		$$ = number_ast;
	}
	}
|	
	type_decl n_list
	{
	if (NOT_ONLY_PARSE)
	{
		// ADD CODE HERE
		CHECK_INVARIANT(($2 != NULL), "Name cannot be null");
		Data_Type type = other_data_type;
		string dat_name = *$1;
		Ast * number_ast = new Decl_Ast(type,$2,dat_name,get_line_number());
		$$ = number_ast;
	}
	}
;

n_list:
	NAME
	{
	if(NOT_ONLY_PARSE)
	{
		list<string> * name_list = new list<string>();
		name_list->push_back(*$1);
		$$ = name_list;
	}
	}
|
	n_list ',' NAME
	{
	if(NOT_ONLY_PARSE)
	{
		list<string> * name_list = $1;
		name_list->push_back(*$3);
		$$ = name_list;
	}
	}
;
//////// added to statement list
statement_list:
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		
		$$ = new Sequence_Ast(get_line_number());
	}
	}
|
	statement_list statement
	{
	if (NOT_ONLY_PARSE)
	{

		//ADD CODE HERE

		
		Sequence_Ast * seq = $1;
		Ast * ass_ast = $2;
		seq->ast_push_back(ass_ast);
		$$ = seq;
	}
	}
;

statement:
	assignment_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "assignment statement is null");

		$$ = $1;
	}
	}
|
	mult_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "mult statement is null");

		$$ = $1;
	}
	}
|
	pvt_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "pvt statement is null");

		$$ = $1;
	}
	}
|
	send_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "send statement is null");

		$$ = $1;
	}
	}
|
	session_call
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "session_call statement is null");

		$$ = $1;
	}
	}
|
	tying_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "tying statement is null");

		$$ = $1;
	}
	}
|
	declaration_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "declaration statement is null");

		$$ = $1;
	}
	}
;

mult_statement:
	'{' statement_list '}'
	{
	if (NOT_ONLY_PARSE)
	{
	
		$$ = $2;
	}
	}
;

pvt_statement:
	IN party_id ':' '{' statement_list '}'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * name_ast1 = $5;
		Ast * name_ast2 = $2;

		Ast * ass_ast = new In_Ast(name_ast1, name_ast2, get_line_number());
		
		$$ = ass_ast;
	}
	}
;

tying_statement:
	party_id ':' NAME ';'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * lhs = $1;
		string rhs = *$3;

		Ast * ass_ast = new Tying_Ast(lhs, rhs, get_line_number());

		$$ = ass_ast;
	}
	}
;
// Make sure to call check_ast in assignment_statement and arith_expression
// Refer to error_display.hh for displaying semantic errors if any
assignment_statement:
	variable ASSIGN arith_expression ';'
	{
	if (NOT_ONLY_PARSE)
	{

		// CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		// //ADD YOUR CODE HERE
		Ast * name_ast1 = $1;
		Ast * name_ast2 = $3;

		Ast * ass_ast = new Assignment_Ast(name_ast1, name_ast2, get_line_number());
		
		// ass_ast->check_ast();
		$$ = ass_ast;
	}
	}
|
	variable ASSIGN random ';'
	{
	if (NOT_ONLY_PARSE)
	{

		// CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		// //ADD YOUR CODE HERE
		Ast * name_ast1 = $1;
		Ast * name_ast2 = $3;

		Ast * ass_ast = new Assignment_Ast(name_ast1, name_ast2, get_line_number());
		
		// ass_ast->check_ast();
		$$ = ass_ast;
	}
	}
;

random:
	RAND
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * number_ast = new Random_Ast(get_line_number());
		$$ = number_ast;
	}
	}
;

session_call:
	OPEN NAME AS NAME '{' statement_list '}' ';'
	{
	if(NOT_ONLY_PARSE)
	{
		
		string session_name = *$2;
		string proc_name = *$4;

		Ast * seq = $6;

		Ast * func = new Session_call_Ast(seq, proc_name, session_name, get_line_number());
		
		$$ = func;
	}
	}
;

send_statement:
	party_id '.' variable SEND party_id '.' variable ';'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * name_ast1 = $1;
		Ast * var_ast1 = $3;

		Ast * name_ast2 = $5;
		Ast * var_ast2 = $7;

		int type = 1;

		Ast * ass_ast = new Send_Ast(name_ast1, name_ast2, var_ast1, var_ast2, get_line_number(), type);

		$$ = ass_ast;
	}
	}
|
	party_id SEND party_id '.' variable ';'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * name_ast1 = $1;
		Ast * var_ast1 = NULL;

		Ast * name_ast2 = $3;
		Ast * var_ast2 = $5;

		int type = 2;

		Ast * ass_ast = new Send_Ast(name_ast1, name_ast2, var_ast1, var_ast2, get_line_number(), type);

		$$ = ass_ast;
	}
	}
|
	party_id '.' variable SEND party_id ';'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * name_ast1 = $1;
		Ast * var_ast1 = $3;

		Ast * name_ast2 = $5;
		Ast * var_ast2 = NULL;

		int type = 3;

		Ast * ass_ast = new Send_Ast(name_ast1, name_ast2, var_ast1, var_ast2, get_line_number(), type);

		$$ = ass_ast;
	}
	}
// |
// 	party_id '.' variable SEND NAME '.' party_id ';'
// 	{
// 	if(NOT_ONLY_PARSE)
// 	{
// 		// CHECK_INVARIANT(($3 != NULL), "Expression cannot be null in print");
// 		// //ADD YOUR CODE HERE
// 		// Ast * arith_ast = $3;

// 		// Ast * pr_ast = new Print_Ast(arith_ast, get_line_number());
		
// 		// $$ = pr_ast;
// 	}
// 	}
;

///////////////////////////////////////////////////////
party_id:
	NAME
	{
	if(NOT_ONLY_PARSE)
	{
		string name_ast1 = *$1;
		int type = 1;

		Ast * ass_ast = new Party_Ast(name_ast1, type, get_line_number());

		$$ = ass_ast;
	}
	}
|
	ENV
	{
	if(NOT_ONLY_PARSE)
	{
		string name_ast1 = "ENV";
		int type = 2;

		Ast * ass_ast = new Party_Ast(name_ast1, type, get_line_number());

		$$ = ass_ast;
	}
	}
|
	IDEAL
	{
	if(NOT_ONLY_PARSE)
	{
		string name_ast1 = "IDEAL";
		int type = 3;

		Ast * ass_ast = new Party_Ast(name_ast1, type, get_line_number());

		$$ = ass_ast;
	}
	}
;

arith_expression:
	operand '+' operand
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Plus_Ast(op1, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	operand '-' operand
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * mi_ast = new Minus_Ast(op1, op2, get_line_number());
		$$ = mi_ast;
	}
	}	
|
	operand '*' operand
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * ml_ast = new Mult_Ast(op1, op2, get_line_number());
		$$ = ml_ast;
	}
	}
|
	operand '/' operand
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * div_ast = new Divide_Ast(op1, op2, get_line_number());
		$$ = div_ast;
	}
	}
|
	'-' operand %prec UMINUS
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($2 != NULL)), "lhs cannot be null");

		Ast * op1 = $2;

		Ast * um_ast = new UMinus_Ast(op1, NULL, get_line_number());
		$$ = um_ast;
	}
	}
|
	'(' operand ')'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Operand cannot be null");
		$$ = $2;
	}
	}
|
	expression_term
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Expression cannot be null");
		$$ = $1;
	}
	}
		//ADD RELEVANT CODE ALONG WITH GRAMMAR RULES HERE
                // SUPPORT binary +, -, *, / operations, unary -, and allow parenthesization
                // i.e. E -> (E)
                // Connect the rules with the remaining rules given below
;

operand:
	arith_expression
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Expression cannot be null");
		$$ = $1;
		//ADD CODE HERE
	}
	}
;

expression_term:
	variable
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Variable cannot be null");
		$$ = $1;
		//ADD CODE HERE
	}
	}
|
	constant
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Constant cannot be null");
		$$ = $1;
		//ADD CODE HERE
	}
	}
;

variable:
	NAME
	{
	if (NOT_ONLY_PARSE)
	{

		string name = *$1;

		$$ = new Name_Ast(*$1, get_line_number());

	}
	}
;

constant:
	INTEGER_NUMBER
	{
	if (NOT_ONLY_PARSE)
	{
		int cons = $1;
		Ast * number_ast = new Number_Ast<int>(cons, get_line_number());
		$$ = number_ast;
		//ADD CODE HERE
	}
	}
;