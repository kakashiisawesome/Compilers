#include "Token.h"

struct Node {
	Token token;
	Node* left_node;
	Node* right_node;
	
	void print();

	Node* newNode(Token t);
};

