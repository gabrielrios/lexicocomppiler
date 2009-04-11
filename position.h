/*
 *  position.h
 *  lexicocompiler
 *
 *  Created by Gabriel Rios on 07/04/09.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#ifndef POSITION_H
#define	POSITION_H

#include <sstream>
#include <string>

using namespace std;

class Position {
	public:
		int coluna, linha;
		Position(int, int);
		string to_str();

};

#endif
