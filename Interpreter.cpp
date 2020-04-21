#include "Interpreter.h"

#define UNDEFINED_SYMBOL "Undefined symbol "

string us;

string Interpreter::interpret() {
	tree = parser.parse();
	string res = visit(tree.root);
	if (res == UNDEFINED_SYMBOL) {
		return UNDEFINED_SYMBOL + us;
	}
	else {
		return res;
	}
}

bool isUnary(Node* node) {
	if ((node->left_node != NULL) && (node->right_node == NULL)) {
		return true;
	}
	else {
		return false;
	}
}


string Interpreter::visitLet(Node* node) {

	Token id_token = node->left_node->token;
	string rval = visit(node->right_node);

	if (rval == UNDEFINED_SYMBOL) {
		return UNDEFINED_SYMBOL;
	}
	
	Object runtime_obj(rval);

	Symbol v;
	v.name = id_token.value;

	if (symbolTable.find(v.name) != symbolTable.end()) {
		v = symbolTable[v.name];
	}

	if (runtime_obj.kind == Kind::INT) {
		v.type = INTEGER;
	}
	else {
		v.type = FLOAT;
	}
	symbolTable[v.name] = v;

	runtime_mem[v.name] = runtime_obj;

	return runtime_obj.getString();

}

string Interpreter::visitAssign(Node* node) {

	Token id_token = node->left_node->token;

	if (symbolTable.find(id_token.value) == symbolTable.end()) {
		us = id_token.value;
		return UNDEFINED_SYMBOL;
	}

	string rval = visit(node->right_node);
	if (rval == UNDEFINED_SYMBOL) {
		return UNDEFINED_SYMBOL;
	}

	Object runtime_obj(rval);

	Symbol v = symbolTable[id_token.value];

	//Only need to update type and runtime value of pre-existing symbol
	if (runtime_obj.kind == Kind::INT) {
		v.type = INTEGER;
	}
	else {
		v.type = FLOAT;
	}
	symbolTable[v.name] = v;

	runtime_mem[v.name] = runtime_obj;

	return runtime_obj.getString();

}

string Interpreter::visit(Node* node) {
	if (node->token.type == PLUS) {
		if (isUnary(node)) {
			string ls = visit(node->left_node);
			if (ls == UNDEFINED_SYMBOL) {
				return UNDEFINED_SYMBOL;
			}
			Object lobj(ls);
			return lobj.getString();
		}

		string ls = visit(node->left_node);
		string rs = visit(node->right_node);

		if (ls == UNDEFINED_SYMBOL) {
			return UNDEFINED_SYMBOL;
		}

		if (rs == UNDEFINED_SYMBOL) {
			return UNDEFINED_SYMBOL;
		}

		Object lobj(ls);
		Object robj(rs);

		Object res = lobj + robj;
		
		return res.getString();
	}
	else if (node->token.type == MINUS) {
		if (isUnary(node)) {
			string ls = visit(node->left_node);
			if (ls == UNDEFINED_SYMBOL) {
				return UNDEFINED_SYMBOL;
			}
			Object lobj("-"+ls);
			return lobj.getString();
		}

		string ls = visit(node->left_node);
		string rs = visit(node->right_node);

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
		string ls = visit(node->left_node);
		string rs = visit(node->right_node);

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
		string ls = visit(node->left_node);
		string rs = visit(node->right_node);

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

		return runtime_mem[symbolTable[node->token.value].name].getString();
	}
}

string postorder(Node* node) {
	string l, r;
	if (node->left_node) {
		l = postorder(node->left_node);
	}

	if (node->right_node) {
		r = postorder(node->right_node);
	}

	return l + r + node->token.value;
}

string Interpreter::getPostfix() {
	if (!tree.root) {
		cout << "No AST generated.\n";
		exit(EXIT_FAILURE);
	}

	return postorder(tree.root);

}

