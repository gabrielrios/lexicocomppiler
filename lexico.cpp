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
	tkEOF = Token("tkEOF", "", -1);
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
		str.push_back('\n');
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
	if (linha >= source.size()) {
		return -1;
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

Token Lexico::is_keyword(string lexema) {
	Token tmp = search_token(lexema, BY_PADRAO);
	if (tmp.is_null()) {
		return search_token("tkIdentificador", BY_TOKEN);
	}
	return tmp;
}

Token Lexico::next_token() {
	Token tk;
	int old_line;
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
				} else if (c == '{'){
					estado = 10;
				} else if (c == '=') {
					estado = 13;
				} else if (c == '<') {
					estado = 16;
				} else if (c == '>') {
					estado = 20;
				} else if (c == '+') {
					estado = 23;
				} else if (c == '-') {
					estado = 24;
				} else if (c == '*') {
					estado = 25;
				} else if (c == ',') {
					estado = 26;
				} else if (c == ';') {
					estado = 27;
				} else if (c == ':') {
					estado = 28;
				} else if (c == '(') {
					estado = 29;
				} else if (c == ')') {
					estado = 30;
				} else if (c == -1) {
					return tkEOF;
				} else {
					cout << source[linha];
					for (int x = coluna-1; x >= 0; x--)
						cout << "-";
					cout << "^ ERRO: Caractere Inválido" << endl;;
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
				tk = is_keyword(lexema);
				if (tk._id == 0) {
					insert_symbol(tk, lexema, linha+1, (coluna+1)-(lexema.size()-1));
				} ;
				return tk;
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
				tk = search_token("tkConstante", BY_TOKEN);
				insert_symbol(tk, lexema, linha+1, (coluna+1)-(lexema.size()-1));
				return tk;
			case 5:
				c = next_char();				
				if (c == '"') {
					estado = 6;
				} else if (c == '\n') {
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
				tk = search_token("tkLiteral", BY_TOKEN);
				insert_symbol(tk, lexema, linha+1, (coluna+1)-(lexema.size()-1));
				return tk;
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
				if (c == '\n') {
					estado = 0;
					cout << "Comentario" << endl;
				}
				break;
			case 9:
				return search_token("/", BY_PADRAO);
			case 10:
				c = next_char();
				if (c == '*') {
					estado = 11;
					old_line = linha;
				} else {
					estado = 0;
					// caracter inválido
				}
				break;
			case 11:
				c = next_char();
				if (c == '*') {
					estado = 12;
				} else if (c == -1) {
					estado = 0;
					cout << source[old_line] << endl;
					cout << "ERRO: Comentário não fechado" << endl;
					linha = old_line + 1;
				}
				break;
			case 12:
				c = next_char();
				if (c == '}') {
					estado = 0;
					cout << "Comentario"<< endl;
				} else {
					prev_char();
					estado = 11;
				}
				break;
			case 13:
				c = next_char();
				if (c == '=') {
					estado = 14;
				} else {
					estado = 15;
				}
				break;
			case 14:
				return search_token("==", BY_PADRAO);
			case 15:
				prev_char();
				return search_token("=", BY_PADRAO);
			case 16:
				c = next_char();
				if (c == '>') {
					estado = 17;
				} else if (c == '=') {
					estado = 18;
				} else {
					estado = 19;
				}
				break;
			case 17:
				return search_token("<>", BY_PADRAO);
			case 18:
				return search_token("<=", BY_PADRAO);
			case 19:
				prev_char();
				return search_token("<", BY_PADRAO);
			case 20:
				c = next_char();
				if (c == '=') {
					estado = 21;
				} else {
					estado = 22;
				}
				break;
			case 21:
				return search_token(">=", BY_PADRAO);
			case 22:
				prev_char();
				return search_token(">", BY_PADRAO);
			case 23:
				return search_token("+", BY_PADRAO);
			case 24:
				return search_token("-", BY_PADRAO);
			case 25:
				return search_token("*", BY_PADRAO);
			case 26:
				return search_token(",", BY_PADRAO);
			case 27:
				return search_token(";", BY_PADRAO);
			case 28:
				return search_token(":", BY_PADRAO);
			case 29:
				return search_token("(", BY_PADRAO);			
			case 30:
				return search_token(")", BY_PADRAO);
			default:
				break;
		}
		lexema.push_back(c);

#ifdef DEBUG
		printf("%i- %i\n", estado, c);
#endif
	}
}

bool Lexico::load_tokens() {
	fstream tokens_list;
	string tmp_alias, tmp_padrao;
	int count = 0;
	
	tokens_list.open("/Users/gabriel/lexicocompiler/tokens_list.dat", ios_base::in); // TODO: mudar caminho do arquivo
	
	if (!tokens_list) {
		cerr << "Erro ao abrir o arquivo de tokens" << endl;
		return false;
	}
	
	while (tokens_list >> tmp_alias >> tmp_padrao) {
		if (tmp_padrao == "#") {
			tmp_padrao = "";
		}
		tokens.push_back(Token(tmp_alias, tmp_padrao, count++));
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