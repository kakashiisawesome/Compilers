#include "Header.h"
#include "Token.h"
using namespace std;





class Lexer {
public:
	string input;
	int pos = 0;
	Token curr_token;

	Lexer(string inp) {
		input = inp;
	}

	Lexer() {}

	Token getNextToken();
	Token peek();
};
