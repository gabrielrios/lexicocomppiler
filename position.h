/*
 *  position.h
 *
 *  Created by Gabriel Rios & H�lder Almeida.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#include <sstream>
#include <string>

using namespace std;

//posi��o com as coordenadas em linha e coluna
class Position {
	public:
		int coluna, linha;
		Position(int, int);
		string to_str();

};
