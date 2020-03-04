#include "Header.h"
using namespace std;

#define EOF "EOF"
#define NUM "NUM"
#define PLUS "PLUS"
#define MINUS "MINUS"

class Token {
public:
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
public:

	string input;
	int pos = 0;
	Token curr_token;
	bool flip = false;		//Sign of the expr on the right. If true, then '-' sign is applied to expr. For case NUM - expr => NUM + (-expr) 

	Interpreter(string input) {
		this->input = input;
		curr_token = getNextToken();		//Initialize curr_token to first token
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

		if (isdigit(input[pos])) {
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

	// Recursive Descent Parser - LL(1) + Interpreter

	void match(string tokenType) {	//Parse the token 
		if (curr_token.type != tokenType) {
			error();
		}
		curr_token = getNextToken();
	}

	float Num() {				//NUM -> integer
		Token t = curr_token;
		match(NUM);
		return stof(t.value);
	}

	float expr() {		// expr -> NUM + expr|NUM - expr|NUM

		if (pos == input.size()) {	//This means last token or input with single token
			flip = false;
			return Num();		//expr -> NUM 
		}
		else {

			float left_op = Num();

			if (curr_token.type == PLUS) {	//expr -> NUM + expr
				match(PLUS);
				bool curr_flip = flip;
				flip = false;
				float right_op = expr();
				if (curr_flip) {
					flip = false;
					return left_op - right_op;
				}
				return left_op + right_op;
			}
			else {		//expr -> NUM - expr
				match(MINUS);
				bool curr_flip = flip;
				flip = true;
				float right_op = expr();

				if (curr_flip) {
					flip = false;
					return left_op + right_op;
				}
				return left_op - right_op;
				 
			}
			
			
		}

	}

	// Initial parser (Non recursive)

	//float evaluate() {

	//	int opr = 0;
	//	float res = 0;



	//	while ((curr_token = getNextToken()).type != EOF) {
	//		if (curr_token.type != NUM) {	//Number
	//			error();
	//		}

	//		float temp = stof(curr_token.value);
	//		if (opr == 2) {
	//			temp *= -1;		//MINUS case
	//		}

	//		res += temp;		//Interpretation

	//		curr_token = getNextToken();

	//		if (curr_token.type == PLUS) {	//Operator
	//			opr = 1;
	//		}
	//		else if (curr_token.type == MINUS) {
	//			opr = 2;
	//		}
	//		else if (curr_token.type == EOF) {
	//			break;
	//		}
	//		else {
	//			error();
	//		}
	//	}



	//	return res;



	//}

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
	cout << "Res : " << i.expr();
}