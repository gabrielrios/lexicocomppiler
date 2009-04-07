/*
 *  symbol.h
 *  lexicocompiler
 *
 *  Created by Gabriel Rios on 07/04/09.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include "token.h"
#include "position.h"
#include <string>
#include <vector>

using namespace std;

class Symbol {
public:
	Token token;
	string lexema;
	vector<Position> pares;
	Symbol(Token, string, int, int);
	string to_str();
	void add_position(int, int);
};

#endif