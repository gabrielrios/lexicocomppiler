#include "token.h"
#include <sstream>

Token::Token() {
	alias = "null";
}

Token::Token(string texto) {
	alias = texto;
}

Token::Token(string _alias, string _padrao, int n) {
	alias = _alias;
	padrao = _padrao;
	_id = n;
}

bool Token::operator!=(Token otr) {
	return alias != otr.alias;
}

bool Token::operator==(Token otr) {
	return alias == otr.alias;
}

bool Token::is_null() {
	return alias == "null";
}

string Token::to_str() {
	char format[100];
	sprintf(format, "%-16s %4d", alias.c_str(), _id);
	return string(format);
}
