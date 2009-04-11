/*
 *  token.cpp
 *  lexicocompiler
 *
 *  Created by Gabriel Rios on 31/03/09.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

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
//	stringstream tk_str;
//	tk_str << "#" << _id << " " << alias;
	char format[100];
	sprintf(format, "%-16s %4d", alias.c_str(), _id);
	return string(format);
}
