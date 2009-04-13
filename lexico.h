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
	int linha, coluna, versao;
	string lexema; //acumulador do lexema;
	stringstream file_name;
	vector<string> source;
	int estado;
	int last_error_line;
	int last_symbol_position;
	vector<Symbol> table_symbols;
	vector<Token> tokens;
	vector<Error> list_erros;
	
public:
	Token tkEOF;
	Lexico(string source_path, int versao);	// Abre arquivo fonte, e inicializa tabela de token
	bool load_tokens();  // Carrega tokens do arquivo
	Token next_token();
	Token next_token_v2();	 // Pega o proximo token e retorna para o analisador sintatico
	Token next_token_v1();
	Token search_token(string value, SearchMethod by); // Busca o token na tabela de tokens
	bool insert_symbol(Token tk, string lexema, int linha, int coluna); // insere um simbolo na tabela
	int find_symbol(string lexema); //verifica se o simbolo já existe na tabela de simbolos
	char next_char(); // pega o próximo caractere
	char prev_char(); // pega o caractere anterior
	Token is_keyword(string lexema); // verifica se é uma palavra-chave e retorna o token da mesma, ou tkIdentificador se não for
	vector<Error> get_line_errors(int line);
	bool print_file_symbol();
	bool print_file_errors();
};

#endif
