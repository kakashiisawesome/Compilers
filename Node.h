#include "Token.h"

struct Node {
	Token token;
	vector<Node*> subnodes;
	
	Node* newNode(Token t);
};

