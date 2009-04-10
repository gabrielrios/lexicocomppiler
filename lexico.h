/*
 *  lexico.h
 *  lexicocompiler
 *
 *  Created by Gabriel Rios on 07/04/09.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#ifndef LEXICO_H
#define LEXICO_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "token.h"
#include "symbol.h"


using namespace std;

typedef enum eSearchMethod {
	BY_TOKEN,
	BY_PADRAO
}SearchMethod;

class Lexico {
public:
	int linha, coluna;
	string lexema; //acumulador do lexema;
	//fstream source;
	vector<string> source;
	int estado;
	vector<Symbol> table_symbols;
	vector<Token> tokens;
	
public:
	Token tkEOF;
	Lexico(string source_path);	// Abre arquivo fonte, e inicializa tabela de token
	bool load_tokens();  // Carrega tokens do arquivo
	Token next_token();	 // Pega o proximo token e retorna para o analisador sintatico
	Token search_token(string value, SearchMethod by); // Busca o token na tabela de tokens
	bool insert_symbol(Token tk, string lexema, int linha, int coluna); // insere um simbolo na tabela
	int find_symbol(string lexema); //verifica se o simbolo já existe na tabela de simbolos
	char next_char(); // pega o próximo caractere
	char prev_char(); // pega o caractere anterior
};

#endif