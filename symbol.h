/*
 *  symbol.h
 *
 *  Created by Gabriel Rios & H�lder Almeida.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#include "token.h"
#include "position.h"
#include <string>
#include <vector>

using namespace std;

//representa uma cÈlula da tabela de sÌmbolos
class Symbol {
public:
	Token token;
	string lexema;
	vector<Position> pares;
	Symbol(Token, string, int, int);
	string to_str();
	void add_position(int, int);
};
