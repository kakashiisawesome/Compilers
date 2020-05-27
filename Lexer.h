#include "Header.h"
#include "Token.h"
using namespace std;

class Lexer {
public:
	vector<string> code;
	string line;
	int pos = 0;
	int line_no = 0;
	Token curr_token;

	Lexer(vector<string> c) {
		code = c;
		line = code[line_no];
	}

	Lexer() {}

	Token getNextToken();
	Token peek();
	bool updateLine();
};
