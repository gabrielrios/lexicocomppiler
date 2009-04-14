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

//representa uma c�lula da tabela de s�mbolos
class Symbol {
public:
	Token token;
	string lexema;
	vector<Position> pares;
	Symbol(Token, string, int, int);
	//formata a forma de impress�o do s�mbolo
	string to_str();
	//adiciona uma posi��o no s�mbolo
	void add_position(int, int);
};
