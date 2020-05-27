#include "Header.h"
#include "Interpreter.h"
using namespace std;

int main() {

	string inp;
	vector<string> code;
	Interpreter i;
	Lexer lex;
	Parser p;
	cout << "Starting...\n";
	cout << "Enter input expr or q to quit: \n";
	cout << ">  ";
	getline(cin, inp);
	while (inp != "q") {
		
		while ((!inp.empty())) {
			code.push_back(inp);

			cout << ".. ";
			getline(cin, inp);

		}

		lex = Lexer(code);
		p = Parser(lex);
		i.parser = p;
		string res = i.interpret();

		cout << res << "\n";
		code.clear();
		cout << ">  ";
		getline(cin, inp);
	}

	
	
	
	

}
