#include "Header.h"
#ifndef TOKEN_H
#define TOKEN_H

#define EOF "EOF"
#define NUM "NUM"
#define PLUS "PLUS"
#define MINUS "MINUS"
#define MUL "MUL"
#define DIV "DIV"
#define LPAREN "("
#define RPAREN ")"
#define ASSIGN "="


// Keywords
#define LET "LET"
#define INTEGER "INTEGER"
#define FLOAT "FLOAT"
#define ID "ID"
#define STRING_LIT "STRING_LIT"
#define CHAR "CHAR"

using namespace std;
class Token {
public:
	string type;
	string value;


	Token(string type, string value) {
		this->type = type;
		this->value = value;
	}

	Token() {

	}

	string getString() {
		return "Token(" + this->type + "," + this->value + ")";
	}
};

#endif // !TOKEN_H


