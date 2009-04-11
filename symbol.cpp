/*
 *  symbol.cpp
 *  lexicocompiler
 *
 *  Created by Gabriel Rios on 07/04/09.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#include "symbol.h"

Symbol::Symbol(Token tk, string lex, int linha, int coluna) {
	token = tk;
	lexema = lex;
	pares.push_back(Position(linha, coluna));
}

string Symbol::to_str() {
	string pares_str = "";
	for (int i = 0; i < pares.size(); i++) {
		pares_str += pares[i].to_str();
	}
	char format[200];
	sprintf(format, "%-16s  %-26s  ", token.alias.c_str(), lexema.c_str());
	//return token.to_str() + " | " + lexema + " | " + pares_str;
	return string(format) + " " + pares_str;
}

void Symbol::add_position(int linha, int coluna) {
	pares.push_back(Position(linha, coluna));
}
