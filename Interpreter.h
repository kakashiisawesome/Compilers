#include "Header.h"
#include "Parser.h"
#include "Symbol.h"

using namespace std;

class Interpreter {
public:
	Parser parser;
	Interpreter(Parser p) {
		parser = p;
	}

	map<string, Symbol> symbolTable;
	map<string, string> runtime_mem;

	string interpret();

	string visit(Node* node);
	string visitLet(Node* node);
	string visitAssign(Node* node);

	string getPostfix();
private:
	AST tree;
};

