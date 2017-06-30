#include <iostream>
#include <fstream>
#include <typeinfo>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "procedure.hh"
#include "program.hh"
//extern map<string,string> global_strings;
Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register)
{
	stringstream msg;
	msg << "No create_store_stmt() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_assembly()
{
	stringstream msg;
	msg << "No print_assembly() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_icode()
{
	stringstream msg;
	msg << "No print_icode() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Code_For_Ast & Assignment_Ast::compile()
{
	//cout<<"assignment begin";
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Assignment_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Assignment_Ast");

	Code_For_Ast & load_stmt = rhs->compile();

	Register_Descriptor * load_register = load_stmt.get_reg();
	CHECK_INVARIANT(load_register, "Load register cannot be null in Assignment_Ast");
	load_register->set_use_for_expr_result();

	Code_For_Ast store_stmt = lhs->create_store_stmt(load_register);

	CHECK_INVARIANT((load_register != NULL), "Load register cannot be null in Assignment_Ast");
	load_register->reset_use_for_expr_result();

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (load_stmt.get_icode_list().empty() == false)
		ic_list = load_stmt.get_icode_list();

	if (store_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, load_register);
	//cout<<"assignment end";
	return *assign_stmt;
}

Code_For_Ast & In_Ast::compile()
{
	//cout<<"assignment begin";
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Assignment_Ast");

	//WRITE CODE HERE
}

/////////////////////////////////////////////////////////////////

Code_For_Ast & Name_Ast::compile()
{
	//cout<<"into compiing Name_Ast";
	CHECK_INVARIANT((variable_symbol_entry != NULL), "Symbol table entry cannot be null in Assignment_Ast");
	Register_Descriptor* reg_desc;
	//cout<<node_data_type<<endl;
	Tgt_Op op;
	if(node_data_type==int_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<gp_data>();	
		op = load;
	} 
	else if(node_data_type==double_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<float_reg>();
		op = load_d;
		//cout<<"got float register";
	}

	// if(variable_symbol_entry->get_variable_name() == "number")
	// 	cout<<"Scope is: "<<variable_symbol_entry->get_symbol_scope()<<endl;
	Register_Addr_Opd* reg_opd = new Register_Addr_Opd(reg_desc);
	variable_symbol_entry->set_shachi(shachi);
	Mem_Addr_Opd* mem_addr = new Mem_Addr_Opd(*variable_symbol_entry);


	Move_IC_Stmt* ic_stmt = new Move_IC_Stmt(op,mem_addr,reg_opd);
	list<Icode_Stmt *> ics_list=*new list<Icode_Stmt *>;
	ics_list.push_back(ic_stmt);
	
	Code_For_Ast* name = new Code_For_Ast(ics_list, reg_desc);

	return *name;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
	//cout<<"into Name_Ast";
	CHECK_INVARIANT((variable_symbol_entry != NULL), "Symbol table entry cannot be null in Assignment_Ast");
	
	Register_Addr_Opd* reg_opd = new Register_Addr_Opd(store_register);
	Mem_Addr_Opd* mem_addr = new Mem_Addr_Opd(*variable_symbol_entry);
	Tgt_Op op;
	if(node_data_type==int_data_type)
	{
		op=store;
	}
	else if(node_data_type==double_data_type)
	{
		op = store_d;
	}
	Move_IC_Stmt* ic_stmt = new Move_IC_Stmt(op,reg_opd,mem_addr);
	//if (command_options.is_do_lra_selected() == false)
	variable_symbol_entry->free_register(store_register);

	// else
	// {
	// 	variable_symbol_entry->update_register(store_register);
	// 	store_register->reset_use_for_expr_result();
	// }
	
	list<Icode_Stmt *> ics_list=*new list<Icode_Stmt *>;
	ics_list.push_back(ic_stmt);
	
	Code_For_Ast* name = new Code_For_Ast(ics_list, store_register);
	//cout<<"end Name_Ast";
	return *name;
}


///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile()
{
	//cout<<"into Number_Ast";
	Register_Descriptor* reg_desc;
	Tgt_Op op;
	if(node_data_type==int_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<gp_data>();
		op=imm_load;
	}
	else
	{
		reg_desc= machine_desc_object.get_new_register<float_reg>();
		op = imm_load_d;
	}
	 
	Register_Addr_Opd* reg_opd = new Register_Addr_Opd(reg_desc);
	Const_Opd <DATA_TYPE>* const_opd = new Const_Opd<DATA_TYPE>(constant);

	
	Move_IC_Stmt* ic_stmt = new Move_IC_Stmt(op,const_opd,reg_opd);
	list<Icode_Stmt *> ics_list = *new list<Icode_Stmt *>;
	ics_list.push_back(ic_stmt);
	
	Code_For_Ast* number = new Code_For_Ast(ics_list, reg_desc);
	//cout<<"end Number_Ast";
	return *number;	
}

///////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Relational_Expr_Ast::compile()
{
	CHECK_INVARIANT((lhs_condition != NULL), "Lhs condition cannot be null in Relation_Expr_Ast");
	CHECK_INVARIANT((rhs_condition != NULL), "Rhs condition cannot be null in Relation_Expr_Ast");

	Code_For_Ast lhs_stmt = lhs_condition->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	Register_Addr_Opd* lhs_opd = new Register_Addr_Opd(lhs_register);
	CHECK_INVARIANT((lhs_register != NULL), "Lhs register cannot be null in Relation_Expr_Ast");
	
	Code_For_Ast & rhs_stmt = rhs_condition->compile();

	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	Register_Addr_Opd* rhs_opd = new Register_Addr_Opd(rhs_register);
	CHECK_INVARIANT(rhs_register, "Rhs register cannot be null in Relation_Expr_Ast");

	// Store the statement in ic_list
	Register_Descriptor* reg_desc = machine_desc_object.get_new_register<gp_data>();
	Register_Addr_Opd* reg_opd = new Register_Addr_Opd(reg_desc);
	

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	Code_For_Ast * rel_stmt;
	Compute_IC_Stmt* rel_ic;
	Tgt_Op op;
	if(rel_op == less_equalto)
	{
		op = sle;
		rel_ic = new Compute_IC_Stmt(op,reg_opd,lhs_opd,rhs_opd);	
	} 
	else if(rel_op == less_than)
	{
		op = slt;
		rel_ic = new Compute_IC_Stmt(op,reg_opd,lhs_opd,rhs_opd);	
	}
	else if(rel_op == greater_than)
	{
		op = sgt;
		rel_ic = new Compute_IC_Stmt(op,reg_opd,lhs_opd,rhs_opd);	
	}
	else if(rel_op == greater_equalto)
	{
		op = sge;
		rel_ic = new Compute_IC_Stmt(op,reg_opd,lhs_opd,rhs_opd);	
	}
	else if(rel_op == equalto)
	{
		op = seq;
		rel_ic = new Compute_IC_Stmt(op,reg_opd,lhs_opd,rhs_opd);	
	}
	else if(rel_op == not_equalto)
	{
		op = sne;
		rel_ic = new Compute_IC_Stmt(op,reg_opd,lhs_opd,rhs_opd);	
	}
	ic_list.push_back(rel_ic);
	if (ic_list.empty() == false)
		rel_stmt = new Code_For_Ast(ic_list,reg_desc);
	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();

	return *rel_stmt;	
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Boolean_Expr_Ast::compile()
{
	if(bool_op != boolean_not)
	{
		CHECK_INVARIANT((lhs_op != NULL), "Lhs op cannot be null in Boolean_Expr_Ast");
		CHECK_INVARIANT((rhs_op != NULL), "Rhs op cannot be null in Boolean_Expr_Ast");

		Code_For_Ast lhs_stmt;
		Register_Addr_Opd* lhs_opd;
		Register_Descriptor * lhs_register;

		
		lhs_stmt = lhs_op->compile();
		lhs_register = lhs_stmt.get_reg();
		lhs_opd = new Register_Addr_Opd(lhs_register);
		CHECK_INVARIANT((lhs_register != NULL), "Lhs register cannot be null in Boolean_Expr_Ast");	
		
		Code_For_Ast & rhs_stmt = rhs_op->compile();

		Register_Descriptor * rhs_register = rhs_stmt.get_reg();
		Register_Addr_Opd* rhs_opd = new Register_Addr_Opd(rhs_register);
		CHECK_INVARIANT(rhs_register, "Rhs register cannot be null in Boolean_Expr_Ast");

		// Store the statement in ic_list
		Register_Descriptor* reg_desc = machine_desc_object.get_new_register<gp_data>();
		Register_Addr_Opd* reg_opd = new Register_Addr_Opd(reg_desc);
		

		// Store the statement in ic_list

		list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

		if (lhs_stmt.get_icode_list().empty() == false)
			ic_list = lhs_stmt.get_icode_list();
		if (rhs_stmt.get_icode_list().empty() == false)
			ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

		Compute_IC_Stmt * boolean_ic;
		Tgt_Op op;
		if(bool_op == boolean_not)
		{
			op = not_t;
			boolean_ic = new Compute_IC_Stmt(op,reg_opd,NULL,rhs_opd);	
		} 
		else if(bool_op == boolean_or)
		{
			op = or_t;
			boolean_ic = new Compute_IC_Stmt(op,reg_opd,lhs_opd,rhs_opd);	
		}
		else if(bool_op == boolean_and)
		{
			op = and_t;
			boolean_ic = new Compute_IC_Stmt(op,reg_opd,lhs_opd,rhs_opd);	
		}
		Code_For_Ast * boolean_stmt;
		ic_list.push_back(boolean_ic);
		
		if (ic_list.empty() == false)
			boolean_stmt = new Code_For_Ast(ic_list,reg_desc);
		
		if(bool_op != boolean_not)
			lhs_register->reset_use_for_expr_result();
		rhs_register->reset_use_for_expr_result();

		return *boolean_stmt;
	}
	else
	{
		CHECK_INVARIANT((rhs_op != NULL), "Rhs op cannot be null in Boolean_Expr_Ast");

		Register_Descriptor* reg_desc1 = machine_desc_object.get_new_register<gp_data>();
		Register_Addr_Opd* reg_opd1 = new Register_Addr_Opd(reg_desc1);
		Const_Opd <int>* const_opd1 = new Const_Opd<int>(1);
		Tgt_Op op1 = imm_load;
		Move_IC_Stmt* ic_stmt1 = new Move_IC_Stmt(op1,const_opd1,reg_opd1);

		Code_For_Ast & rhs_stmt = rhs_op->compile();

		Register_Descriptor * rhs_register = rhs_stmt.get_reg();
		Register_Addr_Opd* rhs_opd = new Register_Addr_Opd(rhs_register);
		CHECK_INVARIANT(rhs_register, "Rhs register cannot be null in Boolean_Expr_Ast");

		// Store the statement in ic_list
		Register_Descriptor* reg_desc = machine_desc_object.get_new_register<gp_data>();
		Register_Addr_Opd* reg_opd = new Register_Addr_Opd(reg_desc);
		

		// Store the statement in ic_list

		list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

		ic_list.push_back(ic_stmt1);

		if (rhs_stmt.get_icode_list().empty() == false)
			ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

		Compute_IC_Stmt * boolean_ic;
		Tgt_Op op;
		if(bool_op == boolean_not)
		{
			op = not_t;
			boolean_ic = new Compute_IC_Stmt(op,reg_opd,rhs_opd,reg_opd1);	
		} 
		Code_For_Ast * boolean_stmt;
		ic_list.push_back(boolean_ic);
		
		if (ic_list.empty() == false)
			boolean_stmt = new Code_For_Ast(ic_list,reg_desc);
		
		rhs_register->reset_use_for_expr_result();
		reg_desc1->reset_use_for_expr_result(); // interestingly worked !
		return *boolean_stmt;
	}
}
///////////////////////////////////////////////////////////////////////

Code_For_Ast & Selection_Statement_Ast::compile()
{
	CHECK_INVARIANT((then_part != NULL), "Then part cannot be null in Selection_Statement_Ast");
	CHECK_INVARIANT((else_part != NULL), "Else part cannot be null in Selection_Statement_Ast");
	CHECK_INVARIANT((cond != NULL), "Cond cannot be null in Selection_Statement_Ast");
	
	
	
	Code_For_Ast cond_stmt = cond->compile();
	Register_Descriptor * cond_register = cond_stmt.get_reg();
	Register_Addr_Opd* cond_opd = new Register_Addr_Opd(cond_register);
	CHECK_INVARIANT((cond_register != NULL), "Cond register cannot be null in Selection_Statement_Ast");
	



	Code_For_Ast lhs_stmt = then_part->compile();

	// Store the statement in ic_list
	


	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (cond_stmt.get_icode_list().empty() == false)
		ic_list = cond_stmt.get_icode_list();
	Register_Descriptor* reg = new Register_Descriptor(zero,"zero",int_num,gp_data); 
	Register_Addr_Opd* reg_opd = new Register_Addr_Opd(reg);
	Tgt_Op op = beq;
	Code_For_Ast & rhs_stmt = else_part->compile();
	string label0 = get_new_label();
	string label1 = get_new_label();
	Control_Flow_IC_Stmt* beq_stmt = new Control_Flow_IC_Stmt(op,cond_opd,reg_opd,label0); 
	

	

	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	Register_Addr_Opd* lhs_opd = new Register_Addr_Opd(lhs_register);
	CHECK_INVARIANT((lhs_register != NULL), "Lhs register cannot be null in Selection_Statement_Ast");




	ic_list.push_back(beq_stmt);
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), lhs_stmt.get_icode_list());

	

	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	Register_Addr_Opd* rhs_opd = new Register_Addr_Opd(rhs_register);
	CHECK_INVARIANT(rhs_register, "Rhs register cannot be null in Selection_Statement_Ast");

	
	Label_IC_Stmt* label_stmt1 = new Label_IC_Stmt(j,NULL,label1);
	ic_list.push_back(label_stmt1);
	Label_IC_Stmt* label_stmt2 = new Label_IC_Stmt(label,NULL,label0);
	ic_list.push_back(label_stmt2);
	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());
	Label_IC_Stmt* label_stmt3 = new Label_IC_Stmt(label,NULL,label1);
	ic_list.push_back(label_stmt3);
	
	Code_For_Ast * selection_stmt;
	
	if (ic_list.empty() == false)
		selection_stmt = new Code_For_Ast(ic_list,cond_stmt.get_reg());
	
	cond_register->reset_use_for_expr_result();

	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();

	return *selection_stmt;	
}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Iteration_Statement_Ast::compile()
{
	CHECK_INVARIANT((body != NULL), "Body cannot be null in Iteration_Statement_Ast");
	CHECK_INVARIANT((cond != NULL), "Cond cannot be null in Iteration_Statement_Ast");
	
	
	if(is_do_form==false)
	{
		Code_For_Ast cond_stmt = cond->compile();
		Register_Descriptor * cond_register = cond_stmt.get_reg();
		Register_Addr_Opd* cond_opd = new Register_Addr_Opd(cond_register);
		CHECK_INVARIANT((cond_register != NULL), "Cond register cannot be null in Iteration_Statement_Ast");
		
		string label0 = get_new_label();
		string label1 = get_new_label();


		Code_For_Ast body_stmt = body->compile();

		// Store the statement in ic_list
		

		// Store the statement in ic_list

		list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
		Label_IC_Stmt* label_stmt1 = new Label_IC_Stmt(j,NULL,label1);
		ic_list.push_back(label_stmt1);

		Label_IC_Stmt* label_stmt2 = new Label_IC_Stmt(label,NULL,label0);
		ic_list.push_back(label_stmt2);

		if (body_stmt.get_icode_list().empty() == false)
			ic_list.splice(ic_list.end(), body_stmt.get_icode_list());

		Label_IC_Stmt* label_stmt3 = new Label_IC_Stmt(label,NULL,label1);
		ic_list.push_back(label_stmt3);

		if (cond_stmt.get_icode_list().empty() == false)
			ic_list.splice(ic_list.end(), cond_stmt.get_icode_list());

		Register_Descriptor* reg = new Register_Descriptor(zero,"zero",int_num,gp_data); 

		Register_Addr_Opd* reg_opd = new Register_Addr_Opd(reg);
		Tgt_Op op = bne;
		Control_Flow_IC_Stmt* bne_stmt = new Control_Flow_IC_Stmt(op,cond_opd,reg_opd,label0); 
		ic_list.push_back(bne_stmt);

		
		Code_For_Ast * iteration_stmt;
		
		if (ic_list.empty() == false)
			iteration_stmt = new Code_For_Ast(ic_list,cond_stmt.get_reg());
		
		cond_register->reset_use_for_expr_result();


		return *iteration_stmt;	
	}
	else
	{
		Code_For_Ast cond_stmt = cond->compile();
		Register_Descriptor * cond_register = cond_stmt.get_reg();
		Register_Addr_Opd* cond_opd = new Register_Addr_Opd(cond_register);
		CHECK_INVARIANT((cond_register != NULL), "Cond register cannot be null in Iteration_Statement_Ast");
		
		string label0 = get_new_label();
		string label1 = get_new_label();

		Code_For_Ast body_stmt = body->compile();

		// Store the statement in ic_list
		


		// Store the statement in ic_list

		list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
		

		Label_IC_Stmt* label_stmt2 = new Label_IC_Stmt(label,NULL,label0);
		ic_list.push_back(label_stmt2);


		if (body_stmt.get_icode_list().empty() == false)
			ic_list.splice(ic_list.end(), body_stmt.get_icode_list());
		
		Label_IC_Stmt* label_stmt3 = new Label_IC_Stmt(label,NULL,label1);
		ic_list.push_back(label_stmt3);

		if (cond_stmt.get_icode_list().empty() == false)
			ic_list.splice(ic_list.end(), cond_stmt.get_icode_list());
		

		Register_Descriptor* reg = new Register_Descriptor(zero,"zero",int_num,gp_data); 

		Register_Addr_Opd* reg_opd = new Register_Addr_Opd(reg);
		Tgt_Op op = bne;
		Control_Flow_IC_Stmt* bne_stmt = new Control_Flow_IC_Stmt(op,cond_opd,reg_opd,label0); 
		ic_list.push_back(bne_stmt);

		
		Code_For_Ast * iteration_stmt;
		
		if (ic_list.empty() == false)
			iteration_stmt = new Code_For_Ast(ic_list,cond_stmt.get_reg());
		
		cond_register->reset_use_for_expr_result();


		return *iteration_stmt;	
	}
	
	
}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Plus_Ast::compile()
{
	//cout<<"Plus_Ast begin";
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Plus_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Plus_Ast");


	Code_For_Ast lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	Register_Addr_Opd* lhs_opd = new Register_Addr_Opd(lhs_register);
	CHECK_INVARIANT((lhs_register != NULL), "Lhs register cannot be null in Plus_Ast");
	
	Code_For_Ast & rhs_stmt = rhs->compile();

	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	Register_Addr_Opd* rhs_opd = new Register_Addr_Opd(rhs_register);
	CHECK_INVARIANT(rhs_register, "Rhs register cannot be null in Plus_Ast");

	// Store the statement in ic_list
	Register_Descriptor* reg_desc; 
	Register_Addr_Opd* reg_opd; 
	
	

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	Compute_IC_Stmt* add_ic;
	if(node_data_type == int_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<gp_data>();
		reg_opd = new Register_Addr_Opd(reg_desc);
		add_ic = new Compute_IC_Stmt(add,reg_opd,lhs_opd,rhs_opd);	
	} 
	else if(node_data_type == double_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<float_reg>();
		reg_opd =new Register_Addr_Opd(reg_desc);
		add_ic = new Compute_IC_Stmt(add_d,reg_opd,lhs_opd,rhs_opd);	
	}
	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();
	Code_For_Ast * add_stmt;
	ic_list.push_back(add_ic);
	if (ic_list.empty() == false)
		add_stmt = new Code_For_Ast(ic_list,reg_desc);
	//cout<<"Plus_Ast end";
	return *add_stmt;	
}

/////////////////////////////////////////////////////////////////

Code_For_Ast & Minus_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Minus_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Minus_Ast");

	Code_For_Ast lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	Register_Addr_Opd* lhs_opd = new Register_Addr_Opd(lhs_register);
	CHECK_INVARIANT((lhs_register != NULL), "Lhs register cannot be null in Minus_Ast");
	
	Code_For_Ast & rhs_stmt = rhs->compile();

	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	Register_Addr_Opd* rhs_opd = new Register_Addr_Opd(rhs_register);
	CHECK_INVARIANT(rhs_register, "Rhs register cannot be null in Minus_Ast");

	// Store the statement in ic_list
	Register_Descriptor* reg_desc;
	Register_Addr_Opd* reg_opd;
	

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	Compute_IC_Stmt * sub_ic;
	if(node_data_type == int_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<gp_data>();
		reg_opd =new Register_Addr_Opd(reg_desc);
		sub_ic = new Compute_IC_Stmt(sub,reg_opd,lhs_opd,rhs_opd);	
	} 
	else if(node_data_type == double_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<float_reg>();
		reg_opd =new Register_Addr_Opd(reg_desc);
		sub_ic = new Compute_IC_Stmt(sub_d,reg_opd,lhs_opd,rhs_opd);	
	}
	Code_For_Ast * sub_stmt;
	ic_list.push_back(sub_ic);
	if (ic_list.empty() == false)
		sub_stmt = new Code_For_Ast(ic_list,reg_desc);
	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();

	return *sub_stmt;	
}

//////////////////////////////////////////////////////////////////

Code_For_Ast & Mult_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Mult_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Mult_Ast");

	Code_For_Ast lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	Register_Addr_Opd* lhs_opd = new Register_Addr_Opd(lhs_register);
	CHECK_INVARIANT((lhs_register != NULL), "Lhs register cannot be null in Mult_Ast");
	
	Code_For_Ast & rhs_stmt = rhs->compile();

	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	Register_Addr_Opd* rhs_opd = new Register_Addr_Opd(rhs_register);
	CHECK_INVARIANT(rhs_register, "Rhs register cannot be null in Mult_Ast");

	// Store the statement in ic_list
	Register_Descriptor* reg_desc;
	Register_Addr_Opd* reg_opd ;
	

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	Compute_IC_Stmt * mult_ic;
	if(node_data_type == int_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<gp_data>();
		reg_opd = new Register_Addr_Opd(reg_desc);
		mult_ic = new Compute_IC_Stmt(mult,reg_opd,lhs_opd,rhs_opd);	
	} 
	else if(node_data_type == double_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<float_reg>();
		reg_opd = new Register_Addr_Opd(reg_desc);
		mult_ic = new Compute_IC_Stmt(mult_d,reg_opd,lhs_opd,rhs_opd);	
	}
	Code_For_Ast * mult_stmt;
	ic_list.push_back(mult_ic);
	if (ic_list.empty() == false)
		mult_stmt = new Code_For_Ast(ic_list,reg_desc);
	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();

	return *mult_stmt;	
	
}

////////////////////////////////////////////////////////////////////

Code_For_Ast & Conditional_Operator_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Conditional_Operator_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Conditional_Operator_Ast");
	CHECK_INVARIANT((cond != NULL), "Cond cannot be null in Conditional_Operator_Ast");
	

	
	Code_For_Ast cond_stmt = cond->compile();

	Register_Descriptor * cond_register = cond_stmt.get_reg();
	Register_Addr_Opd* cond_opd = new Register_Addr_Opd(cond_register);
	CHECK_INVARIANT((cond_register != NULL), "Cond register cannot be null in Conditional_Operator_Ast");
	



	Code_For_Ast lhs_stmt = lhs->compile();

	
	// Store the statement in ic_list
	

	
	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (cond_stmt.get_icode_list().empty() == false)
		ic_list = cond_stmt.get_icode_list();
	Register_Descriptor* reg = new Register_Descriptor(zero,"zero",int_num,gp_data); 
	Register_Addr_Opd* reg_opd = new Register_Addr_Opd(reg);
	Tgt_Op op = beq;
	Code_For_Ast & rhs_stmt = rhs->compile();
	string label0 = get_new_label();
	string label1 = get_new_label();
	Control_Flow_IC_Stmt* beq_stmt = new Control_Flow_IC_Stmt(op,cond_opd,reg_opd,label0); 
	

	

	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	Register_Addr_Opd* lhs_opd = new Register_Addr_Opd(lhs_register);
	CHECK_INVARIANT((lhs_register != NULL), "Lhs register cannot be null in Conditional_Operator_Ast");


	

	ic_list.push_back(beq_stmt);
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), lhs_stmt.get_icode_list());

	
	

	

	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	Register_Addr_Opd* rhs_opd = new Register_Addr_Opd(rhs_register);
	CHECK_INVARIANT(rhs_register, "Rhs register cannot be null in Conditional_Operator_Ast");

	Register_Descriptor* reg_desc;
	Register_Addr_Opd* reg_opd1;
	
	op = or_t;
	Compute_IC_Stmt* boolean_or1;
	if(node_data_type==int_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<gp_data>();
		reg_opd1 = new Register_Addr_Opd(reg_desc);
		boolean_or1= new Compute_IC_Stmt(op,reg_opd1,lhs_opd,reg_opd);
	} 
	else if(node_data_type==double_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<float_reg>();
		reg_opd1 = new Register_Addr_Opd(reg_desc);
		boolean_or1= new Compute_IC_Stmt(op,reg_opd1,lhs_opd,reg_opd);
	}
	ic_list.push_back(boolean_or1);	

	Label_IC_Stmt* label_stmt1 = new Label_IC_Stmt(j,NULL,label1);
	ic_list.push_back(label_stmt1);
	Label_IC_Stmt* label_stmt2 = new Label_IC_Stmt(label,NULL,label0);
	ic_list.push_back(label_stmt2);
	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	Compute_IC_Stmt* boolean_or2 = new Compute_IC_Stmt(op,reg_opd1,rhs_opd,reg_opd);
	
	
	ic_list.push_back(boolean_or2);	
	Label_IC_Stmt* label_stmt3 = new Label_IC_Stmt(label,NULL,label1);
	ic_list.push_back(label_stmt3);
	
	Code_For_Ast * selection_stmt;
	
	if (ic_list.empty() == false)
		selection_stmt = new Code_For_Ast(ic_list,reg_desc);
	
	cond_register->reset_use_for_expr_result();

	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();

	return *selection_stmt;
}


////////////////////////////////////////////////////////////////////

Code_For_Ast & Divide_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Divide_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Divide_Ast");

	Code_For_Ast lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	Register_Addr_Opd* lhs_opd = new Register_Addr_Opd(lhs_register);
	CHECK_INVARIANT((lhs_register != NULL), "Lhs register cannot be null in Divide_Ast");
	
	Code_For_Ast & rhs_stmt = rhs->compile();

	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	Register_Addr_Opd* rhs_opd = new Register_Addr_Opd(rhs_register);
	CHECK_INVARIANT(rhs_register, "Rhs register cannot be null in Divide_Ast");

	// Store the statement in ic_list
	Register_Descriptor* reg_desc;
	Register_Addr_Opd* reg_opd;
	
	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	Compute_IC_Stmt * div_ic;
	if(node_data_type == int_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<gp_data>();
		reg_opd = new Register_Addr_Opd(reg_desc);
		div_ic = new Compute_IC_Stmt(divd,reg_opd,lhs_opd,rhs_opd);	
	} 
	else if(node_data_type == double_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<float_reg>();
		reg_opd = new Register_Addr_Opd(reg_desc);
		div_ic = new Compute_IC_Stmt(div_d,reg_opd,lhs_opd,rhs_opd);	
	}
	Code_For_Ast * div_stmt;
	ic_list.push_back(div_ic);
	if (ic_list.empty() == false)
		div_stmt = new Code_For_Ast(ic_list,reg_desc);
	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();

	return *div_stmt;	

}


//////////////////////////////////////////////////////////////////////

Code_For_Ast & UMinus_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in UMinus_Ast");
	
	Code_For_Ast lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	Register_Addr_Opd* lhs_opd = new Register_Addr_Opd(lhs_register);
	CHECK_INVARIANT((lhs_register != NULL), "Lhs register cannot be null in UMinus_Ast");
	
	
	// Store the statement in ic_list
	Register_Descriptor* reg_desc;
	Register_Addr_Opd* reg_opd;
	

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	Compute_IC_Stmt * umin_ic;
	if(node_data_type == int_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<gp_data>();
		reg_opd = new Register_Addr_Opd(reg_desc);
		
		umin_ic = new Compute_IC_Stmt(uminus,reg_opd,lhs_opd,NULL);	
	} 
	else if(node_data_type == double_data_type)
	{
		reg_desc= machine_desc_object.get_new_register<float_reg>();
		reg_opd = new Register_Addr_Opd(reg_desc);
		
		umin_ic = new Compute_IC_Stmt(uminus_d,reg_opd,lhs_opd,NULL);	
	}
	Code_For_Ast * umin_stmt;
	ic_list.push_back(umin_ic);
	if (ic_list.empty() == false)
		umin_stmt = new Code_For_Ast(ic_list,reg_desc);
	lhs_register->reset_use_for_expr_result();


	return *umin_stmt;	

}

/////////////////////////////////////////////////////////////////////////////
//updated
Code_For_Ast & Return_Ast::compile()
{
	//YOUR CODE HERE
	Code_For_Ast* ret_ast;
	list<Icode_Stmt*> & ic_list=*new list<Icode_Stmt*>();
	if(is_void)
	{
		Label_IC_Stmt* fn_ic = new Label_IC_Stmt(j,NULL,"epilogue_"+function_name);
		ic_list.push_back(fn_ic);
		ret_ast = new Code_For_Ast(ic_list,NULL);
		ra_icode_list.assign(ic_list.begin(),ic_list.end());
		
		return *ret_ast;
	}
	Code_For_Ast val_code = val->compile();
	ic_list = val_code.get_icode_list();
	//Return_IC_Stmt* ret_ic = new Return_IC_Stmt();
	//ic_list.push_back(ret_ic); 
	// need to check what other icode statements are to be pushed.. e.g. loading result in register v0?
	
	// result register in Code_For_Ast could stay empty?
	// if(ret_type==int_data_type || ret_type == void_data_type)
	// {
	// 	ret_ast = new Code_For_Ast(ic_list,NULL);

	// }
	// else if(ret_type == double_data_type)
	// {
	// 	ret_ast = new Code_For_Ast(ic_list,NULL);
	// }
	if(ret_type==int_data_type)
	{
		Register_Descriptor* reg_v1 = new Register_Descriptor(v1,"v1",int_num,gp_data); // check !
		Register_Addr_Opd* reg_v1_opd = new Register_Addr_Opd(reg_v1);
		Move_IC_Stmt* m_ic = new Move_IC_Stmt(mov,new Register_Addr_Opd(val_code.get_reg()),reg_v1_opd);
		ic_list.push_back(m_ic);
		// cout<<ic_list.size()<<endl;

		Label_IC_Stmt* fn_ic = new Label_IC_Stmt(j,NULL,"epilogue_"+function_name);
		ic_list.push_back(fn_ic);
		// cout<<ic_list.size()<<endl;

		ret_ast = new Code_For_Ast(ic_list,NULL);
	}
	else if(ret_type==double_data_type)
	{
		Register_Descriptor* reg_v1 = new Register_Descriptor(f0,"f0",int_num,float_reg); // check !
		Register_Addr_Opd* reg_v1_opd = new Register_Addr_Opd(reg_v1);
		Move_IC_Stmt* m_ic = new Move_IC_Stmt(move_d,new Register_Addr_Opd(val_code.get_reg()),reg_v1_opd);
		ic_list.push_back(m_ic);
		// cout<<ic_list.size()<<endl;
		Label_IC_Stmt* fn_ic = new Label_IC_Stmt(j,NULL,"epilogue_"+function_name);
		ic_list.push_back(fn_ic);
		// cout<<ic_list.size()<<endl;
		ret_ast = new Code_For_Ast(ic_list,NULL);
	}
	ra_icode_list.assign(ic_list.begin(),ic_list.end());

	return *ret_ast;

	// add jump to epilogue
}

Code_For_Ast & Function_call_Ast::compile()
{
	list<Ast*>::iterator it = param_list->begin();
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	list<Icode_Stmt*> sw_list;
	int curr_offset = 0;
	Tgt_Op op;
	Symbol_Table_Entry se;
	for(;it!=param_list->end();it++)
	{

		Code_For_Ast curr = (*it)->compile();
		if (curr.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), curr.get_icode_list());
		se.set_symbol_scope(formal);
		se.set_start_offset(curr_offset);
		se.set_var_name("WeHere");
		curr_offset-=4;
		Mem_Addr_Opd* md= new Mem_Addr_Opd(se);
		if((*it)->get_data_type()==double_data_type)
			op = store_d;
		else if((*it)->get_data_type()==int_data_type)
			op = store;
		//cout<<"Offset is: "<<(se).get_start_offset()<<endl;
		Move_IC_Stmt* m_ic=new Move_IC_Stmt(op,new Register_Addr_Opd(curr.get_reg()),md); 
		//cout<<"Scope is: "<<(m_ic->get_result()->get_symbol_entry()).get_symbol_scope()<<endl;
		ic_list.push_back(m_ic);
		machine_desc_object.clear_local_register_mappings();

		//Register_Addr_Opd* rd= new Register_Addr_Opd(curr.get_reg());
		//new Move_IC_Stmt(sw,rd,)
		// need to store the results of each parameter in particular register or push to stack
	}
	Register_Descriptor* reg_sp = new Register_Descriptor(sp,"sp",int_num,gp_data); // check !
	Register_Addr_Opd* reg_opd=new Register_Addr_Opd(reg_sp);
	Const_Opd<int>* cons_opd = new Const_Opd<int>(4*(param_list->size()));
	Compute_IC_Stmt* c_ic = new Compute_IC_Stmt(sub,reg_opd,reg_opd,cons_opd);
	ic_list.push_back(c_ic);
	Label_IC_Stmt* fn_ic = new Label_IC_Stmt(jal,NULL,proc->get_proc_name());
	//Function_Call_IC_Stmt* fn_ic = new Function_Call_IC_Stmt(proc->get_proc_name());
	ic_list.push_back(fn_ic);
	c_ic = new Compute_IC_Stmt(add,reg_opd,reg_opd,cons_opd);
	ic_list.push_back(c_ic);
	if(get_data_type()==int_data_type)
	{
		Register_Descriptor* reg_v0 = new Register_Descriptor(v0,"v0",int_num,gp_data); // check !
		Register_Descriptor* reg_v1 = new Register_Descriptor(v1,"v1",int_num,gp_data); // check !
		Register_Addr_Opd* reg_v0_opd = new Register_Addr_Opd(reg_v0);
		Register_Addr_Opd* reg_v1_opd = new Register_Addr_Opd(reg_v1);
		Move_IC_Stmt* m_ic = new Move_IC_Stmt(mov,reg_v1_opd,reg_v0_opd);
		ic_list.push_back(m_ic);

	}
	else if(get_data_type()==double_data_type)
	{
		Register_Descriptor* reg_v0 = new Register_Descriptor(v0,"f2",int_num,gp_data); // check !
		Register_Descriptor* reg_v1 = new Register_Descriptor(v1,"f0",int_num,gp_data); // check !
		Register_Addr_Opd* reg_v0_opd = new Register_Addr_Opd(reg_v0);
		Register_Addr_Opd* reg_v1_opd = new Register_Addr_Opd(reg_v1);
		Move_IC_Stmt* m_ic = new Move_IC_Stmt(move_d,reg_v1_opd,reg_v0_opd);
		ic_list.push_back(m_ic);

	}
	
	// do we need a result register for this Code_For_Ast ?
	Code_For_Ast* fn_ast;
	if(node_data_type==int_data_type)
	{
		Register_Descriptor* reg_desc= machine_desc_object.get_new_register<gp_data>();
		fn_ast =new Code_For_Ast(ic_list,reg_desc);
	}
	else if(node_data_type==double_data_type)
	{
		Register_Descriptor* reg_desc= machine_desc_object.get_new_register<float_reg>();
		fn_ast =new Code_For_Ast(ic_list,reg_desc);

	}
	else if(node_data_type==void_data_type)
	{
		fn_ast =new Code_For_Ast(ic_list,NULL);
	}
	machine_desc_object.clear_local_register_mappings();

	return *fn_ast;
	// add jal and the move statements succeeding function call
	// need to add a result register too
	//YOUR CODE HERE
	/////////////////////////////
	// list<Ast*>::iterator it = param_list->begin();
	// list<Icode_Stmt*> ic_list;
	// list<Icode_Stmt*> sw_list;
	// Symbol_Table_Entry* se = new Symbol_Table_Entry(); 
	// int curr_offset = 0;
	// for(;it!=param_list->end();it++)
	// {
	// 	se->set_symbol_scope(formal);
	// 	se->set_start_offset(curr_offset);
	// 	curr_offset-=4;
	// 	Mem_Addr_Opd* md= new Mem_Addr_Opd(*se);
	// 	Move_IC_Stmt* m_ic=new Move_IC_Stmt(sw,new Register_Addr_Opd((*it)->get_reg()),md); 
	// }
	// Register_Descriptor* reg_sp = new Register_Descriptor(sp,"sp",int_num,gp_data); // check !
	// Register_Addr_Opd* reg_opd=new Register_Addr_Opd(reg_sp);
	// Const_Opd* cons_opd = new Const_Opd(4*(param_list->size()));
	// Compute_IC_Stmt* c_ic = new Compute_IC_Stmt(sub,reg_opd,reg_opd,cons_opd);
}

Code_For_Ast & String_Ast::compile()
{
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	
	//global_strings[new_string_label]=value;
	// do we really require to generate any icode list?
	//Register_Descriptor* reg_a0 = new Register_Descriptor(a0,"a0",int_num,gp_data);
	//Register_Addr_Opd* rd = new Register_Addr_Opd(reg_a0);
	//Label_IC_Stmt* l_ic = new Label_IC_Stmt(la,rd,new_string_label);
	//ic_list.push_back(l_ic);
	Code_For_Ast* str_code_ast =new Code_For_Ast(ic_list,NULL);
	return *str_code_ast;

}

Code_For_Ast & Print_Ast::compile()
{
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	Code_For_Ast param_code = param->compile();
	if(param_code.get_icode_list().empty()==false)
		ic_list.splice(ic_list.end(), param_code.get_icode_list());
	Const_Opd<int>* cons_4 = new Const_Opd<int>(4);
	Const_Opd<int>* cons_m4 = new Const_Opd<int>(-4);
	Const_Opd<int>* cons_8 = new Const_Opd<int>(8);
	Const_Opd<int>* cons_m8 = new Const_Opd<int>(-8);
	Const_Opd<int>* cons_1 = new Const_Opd<int>(1);
	Const_Opd<int>* cons_0 = new Const_Opd<int>(0);
	Const_Opd<int>* cons_3 = new Const_Opd<int>(3);
	
	Register_Descriptor* reg_a0 = new Register_Descriptor(a0,"a0",int_num,gp_data);
	Register_Addr_Opd* reg_a0_opd = new Register_Addr_Opd(reg_a0);

	Register_Descriptor* reg_v0 = new Register_Descriptor(v0,"v0",int_num,gp_data);
	Register_Addr_Opd* reg_v0_opd = new Register_Addr_Opd(reg_v0);
	
	Register_Descriptor* reg_sp = new Register_Descriptor(sp,"sp",int_num,gp_data);
	Register_Addr_Opd* reg_sp_opd = new Register_Addr_Opd(reg_sp);
	
	Register_Descriptor* reg_v1 = new Register_Descriptor(v1,"v1",int_num,gp_data);
	Register_Addr_Opd* reg_v1_opd = new Register_Addr_Opd(reg_v1);
	

	Register_Descriptor* reg_f12 = new Register_Descriptor(f12,"f12",float_num,float_reg);
	Register_Addr_Opd* reg_f12_opd = new Register_Addr_Opd(reg_f12);
	
	Symbol_Table_Entry* se = new Symbol_Table_Entry();
	se->set_start_offset(0);
	se->set_symbol_scope(formal);
	Mem_Addr_Opd* m_opd = new Mem_Addr_Opd(*se);

	Move_IC_Stmt* m_ic;
	// = new Move_IC_Stmt(imm_load,cons_4,reg_v0_opd);
	//ic_list.push_back(m_ic);
	//delete m_ic;
	Compute_IC_Stmt* c_ic = new Compute_IC_Stmt(imm_add,reg_sp_opd,reg_sp_opd,cons_m4);
	ic_list.push_back(c_ic);
	
	m_opd->get_symbol_entry().set_symbol_scope(formal);
	cout<<"In M_opd"<<m_opd->get_symbol_entry().get_symbol_scope()<<endl;
	
	m_ic = new Move_IC_Stmt(store,reg_v0_opd,m_opd);
	ic_list.push_back(m_ic);
	ic_list.push_back(c_ic);
	m_ic = new Move_IC_Stmt(store,reg_a0_opd,m_opd);
	ic_list.push_back(m_ic);
	c_ic = new Compute_IC_Stmt(imm_add,reg_sp_opd,reg_sp_opd,cons_m8);
	ic_list.push_back(c_ic);
	m_ic = new Move_IC_Stmt(store_d,reg_f12_opd,m_opd);
	ic_list.push_back(m_ic);
	
	
	
	if(param->get_data_type()==string_data_type)
	{
		Label_IC_Stmt* l_ic = new Label_IC_Stmt(la,reg_a0_opd,param->get_label());
		ic_list.push_back(l_ic);
		m_ic = new Move_IC_Stmt(imm_load,cons_4,reg_v0_opd);
		ic_list.push_back(m_ic);
	}
	else if(param->get_data_type()==int_data_type)
	{
		Register_Addr_Opd* temp= new Register_Addr_Opd(param_code.get_reg());
		m_ic = new Move_IC_Stmt(mov,temp,reg_a0_opd );
		ic_list.push_back(m_ic);
		m_ic = new Move_IC_Stmt(imm_load,cons_1,reg_v0_opd);
		ic_list.push_back(m_ic);
	}
	else if(param->get_data_type()==double_data_type)
	{
		Register_Addr_Opd* temp= new Register_Addr_Opd(param_code.get_reg());

		m_ic = new Move_IC_Stmt(move_d, temp,reg_f12_opd);
		ic_list.push_back(m_ic);
		m_ic = new Move_IC_Stmt(imm_load,cons_3,reg_v0_opd);
		ic_list.push_back(m_ic);
	}

	Syscall_IC_Stmt* s_ic = new Syscall_IC_Stmt();
	ic_list.push_back(s_ic);
	m_ic = new Move_IC_Stmt(load_d,m_opd,reg_f12_opd);
	ic_list.push_back(m_ic);
	c_ic = new Compute_IC_Stmt(imm_add,reg_sp_opd,reg_sp_opd,cons_8);
	ic_list.push_back(c_ic);
	m_ic = new Move_IC_Stmt(load,m_opd,reg_a0_opd);
	ic_list.push_back(m_ic);
	c_ic = new Compute_IC_Stmt(imm_add,reg_sp_opd,reg_sp_opd,cons_4);
	ic_list.push_back(c_ic);
	m_ic = new Move_IC_Stmt(load,m_opd,reg_v0_opd);
	ic_list.push_back(m_ic);
	ic_list.push_back(c_ic);
	// m_ic = new Move_IC_Stmt(imm_load,reg_v0_opd,cons_0);
	// ic_list.push_back(m_ic);
	// m_ic = new Move_IC_Stmt(mov,reg_v1_opd,reg_v0_opd);
	// ic_list.push_back(m_ic);
	Code_For_Ast* print_code_ast =new Code_For_Ast(ic_list,NULL);
	machine_desc_object.clear_local_register_mappings();

	return *print_code_ast;
}

//////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Sequence_Ast::compile()
{
	//cout<<"here"<<endl;

	list<Ast*>::iterator it = statement_list.begin();
	for(;it!=statement_list.end();it++)
	{
		Code_For_Ast next = (*it)->compile();
		//cout<<"size of icode list : "<<next.get_icode_list().size()<<endl;
		if (next.get_icode_list().empty() == false)
			{
				//cout<<"adding icode"<<endl;
				sa_icode_list.splice(sa_icode_list.end(), next.get_icode_list());
			}

	}
	Code_For_Ast * seq_stmt;
	Register_Descriptor* reg_desc = machine_desc_object.get_new_register<gp_data>();

	reg_desc->reset_use_for_expr_result();
	seq_stmt = new Code_For_Ast(sa_icode_list,reg_desc);
	//cout<<"done loop for seq ast"<<endl;
	
	return *seq_stmt;		
}

void Sequence_Ast::print_assembly(ostream & file_buffer)
{
	
	list<Icode_Stmt*>::iterator it = sa_icode_list.begin();
	for(;it != sa_icode_list.end(); it++)
	{
		(*it)->print_assembly(file_buffer);
		//file_buffer<<endl;
	}
}

void Sequence_Ast::print_icode(ostream & file_buffer)
{
	list<Icode_Stmt*>::iterator it = sa_icode_list.begin();
	for(;it != sa_icode_list.end(); it++)
	{
		//cout<<"\nnext it\n";
		(*it)->print_icode(file_buffer);

	}
}

void Return_Ast::print(ostream & file_buffer)
{
	
	list<Icode_Stmt*>::iterator it = ra_icode_list.begin();
	for(;it !=ra_icode_list.end(); it++)
	{
		(*it)->print_assembly(file_buffer);
		//file_buffer<<endl;
	}
}

//////////////////////////////////////////////////////////////////////////////

template class Number_Ast<double>;
template class Number_Ast<int>;
