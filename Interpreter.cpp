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
	Token id = node->left_node->token;
	string rval = visit(node->right_node);

	if (rval == UNDEFINED_SYMBOL) {
		return UNDEFINED_SYMBOL;
	}

	float r = stof(rval);
	
	if (symbolTable.find(id.value) == symbolTable.end()) {
		VarSymbol v;
		if (floor(r) == r) {
			v.name = id.value;
			v.type = INTEGER;
		}
		else {
			v.name = id.value;
			v.type = REAL;
		}
		symbolTable[id.value] = v;
	}

	runtime_mem[symbolTable[id.value].name] = (symbolTable[id.value].type == INTEGER) ? to_string(stoi(rval)) : to_string(r);

	return runtime_mem[symbolTable[id.value].name];

	
}

string Interpreter::visitAssign(Node* node) {
	Token id = node->left_node->token;
	if (symbolTable.find(id.value) == symbolTable.end()) {
		us = id.value;
		return UNDEFINED_SYMBOL;
	}

	string rval = visit(node->right_node);
	if (rval == UNDEFINED_SYMBOL) {
		return UNDEFINED_SYMBOL;
	}

	float r = stof(rval);

	Symbol v = symbolTable[id.value];
	if (floor(r) == r) {
		v.name = id.value;
		v.type = INTEGER;
	}
	else {
		v.name = id.value;
		v.type = REAL;
	}
	symbolTable[id.value] = v;

	runtime_mem[symbolTable[id.value].name] = (symbolTable[id.value].type == INTEGER) ? to_string(stoi(rval)) : to_string(r);

	return runtime_mem[symbolTable[id.value].name];


}

string Interpreter::visit(Node* node) {
	if (node->token.type == PLUS) {
		if (isUnary(node)) {
			string ls = visit(node->left_node);
			if (ls == UNDEFINED_SYMBOL) {
				return UNDEFINED_SYMBOL;
			}
			float l = stof(ls);
			return to_string(l);
		}

		string ls = visit(node->left_node);
		string rs = visit(node->right_node);

		if (ls == UNDEFINED_SYMBOL) {
			return UNDEFINED_SYMBOL;
		}

		if (rs == UNDEFINED_SYMBOL) {
			return UNDEFINED_SYMBOL;
		}

		float l = stof(ls);
		float r = stof(rs);
		return to_string(l + r);
	}
	else if (node->token.type == MINUS) {
		if (isUnary(node)) {
			string ls = visit(node->left_node);
			if (ls == UNDEFINED_SYMBOL) {
				return UNDEFINED_SYMBOL;
			}
			float l = stof(ls);
			l = l * (-1);
			return to_string(l);
		}

		string ls = visit(node->left_node);
		string rs = visit(node->right_node);

		if (ls == UNDEFINED_SYMBOL) {
			return UNDEFINED_SYMBOL;
		}

		if (rs == UNDEFINED_SYMBOL) {
			return UNDEFINED_SYMBOL;
		}

		float l = stof(ls);
		float r = stof(rs);
		return to_string(l - r);
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

		float l = stof(ls);
		float r = stof(rs);
		return to_string(l * r);
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

		float l = stof(ls);
		float r = stof(rs);
		return to_string(l / r);
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

		return runtime_mem[symbolTable[node->token.value].name];
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

