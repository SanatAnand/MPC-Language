%filenames="scanner"
%lex-source="scanner.cc"

digit [0-9]
operator [\(\)\{\};\,\.:\[\]|#\%]
alphabet [a-zA-Z]
arith [-+\?\:*/]
comparators [<>=]

%%
//ADD YOUR CODE HERE

protocol	{
	store_token_name("PROTOCOL");
	return Parser::PROTOCOL;
}

".."	{
	store_token_name("DOUBLE DOT");
	return Parser::DOUBLE_DOT;
}

"::"	{
	store_token_name("DOUBLE COLON");
	return Parser::DOUBLE_COLON;
}

from	{
	store_token_name("FROM");
	return Parser::FROM;
}

start	{
	store_token_name("START");
	return Parser::START;
}

bool	{
	store_token_name("BOOL");
	return Parser::BOOL;
}

with	{
	store_token_name("WITH");
	return Parser::WITH;
}

abort	{
	store_token_name("ABORT");
	return Parser::ABORTING;
}

true	{
	store_token_name("TRUE");
	return Parser::TRUE;
}

false	{
	store_token_name("FALSE");
	return Parser::FALSE;
}

party	{
	store_token_name("PARTY");
	return Parser::PARTY;
}

and	{
	store_token_name("AND");
	return Parser::AND;
}

environ	{
	store_token_name("ENV_PORT");
	return Parser::ENV_PORT;
}

random	{
	store_token_name("RAND_PORT");
	return Parser::RAND_PORT;
}

"<=" {
	store_token_name("LE");
	return Parser::LE;
}

">=" {
	store_token_name("GE");
	return Parser::GE;
}

"==" {
	store_token_name("EQ");
	return Parser::EQ;
}

"<" {
	store_token_name("LT");
	return Parser::LT;
}

">" {
	store_token_name("GT");
	return Parser::GT;
}

"!=" {
	store_token_name("NE");
	return Parser::NE;
}

"&&" {
	store_token_name("B_AND");
	return Parser::B_AND;
}

"||" {
	store_token_name("OR");
	return Parser::OR;
}

"!" {
	store_token_name("NOT");
	return Parser::NOT;
}

"=>"	{
	store_token_name("SEND");
	return Parser::SEND;
}

field	{
	store_token_name("FIELD");
	return Parser::FIELD;
}

ideal	{
	store_token_name("IDEAL");
	return Parser::IDEAL;
}

int 	{
	store_token_name("INTEGER");
	return Parser::INTEGER;
}

in 	{
	store_token_name("IN");
	return Parser::IN;
}

open 	{
	store_token_name("OPEN");
	return Parser::OPEN;
}

partial 	{
	store_token_name("PARTIAL");
	return Parser::PARTIAL;
}

as 	{
	store_token_name("AS");
	return Parser::AS;
}

[=]	{
	store_token_name("ASSIGN_OP");
	return Parser::ASSIGN;
}

[$]	{
	store_token_name("RAND");
	return Parser::RAND;
}

uses	{
	store_token_name("USES");
	return Parser::USES;
}

wrap	{
	store_token_name("WRAP");
	return Parser::WRAP;
}

before	{
	store_token_name("BEFORE");
	return Parser::BEFORE;
}

after	{
	store_token_name("AFTER");
	return Parser::AFTER;
}

each	{
	store_token_name("EACH");
	return Parser::EACH;
}

session	{
	store_token_name("SESSION");
	return Parser::SESSION;
}

unknown	{
	store_token_name("UNKNOWN");
	return Parser::UNKNOWN;
}

port	{
	store_token_name("PORT");
	return Parser::PORT;
}

inport	{
	store_token_name("INPORT");
	return Parser::INPORT;
}

outport	{
	store_token_name("OUTPORT");
	return Parser::OUTPORT;
}

for	{
	store_token_name("FOR");
	return Parser::FOR;
}

of	{
	store_token_name("OF");
	return Parser::OF;
}

except	{
	store_token_name("EXCEPT");
	return Parser::EXCEPT;
}

where	{
	store_token_name("WHERE");
	return Parser::WHERE;
}

if	{
	store_token_name("IF");
	return Parser::IF;
}

else	{
	store_token_name("ELSE");
	return Parser::ELSE;
}

set	{
	store_token_name("SET");
	return Parser::SET;
}

connect	{
	store_token_name("CONNECT");
	return Parser::CONNECT;
}

to	{
	store_token_name("TO");
	return Parser::TO;
}

forward	{
	store_token_name("FORWARD");
	return Parser::FORWARD;
}

{operator} {
	store_token_name("META CHAR");
	return matched()[0];
}

{arith} {
	store_token_name("ARITHOP");
	return matched()[0];
}

{digit}+	{
			ParserBase::STYPE__ *val = getSval();
            val->integer_value = atoi(matched().c_str());
			store_token_name("NUM");
			return Parser::INTEGER_NUMBER;
		}

([_]|{alphabet})({alphabet}|{digit}|[_])*	{
			ParserBase::STYPE__ *val = getSval();
            val->string_value = new string(matched());
			store_token_name("NAME");
			return Parser::NAME;
		}

\n    		|
";;".*  	|
[ \t]*";;".*	|
[ \t]*"//".*	|
[ \t]*[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]	|
[ \t]		{
			if (command_options.is_show_tokens_selected())
				ignore_token();
		}

[/][*]	{
	string error_message;
	error_message =  "Unterminated comment";
	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, error_message, lineNr());

}

.		{ 
			string error_message;
			error_message =  "Illegal character `" + matched();
			error_message += "' on line " + lineNr();
			
			CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, error_message, lineNr());
		}
