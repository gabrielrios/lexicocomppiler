/*
 *  token.cpp
 *  lexicocompiler
 *
 *  Created by Gabriel Rios on 31/03/09.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#include "token.h"

Token::Token() {
	alias = "";
}

Token::Token(string texto) {
	alias = texto;
}

Token::Token(string _alias, string _padrao) {
	alias = _alias;
	padrao = _padrao;
}

bool Token::operator!=(Token otr) {
	return alias != otr.alias;
}

bool Token::operator==(Token otr) {
	return alias == otr.alias;
}


string Token::to_str() {
	return alias;
}