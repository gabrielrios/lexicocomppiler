/*
 *  position.cpp
 *  lexicocompiler
 *
 *  Created by Gabriel Rios on 07/04/09.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#include "position.h"

Position::Position(int l, int c) {
	linha = l;
	coluna = c;
}

string Position::to_str() {
	stringstream ss;
	ss << "(" << linha << "," << coluna << ")";
	return ss.str();
}