#include "Node.h"

/* Helper function that allocates a
new node */
Node* Node::newNode(Token t)
{
	Node* node = new Node;

	node->token = t;
	return (node);
}
