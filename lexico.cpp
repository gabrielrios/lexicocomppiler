/*
 *  lexico.cpp
 *  lexicocompiler
 *
 *  Created by Gabriel Rios on 07/04/09.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#include "lexico.h"
#include <fstream>
#include <iostream>

using namespace std;

Lexico::Lexico() {
	tkEOF = Token("tkEOF");
	load_tokens();
}

Token Lexico::next_token() {
	return Token("tkEOF");
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
	for(int i = 0; i < tokens.size(); i++) {
		cout << tokens[i].to_str() << endl;
	}
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