#include "Token.h"

#define GLOBAL "GLOBAL"
#define LOCAL "LOCAL"



struct Symbol {
	string name;
	string type;
	string scope;
	Symbol(string name, string type) {
		this->name = name;
		this->type = type;
	}
	Symbol() {

	}

	bool operator <(const Symbol& b) const {
		return this->name < b.name;
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

