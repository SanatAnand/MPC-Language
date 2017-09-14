%scanner scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc


%union 
{
	//ADD CODE HERE

	bool boolean_value;
	int integer_value;
	float float_value;
	std::string * string_value;
	list<Ast *> * ast_list;
	list<string> * n_list;
	Sequence_Ast * sequence_ast;
	Ast * ast;
	connection_param conn;
	pair<Ast*, condition> *conditional;
	Arithmetic_Expr_Ast * arith;
	pair<Data_Type, Ast*> *port_decl;
	pair<Data_Type, string> * type;
	list<pair<Data_Type, string> *> * decl_entry_list;
	pair<string, list<Ast*>*> * p_call;
	pair<bool, Sequence_Ast*> *seq;
	list<pair<Data_Type, Ast*> *>* decl_list;
	pair<bool, Ast*> * opt_each_expr;
	list<pair<pair<Data_Type, string> *, Ast*> *> * port_list;
	pair<pair<Data_Type, string> *, Ast*> * port;
	pair<bool, list<pair<pair<Data_Type, string> *, Ast*> *> *> * opt_port;
};

//ADD TOKENS HERE

%token <integer_value> INTEGER_NUMBER
%token <string_value> NAME
%token INTEGER FIELD PROTOCOL
%token ASSIGN PARTY SEND
%token IDEAL ENV IN
%token OPEN AS RAND
%token USES ABORTING WRAP START BEFORE
%token AFTER EACH SESSION UNKNOWN PORT INPORT OUTPORT
%token FOR OF EXCEPT WHERE IF SET CONNECT TO FORWARD AND
%token ENV_PORT RAND_PORT BOOL PARTIAL WITH

%token DOUBLE_COLON DOUBLE_DOT

%nonassoc FROM
%left OR
%left B_AND
%nonassoc EQ NE
%nonassoc LT LE GE GT
%left '+' '-'
%left '*' '/' '%'
%right UMINUS NOT
%left '.'
%right '|'
%nonassoc '('
%nonassoc THEN
%nonassoc ELSE

//ADD CODE HERE

%type <sequence_ast> statement_list

%type <ast_list> dimlist
%type <ast> term
%type <ast> decl_term
%type <type> type
%type <boolean_value> opt_each
%type <boolean_value> opt_partial
%type <conditional> opt_cond
%type <ast> exp
%type <ast_list> exp_list
%type <ast_list> exp_list_elem
%type <conn> to_and

%type <ast> protocol
%type <ast> prot_decl
%type <ast> p_list_elem
%type <ast> arg
%type <ast_list> p_list
%type <ast_list> arg_list
%type <ast_list> opt_arg_list
%type <ast_list> opt_uses
%type <ast_list> prot_decl_list
%type <ast_list> varlist
%type <ast_list> opt_id_list
%type <ast_list> id_list
%type <p_call> prot_call
%type <port> port_decl


%type <ast> variable
%type <ast> constant

%type <ast> statement
%type <ast> assignment_statement
%type <ast> mult_statement
%type <ast> pvt_statement
%type <ast> with_statement
%type <ast> send_statement
%type <ast> session_call
%type <ast> tying_statement
%type <ast> declaration_statement
%type <ast> loop_statement
%type <ast> cond_statement
%type <ast> wrap_statement
%type <ast> connect_statement
%type <ast> abort_statement
%type <ast> seq_statement
%type <ast> start_statement
%type <ast> set_statement
%type <ast> forward_statement

%type <port_list> port_decl_list
%type <port_list> port_clause
%type <opt_port> opt_port_clause
%type <ast> each_expr
%type <opt_each_expr> opt_each_expr
// %type <ast> party_id
// %type <ast> random
// %type <ast> variable_declaration
// %type <ast> declaration
// %type <n_list> party_list
// %type <declar> var_decl
// %type <decl_entry_list> varlist
// %type <decl_entry_list> opt_varlist
// %type <string_value> type_decl
// %type <n_list> n_list

%start program

%%

program:
	protocol_list
	{
	if(NOT_ONLY_PARSE)
	{
		// program_object.set_procedure($1, get_line_number());
		// program_object.global_list_in_proc_check();
	}
	}
;

protocol_list:
	protocol
	{
	if(NOT_ONLY_PARSE)
	{
		// nothing here
	}
	}
|
	protocol protocol_list
	{
	if(NOT_ONLY_PARSE)
	{
		// nothing here
	}
	}
;


protocol:
	prot_decl '{' 
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Protocol declaration cannot be null");
	}
	}
	opt_uses 
	{
	if(NOT_ONLY_PARSE)
	{
		// CHECK_INVARIANT(($4 != NULL), "Uses  cannot be null");
	}
	}
	statement_list '}'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($6 != NULL), "Statement list cannot be null");
		Protocol_Ast * protocol = new Protocol_Ast( (Protocol_Decl_Ast*)$1, $4, $6, get_line_number());
		program_object.add_protocol(protocol, ((Protocol_Decl_Ast*)$1)->get_protocol_name(), get_line_number());
		$$ = protocol;
	}
	}
;

prot_decl:
	PROTOCOL NAME '(' opt_arg_list ')' ':' PARTY p_list
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Protocol name cannot be null");
		CHECK_INVARIANT(($4 != NULL), "Argument list cannot be null");
		CHECK_INVARIANT(($8 != NULL), "Party list cannot be null");				
		Ast * protocol_decl = new Protocol_Decl_Ast(*$2, $4, $8, get_line_number());
		$$ = protocol_decl;
	}
	}
;

p_list:
	p_list_elem
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Party element cannot be null");
		list<Ast*>* party_list = new list<Ast*>();
		party_list->push_back($1);
		$$ = party_list;
	}
	}
|
	p_list ',' p_list_elem
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Party element cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Party list cannot be null");
		list<Ast*>* party_list = $1;
		party_list->push_back($3);
		$$ = party_list;		
	}
	}
;

p_list_elem:
	exp opt_port_clause
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Party expression cannot be null");		
		bool port_clause_exists = ($2!=NULL);
		bool each_present;
		list<pair<pair<Data_Type, string> *, Ast*> *> * seq = NULL;
		if(port_clause_exists)
		{
			each_present = $2->first;
			seq = $2->second;
		}
		Ast * port_ast = new Port_Clause_Ast($1, port_clause_exists, each_present, seq, get_line_number());
		$$ = port_ast;
	}
	}
;

opt_port_clause:
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = NULL;
	}
	}
|
	opt_each WITH port_clause
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($3!=NULL),"Port clause cannot ne null");
		pair<bool, list<pair<pair<Data_Type, string> *, Ast*> *> *> *seq = new pair<bool, list<pair<pair<Data_Type, string> *, Ast*> *> *>($1,$3);
		$$ = seq;
	}
	}
;

port_clause:
	'{' port_decl_list '}'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2!=NULL),"Port declaration list cannot be null");
		$$ = $2;
	}
	}
// |
// 	type term
// 	{
// 	if(NOT_ONLY_PARSE)
// 	{
// 		CHECK_INVARIANT(($1 != NULL), "Type cannot be null");		
// 		CHECK_INVARIANT(($2 != NULL), "Term cannot be null");

// 		pair<Data_Type, string> * type = $1;
// 		list<Ast *> *varlist = new list<Ast*>();
// 		varlist->push_back($2);

// 		Ast * ass_ast = new Decl_Ast(type->first, varlist, type->second, get_line_number());

// 		Sequence_Ast* statement_list = new Sequence_Ast(get_line_number());
// 		statement_list->ast_push_back(ass_ast);
// 		$$ = statement_list;
// 	}
// 	}
;

port_decl_list:
	port_decl
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Statement cannot be null");
		list<pair<pair<Data_Type, string> *, Ast*> *> * statement_list = new list<pair<pair<Data_Type, string> *, Ast*> *>();
		statement_list->push_back($1);
		$$ = statement_list;
	}
	}
|
	port_decl_list ',' port_decl
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Statement list cannot be null");		
		CHECK_INVARIANT(($3 != NULL), "Statement cannot be null");		
		list<pair<pair<Data_Type, string> *, Ast*> *> * statement_list = $1;
		statement_list->push_back($3);
		$$ = statement_list;
	}
	}
;

port_decl:
	type term
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Type cannot be null");		
		CHECK_INVARIANT(($2 != NULL), "Term cannot be null");

		pair<Data_Type, string> * type = $1;

		pair<pair<Data_Type, string> *, Ast*> * ass_ast = new pair<pair<Data_Type, string> *, Ast*>(type, $2);

		$$ = ass_ast;
	}
	}
;

opt_arg_list:
	{
	if(NOT_ONLY_PARSE)
	{
		list<Ast*>* arg_list = new list<Ast*>();
		$$ = arg_list;
	}
	}
|
	arg_list
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT( ($1!=NULL), "Argument list cannot be null");
		$$ = $1;
	}
	}
;

arg_list:
	arg
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT( ($1!=NULL), "Argument cannot be null");
		list<Ast*>* arg_list = new list<Ast*>();
		arg_list->push_back($1);
		$$ = arg_list;
	}
	}
|
	arg_list ',' arg
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT( ($3!=NULL), "Argument cannot be null");
		CHECK_INVARIANT( ($1!=NULL), "Argument list cannot be null");
		list<Ast*>* arg_list = $1;
		arg_list->push_back($3);
		$$ = arg_list;
	}
	}
;

arg:
	type term
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Type cannot be null");		
		CHECK_INVARIANT(($2 != NULL), "Term cannot be null");
		Ast* arg = new Argument_Ast($2, $1->first, $1->second, get_line_number());
		$$ = arg;
	}
	}
|
	'(' prot_decl ')'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Protocol declaration cannot be null");		
		$$ = new Prot_Arg_Ast($2, get_line_number());
	}
	}
;

term:
	variable dimlist
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Variable cannot be null in term");
		CHECK_INVARIANT(($2 != NULL), "Dimlist cannot be null in term");
		Ast* name_ast = $1;
		list<Ast*> *dim = $2;
		$$ = new Term_Ast(name_ast, dim, variable, get_line_number());
	}
	}
|
	'#' variable
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Variable cannot be null in term");
		Ast* name_ast = $2;
		list<Ast*> *dim = NULL;
		$$ = new Term_Ast(name_ast, dim, iterator_variable, get_line_number());
	}
	}
|
	'@'
	{
	if(NOT_ONLY_PARSE)
	{
		list<Ast*> *dim = NULL;
		$$ = new Term_Ast(NULL, dim, wrapper, get_line_number());
	}
	}
|
	RAND
	{
	if(NOT_ONLY_PARSE)
	{
		list<Ast*> *dim = NULL;
		$$ = new Term_Ast(NULL, dim, random_sample, get_line_number());
	}
	}
|
	constant
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Constant cannot be null in term");
		Ast* name_ast = $1;
		list<Ast*> *dim = NULL;
		$$ = new Term_Ast(name_ast, dim, constant, get_line_number());
	}
	}
|
	ENV
	{
	if(NOT_ONLY_PARSE)
	{
		list<Ast*> *dim = NULL;
		$$ = new Term_Ast(NULL, dim, environment, get_line_number());
	}
	}
|
	IDEAL
	{
	if(NOT_ONLY_PARSE)
	{
		list<Ast*> *dim = NULL;
		$$ = new Term_Ast(NULL, dim, ideal, get_line_number());
	}
	}
|
	RAND_PORT
	{
	if(NOT_ONLY_PARSE)
	{
		list<Ast*> *dim = NULL;
		$$ = new Term_Ast(NULL, dim, rand_port, get_line_number());
	}
	}
|
	ENV_PORT
	{
	if(NOT_ONLY_PARSE)
	{
		list<Ast*> *dim = NULL;
		$$ = new Term_Ast(NULL, dim, env_port, get_line_number());
	}
	}
|
	DOUBLE_DOT
	{
	if(NOT_ONLY_PARSE)
	{
		list<Ast*> *dim = NULL;
		$$ = new Term_Ast(NULL, dim, double_dot, get_line_number());
	}
	}
;

dimlist: 
	{
	if(NOT_ONLY_PARSE)
	{
		list<Ast *> *dim_list = new list<Ast *>();
		$$ = dim_list;
	}
	}
|
	dimlist '[' exp ']'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($3 != NULL), "Exp cannot be null in dimlist");
		CHECK_INVARIANT(($1 != NULL), "Dimlist cannot be null");
		$1->push_back($3);
		
		$$ = $1;
	}
	}
|
	dimlist '[' ']'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Dimlist cannot be null");
		$1->push_back(NULL);
		
		$$ = $1;
	}
	}
;

exp:
	exp '+' exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Plus_Ast(op1, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	exp '-' exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Minus_Ast(op1, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	exp '*' exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Mult_Ast(op1, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	exp '/' exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Divide_Ast(op1, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	exp '%' exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Modulo_Ast(op1, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	'-' exp %prec UMINUS
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($2 != NULL)), "lhs cannot be null");

		Ast * op1 = $2;

		Ast * um_ast = new UMinus_Ast(op1, NULL, get_line_number());
		$$ = um_ast;
	}
	}
|
	exp B_AND exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Boolean_Expr_Ast(op1, boolean_and, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	exp OR exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Boolean_Expr_Ast(op1, boolean_or, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	NOT exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($2 != NULL)), "lhs cannot be null");

		Ast * op1 = $2;

		Ast * pl_ast = new Boolean_Expr_Ast(op1, boolean_not, NULL, get_line_number());
		$$ = pl_ast;
	}
	}
|
	exp EQ exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Relational_Expr_Ast(op1, equalto, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	exp LT exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Relational_Expr_Ast(op1, less_than, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	exp GT exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Relational_Expr_Ast(op1, greater_than, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	exp NE exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Relational_Expr_Ast(op1, not_equalto, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	exp LE exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Relational_Expr_Ast(op1, less_equalto, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	exp GE exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Relational_Expr_Ast(op1, greater_equalto, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	'(' exp ')'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Operand cannot be null");
		$$ = $2;
	}
	}
|
	exp '|' exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Port_Expr_Ast(op1, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	exp '.' exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new Party_Expr_Ast(op1, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	exp FROM exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * op1 = $1;
		Ast * op2 = $3;

		Ast * pl_ast = new From_Expr_Ast(op1, op2, get_line_number());
		$$ = pl_ast;
	}
	}
|
	exp_list
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Exp List cannot be null");
		list<Ast*> *exp_list = $1;

		Ast * pl_ast = new Expression_List_Ast(exp_list, get_line_number());
		$$ = pl_ast;
	}
	}
|
	term
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Term cannot be null");
		$$ = $1;
	}
	}
;


opt_uses:
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = NULL;
	}
	}
|
	USES prot_decl_list ';'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Declaration list cannot be null");		
		$$ = $2;
	}
	}
;

prot_decl_list:
	prot_decl
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Declaration cannot be null");		
		list<Ast*>* protocol_decl_list = new list<Ast*>();
		protocol_decl_list->push_back($1);
		$$ = protocol_decl_list;
	}
	}
|
	prot_decl_list prot_decl
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Declaration cannot be null");		
		CHECK_INVARIANT(($1 != NULL), "Declaration list cannot be null");		
		list<Ast*>* protocol_decl_list = $1;
		protocol_decl_list->push_back($2);		
		$$ = protocol_decl_list;
	}
	}
;

statement_list:
	statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Statement cannot be null");
		Sequence_Ast* statement_list = new Sequence_Ast(get_line_number());
		statement_list->ast_push_back($1);
		$$ = statement_list;
	}
	}
|
	statement_list statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Statement list cannot be null");		
		CHECK_INVARIANT(($2 != NULL), "Statement cannot be null");		
		Sequence_Ast* statement_list = $1;
		statement_list->ast_push_back($2);
		$$ = statement_list;
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
	with_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "with statement is null");
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
|
	protocol
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "protocol statement is null");
		$$ = $1;
	}
	}
|
	loop_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "loop statement is null");
		$$ = $1;
	}
	}
|
	cond_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "cond statement is null");
		$$ = $1;
	}
	}
|
	wrap_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "wrap statement is null");
		$$ = $1;
	}
	}
|
	connect_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "connect statement is null");
		$$ = $1;
	}
	}
|
	abort_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "abort statement is null");
		$$ = $1;
	}
	}
|
	seq_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "seq statement is null");
		$$ = $1;
	}
	}
|
	start_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "start statement is null");
		$$ = $1;
	}
	}
|
	set_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "set statement is null");
		$$ = $1;
	}
	}
|
	forward_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL)), "forward statement is null");
		$$ = $1;
	}
	}
|
	error ';'
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = new Abort_Ast(get_line_number());
	}
	}
;

abort_statement:
	ABORTING ';'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * ass_ast = new Abort_Ast(get_line_number());
		$$ = ass_ast;
	}
	}
;

wrap_statement:
	WRAP statement
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * name_ast1 = $2;
		Ast * ass_ast = new Wrap_Ast(name_ast1, get_line_number());
		$$ = ass_ast;
	}
	}
;

start_statement:
	START exp ';'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * name_ast1 = $2;
		Ast * ass_ast = new Start_Ast(name_ast1, get_line_number());
		$$ = ass_ast;
	}
	}
;

seq_statement:
	BEFORE opt_each_expr '{' statement_list '}'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Each Expression cannot be null");
		CHECK_INVARIANT(($4 != NULL), "Statement cannot be null");
		Ast * name_ast1 = $2->second;
		bool each = $2->first;
		Ast * name_ast2 = $4;
		order o = before;

		Ast * ass_ast = new Seq_Ast(name_ast1, name_ast2, each, o, get_line_number());
		$$ = ass_ast;
	}
	}
|
	AFTER opt_each_expr '{' statement_list '}'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Each Expression cannot be null");
		CHECK_INVARIANT(($4 != NULL), "Statement cannot be null");
		Ast * name_ast1 = $2->second;
		bool each = $2->first;
		Ast * name_ast2 = $4;
		order o = after;

		Ast * ass_ast = new Seq_Ast(name_ast1, name_ast2, each, o, get_line_number());
		$$ = ass_ast;
	}
	}
;

opt_each:
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = false;
	}
	}
|
	EACH
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = true;
	}
	}
;

assignment_statement:
	exp ASSIGN exp ';'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * name_ast1 = $1;
		Ast * name_ast2 = $3;
		Ast * ass_ast = new Assignment_Ast(name_ast1, name_ast2, get_line_number());
		$$ = ass_ast;
	}
	}
;

mult_statement:
	'{' statement_list '}'
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = $2;
	}
	}
;

pvt_statement:
	IN opt_each_expr '{' statement_list '}'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * name_ast1 = $4;
		Ast * name_ast2 = $2->second;
		bool each = $2->first;

		Ast * ass_ast = new In_Ast(name_ast1, name_ast2, each, get_line_number());
		
		$$ = ass_ast;
	}
	}
;

with_statement:
	WITH opt_each_expr '{' port_decl_list '}' ';'
	{
	if(NOT_ONLY_PARSE)
	{
		list<pair<pair<Data_Type, string> *, Ast*> *> * name_ast1 = $4;
		Ast * name_ast2 = $2->second;
		bool each = $2->first;

		Ast * ass_ast = new With_Ast(name_ast1, name_ast2, each, get_line_number());
		
		$$ = ass_ast;
	}
	}
;

send_statement:
	exp SEND exp ';'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * name_ast1 = $1;
		Ast * name_ast2 = $3;
		Ast * ass_ast = new Send_Ast(name_ast1, name_ast2, get_line_number());
		$$ = ass_ast;
	}
	}
;

session_call:
	opt_partial OPEN term AS prot_call '{' statement_list '}'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * s_name = $3;
		Ast * seq = $7;
		bool partial = $1;
		pair<string, list<Ast*>* >* p_call = $5;

		Ast * ass_ast = new Session_call_Ast(s_name, seq, partial, p_call->first, p_call->second, get_line_number());
		$$ = ass_ast;
	}
	}
;

opt_partial:
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = false;
	}
	}
|
	PARTIAL
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = true;
	}
	}
;

prot_call:
	NAME '(' opt_id_list ')'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT( ($3!=NULL), "Id list cannot be null");
		pair<string, list<Ast*>* >* p_call = new pair<string, list<Ast*>* >(*$1, $3);
		$$ = p_call;
	}
	}
;

opt_id_list:
	{
	if(NOT_ONLY_PARSE)
	{
		list<Ast*>* arg_list = new list<Ast*>();
		$$ = arg_list;
	}
	}
|
	id_list
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT( ($1!=NULL), "Id list cannot be null");
		$$ = $1;
	}
	}
;

id_list:
	variable
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT( ($1!=NULL), "Variable cannot be null");
		list<Ast*>* arg_list = new list<Ast*>();
		arg_list->push_back($1);
		$$ = arg_list;
	}
	}
|
	id_list ',' variable
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT( ($1!=NULL), "Argument list cannot be null");
		CHECK_INVARIANT( ($3!=NULL), "Argument cannot be null");
		list<Ast*>* arg_list = $1;
		arg_list->push_back($3);
		$$ = arg_list;
	}
	}
;

tying_statement:
	exp ':' exp ';'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * lhs = $1;
		Ast * rhs = $3;

		Ast * ass_ast = new Tying_Ast(lhs, rhs, get_line_number());

		$$ = ass_ast;
	}
	}
;

declaration_statement:
	type varlist ';'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Type cannot be null");		
		CHECK_INVARIANT(($2 != NULL), "Variable list cannot be null");

		pair<Data_Type, string> * type = $1;
		list<Ast *> *varlist = $2;

		Ast * ass_ast = new Decl_Ast(type->first, varlist, type->second, get_line_number());
		$$ = ass_ast;
	}
	}
;

type:
	PARTY
	{
	if(NOT_ONLY_PARSE)
	{
		string name = "PARTY";
		Data_Type type = party_data_type;
		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);
		$$ = declar;
	}
	}
|
	INTEGER
	{
	if(NOT_ONLY_PARSE)
	{
		string name = "INTEGER";
		Data_Type type = int_data_type;
		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);
		$$ = declar;
	}
	}
|
	SESSION
	{
	if(NOT_ONLY_PARSE)
	{
		string name = "SESSION";
		Data_Type type = session_data_type;
		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);
		$$ = declar;
	}
	}
|
	UNKNOWN
	{
	if(NOT_ONLY_PARSE)
	{
		string name = "UNKNOWN";
		Data_Type type = unknown_data_type;
		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);
		$$ = declar;
	}
	}
|
	PORT
	{
	if(NOT_ONLY_PARSE)
	{
		string name = "PORT";
		Data_Type type = port_data_type;
		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);
		$$ = declar;
	}
	}
|
	INPORT
	{
	if(NOT_ONLY_PARSE)
	{
		string name = "INPORT";
		Data_Type type = inport_data_type;
		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);
		$$ = declar;
	}
	}
|
	OUTPORT
	{
	if(NOT_ONLY_PARSE)
	{
		string name = "OUTPORT";
		Data_Type type = outport_data_type;
		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);
		$$ = declar;
	}
	}
|
	BOOL
	{
	if(NOT_ONLY_PARSE)
	{
		string name = "BOOL";
		Data_Type type = bool_data_type;
		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);
		$$ = declar;
	}
	}
|
	FIELD
	{
	if(NOT_ONLY_PARSE)
	{
		string name = "FIELD";
		Data_Type type = field_data_type;
		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);
		$$ = declar;
	}
	}
|
	NAME
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Name cannot be null");
		string name = *$1;
		Data_Type type = other_data_type;
		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);
		$$ = declar;
	}
	}
;

varlist:
	decl_term
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Term cannot be null");
		list<Ast*>* varlist = new list<Ast*>();
		varlist->push_back($1);
		$$ = varlist;
	}
	}
|
	varlist ',' decl_term
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($3 != NULL), "Term cannot be null");
		CHECK_INVARIANT(($1 != NULL), "Varlist cannot be null");
		list<Ast*>* varlist = $1;
		varlist->push_back($3);	
		$$ = varlist;
	}
	}
;

decl_term:
	term opt_port_clause
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Term cannot be null");
		bool port_clause_exists = ($2!=NULL);

		bool each_present;
		list<pair<pair<Data_Type, string> *, Ast*> *> * seq = NULL;
		if(port_clause_exists)
		{
			each_present = $2->first;
			seq = $2->second;
		}
		
		Ast * term_ast = new Decl_Term_Ast($1,port_clause_exists,seq,each_present,get_line_number());
		$$ = term_ast;
	}
	}
;

loop_statement:
	FOR each_expr statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Each expression cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Body cannot be null");
		Ast* loop_statement = new Iteration_Statement_Ast($2, $3, get_line_number());
		$$ = loop_statement;
	}
	}
;

opt_each_expr:
	exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Expression cannot be null");
		$$ = new pair<bool, Ast*>(false,$1);
	}
	}
|
	each_expr
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Each expression cannot be null");
		$$ = new pair<bool, Ast*>(true,$1);
	}
	}
;

each_expr:
	EACH variable OF exp opt_cond
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Variable cannot be null");		
		CHECK_INVARIANT(($4 != NULL), "Expression cannot be null");		
		CHECK_INVARIANT(($5 != NULL), "Opt-Condition cannot be null");		
		Ast* loop_statement = new Each_Statement_Ast($5->first, $5->second, $2, $4, get_line_number());
		$$ = loop_statement;
	}
	}
|
	EACH exp opt_cond
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Expression cannot be null");		
		CHECK_INVARIANT(($3 != NULL), "Opt-Condition cannot be null");		
		Ast* loop_statement = new Each_Statement_Ast($3->first, $3->second, NULL, $2, get_line_number());
		$$ = loop_statement;
	}
	}
|
	EACH variable OF exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Variable cannot be null");		
		CHECK_INVARIANT(($4 != NULL), "Expression cannot be null");		
		Ast* loop_statement = new Each_Statement_Ast(NULL, no_cond, $2, $4, get_line_number());
		$$ = loop_statement;
	}
	}
|
	EACH exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Expression cannot be null");		
		Ast* loop_statement = new Each_Statement_Ast(NULL, no_cond, NULL, $2, get_line_number());
		$$ = loop_statement;
	}
	}
;

opt_cond:
	EXCEPT exp
	{
	if(NOT_ONLY_PARSE)
	{
		pair<Ast*, condition> *clause = new pair<Ast*, condition>($2, except);
		$$ = clause;
	}
	}
|
	WHERE exp
	{
	if(NOT_ONLY_PARSE)
	{
		pair<Ast*, condition> *clause = new pair<Ast*, condition>($2, where);
		$$ = clause;
	}
	}
;

cond_statement:
	IF '(' exp ')' statement          %prec THEN
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($3 != NULL), "Expression cannot be null");
		CHECK_INVARIANT(($5 != NULL), "Statement cannot be null");
		Selection_Statement_Ast * cond_statement = new Selection_Statement_Ast($3, $5, NULL, get_line_number());
		$$ = (Ast*) cond_statement;
	}
	}
|
	IF '(' exp ')' statement ELSE statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($3 != NULL), "Expression cannot be null");
		CHECK_INVARIANT(($5 != NULL), "Statement cannot be null");
		CHECK_INVARIANT(($7 != NULL), "Statement cannot be null");
		Selection_Statement_Ast * cond_statement = new Selection_Statement_Ast($3, $5, $7, get_line_number());
		$$ = (Ast*) cond_statement;
	}
	}
;

set_statement:
	SET exp AS prot_call DOUBLE_COLON exp ';'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * lhs = $2;
		Ast * rhs = $6;
		pair<string, list<Ast*>* >* p_call = $4;

		Ast * ass_ast = new Set_Ast(lhs, rhs, p_call->first, p_call->second, get_line_number());

		$$ = ass_ast;
	}
	}
;

connect_statement:
	CONNECT exp to_and exp ';'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * lhs = $2;
		Ast * rhs = $4;
		connection_param c = $3;

		Ast * ass_ast = new Connect_Ast(lhs, rhs, c, get_line_number());

		$$ = ass_ast;
	}
	}
;

to_and:
	TO
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = conn_to;
	}
	}
|
	AND
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = conn_and;
	}
	}
;

exp_list:
	'{' exp_list_elem '}'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "exp_list_elem cannot be null");
		$$ = $2;
	}
	}
;

exp_list_elem:
	exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Exp cannot be null in list");
		list<Ast*> *name_list = new list<Ast*>();
		name_list->push_back($1);

		$$ = name_list;
	}
	}
|
	exp_list_elem ',' exp
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($3 != NULL), "Exp cannot be null in list");
		$1->push_back($3);
		
		$$ = $1;
	}
	}
;

forward_statement:
	FORWARD exp TO exp ';'
	{
	if(NOT_ONLY_PARSE)
	{
		Ast * lhs = $2;
		Ast * rhs = $4;
		Ast * ass_ast = new Forward_Ast(lhs, rhs, get_line_number());
		$$ = ass_ast;
	}
	}
;

variable:
	NAME
	{
	if(NOT_ONLY_PARSE)
	{
		string name = *$1;
		$$ = new Name_Ast(*$1, get_line_number());
	}
	}
;

constant:
	INTEGER_NUMBER
	{
	if(NOT_ONLY_PARSE)
	{
		int cons = $1;
		Ast * number_ast = new Number_Ast<int>(cons, get_line_number());
		$$ = number_ast;
	}
	}
;

//////////////////////////////////////////////////////

// %%
// void yyerror(char *s){
//     fprintf(stderr,"%s Error at line: %d\n",s, linenum);
// }
// int yywrap(){
//     return 1;
// }
// int main(int argc, char *argv[])
// {
//     /* Call the lexer, then quit. */
//     yyin=fopen(argv[1],"r");
//     yyparse();
//     fclose(yyin);
//     return 0;
// }
