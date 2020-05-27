#include "Header.h"
#include "Lexer.h"
#include "Node.h"

using namespace std;


class AST {
public:
	

	vector<Node*> statements;

	AST(vector<Node*> s) {
		statements = s;
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
	Node* stringOrChar();

	Node* returnStatement();
	Node* letStatement();
	Node* assignStatement();
	Node* functionDef();
	Node* functionCall();

	Node* statement();

	AST parse();
};