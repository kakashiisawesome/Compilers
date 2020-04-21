#include "Token.h"
using namespace std;

enum Kind{ INT, DOUBLE };

union Value {
	int64_t IntVal;
	double DoubleVal;
	

	Value(int v) { 
		IntVal = v; 
	}
	Value(double v) { 
		DoubleVal = v; 
	}


	Value() {};


};

struct Object {
	Value value;
	Kind kind;
	Object() {}

	Object(string val) {
		float v = stof(val);
		if (v < 0) {
			if (ceil(v) == v) {
				kind = Kind::INT;
				value = Value(stoi(val));
			}
			else {
				kind = Kind::DOUBLE;
				value = Value(stod(val));
			}
		}
		else {
			if (floor(v) == v) {
				kind = Kind::INT;
				value = Value(stoi(val));
			}
			else {
				kind = Kind::DOUBLE;
				value = Value(stod(val));
			}
		}
	}

	string getString() {
		switch (kind) {
		case INT:
			return to_string(value.IntVal);
		case DOUBLE:
			return to_string(value.DoubleVal);
		}
	}

	Object operator +(Object& b) {
		if ((kind == Kind::DOUBLE)&&(b.kind == Kind::DOUBLE)) {
			value.DoubleVal += b.value.DoubleVal;
			return *this;
		}
		if ((kind == Kind::DOUBLE) && (b.kind == Kind::INT)) {
			value.DoubleVal += b.value.IntVal;
			return *this;
		}
		if ((kind == Kind::INT) && (b.kind == Kind::DOUBLE)) {
			b.value.DoubleVal += value.IntVal;
			return b;
		}
		else {
			value.IntVal += b.value.IntVal;
			return *this;
		}

	}

	Object operator -(Object& b) {
		if ((kind == Kind::DOUBLE) && (b.kind == Kind::DOUBLE)) {
			value.DoubleVal -= b.value.DoubleVal;
			return *this;
		}
		if ((kind == Kind::DOUBLE) && (b.kind == Kind::INT)) {
			value.DoubleVal -= b.value.IntVal;
			return *this;
		}
		if ((kind == Kind::INT) && (b.kind == Kind::DOUBLE)) {
			b.value.DoubleVal = value.IntVal - b.value.DoubleVal;
			return b;
		}
		else {
			value.IntVal -= b.value.IntVal;
			return *this;
		}

	}

	Object operator *(Object& b) {
		if ((kind == Kind::DOUBLE) && (b.kind == Kind::DOUBLE)) {
			value.DoubleVal *= b.value.DoubleVal;
			return *this;
		}
		if ((kind == Kind::DOUBLE) && (b.kind == Kind::INT)) {
			value.DoubleVal *= b.value.IntVal;
			return *this;
		}
		if ((kind == Kind::INT) && (b.kind == Kind::DOUBLE)) {
			b.value.DoubleVal *= value.IntVal;
			return b;
		}
		else {
			value.IntVal *= b.value.IntVal;
			return *this;
		}

	}

	Object operator /(Object& b) {
		if ((kind == Kind::DOUBLE) && (b.kind == Kind::DOUBLE)) {
			value.DoubleVal /= b.value.DoubleVal;
			return *this;
		}
		if ((kind == Kind::DOUBLE) && (b.kind == Kind::INT)) {
			value.DoubleVal /= b.value.IntVal;
			return *this;
		}
		if ((kind == Kind::INT) && (b.kind == Kind::DOUBLE)) {
			b.value.DoubleVal = value.IntVal / b.value.DoubleVal;
			return b;
		}
		else {
			value.IntVal /= b.value.IntVal;
			return *this;
		}

	}

};