/*
 *  lexico.cpp
 *  lexicocompiler
 *
 *  Created by Gabriel Rios on 07/04/09.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#include "lexico.h"
//#define DEBUG

using namespace std;

Lexico::Lexico(string source_path) {
	fstream _source;
	tkEOF = Token("tkEOF");
	estado = linha = 0;
	coluna = -1;
	_source.open(source_path.c_str(), ios_base::in);
	if (!_source) {
		cerr << "Erro ao abrir o código fonte" << endl;
		exit(-1);
	}
	if (!load_tokens()) {
		exit(-1);
	}
	string str;
	while (getline(_source, str)) {
		source.push_back(str);
	}
	*(source.end() - 1);
}

char Lexico::next_char() {
	coluna++;
	if (coluna >= source[linha].size()) {
		linha++;
		coluna = 0;
	}
	return source[linha][coluna];
}

char Lexico::prev_char() {
	coluna--;
	if (coluna <= -1) {
		linha--;
		coluna = source[linha].size()-1;
	}
	return source[linha][coluna];
}

Token Lexico::next_token() {
	char c;
	estado = 0;
	while (1) {
		switch (estado) {
			case 0:
				lexema = "";
				c = next_char();
				if (isspace(c)) {
					estado = 0;
				} else if (isalpha(c))	{
					estado = 1;
				} else if (isdigit(c)) {
					estado = 3;
				} else if (c == '"') {
					estado = 5;
				} else if (c == '/') {
					estado = 7;
				} else if (0){
					
				} else {
				}
				break;
			case 1:
				c = next_char();
				if (isalpha(c) || isdigit(c) || c == '_') {
					estado = 1;
				} else {
					estado = 2;
				}
				break;
			case 2:
				prev_char();
				lexema.erase(lexema.size()-1);
				// TODO: verificar se é identificador ou palavra reservada
				// TODO: inserir tabela simbolo 
				cout << lexema << endl;
				return search_token("tkIdentificador", BY_TOKEN);
			case 3:
				c = next_char();
				if (isdigit(c)) {
					estado = 3;
				} else {
					estado = 4;
				}
				break;
			case 4:
				prev_char();
				// TODO: Inserir na tabela de simbolo
				return search_token("tkConstante", BY_TOKEN);
			case 5:
				c = next_char();				
				if (c == '"') {
					estado = 6;
				} else if (c == 13) { // MAC ONLY?
					estado = 0;
					cout << source[linha] << endl;
					for (int x = source[linha].size()-lexema.size(); x >= 0; x--)
						cout << "-";
					cout << "^ Literal não fechado" << endl;
				} else {
					estado = 5;
				}
				break;
			case 6:
				// TODO: Inserir na tablea de simbolo
				return search_token("tkLiteral", BY_TOKEN);
			case 7:
				c = next_char();
				if (c == '*') {
					estado = 8;
				} else {
					estado = 9; // ?
				}
				break;
			case 8:
				c = next_char();
				if (c == 13) {
					estado = 0;
					cout << "Comentario" << endl;
				}
				break;
			case 9:
				return search_token("/", BY_PADRAO);
			default:
				break;
		}
		lexema.push_back(c);

#ifdef DEBUG
		printf("%i- %i\n", estado, c);
#endif
	}
	//return Token("tkEOF");
}

bool Lexico::load_tokens() {
	//tkEOF = Token("tkEOF");
	fstream tokens_list;
	string tmp_alias, tmp_padrao;
	
	tokens_list.open("/Users/gabriel/lexicocompiler/tokens_list.dat", ios_base::in); // TODO: mudar caminho do arquivo
	
	if (!tokens_list) {
		cerr << "Erro ao abrir o arquivo de tokens" << endl;
		return false;
	}
	
	while (tokens_list >> tmp_alias >> tmp_padrao) {
		if (tmp_padrao == "#") {
			tmp_padrao = "";
		}
		tokens.push_back(Token(tmp_alias, tmp_padrao));
	}
	
#ifdef DEBUG
//	for(int i = 0; i < tokens.size(); i++) {
//		cout << tokens[i].to_str() << endl;
//	}
#endif
	
	return true;
}

Token Lexico::search_token(string value, SearchMethod by) {
	if (by == BY_PADRAO) { // Procura pelo lexema
		for (int i = 0; i < tokens.size(); i++) {
			if (tokens[i].padrao == value) {
				return tokens[i];
			}
		}
	} else if (by == BY_TOKEN) {
		for (int i = 0; i < tokens.size(); i++) {
			if (tokens[i].alias == value) {
				return tokens[i];
			}
		}
	}
	
	return Token();
}

int Lexico::find_symbol(string lexema) {
	for (int i = 0; i< table_symbols.size(); i++) {
		if (table_symbols[i].lexema == lexema) {
			return i;
		}
	}
	return -1;
}

bool Lexico::insert_symbol(Token tk, string lexema, int linha, int coluna) {
	int pos_sym;
	pos_sym = find_symbol(lexema);
	if (pos_sym == -1) {
		table_symbols.push_back(Symbol(tk, lexema, linha, coluna));
	} else {
		table_symbols[pos_sym].add_position(linha, coluna);
	}
	return true;
}