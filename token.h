/*
 *  token.h
 *  lexicocompiler
 *
 *  Created by Gabriel Rios on 31/03/09.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

class Token {
	private:
		string alias;
		string padrao;
	public:
		//Token();
		Token(string var);
		Token(string var1, string var2);
		bool operator!=(Token otr);
};


#endif