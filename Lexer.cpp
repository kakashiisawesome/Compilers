#include "Lexer.h"


void error() {
	cout << "Lexer error\n";
	exit(EXIT_FAILURE);
}

Token Lexer::peek() {
	int peek_pos = pos + 1;
	if (peek_pos >= input.size()) {
		return Token(EOF, EOF);
	}
	while (isspace(input[peek_pos])) {
		peek_pos++;
	}

	if (isdigit(input[peek_pos])) {
		string temp;
		while (isdigit(input[peek_pos])) {
			temp += input[peek_pos];
			peek_pos++;
		}

		if (input[peek_pos] == '.') {
			temp += input[peek_pos];
			peek_pos++;
		}

		while (isdigit(input[peek_pos])) {
			temp += input[peek_pos];
			peek_pos++;
		}

		return Token(NUM, temp);
	}

	if (isalnum(input[peek_pos])) {

		if (isalpha(input[peek_pos])) {
			string temp;
			while (isalnum(input[peek_pos])) {
				temp += input[peek_pos];
				peek_pos++;
			}

			// Keywords
			if (temp == "let") {
				return Token(LET, temp);
			}

			return Token(ID, temp);
		}


	}


	else if (input[peek_pos] == '+') {
		return Token(PLUS, "+");
	}
	else if (input[peek_pos] == '-') {
		return Token(MINUS, "-");
	}
	else if (input[peek_pos] == '*') {
		return Token(MUL, "*");
	}
	else if (input[peek_pos] == '/') {
		return Token(DIV, "/");
	}
	else if (input[peek_pos] == '(') {
		return Token(LPAREN, "(");
	}
	else if (input[peek_pos] == ')') {
		return Token(RPAREN, ")");
	}
	else if (input[peek_pos] == '=') {
		return Token(ASSIGN, "=");
	}
	else {
		error();
	}

}

Token Lexer::getNextToken() {

	if (pos >= input.size()) {
		return Token(EOF, EOF);
	}

	while (isspace(input[pos])) {
		pos++;
	}

	if (isdigit(input[pos])) {
		string temp;
		while (isdigit(input[pos])) {
			temp += input[pos];
			pos++;
		}

		if (input[pos] == '.') {
			temp += input[pos];
			pos++;
		}

		while (isdigit(input[pos])) {
			temp += input[pos];
			pos++;
		}

		return Token(NUM, temp);
	}

	if (isalnum(input[pos])) {

		if (isalpha(input[pos])) {
			string temp;
			while (isalnum(input[pos])) {
				temp += input[pos];
				pos++;
			}

			// Keywords
			if (temp == "let") {
				return Token(LET, temp);
			}

			return Token(ID, temp);
		}

		

	}

	
	else if (input[pos] == '+') {
		pos++;
		return Token(PLUS, "+");
	}
	else if (input[pos] == '-') {
		pos++;
		return Token(MINUS, "-");
	}
	else if (input[pos] == '*') {
		pos++;
		return Token(MUL, "*");
	}
	else if (input[pos] == '/') {
		pos++;
		return Token(DIV, "/");
	}
	else if (input[pos] == '(') {
		pos++;
		return Token(LPAREN, "(");
	}
	else if (input[pos] == ')') {
		pos++;
		return Token(RPAREN, ")");
	}
	else if (input[pos] == '=') {
		pos++;
		return Token(ASSIGN, "=");
	}
	else if (input[pos] == '\'') {
		if (input[pos + 1] == '\'') { error(); }
		string ch = "";
		ch += input[pos + 1];
		pos += 3;
		return Token(CHAR, ch);
	}
	else if (input[pos] == '"') {
		pos++;
		string s;
		if (input[pos] == '"') { error(); }
		while (input[pos] != '"') {
			s += input[pos];
			pos++;
		}
		pos++;
		return Token(STRING_LIT, s);
	}
	else {
		error();
	}



}

