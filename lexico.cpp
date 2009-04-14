/*
 *  lexico.cpp
 *
 *  Created by Gabriel Rios & Hélder Almeida.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "lexico.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>


//encapsulamento da "macro" tolower para poder ser utilizada
//na função transform
char to_lower (const char c) {
	return tolower(c);
}

using namespace std;

//definição do construtor da classe Lexico
Lexico::Lexico(string source_path, int _versao) {
	fstream _source; //nome do arquivo de entrada
	estado = 0; //definindo o estado inicial 0
    linha = 0; //define a linha de leitura do arquivo
    last_error_line = 0; //última linha que encontrou erro
	coluna = -1; //define a coluna de leitura do arquivo
	versao = _versao; //a versão que será usada pelo analisador, pode ser 1 ou 2
	file_name << source_path;
	_source.open(source_path.c_str(), ios_base::in);
	if (!_source) {
		cerr << "Erro ao abrir o câˆšâ‰¥digo fonte" << endl;
		exit(-1);
	}
	if (!load_tokens()) {
		exit(-1);
	}
	string str;
	tkEOF = search_token("tkEOF", BY_TOKEN);
    //pega linha por linha do arquivo de entrada e armazena no vector
	while (getline(_source, str)) {
		str.push_back('\n'); //colocando '\n' no fim de cada linha, devido à função getline removê-lo
		string::size_type st;
		while ((st = str.find('\t')) != string::npos){ //substituindo os '\t's por 2 espaços
            str.replace(st, 1, "  ");
        }
		source.push_back(str);
	}
}

//a depender da escolha da versão chama a next_token_v1 ou next_token_v2
Token Lexico::next_token() {
	if (versao == 1) {
		return next_token_v1();
	} else {
		return next_token_v2();
	}
}

//lê o próximo caractere do vector com o código fonte
char Lexico::next_char() {
	coluna++;
	if (coluna >= source[linha].size()) {
		linha++;
		coluna = 0;
	}
	if (linha >= source.size()) {
		return -1;
	}
	lexema.push_back(source[linha][coluna]); //constrói o lexema, inserindo o caractere atual
	return source[linha][coluna];
}

//aponta para o caractere anterior a que o método next_char() apontou
char Lexico::prev_char() {
	coluna--;
	if (coluna <= -1) {
		linha--;
		coluna = source[linha].size()-1;
	}
	lexema.erase(lexema.size() -1); //apagando o último caractere inserido no lexema
	return source[linha][coluna];
}

bool Lexico::is_alpha(char _char){
    if(tolower(_char) > 96 && tolower(_char) < 123){
        return true;
    } else {
        return false;
    }
}
bool Lexico::is_digit(char _char){
    if(tolower(_char) > 47 && tolower(_char) < 58){
        return true;
    } else {
        return false;
    }
}
bool Lexico::is_space(char _char){
    if(_char == ' ' || _char == 32 || _char == '\n' || _char == 13 || _char == '\t' || _char == 9){
        return true;
    } else {
        return false;
    }
}

//verifica se o lexema atual representa uma palavra reservada
//se representar então retorna o token referente à palavra chave
//caso contrário retorna o token identificador "tkIdentificador"
Token Lexico::is_keyword(string lexema) {
	transform(lexema.begin(), lexema.end(), lexema.begin(), to_lower);
	Token tmp = search_token(lexema, BY_PADRAO);
	if (tmp.is_null()) {
		return search_token("tkIdentificador", BY_TOKEN);
	}
	return tmp;
}

//executa a análise léxica versão 2, usando estados
Token Lexico::next_token_v2() {
	Token tk; //guarda o token a ser retornado
	int old_line, old_column; //a linha e coluna onde o token foi iniciado
	char _char; //guarda cada caractere a ser analisado
	estado = 0; //o primeiro estado, estado inicial
	while (1) {
		switch (estado) {
            //no estado 0 recebe-se o primeiro caractere para definir o token,
            //após lê-lo identifica-se entre as opções abaixo para qual estado seguir
			case 0:
				lexema = "";
				_char = next_char();
				if (is_space(_char)) {
					estado = 0;
				} else if (is_alpha(_char))	{
					estado = 1;
				} else if (is_digit(_char)) {
					estado = 3;
				} else if (_char == '"') {
					estado = 5;
				} else if (_char == '/') {
					estado = 7;
				} else if (_char == '{'){
					estado = 10;
				} else if (_char == '=') {
					estado = 13;
				} else if (_char == '<') {
					estado = 16;
				} else if (_char == '>') {
					estado = 20;
				} else if (_char == '+' || _char == '-' || _char == '*' || _char == ',' || _char == ';' || _char == ':' || _char == '(' || _char == ')' ) {
					estado = 23;
				} else if (_char == -1) {
					return tkEOF;
				} else {
                    //caso o caractere não seja válido adiciona um erro na lista de erros
                    list_erros.push_back(Error(linha, coluna, ER_CARACTERE_INVALIDO, _char));
				}
				break;
			//caso tenha encontrado uma letra no estado 0
			//ou ter encontrado letra ou dígito no estado 1
			case 1:
				_char = next_char();
				if (is_alpha(_char) || is_digit(_char) || _char == '_') {
					estado = 1;
				} else {
					estado = 2;
				}
				break;
			//caso tenha encontrado, no estado 1, algum caractere 
            //que não possa entrar no padrão do identificador
            //retorna o token identificador ou o token da palavra reservada
			case 2:
				prev_char();
				tk = is_keyword(lexema);
				if (tk._id == 0) {
					insert_symbol(tk, lexema, linha+1, (coluna+1)-(lexema.size()-1));
				} ;
				return tk;
			//caso tenha encontrado um dígito no estado 0
			//continua neste estado até encontrar algum caractere que não seja dígito
			//quando encontra segue para o estado 4
			case 3:
				_char = next_char();
				if (isdigit(_char)) {
					estado = 3;
				} else {
					estado = 4;
				}
				break;
			//foi encontrado algum caractere diferente de dígito no estado 3
            //retorna o token constante
			case 4:
				prev_char();
				tk = search_token("tkConstante", BY_TOKEN);
				insert_symbol(tk, lexema, linha+1, (coluna+1)-(lexema.size()-1));
				return tk;
			//quando encontra aspas duplas, definindo assim o início de um literal\
			//segue para o estado 6 caso encontre outra aspas duplas
			//segue para o estado 0 caso encontre final de linha, gerando um erro na lista de erros
			//continua no estado 5 caso não encontre aspas duplas ou fim de linha
			case 5:
				_char = next_char();				
				if (_char == '"') {
					estado = 6;
				} else if (_char == '\n') {
					estado = 0;
					list_erros.push_back(Error(linha, source[linha].size()-lexema.size()+1, ER_LITERAL_NAO_FECHADO));
				} else {
					estado = 5;
				}
				break;
			//caso tenha encontrado aspas duplas no estado 5, finalizando o literal
			//armazena o literal, sem as aspas, no lexema\
			//retorna o token literal
			case 6:
				lexema.erase(lexema.begin());
				lexema.erase(lexema.end()-1);
				tk = search_token("tkLiteral", BY_TOKEN);
				insert_symbol(tk, lexema, linha+1, coluna-(lexema.size()-1));
				return tk;
			//
			case 7:
				_char = next_char();
				if (_char == '*') {
					estado = 8;
				} else {
					estado = 9; // ?
				}
				break;
			case 8:
				_char = next_char();
				if (_char == '\n') {
					estado = 0;
				}
				break;
			case 9:
                prev_char();
				return search_token("/", BY_PADRAO);
			case 10:
				_char = next_char();
				if (_char == '*') {
					estado = 11;
					old_line = linha;
					old_column = coluna;
				} else {
					_char = prev_char();
					estado = 0;
					list_erros.push_back(Error(linha, coluna, ER_CARACTERE_INVALIDO, _char));
				}
				break;
			case 11:
				_char = next_char();
				if (_char == '*') {
					estado = 12;
				} else if (_char == -1) {
					estado = 0;
					list_erros.push_back(Error(old_line, old_column, ER_COMENTARIO_NAO_FECHADO));
					linha = old_line+1;
					coluna = 0;
				}
				break;
			case 12:
				_char = next_char();
				if (_char == '}') {
					estado = 0;
				} else if (_char == '*') {
					estado = 12;
				} else {
					prev_char();
					estado = 11;
				}
				break;
			case 13:
				_char = next_char();
				if (_char == '=') {
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
				_char = next_char();
				if (_char == '>') {
					estado = 17;
				} else if (_char == '=') {
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
				_char = next_char();
				if (_char == '=') {
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
				return search_token(lexema, BY_PADRAO);
			default:
				break;
		}
	}
}

bool Lexico::load_tokens() {
	fstream tokens_list;
	string tmp_alias, tmp_padrao;
	int count = 0;
	
	tokens_list.open("tokens_list.dat", ios_base::in); // TODO: mudar caminho do arquivo
	
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
	for(int i = 0; i < tokens.size(); i++) {
		cout << tokens[i].to_str() << tokens[i].padrao << endl;
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
		last_symbol_position = table_symbols.size()-1;
	} else {
		table_symbols[pos_sym].add_position(linha, coluna);
		last_symbol_position = pos_sym;
	}
	return true;
}

vector<Error> Lexico::get_line_errors(int line){
    vector<Error> _errors;
    
    for(int i = last_error_line; i < list_erros.size(); i++){
        if (list_erros[i].linha == line){
            _errors.push_back(list_erros[i]);
        }
        else{
            last_error_line = i;
            break;
        }
    }
    return _errors;
}

bool Lexico::print_file_symbol(){
    fstream file_symbols;
    char format[10];
    stringstream output_name;
    output_name << file_name.str() << "_simbolos.txt";
    
    file_symbols.open(output_name.str().c_str(), ios_base::out);

    file_symbols << "Tabela de sÃŒmbolos" << endl << endl;
    file_symbols << "Arquivo de entrada (versÃ£o " << versao << "): " << file_name.str() << endl << endl << endl;
    file_symbols << "Pos	    Token	          Lexema	                   Pares (LL,CC) onde LL=linha e CC=coluna" << endl;
	file_symbols << "-----	----------------  --------------------------   ---------------------------------------" << endl;
	
	for (int i = 0; i < table_symbols.size(); i++) {
		sprintf(format, "%5d", i);
		file_symbols << format << "   " <<  table_symbols[i].to_str() << endl;
	}
	return true;
}

bool Lexico::print_file_errors(){
    vector<Error> errors;
    char format[10];
	fstream file_errors;
	stringstream output_name;
    output_name << file_name.str() << "_errors.txt";
	
	file_errors.open(output_name.str().c_str(), ios_base::out);
	
	file_errors << "CÃ›digo com erros encontrados" << endl << endl;
	file_errors << "Arquivo de entrada (versÃ£o " << versao << "): " << file_name.str() << endl << endl << endl;
	file_errors << "Linha CÃ›digo" << endl;
	file_errors << "----- -----------------------------------------------------" << endl;
	
    for(int i = 0; i < source.size(); i++){
        errors = get_line_errors(i);
        if (errors.empty()) {
			sprintf(format, "%5d ", i+1);
			file_errors << format;
            file_errors << source[i];
        }else{
            for(int j = 0; j < errors.size(); j++){
				sprintf(format, "%5d ", i+1);
				file_errors << format;
                file_errors << source[i];
                sprintf(format, "%5c ", ' ');
                file_errors << format;
                for (int x = errors[j].coluna-1; x >= 0; x--)
					file_errors << "-";
				file_errors << "^ " << endl;
				file_errors << format;
				file_errors << "Erro lÃˆxico na linha " << errors[j].linha+1 << " coluna " << errors[j].coluna+1 << ": ";
				if(errors[j].erro==ER_COMENTARIO_NAO_FECHADO){
                    file_errors << "ComentÂ·rio nâ€žo fechado" << endl;
                }else if(errors[j].erro==ER_LITERAL_NAO_FECHADO){
                    file_errors << "Literal nâ€žo fechado" << endl;
                }else {
                    file_errors << "Caractere invÂ·lido `" << errors[j].invalido << "'" <<  endl;
                }
            }
        }
    }
	file_errors << endl;
	return true;
}

Token Lexico::next_token_v1() {
	char _char;
	int old_line, old_column;
	Token tk;
	do {
		lexema = "";
		_char = next_char();
		if (is_space(_char)) {
			continue;
		} else if (is_alpha(_char))	{
			do {
				_char = next_char();
			} while (is_alpha(_char) || is_digit(_char) || _char == '_');
			prev_char();
			tk = is_keyword(lexema);

			if (tk._id == 0) {
				insert_symbol(tk, lexema, linha+1, (coluna+1)-(lexema.size()-1));
			}
			return tk;
		} else if (is_digit(_char)) {
			do {
				_char = next_char();
			} while (is_digit(_char));
			prev_char();
			tk = search_token("tkConstante", BY_TOKEN);
			insert_symbol(tk, lexema, linha+1, (coluna+1)-(lexema.size()-1));
			return tk;
		} else if (_char == '"') {
			do {
				_char = next_char();
			} while (_char != '"' && _char != '\n');
			if (_char == '"') {
				lexema.erase(lexema.begin());
				lexema.erase(lexema.end()-1);
				tk = search_token("tkLiteral", BY_TOKEN);
				insert_symbol(tk, lexema, linha+1, (coluna+1)-(lexema.size()-1));
				return tk;
			} else {
				list_erros.push_back(Error(linha, source[linha].size()-lexema.size()+1, ER_LITERAL_NAO_FECHADO));
				continue;
			}
		} else if (_char == '/') {
			_char = next_char();
			if (_char == '*') {
				do {
					_char = next_char();
				} while (_char != '\n');
				continue;
			} else {
				prev_char();
				return search_token(lexema, BY_PADRAO);
			}
		} else if (_char == '{'){
			_char = next_char();
			if (_char == '*') {
				old_line = linha;
				old_column = coluna;
				char old_char;
				do {
					old_char = _char;
					_char = next_char();
					if (_char == -1) {
						list_erros.push_back(Error(old_line, old_column, ER_COMENTARIO_NAO_FECHADO, _char));
						linha = old_line + 1;
						coluna = 0;
						break;
					}
				} while(!(old_char == '*' && _char == '}'));
				continue;
			} else {
				_char = prev_char();
				list_erros.push_back(Error(linha, coluna, ER_CARACTERE_INVALIDO, _char));
				continue;
			}
		} else if (_char == '=') {
			_char = next_char();
			if (_char != '=') {
				prev_char();
			}
			return search_token(lexema, BY_PADRAO);
		} else if (_char == '<') {
			_char = next_char();
			if (_char != '>' && _char != '=') {
				prev_char();
			}
			return search_token(lexema, BY_PADRAO);
		} else if (_char == '>') {
			_char = next_char();
			if (_char != '=') {
				prev_char();
			}
			return search_token(lexema, BY_PADRAO);			
		} else if (_char == '+' || _char == '-' || _char == '*' || _char == ',' || _char == ';' || _char == ':' || _char == '(' || _char == ')' ) {
			return search_token(lexema, BY_PADRAO);
		} else if (_char == -1) {
			return tkEOF;
		} else {
			list_erros.push_back(Error(linha, coluna, ER_CARACTERE_INVALIDO, _char));
		}
	} while (_char != -1);
	
	return Token();
}
