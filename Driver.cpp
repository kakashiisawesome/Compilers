#include "Header.h"
#include "Interpreter.h"
using namespace std;

int main() {

	cout << "Starting...\n";
	cout << "Enter input expr or q to quit: \n";
	string inp;
	cout << "> ";
	getline(cin, inp);
	Lexer lex(inp);
	Parser p(lex);
	Interpreter i(p);
	while (inp != "q") {
		
		string res = i.interpret();
		cout << res << "\n";
		//cout << "Postfix : " << i.getPostfix() << "\n";
		cout << "> ";
		getline(cin, inp);
		Lexer lex(inp);
		Parser p(lex);
		i.parser = p;
	}
}
