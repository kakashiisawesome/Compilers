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
	Interpreter() {
	
	}

	map<string, Symbol> symbolTable;
	map<string, Node*> functionTable;
	map<Symbol, Object> runtime_mem;

	string interpret();

	string visit(Node* node);
	string visitLet(Node* node);
	string visitAssign(Node* node);
	string functionDef(Node* node);
	string functionCall(Node* node, bool ls);
	string visitReturn(Node* node);
	bool isStringOrChar(Node* node);

	string getPostfix();
private:
	AST tree;

	struct stack_frame {
		map<Symbol, Object> runtime_mem;
		stack_frame(map<Symbol, Object> r) {
			runtime_mem = r;
		}
	};

	vector<stack_frame> program_stack;
	void storeContext(Node* node);
	void restoreContext();
	void error() {
		cout << "Interpreter error\n";
		exit(EXIT_FAILURE);
	}

};

