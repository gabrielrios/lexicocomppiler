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
    // Código do token
		int _id; 
		// Nome do Token
		string alias; 
		// Padrão reconhecido que gera o token
		string padrao; 
	public:
		Token();
		Token(string var);
		Token(string var1, string var2, int n);
		//redefinindo o operador == para comparar o alias do token
		bool operator==(Token otr); 
		//redefinindo o operador != para comparar o alias do token
		bool operator!=(Token otr); 
		//para verificar se o token possui alias ou este é nulo
		bool is_null(); 
		//formatando a forma de imprimi-los
		string to_str(); 
};
#endif
