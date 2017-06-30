#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "program.hh"

/****************************** Class Ics_Opd *****************************/

Register_Descriptor * Ics_Opd::get_reg()
{
	//TODO
}

/****************************** Class Mem_Addr_Opd *****************************/

Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry se) 
{
	//TODO
	string name = se.get_variable_name();
	symbol_entry = new Symbol_Table_Entry(name, se.get_data_type(), se.get_lineno());
	if(name == "WeHere")
		symbol_entry->set_symbol_scope(se.get_symbol_scope());
	symbol_entry->set_start_offset(se.get_start_offset());
	symbol_entry->shachi = se.shachi;
}

Mem_Addr_Opd & Mem_Addr_Opd::operator=(const Mem_Addr_Opd & rhs)
{
	//TODO
	symbol_entry = rhs.symbol_entry;
	return *this;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer<<symbol_entry->get_variable_name();//get_register()->get_register();
	// cout<< "hello Register_Addr_Opd\n";
}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer) 
{
	Table_Scope symbol_scope = symbol_entry->get_symbol_scope();

	cout<<"Name is "<<symbol_entry->get_variable_name()<<endl;
	CHECK_INVARIANT(((symbol_scope == local) || (symbol_scope == global) || (symbol_scope==formal)), 
			"Wrong scope value");

	if(symbol_entry->shachi == 2)
	{
		file_buffer << symbol_entry->get_variable_name();
	}
	else if (symbol_scope == local)
	{
		int offset = symbol_entry->get_start_offset();
		if(symbol_entry->shachi == 1)
			offset +=8;
		// if(symbol_entry->get_variable_name() == "WeHere")
			//cout<<"offset received ....."<<offset<<endl;
		file_buffer << offset << "($fp)";
	}
	else if (symbol_scope == formal)
	{
		int offset = symbol_entry->get_start_offset();
		// if(symbol_entry->get_variable_name() == "WeHere")

		//cout<<"offset received ....."<<offset<<endl;
		file_buffer << offset << "($sp)";
	}
}

/****************************** Class Register_Addr_Opd *****************************/

Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * reg) 
{
	//TODO
	register_description=reg;
}

Register_Descriptor * Register_Addr_Opd::get_reg()    
{ 
	//TODO 
	return register_description;
}

Register_Addr_Opd& Register_Addr_Opd::operator=(const Register_Addr_Opd& rhs)
{
	//TODO
	register_description = rhs.register_description;
	return *this;
}

void Register_Addr_Opd::print_ics_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer<<register_description->get_name();
}

void Register_Addr_Opd::print_asm_opd(ostream & file_buffer) 
{
	file_buffer<<"$"<<register_description->get_name();
	//TODO
}

/****************************** Class Const_Opd *****************************/

template <class DATA_TYPE>
Const_Opd<DATA_TYPE>::Const_Opd(DATA_TYPE n) 
{
	//TODO
	num = n;
}

template <class DATA_TYPE>
Const_Opd<DATA_TYPE> & Const_Opd<DATA_TYPE>::operator=(const Const_Opd<DATA_TYPE> & rhs)
{
	//TODO
	num = rhs.num;
	return *this;
}

template <class DATA_TYPE>
void Const_Opd<DATA_TYPE>::print_ics_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer<<num;
}

template <class DATA_TYPE>
void Const_Opd<DATA_TYPE>::print_asm_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer<<to_string(num);
}

/****************************** Class Icode_Stmt *****************************/

Instruction_Descriptor & Icode_Stmt::get_op()
{ 
	return op_desc; 
}

Ics_Opd * Icode_Stmt::get_opd1()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd1 not implemented");
}

Ics_Opd * Icode_Stmt::get_opd2()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd2 not implemented");
}

Ics_Opd * Icode_Stmt::get_result()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Result not implemented");
}

void Icode_Stmt::set_opd1(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd1 not implemented");
}

void Icode_Stmt::set_opd2(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd2 not implemented");
}

void Icode_Stmt::set_result(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual methos set_Result not implemented");
}

/*************************** Class Move_IC_Stmt *****************************/

Move_IC_Stmt::Move_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * res)
{
	//cout<<"move_ic constructor "<<op<<endl;
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;   
	result = res; 
}

Ics_Opd * Move_IC_Stmt::get_opd1()          { return opd1; }
Ics_Opd * Move_IC_Stmt::get_result()        { return result; }

void Move_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Move_IC_Stmt::set_result(Ics_Opd * io) { result = io; }

Move_IC_Stmt& Move_IC_Stmt::operator=(const Move_IC_Stmt& rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	result = rhs.result; 

	return *this;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");

	string operation_name = op_desc.get_name();

	Icode_Format ic_format = op_desc.get_ic_format();

	switch (ic_format)
	{
	case i_r_op_o1: 
			file_buffer << "\t" << operation_name << ":    \t";
			result->print_ics_opd(file_buffer);
			file_buffer << " <- ";
			opd1->print_ics_opd(file_buffer);
			file_buffer << "\n";

			break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				"Intermediate code format not supported");
		break;
	}
}

void Move_IC_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");
	string op_name = op_desc.get_mnemonic();

	Assembly_Format assem_format = op_desc.get_assembly_format();
	switch (assem_format)
	{
	case a_op_r_o1: 
			file_buffer << "\t" << op_name << " ";
			result->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	case a_op_o1_r: 
			file_buffer << "\t" << op_name << " ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			result->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}
/******************************* Class Compute_IC_Stmt ****************************/

Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op inst_op, Ics_Opd * res, Ics_Opd * o1, Ics_Opd * o2)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[inst_op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
	opd1 = o1;
	opd2 = o2;   
	result = res;
}
Compute_IC_Stmt& Compute_IC_Stmt::operator=(const Compute_IC_Stmt& rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	opd2 = rhs.opd2;
	result = rhs.result; 

	return *this;
}

Instruction_Descriptor & Compute_IC_Stmt::get_inst_op_of_ics()
{
	return op_desc;
}

Ics_Opd * Compute_IC_Stmt::get_opd1()
{
	return opd1;
}
void Compute_IC_Stmt::set_opd1(Ics_Opd * io)
{
	opd1 = io;
}

Ics_Opd * Compute_IC_Stmt::get_opd2()
{
	return opd2;
}
void Compute_IC_Stmt::set_opd2(Ics_Opd * io)
{
	opd2 = io;
}

Ics_Opd * Compute_IC_Stmt::get_result()
{
	return result;
}
void Compute_IC_Stmt::set_result(Ics_Opd * io)
{
	result = io;
}

void Compute_IC_Stmt::print_icode(ostream & file_buffer)
{
	//CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a compute IC Stmt");
	//CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a compute IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a compute IC Stmt");

	string operation_name = op_desc.get_name();

	Icode_Format ic_format = op_desc.get_ic_format();

	switch (ic_format)
	{
	case i_r_o1_op_o2: 
			file_buffer << "\t" << operation_name << ":    \t";
			result->print_ics_opd(file_buffer);
			file_buffer << " <- ";
			opd1->print_ics_opd(file_buffer);
			file_buffer<<" , ";
			opd2->print_ics_opd(file_buffer);
			file_buffer << "\n";

			break; 
	case i_r_op_o1:
			file_buffer << "\t" << operation_name << ":    \t";
			result->print_ics_opd(file_buffer);
			file_buffer << " <- ";
			opd1->print_ics_opd(file_buffer);
			file_buffer << "\n";

			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				"Intermediate code format not supported");
		break;
	}
}
void Compute_IC_Stmt::print_assembly(ostream & file_buffer)
{
	//CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a compute IC Stmt");
	//CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a compute IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a compute IC Stmt");
	string op_name = op_desc.get_mnemonic();

	Assembly_Format assem_format = op_desc.get_assembly_format();
	switch (assem_format)
	{
	case a_op_r_o1_o2: 
			file_buffer << "\t" << op_name << " ";
			result->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd2->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	case a_op_o1_o2_r: 
			file_buffer << "\t" << op_name << " ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd2->print_asm_opd(file_buffer);
			file_buffer << ", ";
			result->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 
	case a_op_r_o1:
			file_buffer << "\t" << op_name << " ";
			result->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break;
	case a_op_o1_r:
			file_buffer << "\t" << op_name << " ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			result->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break;
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}
/******************************* Class Control_Flow_IC_Stmt ****************************/
Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * o2, string label)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;
	opd2 = o2;   
	offset = label;
}
	
Control_Flow_IC_Stmt& Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt& rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	opd2 = rhs.opd2;
	offset = rhs.offset; 

	return *this;	
}

Instruction_Descriptor & Control_Flow_IC_Stmt::get_inst_op_of_ics()
{
	return op_desc;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd1()
{
	return opd1;
}
void Control_Flow_IC_Stmt::set_opd1(Ics_Opd * io)
{
	opd1 = io;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd2()
{
	return opd2;
}
void Control_Flow_IC_Stmt::set_opd2(Ics_Opd * io)
{
	opd2 = io;
}
 	
string Control_Flow_IC_Stmt::get_Offset()
{
	return offset;
}
void Control_Flow_IC_Stmt::set_Offset(string label)
{
	offset = label;
}

void Control_Flow_IC_Stmt::print_icode(ostream & file_buffer)
{

	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a control flow IC Stmt");
	CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a control flow IC Stmt");
	//cout<<"begin control flow\n";
	string operation_name = op_desc.get_name();

	Icode_Format ic_format = op_desc.get_ic_format();

	switch (ic_format)
	{
	case i_op_o1_o2_st: 
			file_buffer << "\t" << operation_name << ":    \t";
			opd1->print_ics_opd(file_buffer);
			file_buffer<<" , ";
			opd2->print_ics_opd(file_buffer);
			file_buffer<<" : goto ";
			file_buffer<<offset; // check ?
			file_buffer << "\n";

			break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				"Intermediate code format not supported");
		break;
	}
	//cout<<"end control flow";
}
void Control_Flow_IC_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a control flow IC Stmt");
	CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a control flow IC Stmt");
	string op_name = op_desc.get_mnemonic();

	Assembly_Format assem_format = op_desc.get_assembly_format();
	switch (assem_format)
	{
	case a_op_o1_o2_st: 
			file_buffer << "\t" << op_name << " ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd2->print_asm_opd(file_buffer);
			file_buffer << ", ";
			file_buffer<<offset;
			file_buffer << " \n";

			break; 

	
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}

Label_IC_Stmt::Label_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, string offset)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[inst_op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
	this->opd1 = opd1; 
	this->offset = offset;

}

Label_IC_Stmt& Label_IC_Stmt::operator=(const Label_IC_Stmt& rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	offset = rhs.offset;
	return *this;
}

Instruction_Descriptor & Label_IC_Stmt::get_inst_op_of_ics()
{
	return op_desc;
}

Ics_Opd * Label_IC_Stmt::get_opd1()
{
	return opd1;
}
void Label_IC_Stmt::set_opd1(Ics_Opd * io)
{
	opd1 = io;
}

string Label_IC_Stmt::get_offset()
{
	return offset;
}
void Label_IC_Stmt::set_offset(string label)
{
	offset = label;
}

void Label_IC_Stmt::print_icode(ostream & file_buffer)
{
//	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a control flow IC Stmt");
	
	string operation_name = op_desc.get_name();

	Icode_Format ic_format = op_desc.get_ic_format();

	switch (ic_format)
	{
	case i_op_st:

			if(operation_name == "jump")
				file_buffer << "\t"  << "goto ";			
			else
				if(!(operation_name.size() == 0))
					file_buffer << "\t" << operation_name << ":    \t";
				else file_buffer<<"\n";
			if(opd1 != NULL)
				{
					opd1->print_ics_opd(file_buffer);
					file_buffer<<" , ";
				}

			if(operation_name.size()==0)
				file_buffer<<offset<<":    \t"; // check ?
			else file_buffer<<offset;
			file_buffer << "\n";

			break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				"Intermediate code format not supported");
		break;
	}	
}
void Label_IC_Stmt::print_assembly(ostream & file_buffer)
{
//	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a control flow IC Stmt");
	string op_name = op_desc.get_mnemonic();
	string operation_name = op_desc.get_name();
	Assembly_Format assem_format = op_desc.get_assembly_format();
	//cout<<"assem_format : "<<assem_format<<endl;
	//cout<<op_name<<endl;
	switch (assem_format)
	{
	case a_op_st: 
	///////////////// added following if statement for adding la
			if(op_name=="jal")
			{
				file_buffer<<"\t"<<op_name<<" ";
				file_buffer<<offset<<endl;
			}
			else if(op_name=="la")
			{
				file_buffer<<"\t"<<op_name<<" ";
				opd1->print_asm_opd(file_buffer);
				file_buffer<<", ";
				file_buffer<<offset<<endl;
			}
			else
			{
				if(!(operation_name.size()==0))
				{
					file_buffer << "\t" << op_name << " ";
				}
				else file_buffer<<"\n";
				if(opd1 != NULL)
					{
						opd1->print_asm_opd(file_buffer);
						file_buffer<<" , ";
					}
				if(op_name=="j")
					file_buffer<<offset;
				else
				file_buffer<<offset<<":    \t";
				file_buffer << "\n";
			}
			break; 

	case a_op_r_o1:
		if(op_name=="la")
			{
				file_buffer<<"\t"<<op_name<<" ";
				opd1->print_asm_opd(file_buffer);
				file_buffer<<", ";
				file_buffer<<offset<<endl;
			}
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}
/******************************* Class Code_For_Ast ****************************/

Code_For_Ast::Code_For_Ast()
{
	//TODO
	result_register=NULL;
}

Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg)
{
	//TODO
	ics_list=ic_l;
	result_register=reg;
}

void Code_For_Ast::append_ics(Icode_Stmt & ic_stmt)
{
	ics_list.push_back(&ic_stmt);
	//TODO
}

list<Icode_Stmt *> & Code_For_Ast::get_icode_list()  
{ 
	//TODO 
	return ics_list;
}

Register_Descriptor * Code_For_Ast::get_reg()
{
	//TODO
	return result_register;
}

void Code_For_Ast::set_reg(Register_Descriptor * reg)
{
	//TODO
	result_register = reg;
}

Code_For_Ast& Code_For_Ast::operator=(const Code_For_Ast& rhs)
{
	//TODO
	ics_list=rhs.ics_list;
	result_register=rhs.result_register;
	return *this;
}

/************************ class Instruction_Descriptor ********************************/

Tgt_Op Instruction_Descriptor::get_op()                   	{ return inst_op; }
string Instruction_Descriptor::get_name()                       { return name; }
string Instruction_Descriptor::get_mnemonic()                   { return mnemonic; }
string Instruction_Descriptor::get_ic_symbol()                  { return ic_symbol; }
Icode_Format Instruction_Descriptor::get_ic_format()            { return ic_format; }
Assembly_Format Instruction_Descriptor::get_assembly_format()   { return assem_format; }

Instruction_Descriptor::Instruction_Descriptor (Tgt_Op op, string temp_name, string temp_mnemonic, 
						string ic_sym, Icode_Format ic_form, Assembly_Format as_form)
{
	inst_op = op;
	name = temp_name; 
	mnemonic = temp_mnemonic;
	ic_symbol = ic_sym;
	ic_format = ic_form;
	assem_format = as_form;
}

Instruction_Descriptor::Instruction_Descriptor()
{
	inst_op = nop;
	name = "";
	mnemonic = "";
	ic_symbol = "";
	ic_format = i_nsy;
	assem_format = a_nsy;
}

template class Const_Opd<int>;
template class Const_Opd<double>;


void Function_Call_IC_Stmt::print_icode(ostream & file_buffer)
{

}

void Function_Call_IC_Stmt::print_assembly(ostream & file_buffer)
{

}

void Return_IC_Stmt::print_icode(ostream & file_buffer)
{

}

void Return_IC_Stmt::print_assembly(ostream & file_buffer)
{

}

