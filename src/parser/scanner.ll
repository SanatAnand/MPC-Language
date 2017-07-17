%filenames="scanner"
%lex-source="scanner.cc"

digit [0-9]
operator [\(\)\{\};\,\.]
alphabet [a-zA-Z]
arith [-+\?\:*/]
comparators[<>=]

%%
//ADD YOUR CODE HERE

protocol	{
	store_token_name("PROTOCOL");
	return Parser::PROTOCOL;
}

party	{
	store_token_name("PARTY");
	return Parser::PARTY;
}

"=>"	{
	store_token_name("SEND");
	return Parser::SEND;
}

field	{
	store_token_name("FIELD");
	return Parser::FIELD;
}

env	{
	store_token_name("ENVIRONMENT");
	return Parser::ENV;
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

Open 	{
	store_token_name("OPEN");
	return Parser::OPEN;
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
[ \t]		{
			if (command_options.is_show_tokens_selected())
				ignore_token();
		}

.		{ 
			string error_message;
			error_message =  "Illegal character `" + matched();
			error_message += "' on line " + lineNr();
			
			CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, error_message, lineNr());
		}
