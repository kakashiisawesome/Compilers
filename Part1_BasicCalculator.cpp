#include "Header.h"
using namespace std;

#define EOF "EOF"
#define NUM "NUM"
#define PLUS "PLUS"
#define MINUS "MINUS"

class Token {
public :
	string type;
	string value;
	

	Token(string type, string value) {
		this->type = type;
		this->value = value;
	}

	Token() {
		this->type = "";
		this->value = "";
	}

	string getString() {
		return "Token(" + this->type + "," + this->value + ")";
	}
};

class Interpreter {
public :

	string input;
	int pos = 0;
	Token curr_token;

	Interpreter(string input) {
		this->input = input;
	}

	void error() {
		cout << "Parsing error\n";
		exit(EXIT_FAILURE);
	}

	Token getNextToken() {

		while (isspace(input[pos])) {
			pos++;
		}

		if (pos >= input.size()) {
			return Token(EOF, EOF);
		}

		if (isdigit(input[pos])){
			string temp;
			while (isdigit(input[pos])) {
				temp += input[pos];
				pos++;
			}

			return Token(NUM, temp);
		}
		else if (input[pos] == '+') {
			pos++;
			return Token(PLUS, "+");
		}
		else if (input[pos] == '-') {
			pos++;
			return Token(MINUS, "-");
		}
		else {
			error();
		}
		


	}

	float evaluate() {

		int opr = 0;
		float res = 0;

		

		while ((curr_token = getNextToken()).type != EOF) {
			if (curr_token.type != NUM) {	//Number
				error();
			}

			float temp = stof(curr_token.value);
			if (opr == 2) {
				temp *= -1;		//MINUS case
			}

			res += temp;		//Interpretation

			curr_token = getNextToken();

			if (curr_token.type == PLUS) {	//Operator
				opr = 1;
			}
			else if (curr_token.type == MINUS) {
				opr = 2;
			}
			else if (curr_token.type == EOF) {
				break;
			}
			else {
				error();
			}
		}

		

		return res;
		


	}

};

int main() {
	cout << "Enter input expr : ";
	string inp;
	getline(cin, inp);
	/*int i = 0, count = 0;
	for (auto i : inp) {
		if (isspace(i)) {
			count++;
		}
	}*/
	

	Interpreter i(inp);
	cout << "Res : " << i.evaluate();
}