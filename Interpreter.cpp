#include "Interpreter.h"

#define UNDEFINED_SYMBOL "Undefined symbol "
#define TYPE_MISMATCH "Error: type mismatch."
#define NO_RET "Error: function does not return a value."

string us;
bool isLocalScope = false;

string Interpreter::interpret() {
	tree = parser.parse();
	//vector<string> output;
	string res;
	for (auto statement : tree.statements) {
		res = visit(statement);
	}
	
	return res;
}

bool isUnary(Node* node) {

	if (node->subnodes.size() > 1) {
		return false;
	}
	else {
		return true;
	}
}

bool Interpreter::isStringOrChar(Node* node) {
	if ((runtime_mem[symbolTable[node->token.value]].kind == STRING) || ((node->token.type == STRING_LIT) || (node->token.type == CHAR))) {
		return true;
	}
	else {
		if (node->subnodes.empty()) {
			return false;
		}

		bool l = true;
		for (auto i : node->subnodes) {
			l = l && isStringOrChar(i);
		}
		
		return l;
	}
	
}


string Interpreter::visitLet(Node* node) {

	Token id_token = node->subnodes[0]->token;
	string rval = visit(node->subnodes[1]);
	Object runtime_obj;

	if (rval.empty()) {
		return NO_RET;
	}

	if ((rval == UNDEFINED_SYMBOL) || (rval == TYPE_MISMATCH)) {
		return rval;
	}

	if (isStringOrChar(node->subnodes[1])) {
		runtime_obj = Object(STRING_LIT, rval);
	}
	else {
		runtime_obj = Object(rval);
	}
	

	Symbol v;
	v.name = id_token.value;

	if (symbolTable.find(v.name) != symbolTable.end()) {
		v = symbolTable[v.name];
	}

	switch (runtime_obj.kind) {
	case INT:
		v.type = INTEGER;
		break;
	case DOUBLE:
		v.type = FLOAT;
		break;
	case STRING:
		v.type = STRING_LIT;
		break;
	}

	if (isLocalScope) {
		v.scope = LOCAL;
	}
	else {
		v.scope = GLOBAL;
	}
	

	symbolTable[v.name] = v;

	runtime_mem[v] = runtime_obj;

	return runtime_obj.getString();

}

string Interpreter::visitAssign(Node* node) {

	Object runtime_obj;
	Token id_token = node->subnodes[0]->token;

	if (symbolTable.find(id_token.value) == symbolTable.end()) {
		us = id_token.value;
		return UNDEFINED_SYMBOL;
	}

	string rval = visit(node->subnodes[1]);

	if (rval.empty()) {
		return NO_RET;
	}
	if ((rval == UNDEFINED_SYMBOL) || (rval == TYPE_MISMATCH)) {
		return rval;
	}

	if (isStringOrChar(node->subnodes[1])) {
		runtime_obj = Object(STRING_LIT, rval);
	}
	else {
		runtime_obj = Object(rval);
	}

	Symbol v = symbolTable[id_token.value];
	Object lobj = runtime_mem[v];

	if (lobj.kind != runtime_obj.kind) {
		return "" + runtime_obj.getType() + " to a value of type " + lobj.getType() + "\n";
	}

	//Only need to update runtime value of pre-existing symbol
	runtime_mem[v] = runtime_obj;

	return runtime_obj.getString();

}

string Interpreter::functionDef(Node* node) {
	string function_name = node->token.value;
	if (functionTable.find(function_name) == functionTable.end()) {
		functionTable[node->token.value] = node;
		return "<function : " + function_name + ">\n";
	}
	else {
		return "Error: function " + function_name + " already defined.\n";
	}
	
}

string Interpreter::functionCall(Node* node, bool ls) {
	int i = 0;
	string function_name = node->token.value;
	if (functionTable.find(function_name) == functionTable.end()) {
		return "Error: undefined function " + function_name + "\n";
	}
	else {
		isLocalScope = true;
		storeContext(node);
		string return_value;
		Node* fnode = functionTable[function_name];

		//Skip arguments
		while (fnode->subnodes[i]->token.type != END_ARG) { i++; }
		//Skip END_ARG node
		i++;

		for (; i < fnode->subnodes.size(); i++) {
			if (fnode->subnodes[i]->token.type == RET) {
				return_value = visitReturn(fnode->subnodes[i]);
				break;
			}
			visit(fnode->subnodes[i]);
			
		}
			
		restoreContext();
		isLocalScope = ls;
		return return_value;
	}

}

string Interpreter::visitReturn(Node* node) {
	string return_value;
	return_value = visit(node->subnodes[0]);

	return return_value;

}


void Interpreter::storeContext(Node* node) {

	Node* fnode = functionTable[node->token.value];
	vector<Object> pvalues;
	vector<Node*> fdef_params;
	vector<pair<Symbol, Object>> param_value;

	for (int i = 0; i < fnode->subnodes.size(); i++) {
		if (fnode->subnodes[i]->token.type == END_ARG) {
			break;
		}
		
		fdef_params.push_back(fnode->subnodes[i]);
	}

	for (int i = 0; i < node->subnodes.size(); i++) {
		Object pvalue;
		if (node->subnodes[i]->token.type == ID) {
			pvalue = runtime_mem[symbolTable[node->subnodes[i]->token.value]];
			pvalues.push_back(pvalue);
		}
		else if (node->subnodes[i]->token.type == NUM) {
			pvalue = Object(node->subnodes[i]->token.value);
			pvalues.push_back(pvalue);
		}
		else {
			error();
		}
		
	}

	if (fdef_params.size() != pvalues.size()) {
		error();
	}

	for (int i = 0; i < fdef_params.size(); i++) {
		Symbol ps;
		ps.name = fdef_params[i]->token.value;

		switch (pvalues[i].kind) {
			case INT:
				ps.type = INTEGER;
				break;
			case DOUBLE:
				ps.type = FLOAT;
				break;
			case STRING:
				ps.type = STRING_LIT;
				break;
		}

		ps.scope = LOCAL;

		param_value.push_back(make_pair(ps, pvalues[i]));
	}



	//Push local variables on the stack
	map<Symbol, Object> local_runtime_mem;

	for (auto i : runtime_mem) {
		if (i.first.scope == LOCAL) {
			local_runtime_mem[i.first] = i.second;
			runtime_mem.erase(i.first);
			symbolTable.erase(i.first.name);
		}
	}

	stack_frame s(local_runtime_mem);
	program_stack.push_back(s);

	//Add function parameters to the runtime_mem
	for (auto p : param_value) {
		runtime_mem[p.first] = p.second;
		symbolTable[p.first.name] = p.first;
	}


}

void Interpreter::restoreContext() {

	map<Symbol, Object> global_runtime_mem;
	map<string, Symbol> global_symbol_table;

	//Store global variables 
	for (auto i : runtime_mem) {
		if (i.first.scope == GLOBAL) {
			global_runtime_mem[i.first] = i.second;
			global_symbol_table[i.first.name] = i.first;
		}
	}

	//Clear runtime_mem and symbolTable
	runtime_mem.clear();
	symbolTable.clear();

	//insert global variables
	for (auto i : global_runtime_mem) {
		runtime_mem[i.first] = i.second;
		symbolTable[i.first.name] = i.first;
	}

	//Restore state of runtime_mem & symbolTable from program stack
	stack_frame s = program_stack.back();
	program_stack.pop_back();
	for (auto i : s.runtime_mem) {
		runtime_mem.insert(i);
		symbolTable[i.first.name] = i.first;
	}

}



string Interpreter::visit(Node* node) {
	if (node->token.type == PLUS) {
		if (isUnary(node)) {
			string ls = visit(node->subnodes[0]);
			if (ls == UNDEFINED_SYMBOL) {
				return UNDEFINED_SYMBOL;
			}
			Object lobj(ls);
			return lobj.getString();
		}

		Object lobj, robj;

		string ls = visit(node->subnodes[0]);
		string rs = visit(node->subnodes[1]);

		if (ls == UNDEFINED_SYMBOL) {
			return UNDEFINED_SYMBOL;
		}

		if (rs == UNDEFINED_SYMBOL) {
			return UNDEFINED_SYMBOL;
		}

		if (isStringOrChar(node->subnodes[0])) {
			lobj = Object(STRING_LIT, ls);
			if (!isStringOrChar(node->subnodes[1])) {
				return TYPE_MISMATCH;
			}
			robj = Object(STRING_LIT, rs);

		}
		else {
			lobj = Object(ls);
			if (isStringOrChar(node->subnodes[1])) {
				return TYPE_MISMATCH;
			}
			robj = Object(rs);
		}


		Object res = lobj + robj;
		
		return res.getString();
	}
	else if (node->token.type == MINUS) {
		if (isUnary(node)) {
			string ls = visit(node->subnodes[0]);
			if (ls == UNDEFINED_SYMBOL) {
				return UNDEFINED_SYMBOL;
			}
			Object lobj("-"+ls);
			return lobj.getString();
		}

		string ls = visit(node->subnodes[0]);
		string rs = visit(node->subnodes[1]);

		if (ls == UNDEFINED_SYMBOL) {
			return UNDEFINED_SYMBOL;
		}

		if (rs == UNDEFINED_SYMBOL) {
			return UNDEFINED_SYMBOL;
		}

		Object lobj(ls);
		Object robj(rs);

		Object res = lobj - robj;

		return res.getString();
	}
	else if (node->token.type == MUL) {
		string ls = visit(node->subnodes[0]);
		string rs = visit(node->subnodes[1]);

		if (ls == UNDEFINED_SYMBOL) {
			return UNDEFINED_SYMBOL;
		}

		if (rs == UNDEFINED_SYMBOL) {
			return UNDEFINED_SYMBOL;
		}

		Object lobj(ls);
		Object robj(rs);

		Object res = lobj * robj;

		return res.getString();
	}
	else if (node->token.type == DIV) {
		string ls = visit(node->subnodes[0]);
		string rs = visit(node->subnodes[1]);

		if (ls == UNDEFINED_SYMBOL) {
			return UNDEFINED_SYMBOL;
		}

		if (rs == UNDEFINED_SYMBOL) {
			return UNDEFINED_SYMBOL;
		}

		Object lobj(ls);
		Object robj(rs);

		Object res = lobj / robj;

		return res.getString();
	}
	else if (node->token.type == LET) {
		return visitLet(node);
	}
	else if (node->token.type == ASSIGN) {
		return visitAssign(node);
	}
	else if (node->token.type == NUM) {
		return node->token.value;
	}
	else if (node->token.type == ID) {
		if (symbolTable.find(node->token.value) == symbolTable.end()) {
			us = node->token.value;
			return UNDEFINED_SYMBOL;
		}

		return runtime_mem[symbolTable[node->token.value]].getString();
	}
	else if ((node->token.type == STRING_LIT) || (node->token.type == CHAR)) {
	return node->token.value;
	}
	else if (node->token.type == FUNCTION_DEF) {
	return functionDef(node);
	}
	else {
	return functionCall(node, isLocalScope);
	}
	
}

//string postorder(Node* node) {
//	string l, r;
//	if (node->left_node) {
//		l = postorder(node->left_node);
//	}
//
//	if (node->right_node) {
//		r = postorder(node->right_node);
//	}
//
//	return l + r + node->token.value;
//}

//string Interpreter::getPostfix() {
//	if (!tree.root) {
//		cout << "No AST generated.\n";
//		exit(EXIT_FAILURE);
//	}
//
//	return postorder(tree.root);
//
//}

