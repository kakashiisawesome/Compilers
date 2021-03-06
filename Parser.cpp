#include "Parser.h"

using namespace std;



void Parser::match(string token_type) {
	if (curr_token.type == token_type) {
		curr_token = lexer.getNextToken();
	}
	else {
		error();
	}
}

Node* Parser::factor() {

	if (curr_token.type == PLUS) {
		Node* node = Node().newNode(curr_token);
		match(PLUS);
		node->subnodes.push_back(factor());
		return node;
	}
	
	if (curr_token.type == MINUS) {
		Node* node = Node().newNode(curr_token);
		match(MINUS);
		node->subnodes.push_back(factor());
		return node;
	}

	if (curr_token.type == NUM) {
		Node* node = Node().newNode(curr_token);
		
 		match(NUM);
		return node;
	}

	if (curr_token.type == ID) {
		if (lexer.peek().type == LPAREN) {
			return functionCall();
		}
		Node* node = Node().newNode(curr_token);

		match(ID);
		return node;
	}
	else {
		match(LPAREN);
		Node* node = expr();
		match(RPAREN);
		return node;
	}

}

Node* Parser::term() {
	Node* left_node = factor();
	Node* op_node;

	if ((left_node->token.type == CHAR) || (left_node->token.type == STRING_LIT)) {
		return left_node;
	}

	while ((curr_token.type == MUL) || (curr_token.type == DIV)) {
		
		Token t = curr_token;
		if (curr_token.type == MUL) {
			match(MUL);
		}
		else {
			match(DIV);
		}

		op_node = Node().newNode(t);

		Node* right_node = factor();

		op_node->subnodes.push_back(left_node);
		op_node->subnodes.push_back(right_node);

		left_node = op_node;
	}

	return left_node;
}

Node* Parser::expr() {
	//For string and char
	if ((curr_token.type == STRING_LIT) || (curr_token.type == CHAR)) {
		Node* left_node = stringOrChar();
		Node* op_node;
		while (curr_token.type == PLUS) {
			op_node = Node().newNode(curr_token);
			Node* right_node = stringOrChar();
			op_node->subnodes.push_back(left_node);
			op_node->subnodes.push_back(right_node);
			left_node = op_node;
		}

		return left_node;
	}

	Node* left_node = term();
	Node* op_node;

	while ((curr_token.type == PLUS) || (curr_token.type == MINUS)) {

		Token t = curr_token;
		if (curr_token.type == PLUS) {
			match(PLUS);
		}
		else {
			match(MINUS);
		}

		op_node = Node().newNode(t);

		Node* right_node = term();

		op_node->subnodes.push_back(left_node);
		op_node->subnodes.push_back(right_node);

		left_node = op_node;
	}

	return left_node;
}

Node* Parser::stringOrChar() {
	if (curr_token.type == STRING_LIT) {
		Node* node = Node().newNode(curr_token);

		match(STRING_LIT);
		return node;
	}

	if (curr_token.type == CHAR) {
		Node* node = Node().newNode(curr_token);

		match(CHAR);
		return node;
	}
}

Node* Parser::letStatement() {
	Node* left_node;
	Node* right_node;
	Node* let_node;

	match(LET);

	Token t = curr_token;

	match(ID);
	left_node = Node().newNode(t);

	match(ASSIGN);
	Token l(LET, LET);
	let_node = Node().newNode(l);
	
	right_node = expr();

	let_node->subnodes.push_back(left_node);
	let_node->subnodes.push_back(right_node);

	return let_node;

}

Node* Parser::assignStatement() {
	Node* left_node;
	Node* right_node;
	Node* op_node;

	Token t = curr_token;

	match(ID);
	left_node = Node().newNode(t);

	t = curr_token;
	match(ASSIGN);
	op_node = Node().newNode(t);

	right_node = expr();

	op_node->subnodes.push_back(left_node);
	op_node->subnodes.push_back(right_node);

	return op_node;
}

Node* Parser::functionDef() {
	//def
	match(DEF);

	//Function name
	string fname;
	fname = curr_token.value;
	match(ID);
	Token t(FUNCTION_DEF, fname);
	Node* fnode = Node().newNode(t);

	//Arguments
	match(LPAREN);
	Node* idNode;

	if (curr_token.type == ID) {
		idNode = Node().newNode(curr_token);
		match(ID);
		fnode->subnodes.push_back(idNode);

		while (curr_token.type == COMMA) {
			match(COMMA);
			idNode = Node().newNode(curr_token);
			match(ID);
			fnode->subnodes.push_back(idNode);
		}

	}

	//End of arguments node
	Token end_arg(END_ARG, END_ARG);
	idNode = Node().newNode(end_arg);
	fnode->subnodes.push_back(idNode);

	match(RPAREN);


	//Body
	match(LBRACKET);

	while (curr_token.type != RBRACKET) {
		if (curr_token.type == LET) {
			fnode->subnodes.push_back(letStatement());
		}
		else if ((curr_token.type == ID) && (lexer.peek().type == ASSIGN)) {
			fnode->subnodes.push_back(assignStatement());
		}
		else if (curr_token.type == RET) {
			fnode->subnodes.push_back(returnStatement());
			break;
		}
		else {
			fnode->subnodes.push_back(expr());
		}
	}

	match(RBRACKET);

	return fnode;
	
}

Node* Parser::returnStatement() {
	Node* node = Node().newNode(curr_token);
	match(RET);

	node->subnodes.push_back(expr());

	return node;
}

Node* Parser::functionCall() {
	//Function name
	string fname;
	fname = curr_token.value;
	match(ID);
	Token t(FUNCTION_CALL, fname);
	Node* fnode = Node().newNode(t);

	//Parameters
	match(LPAREN);
	Node* argNode;

	if ((curr_token.type == ID) || (curr_token.type == NUM)) {
		argNode = Node().newNode(curr_token);

		if (curr_token.type == ID) {
			match(ID);
		}
		else {
			match(NUM);
		}
		
		fnode->subnodes.push_back(argNode);

		while (curr_token.type == COMMA) {
			match(COMMA);
			argNode = Node().newNode(curr_token);
			if (curr_token.type == ID) {
				match(ID);
			}
			else {
				match(NUM);
			}
			fnode->subnodes.push_back(argNode);
		}

	}

	match(RPAREN);

	return fnode;
}

Node* Parser::statement() {
	if (curr_token.type == LET) {
		return letStatement();
	}
	else if (curr_token.type == DEF) {
		return functionDef();
	}
	else if ((curr_token.type == ID) && (lexer.peek().type == ASSIGN)) {
		return assignStatement();
	}
	else {
		return expr();
	}
}

AST Parser::parse() {

	vector<Node*> statements;

	for (int i = 0; i < lexer.code.size(); i++) {
		if (curr_token.type == EOF) { break; }
		statements.push_back(statement());
	}

	AST tree(statements);

	return tree;
}