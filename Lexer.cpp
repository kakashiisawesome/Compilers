#include "Lexer.h"


void error() {
	cout << "Lexer error\n";
	exit(EXIT_FAILURE);
}

Token Lexer::peek() {
	int peek_pos = pos + 1;
	if (peek_pos >= line.size()) {
		return Token(EOF, EOF);
	}
	while (isspace(line[peek_pos])) {
		peek_pos++;
	}

	if (isdigit(line[peek_pos])) {
		string temp;
		while (isdigit(line[peek_pos])) {
			temp += line[peek_pos];
			peek_pos++;
		}

		if (line[peek_pos] == '.') {
			temp += line[peek_pos];
			peek_pos++;
		}

		while (isdigit(line[peek_pos])) {
			temp += line[peek_pos];
			peek_pos++;
		}

		return Token(NUM, temp);
	}

	if (isalnum(line[peek_pos])) {

		if (isalpha(line[peek_pos])) {
			string temp;
			while (isalnum(line[peek_pos])) {
				temp += line[peek_pos];
				peek_pos++;
			}

			// Keywords
			if (temp == "let") {
				return Token(LET, temp);
			}

			return Token(ID, temp);
		}


	}


	else if (line[peek_pos] == '+') {
		return Token(PLUS, "+");
	}
	else if (line[peek_pos] == '-') {
		return Token(MINUS, "-");
	}
	else if (line[peek_pos] == '*') {
		return Token(MUL, "*");
	}
	else if (line[peek_pos] == '/') {
		return Token(DIV, "/");
	}
	else if (line[peek_pos] == '(') {
		return Token(LPAREN, "(");
	}
	else if (line[peek_pos] == ')') {
		return Token(RPAREN, ")");
	}
	else if (line[peek_pos] == '=') {
		return Token(ASSIGN, "=");
	}
	else {
		error();
	}

}

bool Lexer::updateLine() {
	if (line_no >= code.size()) {
		return false;
	}
	line = code[line_no];
	return true;
}

Token Lexer::getNextToken() {

	if (pos >= line.size()) {
		pos = 0;
		line_no++;
		if (!updateLine()) {
			return Token(EOF, EOF);
		}
	}

	while (isspace(line[pos])) {
		pos++;
	}

	if (isdigit(line[pos])) {
		string temp;
		while (isdigit(line[pos])) {
			temp += line[pos];
			pos++;
		}

		if (line[pos] == '.') {
			temp += line[pos];
			pos++;
		}

		while (isdigit(line[pos])) {
			temp += line[pos];
			pos++;
		}

		return Token(NUM, temp);
	}

	if (isalnum(line[pos])) {

		if (isalpha(line[pos])) {
			string temp;
			while (isalnum(line[pos])) {
				temp += line[pos];
				pos++;
			}

			// Keywords
			if (temp == "let") {
				return Token(LET, temp);
			}

			return Token(ID, temp);
		}

		

	}

	
	else if (line[pos] == '+') {
		pos++;
		return Token(PLUS, "+");
	}
	else if (line[pos] == '-') {
		pos++;
		return Token(MINUS, "-");
	}
	else if (line[pos] == '*') {
		pos++;
		return Token(MUL, "*");
	}
	else if (line[pos] == '/') {
		pos++;
		return Token(DIV, "/");
	}
	else if (line[pos] == '(') {
		pos++;
		return Token(LPAREN, "(");
	}
	else if (line[pos] == ')') {
		pos++;
		return Token(RPAREN, ")");
	}
	else if (line[pos] == '=') {
		pos++;
		return Token(ASSIGN, "=");
	}
	else if (line[pos] == '\'') {
		if (line[pos + 1] == '\'') { error(); }
		string ch = "";
		ch += line[pos + 1];
		pos += 3;
		return Token(CHAR, ch);
	}
	else if (line[pos] == '"') {
		pos++;
		string s;
		if (line[pos] == '"') { error(); }
		while (line[pos] != '"') {
			s += line[pos];
			pos++;
		}
		pos++;
		return Token(STRING_LIT, s);
	}
	else {
		error();
	}



}

