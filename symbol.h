/*
 *  symbol.h
 *
 *  Created by Gabriel Rios & Hélder Almeida.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#include "token.h"
#include "position.h"
#include <string>
#include <vector>

using namespace std;

//representa uma célula da tabela de símbolos
class Symbol {
public:
	Token token;
	string lexema;
	vector<Position> pares;
	Symbol(Token, string, int, int);
	//formata a forma de impressão do símbolo
	string to_str();
	//adiciona uma posição no símbolo
	void add_position(int, int);
};
