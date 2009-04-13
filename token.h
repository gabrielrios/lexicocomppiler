/*
 *  token.h
 *
 *  Created by Gabriel Rios & H�lder Almeida.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

class Token {
	public:
		int _id; // C�digo do token
		string alias; // Nome do Token
		string padrao; // Padr�o reconhecido que gera o token
	public:
		Token();
		Token(string var);
		Token(string var1, string var2, int n);
		bool operator==(Token otr);
		bool operator!=(Token otr);
		bool is_null();
		string to_str();
};
#endif
