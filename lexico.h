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

//define o tipo de busca no m�todo search_token
typedef enum eSearchMethod {
	BY_TOKEN,
	BY_PADRAO
}SearchMethod;

class Lexico {
public:
  //guarda as coordenadas dos tokens encontrados e a vers�o
	int linha, coluna, versao;
	//acumulador do lexema;
	string lexema;
	//nome do arquivo de entrada
	stringstream file_name; 
	//vector de strings com c�digo fonte
	vector<string> source;
	//estado do aut�mato usado na vers�o 2
	int estado;
	//guarda a �ltima linha que ocorreu erro
	int last_error_line;
	//guarda a posi��o do �ltimo lexema
	int last_symbol_position;
	//guarda a tabela de s�mbolos
	vector<Symbol> table_symbols;
	//guarda os tokens que podem ser reconhecidos � carregado do arquivo tokens_list.dat
	vector<Token> tokens; 
	//guarda a lista de erros encontrados
	vector<Error> list_erros; 
	
public:
  //token para sinalizar fim de arquivo
	Token tkEOF;
	// Abre arquivo fonte, e inicializa tabela de token
	Lexico(string source_path, int versao);	
	// Carrega tokens do arquivo
	bool load_tokens(); 
  // Pega o proximo token e retorna para o analisador sintatico 
	Token next_token(); 
	//next_token utilizado pela vers�o 2;
	Token next_token_v2();	
	//next_token utilizado pela vers�o 1
	Token next_token_v1();  
	// Busca o token na tabela de tokens
	Token search_token(string value, SearchMethod by); 
	// insere um simbolo na tabela
	bool insert_symbol(Token tk, string lexema, int linha, int coluna); 
	//verifica se o simbolo j� existe na tabela de simbolos
	int find_symbol(string lexema); 
	// pega o pr�ximo caractere
	char next_char(); 
	// pega o caractere anterior
	char prev_char(); 
	// verifica se � uma palavra-chave e retorna o token da mesma, ou tkIdentificador se n�o for
	Token is_keyword(string lexema); 
	//retorna os erros da linha
	vector<Error> get_line_errors(int line); 
	//imprime a tabela de s�mbolos
	bool print_file_symbol(); 
	//imprime o arquivo de entrada com a marca��o dos erros
	bool print_file_errors();  
	//verifica se o caractere � uma letra a-zA-Z sem acento
	bool is_alpha(char _char); 
	//verifica se o caractere � um d�gito 0-9
	bool is_digit(char _char); 
	//verifica se o caractere � um espa�o ' ', '\n', '\t'
	bool is_space(char _char); 
};

#endif
