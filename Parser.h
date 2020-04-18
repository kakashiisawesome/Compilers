#include "Header.h"
#include "Lexer.h"
#include "Node.h"

using namespace std;


class AST {
public:
	

	Node* root;

	AST(Node* r) {
		root = r;
	}
	AST(){}
};



class Parser {
public:
	Lexer lexer;
	Token curr_token;

	Parser(Lexer l) {
		lexer = l;
		curr_token = lexer.getNextToken();
	}

	Parser() {}

	void error() {
		cout << "Parser error\n";
		exit(EXIT_FAILURE);
	}

	void match(string token_type);

	Node* factor();

	Node* term();

	Node* expr();

	Node* letStatement();
	Node* assignStatement();

	Node* statement();

	AST parse();
};