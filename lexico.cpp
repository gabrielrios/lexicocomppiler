#include "lexico.h"
#include <algorithm>
#include <fstream>
#include <sstream>

char to_lower (const char c) {
	return tolower(c);
}

using namespace std;

Lexico::Lexico(string source_path) {
	fstream _source;
	estado = linha = last_error_line = 0;
	coluna = -1;
	file_name << source_path;
	_source.open(source_path.c_str(), ios_base::in);
	if (!_source) {
		cerr << "Erro ao abrir o código fonte" << endl;
		exit(-1);
	}
	if (!load_tokens()) {
		exit(-1);
	}
	string str;
	tkEOF = search_token("tkEOF", BY_TOKEN);
	while (getline(_source, str)) {
		str.push_back('\n');
		string::size_type st;
		while ((st = str.find('\t')) != string::npos){
            str.replace(st, 1, "  ");
        }
		source.push_back(str);
	}
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
	lexema.push_back(source[linha][coluna]);
	return source[linha][coluna];
}

char Lexico::prev_char() {
	coluna--;
	if (coluna <= -1) {
		linha--;
		coluna = source[linha].size()-1;
	}
	lexema.erase(lexema.size() -1);
	return source[linha][coluna];
}

Token Lexico::is_keyword(string lexema) {
	transform(lexema.begin(), lexema.end(), lexema.begin(), to_lower);
	Token tmp = search_token(lexema, BY_PADRAO);
	if (tmp.is_null()) {
		return search_token("tkIdentificador", BY_TOKEN);
	}
	return tmp;
}

Token Lexico::next_token() {
	Token tk;
	int old_line, old_column;
	char _char;
	estado = 0;
	while (1) {
		switch (estado) {
			case 0:
				lexema = "";
				_char = next_char();
				if (isspace(_char)) {
					estado = 0;
				} else if (isalpha(_char))	{
					estado = 1;
				} else if (isdigit(_char)) {
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
                    list_erros.push_back(Error(linha, coluna, ER_CARACTERE_INVALIDO, _char));
				}
				break;
			case 1:
				_char = next_char();
				if (isalpha(_char) || isdigit(_char) || _char == '_') {
					estado = 1;
				} else {
					estado = 2;
				}
				break;
			case 2:
				prev_char();
				tk = is_keyword(lexema);
				if (tk._id == 0) {
					insert_symbol(tk, lexema, linha+1, (coluna+1)-(lexema.size()-1));
				} ;
				return tk;
			case 3:
				_char = next_char();
				if (isdigit(_char)) {
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
			case 6:
				lexema.erase(lexema.begin());
				lexema.erase(lexema.end()-1);
				tk = search_token("tkLiteral", BY_TOKEN);
				insert_symbol(tk, lexema, linha+1, coluna-(lexema.size()-1));
				return tk;
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
					prev_char();
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
//		lexema.push_back(_char);

#ifdef DEBUG
		printf("%i- %i\n", estado, _char);
#endif
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

    file_symbols << "Tabela de s�mbolos" << endl << endl;
    file_symbols << "Arquivo de entrada: " << file_name.str() << endl << endl << endl;
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
	int linha = -1;
	fstream file_errors;
	stringstream output_name;
    output_name << file_name.str() << "_errors.txt";
	
	file_errors.open(output_name.str().c_str(), ios_base::out);
	
	file_errors << "C�digo com erros encontrados" << endl << endl;
	file_errors << "Arquivo de entrada: " << file_name.str() << endl << endl << endl;
	file_errors << "Linha C�digo" << endl;
	file_errors << "----- -----------------------------------------------------" << endl;
	
    for(int i = 0; i < source.size(); i++){
        sprintf(format, "%5d ", i+1);
        file_errors << format;
        errors = get_line_errors(i);
        if (errors.empty()) {
            file_errors << source[i];
        }else{
            for(int j = 0; j < errors.size(); j++){
                file_errors << source[i];
                sprintf(format, "%5c ", ' ');
                file_errors << format;
                for (int x = errors[j].coluna-1; x >= 0; x--)
					file_errors << "-";
				file_errors << "^ " << endl;
				file_errors << format;
				file_errors << "Erro l�xico na linha " << errors[j].linha+1 << " coluna " << errors[j].coluna+1 << ": ";
				if(errors[j].erro==ER_COMENTARIO_NAO_FECHADO){
                    file_errors << "Coment�rio n�o fechado" << endl;
                }else if(errors[j].erro==ER_LITERAL_NAO_FECHADO){
                    file_errors << "Literal n�o fechado" << endl;
                }else {
                    file_errors << "Caractere inv�lido `" << errors[j].invalido << "'" <<  endl;
                }
            }
        }
    }
	file_errors << endl;
}

Token Lexico::next_token_v1() {
	char _char;
	int old_line, old_column;
	Token tk;
	do {
		lexema = "";
		_char = next_char();
		if (isspace(_char)) {
			continue;
		} else if (isalpha(_char))	{
			do {
				_char = next_char();
			} while (isalpha(_char) || isdigit(_char) || _char == '_');
			prev_char();
			tk = is_keyword(lexema);

			if (tk._id == 0) {
				insert_symbol(tk, lexema, linha+1, (coluna+1)-(lexema.size()-1));
			}
			return tk;
		} else if (isdigit(_char)) {
			do {
				_char = next_char();
			} while (isdigit(_char));
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
