/*
 *  token.h
 *
 *  Created by Gabriel Rios & Hélder Almeida.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

class Token {
	public:
		int _id; // Código do token
		string alias; // Nome do Token
		string padrao; // Padrão reconhecido que gera o token
	public:
		Token();
		Token(string var);
		Token(string var1, string var2, int n);
		bool operator==(Token otr); //redefinindo o operador == para comparar o alias do token
		bool operator!=(Token otr); //redefinindo o operador != para comparar o alias do token
		bool is_null(); //para verificar se o token possui alias ou este é nulo
		string to_str(); //formatando a forma de imprimi-los
};
#endif
