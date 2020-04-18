#include "Node.h"

/* Helper function that allocates a
new node */
Node* Node::newNode(Token t)
{
	Node* node = new Node;

	node->token = t;
	node->left_node = NULL;
	node->right_node = NULL;
	return (node);
}

void Node::print()
{
	std::cout << "Node(" << token.getString() << ", " << left_node << ", " << right_node << ")\n";
}