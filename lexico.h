/*
 *  lexico.h
 *
 *  Created by Gabriel Rios & Hélder Almeida.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#ifndef LEXICO_H
#define LEXICO_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "token.h"
#include "symbol.h"
#include "error.h"

using namespace std;

typedef enum eSearchMethod {
	BY_TOKEN,
	BY_PADRAO
}SearchMethod;

class Lexico {
public:
	int linha, coluna, versao; //guarda as coordenadas dos 
                               //tokens encontrados e a versão
	string lexema; //acumulador do lexema;
	stringstream file_name; //nome do arquivo de entrada
	vector<string> source; //vector de strings com código fonte
	int estado; //estado do autômato usado na versão 2
	int last_error_line; //guarda a última linha que ocorreu erro
	int last_symbol_position; //guarda a posição do último lexema
	vector<Symbol> table_symbols; //guarda a tabela de símbolos
	vector<Token> tokens; //guarda os tokens que podem ser reconhecidos
	                      //é carregado do arquivo tokens_list.dat
	vector<Error> list_erros; //guarda a lista de erros encontrados
	
public:
	Token tkEOF; //token para sinalizar fim de arquivo
	Lexico(string source_path, int versao);	// Abre arquivo fonte, e inicializa tabela de token
	bool load_tokens();  // Carrega tokens do arquivo
	Token next_token(); // Pega o proximo token e retorna para o analisador sintatico
	Token next_token_v2();	//next_token utilizado pela versão 2;
	Token next_token_v1();  //next_token utilizado pela versão 1
	Token search_token(string value, SearchMethod by); // Busca o token na tabela de tokens
	bool insert_symbol(Token tk, string lexema, int linha, int coluna); // insere um simbolo na tabela
	int find_symbol(string lexema); //verifica se o simbolo já existe na tabela de simbolos
	char next_char(); // pega o próximo caractere
	char prev_char(); // pega o caractere anterior
	Token is_keyword(string lexema); // verifica se é uma palavra-chave e retorna o token da mesma,
                                     // ou tkIdentificador se não for
	vector<Error> get_line_errors(int line); //retorna os erros da linha
	bool print_file_symbol(); //imprime a tabela de símbolos
	bool print_file_errors();  //imprime o arquivo de entrada com a marcação dos erros
	bool is_alpha(char _char); //verifica se o caractere é uma letra a-zA-Z sem acento
	bool is_digit(char _char); //verifica se o caractere é um dígito 0-9
	bool is_space(char _char); //verifica se o caractere é um espaço ' ', '\n', '\t'
};

#endif
