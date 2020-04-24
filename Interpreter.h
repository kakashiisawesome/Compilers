#include "Header.h"
#include "Parser.h"
#include "Symbol.h"
#include "Types.h"

using namespace std;

class Interpreter {
public:
	Parser parser;
	Interpreter(Parser p) {
		parser = p;
	}

	map<string, Symbol> symbolTable;
	map<string, Object> runtime_mem;

	string interpret();

	string visit(Node* node);
	string visitLet(Node* node);
	string visitAssign(Node* node);
	bool isStringOrChar(Node* node);

	string getPostfix();
private:
	AST tree;
};

