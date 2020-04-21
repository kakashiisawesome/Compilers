#include "Token.h"

struct Symbol {
	string name;
	string type;
	Symbol(string name, string type) {
		this->name = name;
		this->type = type;
	}
	Symbol() {

	}
};

struct BuiltInTypeSymbol : Symbol {
	BuiltInTypeSymbol(string Name) : Symbol(Name, "BuiltInType") {
	
	}
};

struct VarSymbol : Symbol {
	VarSymbol(string Name, string DataType) : Symbol(Name, DataType) {

	}

	VarSymbol(): Symbol() {}
};

