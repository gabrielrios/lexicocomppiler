/*
 *  position.cpp
 *
 *  Created by Gabriel Rios & Hélder Almeida.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#include "position.h"

//classe para guardar posição, no padrão de plano cartesiano ( linha, coluna)
Position::Position(int l, int c) {
	linha = l;
	coluna = c;
}

string Position::to_str() {
	stringstream ss;
	ss << "(" << linha << "," << coluna << ")";
	return ss.str();
}
