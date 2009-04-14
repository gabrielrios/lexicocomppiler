/*
 *  token.h
 *
 *  Created by Gabriel Rios & Hélder Almeida.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#include "token.h"
#include <sstream>

//Token::Token... construtores
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

//redefinindo o operador != para comparar o alias do token
bool Token::operator!=(Token otr) {
	return alias != otr.alias;
}

//redefinindo o operador == para comparar o alias do token
bool Token::operator==(Token otr) {
	return alias == otr.alias;
}

//para verificar se o token possui alias ou este é nulo
bool Token::is_null() {
	return alias == "null";
}

//formatando a forma de imprimi-los
string Token::to_str() {
	char format[100];
	sprintf(format, "%-16s %4d", alias.c_str(), _id);
	return string(format);
}
