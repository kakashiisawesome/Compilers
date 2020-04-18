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
		node->left_node = factor();
		return node;
	}
	
	if (curr_token.type == MINUS) {
		Node* node = Node().newNode(curr_token);
		match(MINUS);
		node->left_node = factor();
		return node;
	}

	if (curr_token.type == NUM) {
		Node* node = Node().newNode(curr_token);
		
 		match(NUM);
		return node;
	}

	if (curr_token.type == ID) {
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

		op_node->left_node = left_node;
		op_node->right_node = right_node;

		left_node = op_node;
	}

	return left_node;
}

Node* Parser::expr() {
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

		op_node->left_node = left_node;
		op_node->right_node = right_node;

		left_node = op_node;
	}

	return left_node;
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

	let_node->left_node = left_node;
	let_node->right_node = right_node;

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

	op_node->left_node = left_node;
	op_node->right_node = right_node;

	return op_node;
}

Node* Parser::statement() {
	if (curr_token.type == LET) {
		return letStatement();
	}
	else if ((curr_token.type == ID) && (lexer.peek().type == ASSIGN)) {
		return assignStatement();
	}
	else {
		return expr();
	}
}

AST Parser::parse() {

	Node* root;
	root = statement();
	AST tree(root);

	return tree;
}