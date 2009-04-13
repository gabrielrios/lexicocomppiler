#include "position.h"

Position::Position(int l, int c) {
	linha = l;
	coluna = c;
}

string Position::to_str() {
	stringstream ss;
	ss << "(" << linha << "," << coluna << ")";
	return ss.str();
}
