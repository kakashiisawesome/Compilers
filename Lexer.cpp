#include "Lexer.h"


void error() {
	cout << "Lexer error\n";
	exit(EXIT_FAILURE);
}

Token Lexer::peek() {
	int peek_pos = pos;
	int l = line_no;
	string pline = line;
	if (peek_pos >= pline.size()) {
		peek_pos = 0;
		l++;
		if (l >= code.size()) {
			return Token(EOF, EOF);
		}
		pline = code[l];
	}
	while (isspace(pline[peek_pos])) {
		peek_pos++;
	}

	if (isdigit(pline[peek_pos])) {
		string temp;
		while (isdigit(pline[peek_pos])) {
			temp += pline[peek_pos];
			peek_pos++;
		}

		if (pline[peek_pos] == '.') {
			temp += pline[peek_pos];
			peek_pos++;
		}

		while (isdigit(pline[peek_pos])) {
			temp += pline[peek_pos];
			peek_pos++;
		}

		return Token(NUM, temp);
	}

	if (isalnum(pline[peek_pos])) {

		if (isalpha(pline[peek_pos])) {
			string temp;
			while (isalnum(pline[peek_pos])) {
				temp += pline[peek_pos];
				peek_pos++;
			}

			// Keywords
			if (temp == "let") {
				return Token(LET, temp);
			}

			return Token(ID, temp);
		}


	}


	else if (pline[peek_pos] == '+') {
		return Token(PLUS, "+");
	}
	else if (pline[peek_pos] == '-') {
		return Token(MINUS, "-");
	}
	else if (pline[peek_pos] == '*') {
		return Token(MUL, "*");
	}
	else if (pline[peek_pos] == '/') {
		return Token(DIV, "/");
	}
	else if (pline[peek_pos] == '(') {
		return Token(LPAREN, "(");
	}
	else if (pline[peek_pos] == ')') {
		return Token(RPAREN, ")");
	}
	else if (pline[peek_pos] == '{') {
		return Token(LBRACKET, "{");
	}
	else if (pline[peek_pos] == '}') {
		return Token(RBRACKET, "}");
	}
	else if (pline[peek_pos] == '=') {
		return Token(ASSIGN, "=");
	}
	else if (pline[peek_pos] == '\'') {
		if (pline[peek_pos + 1] == '\'') { error(); }
		string ch = "";
		ch += pline[peek_pos + 1];
		peek_pos += 3;
		return Token(CHAR, ch);
	}
	else if (pline[peek_pos] == '"') {
		peek_pos++;
		string s;
		if (pline[peek_pos] == '"') { error(); }
		while (pline[peek_pos] != '"') {
			s += pline[peek_pos];
			peek_pos++;
		}
		peek_pos++;
		return Token(STRING_LIT, s);
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

			if (temp == "def") {
				return Token(DEF, temp);
			}

			if (temp == "return") {
				return Token(RET, temp);
			}

			return Token(ID, temp);
		}

		

	}
	else if (line[pos] == ',') {
		pos++;
		return Token(COMMA, ",");
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
	else if (line[pos] == '{') {
		pos++;
		return Token(LBRACKET, "{");
	}
	else if (line[pos] == '}') {
		pos++;
		return Token(RBRACKET, "}");
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

