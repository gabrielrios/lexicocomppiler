/*
 *  lexico.h
 *
 *  Created by Gabriel Rios & H�lder Almeida.
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
                               //tokens encontrados e a vers�o
	string lexema; //acumulador do lexema;
	stringstream file_name; //nome do arquivo de entrada
	vector<string> source; //vector de strings com c�digo fonte
	int estado; //estado do aut�mato usado na vers�o 2
	int last_error_line; //guarda a �ltima linha que ocorreu erro
	int last_symbol_position; //guarda a posi��o do �ltimo lexema
	vector<Symbol> table_symbols; //guarda a tabela de s�mbolos
	vector<Token> tokens; //guarda os tokens que podem ser reconhecidos
	                      //� carregado do arquivo tokens_list.dat
	vector<Error> list_erros; //guarda a lista de erros encontrados
	
public:
	Token tkEOF; //token para sinalizar fim de arquivo
	Lexico(string source_path, int versao);	// Abre arquivo fonte, e inicializa tabela de token
	bool load_tokens();  // Carrega tokens do arquivo
	Token next_token(); // Pega o proximo token e retorna para o analisador sintatico
	Token next_token_v2();	//next_token utilizado pela vers�o 2;
	Token next_token_v1();  //next_token utilizado pela vers�o 1
	Token search_token(string value, SearchMethod by); // Busca o token na tabela de tokens
	bool insert_symbol(Token tk, string lexema, int linha, int coluna); // insere um simbolo na tabela
	int find_symbol(string lexema); //verifica se o simbolo j� existe na tabela de simbolos
	char next_char(); // pega o pr�ximo caractere
	char prev_char(); // pega o caractere anterior
	Token is_keyword(string lexema); // verifica se � uma palavra-chave e retorna o token da mesma,
                                     // ou tkIdentificador se n�o for
	vector<Error> get_line_errors(int line); //retorna os erros da linha
	bool print_file_symbol(); //imprime a tabela de s�mbolos
	bool print_file_errors();  //imprime o arquivo de entrada com a marca��o dos erros
	bool is_alpha(char _char); //verifica se o caractere � uma letra a-zA-Z sem acento
	bool is_digit(char _char); //verifica se o caractere � um d�gito 0-9
	bool is_space(char _char); //verifica se o caractere � um espa�o ' ', '\n', '\t'
};

#endif
