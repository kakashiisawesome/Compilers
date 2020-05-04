#include "Header.h"
#include "Interpreter.h"
using namespace std;

int main() {

	string inp;
	vector<string> code, res;
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

		Lexer lex(code);
		Parser p(lex);
		Interpreter i(p);
		res = i.interpret();

		for (auto i : res) {
			cout << i << "\n";
		}
		code.clear();
		res.clear();
		cout << ">  ";
		getline(cin, inp);
	}

	
	
	
	

}
