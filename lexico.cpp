/*
 *  lexico.cpp
 *
 *  Created by Gabriel Rios & H�lder Almeida.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "lexico.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>


//encapsulamento da "macro" tolower para poder ser utilizada
//na fun��o transform
char to_lower (const char c) {
	return tolower(c);
}

using namespace std;

//defini��o do construtor da classe Lexico
Lexico::Lexico(string source_path, int _versao) {
    //nome do arquivo de entrada
	fstream _source;
	
	//definindo o estado inicial 0
	estado = 0;
	
	//define a linha de leitura do arquivo 
    linha = 0; 
    
    //�ltima linha que encontrou erro
    last_error_line = 0; 
    
    //define a coluna de leitura do arquivo
	coluna = -1; 
	
	//a vers�o que ser� usada pelo analisador, pode ser 1 ou 2
	versao = _versao; 
	file_name << source_path;
	_source.open(source_path.c_str(), ios_base::in);
	if (!_source) {
		cerr << "Erro ao abrir o c�digo fonte" << endl;
		exit(-1);
	}
	if (!load_tokens()) {
		exit(-1);
	}
	string str;
	tkEOF = search_token("tkEOF", BY_TOKEN);
	
    //pega linha por linha do arquivo de entrada e armazena no vector
	while (getline(_source, str)) {
		
		//colocando '\n' no fim de cada linha, devido � fun��o getline remov�-lo
		str.push_back('\n'); 
		string::size_type st;
		
		//substituindo os '\t's por 2 espa�os
		while ((st = str.find('\t')) != string::npos){ 
            str.replace(st, 1, "  ");
        }
		source.push_back(str);
	}
}

//a depender da escolha da vers�o chama a next_token_v1 ou next_token_v2
Token Lexico::next_token() {
	if (versao == 1) {
		return next_token_v1();
	} else {
		return next_token_v2();
	}
}

//l� o pr�ximo caractere do vector com o c�digo fonte
char Lexico::next_char() {
	coluna++;
	if (coluna >= source[linha].size()) {
		linha++;
		coluna = 0;
	}
	if (linha >= source.size()) {
		return -1;
	}
	
	 //constr�i o lexema, inserindo o caractere atual
	lexema.push_back(source[linha][coluna]);
	return source[linha][coluna];
}

//aponta para o caractere anterior a que o m�todo next_char() apontou
char Lexico::prev_char() {
	coluna--;
	if (coluna <= -1) {
		linha--;
		coluna = source[linha].size()-1;
	}
	
	//apagando o �ltimo caractere inserido no lexema
	lexema.erase(lexema.size() -1); 
	return source[linha][coluna];
}

//verifica se o caractere est� dentro dos valores da tabela ASCII
//que correspondem �s letras de a-z, 97-122
bool Lexico::is_alpha(char _char){
    if(tolower(_char) > 96 && tolower(_char) < 123){
        return true;
    } else {
        return false;
    }
}

//verifica se o caractere est� dentro dos valores da tabela ASCII
//que correspondem aos d�gitos de 0-9, 48-57
bool Lexico::is_digit(char _char){
    if(tolower(_char) > 47 && tolower(_char) < 58){
        return true;
    } else {
        return false;
    }
}

//verifica se o caractere representa algum valor da tabela ASCII
//que corresponde a alguns dos d�gitos ' ', '\n' e '\t', 32, 13 e 9
bool Lexico::is_space(char _char){
    if(_char == ' ' || _char == 32 || _char == '\n' || _char == 13 || _char == '\t' || _char == 9){
        return true;
    } else {
        return false;
    }
}

//verifica se o lexema atual representa uma palavra reservada
//se representar ent�o retorna o token referente � palavra chave
//caso contr�rio retorna o token identificador "tkIdentificador"
Token Lexico::is_keyword(string lexema) {
	transform(lexema.begin(), lexema.end(), lexema.begin(), to_lower);
	Token tmp = search_token(lexema, BY_PADRAO);
	if (tmp.is_null()) {
		return search_token("tkIdentificador", BY_TOKEN);
	}
	return tmp;
}

//carrega a lista de tokens que est� presente no arquivo "tokens_list.dat"
//armazenando em um vetor de tokens
bool Lexico::load_tokens() {
	fstream tokens_list; 
	string tmp_alias, tmp_padrao;
	int count = 0;
	
	tokens_list.open("tokens_list.dat", ios_base::in);
	
	if (!tokens_list) {
		cerr << "Erro ao abrir o arquivo de tokens" << endl;
		return false;
	}
	
	//armazenando o "nome" do token e o seu padr�o em vari�veis auxiliares
	while (tokens_list >> tmp_alias >> tmp_padrao) {
		if (tmp_padrao == "#") {
			tmp_padrao = "";
		}
		tokens.push_back(Token(tmp_alias, tmp_padrao, count++));
	}
	return true;
}

//procura o token no vetor de tokens, a busca pode ser feita pelo padr�o do token
//ou pelo seu pr�prio nome
Token Lexico::search_token(string value, SearchMethod by) {

	// Procura pelo lexema pelo padr�o
	if (by == BY_PADRAO) { 
		for (int i = 0; i < tokens.size(); i++) {
			if (tokens[i].padrao == value) {
				return tokens[i];
			}
		}
	// Procura pelo lexema pelo padr�o
	} else if (by == BY_TOKEN) {  
		for (int i = 0; i < tokens.size(); i++) {
			if (tokens[i].alias == value) {
				return tokens[i];
			}
		}
	}
	return Token();
}

//encontra o lexema na tabela de s�mbolos e retorna sua posi��o ou -1 caso n�o encontre
int Lexico::find_symbol(string lexema) {
	for (int i = 0; i< table_symbols.size(); i++) {
		if (table_symbols[i].lexema == lexema) {
			return i;
		}
	}
	return -1;
}

//insere um token na tabela de s�mbolos, guardando o lexema, a linha e coluna onde foi encontrado
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

//busca todos os erros de uma determina linha e retorna um vetor com estes
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

//gera o arquivo da tabela de s�mbolos
bool Lexico::print_file_symbol(){
    fstream file_symbols;
    char format[10];
    stringstream output_name;
    output_name << file_name.str() << "_simbolos.txt";
    
    file_symbols.open(output_name.str().c_str(), ios_base::out);

    file_symbols << "Tabela de s�mbolos" << endl << endl;
    file_symbols << "Arquivo de entrada (vers�o " << versao << "): " << file_name.str() << endl << endl << endl;
    file_symbols << "Pos	    Token	          Lexema	                   Pares (LL,CC) onde LL=linha e CC=coluna" << endl;
	file_symbols << "-----	----------------  --------------------------   ---------------------------------------" << endl;
	
	for (int i = 0; i < table_symbols.size(); i++) {
		sprintf(format, "%5d", i);
		file_symbols << format << "   " <<  table_symbols[i].to_str() << endl;
	}
	return true;
}

//gera o arquivo com o c�digo fonte e os erros encontrados
bool Lexico::print_file_errors(){
    vector<Error> errors;
    char format[10];
	fstream file_errors;
	stringstream output_name;
    output_name << file_name.str() << "_errors.txt";
	
	file_errors.open(output_name.str().c_str(), ios_base::out);
	
	file_errors << "C�digo com erros encontrados" << endl << endl;
	file_errors << "Arquivo de entrada (vers�o " << versao << "): " << file_name.str() << endl << endl << endl;
	file_errors << "Linha C�digo" << endl;
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
	return true;
}

//executa a an�lise l�xica vers�o 1, sem usar estados
Token Lexico::next_token_v1() {
	//guarda cada caractere a ser analisado
	char _char; 
	
	//a linha e coluna onde o token foi iniciado
	int old_line, old_column; 
	
	//guarda o token a ser retornado
	Token tk; 
	do {
		lexema = "";
		
		//pegando o caractere a ser analisado
		_char = next_char(); 
		
		//verifica se o caractere atual � um espa�o em branco
		//se for volta ao in�cio do la�o para pegar o pr�ximo caractere
		if (is_space(_char)) {
			continue;
			
		//verifica se o caractere atual � um caractere a-zA-z
		} else if (is_alpha(_char))	{ 
			// procura at� encontrar um caractere diferente de a-zA-z ou 0-9 ou _
            do {
				_char = next_char();
			} while (is_alpha(_char) || is_digit(_char) || _char == '_');
			
			//aponta para o caractere anterior
			prev_char(); 
			
			// verifica se o lexema atual � uma palavra reservada
            // ou um token identificador 
			tk = is_keyword(lexema); 

			//inserindo na tabela de s�mbolos caso seja um token identificador
			if (tk._id == 0) {
				insert_symbol(tk, lexema, linha+1, (coluna+1)-(lexema.size()-1));
			}
			
			//retornando o token encontrado
			return tk; 
			
			//verifica se o caractere atual � um d�gito 0-9
		} else if (is_digit(_char)) { 
			//pegando todos os d�gitos da sequ�ncia at� encontrar um caractere que n�o seja
            do {
				_char = next_char();
			} while (is_digit(_char));
			prev_char();
			
			//procurando pelo token constante
			tk = search_token("tkConstante", BY_TOKEN); 
			insert_symbol(tk, lexema, linha+1, (coluna+1)-(lexema.size()-1));
			return tk;
			
			//verifica se o caractere � uma aspas duplas, iniciando um literal
		} else if (_char == '"') { 
			//ignora todos os caracteres at� encontrar aspas duplas novamente, retornando token literal,
			//ou at� encontrar fim de linha, gerando erro
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
			//verifica se o caractere � uma barra, divis�o ou abre coment�rio de linha
		} else if (_char == '/') { 
			_char = next_char();
			
			//se o pr�ximo caractere for um asterisco h� abertura de coment�rio
			if (_char == '*') { 
				
				//e continua pegando caracteres at� encontrar fim de linha
				do {            
					_char = next_char();
				} while (_char != '\n');
				continue;
			//se o pr�ximo caractere ap�s a barra n�o for um asterisco, ent�o
			} else { 
				
				//aponta um caractere antes e retorna o token divis�o
				prev_char(); 
				return search_token(lexema, BY_PADRAO);
			}
		//se o caractere for abre chaves, verifica se � abertura de coment�rio de bloco
		} else if (_char == '{'){ 
			_char = next_char();
			
			//se encontrar um asterisco ap�s o abre chaves, h� abertura de comen�rio de bloco 
			if (_char == '*') { 
				
				//guarda a linha e a coluna onde come�ou o coment�rio
				old_line = linha; 
				old_column = coluna;
				char old_char;
				//ignora todos os caracteres encontrados at� encontrar a sequ�ncia *}
				//que indica fim de coment�rio de bloco, ou at� encontrar o fim do arquivo, indicando erro
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
		//se o caractere atual for um igual pode ser atribui��o ou igualdade (compara��o)
		} else if (_char == '=') {
			_char = next_char();
			//se o pr�ximo caractere for diferente de igual, retorna token atribui��o
			if (_char != '=') { 
				prev_char();
			}
			return search_token(lexema, BY_PADRAO);
			
		//se o caractere atual for o sinal de menor, o pr�ximo caractere determina
		//se h� um token menor, menor ou igual ou diferente
		} else if (_char == '<') { 
			_char = next_char();
			
			//se o pr�ximo caractere n�o for o sinal de maior ou o sinal de igual
			if (_char != '>' && _char != '=') { 
				
				//aponta-se para um caractere anterior, retornando token menor
				prev_char();                    
			}
			return search_token(lexema, BY_PADRAO);
			
		//se o caractere atual for o sinal de maior, o pr�ximo caractere determina
		//se o token ser� maior ou maior ou igual
		} else if (_char == '>') { 
			_char = next_char();
			if (_char != '=') {
				prev_char();
			}
			return search_token(lexema, BY_PADRAO);	
            //caso o caractere atual seja o sinal de mais ou menos ou asterisco ou virgula ou ponto e virgula
            //ou dois pontos ou abre par�nteses ou fecha par�nteses
            //o retorno do token � imediato, dependendo apenas do caractere encontrado atualmente
		} else if (_char == '+' || _char == '-' || _char == '*' || _char == ',' || _char == ';' || _char == ':' || _char == '(' || _char == ')' ) {
			return search_token(lexema, BY_PADRAO);
		//chegou ao fim do arquivo
		} else if (_char == -1) { 
			return tkEOF;
		//qualquer caractere que n�o tenha sido listado gera erro
		} else {
			list_erros.push_back(Error(linha, coluna, ER_CARACTERE_INVALIDO, _char));
		}
	} while (_char != -1);
	
	return Token();
}

//executa a an�lise l�xica vers�o 2, usando estados
Token Lexico::next_token_v2() {
	//guarda o token a ser retornado
	Token tk; 
	//a linha e coluna onde o token foi iniciado
	int old_line, old_column; 
	//guarda cada caractere a ser analisado
	char _char; 
	//o primeiro estado, estado inicial
	estado = 0; 
	while (1) {
		switch (estado) {
            //no estado 0 recebe-se o primeiro caractere para definir o token,
            //ap�s l�-lo identifica-se entre as op��es abaixo para qual estado seguir
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
                    //caso o caractere n�o seja v�lido adiciona um erro na lista de erros
                    list_erros.push_back(Error(linha, coluna, ER_CARACTERE_INVALIDO, _char));
				}
				break;
			//caso tenha encontrado uma letra no estado 0
			//ou ter encontrado letra ou d�gito no estado 1
			case 1:
				_char = next_char();
				if (is_alpha(_char) || is_digit(_char) || _char == '_') {
					estado = 1;
				} else {
					estado = 2;
				}
				break;
			//caso tenha encontrado, no estado 1, algum caractere 
            //que n�o possa entrar no padr�o do identificador
            //retorna o token identificador ou o token da palavra reservada
			case 2:
				prev_char();
				tk = is_keyword(lexema);
				if (tk._id == 0) {
					insert_symbol(tk, lexema, linha+1, (coluna+1)-(lexema.size()-1));
				} ;
				return tk;
			//caso tenha encontrado um d�gito no estado 0
			//continua neste estado at� encontrar algum caractere que n�o seja d�gito
			//quando encontra segue para o estado 4
			case 3:
				_char = next_char();
				if (is_digit(_char)) {
					estado = 3;
				} else {
					estado = 4;
				}
				break;
			//foi encontrado algum caractere diferente de d�gito no estado 3
            //retorna o token constante
			case 4:
				prev_char();
				tk = search_token("tkConstante", BY_TOKEN);
				insert_symbol(tk, lexema, linha+1, (coluna+1)-(lexema.size()-1));
				return tk;
			//quando encontra aspas duplas, definindo assim o in�cio de um literal\
			//segue para o estado 6 caso encontre outra aspas duplas
			//segue para o estado 0 caso encontre final de linha, gerando um erro na lista de erros
			//continua no estado 5 caso n�o encontre aspas duplas ou fim de linha
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
			//se for encontrado uma barra / no estado 0, ent�o chegamos neste estado
            //que segue para o estado 8 caso encontre um asterisco no pr�ximo caractere
            //ou segue para o estado 9 caso encontre algum caractere diferente de asterisco
			case 7:
				_char = next_char();
				if (_char == '*') {
					estado = 8;
				} else {
					estado = 9;
				}
				break;
			//caso tenha encontrado um asterisco no estado 7, isso siginifica
			//que foi encontrado uma abertura de coment�rio de �nica linha
			//quando encontrar o fim de linha, \n, voltar� ao estado inicial, 0
			case 8:
				_char = next_char();
				if (_char == '\n') {
					estado = 0;
				}
				break;
			//caso n�o tenha encontrado um asterisco no estado 7, isso significa
			//que a barra representa uma divis�o, retornando assim o token correspondente
			case 9:
                prev_char();
				return search_token("/", BY_PADRAO);
			//se for encontrado um caractere abre chaves, {, no estado 0, ent�o chegamos neste estado
            //que segue para o estado 11 caso encontre um asterisco no pr�ximo caractere
            //ou segue para o estado 0 caso encontre algum caractere diferente de asterisco
            //sinalizando erro por caractere inv�lido, {
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
			//caso tenha encontrado um asterisco no estado 10, isso significa
			//que o abre chaves representa abertura de coment�rio de bloco
			//se encontrar um asterisco deve seguir para o estado 12
			//se encontrar fim de arquivo, deve-se sinalizar erro por coment�rio n�o fechado
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
			//caso tenha sido encontrado asterisco no estado 12, ent�o chega-se a este estado
			//se encontrar o caractere fecha aspas, }, significa que o coment�rio foi fechado e
			//deve-se continuar a an�lise a partir do pr�ximo caractere, voltando para o estado 0,
			//mas se for encontrado outro asterisco, deve-se continuar no estado 12
			//at� encontrar o fecha aspas ou outro caractere, o que faz voltar para o estado 11
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
			//caso tenha encontrado sinal de igual no estado 0, chega-se neste estado
			//aqui se encontrar um outro sinal de igual segue-se para o estado 14
			//encontrando qualquer outro caractere segue para o estado 15
			case 13:
				_char = next_char();
				if (_char == '=') {
					estado = 14;
				} else {
					estado = 15;
				}
				break;
			//caso tenha encontrado sinal de igual no estado 13, chega-se neste estado
			//retornando o token igual
			case 14:
				return search_token("==", BY_PADRAO);
			//caso n�o tenha achado sinal de igual no estado 13, retorna-se o token atribui��o
			case 15:
				prev_char();
				return search_token("=", BY_PADRAO);
			//caso tenha encontrado sinal de menor no estado 0, chega-se neste estado
			//aqui se encontrar um sinal de maior segue-se para o estado 17
			//encontrando sinal de igual, segue-se para o estado 18
			//e n�o encontrando ambos, segue para o estado 19
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
			//caso tenha achado sinal de maior no estado 16, ent�o chega-se neste estado
			//e retorna-se token diferente
			case 17:
				return search_token("<>", BY_PADRAO);
			//caso tenha encontrado sinal de igual no estado 16, chega-se neste estado
			//e retorna-se token menor ou igual
			case 18:
				return search_token("<=", BY_PADRAO);
			//caso n�o tenha encontrado sinal de maior, ou sinal de igual, chega-se nesse estado
			//retornando o token menor
			case 19:
				prev_char();
				return search_token("<", BY_PADRAO);
			//caso tenha encontrado sinal de maior no estado 0, chega-se neste estado
			//aqui se encontrar um sinal de igual segue-se para o estado 21
			//e n�o encontrando sinal de igual, segue para o estado 22
			case 20:
				_char = next_char();
				if (_char == '=') {
					estado = 21;
				} else {
					estado = 22;
				}
				break;
			//caso tenha encontrado sinal de igual no estado 20, chega-se neste estado
			//e retorna-se token maior ou igual
			case 21:
				return search_token(">=", BY_PADRAO);
			//caso n�o tenha encontrado sinal de igual chega-se nesse estado
			//retornando o token maior
			case 22:
				prev_char();
				return search_token(">", BY_PADRAO);
			//se no estado 0, encontra sinal de mais, menos, multiplica��o, v�rgula, ponto e v�rgula
			//abre par�nteses, fecha par�nteses e dois pontos
			//retorna-se o token correspondente a este lexema
			case 23:
				return search_token(lexema, BY_PADRAO);
			default:
				break;
		}
	}
}
