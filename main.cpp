#include <iostream>
#include <vector>
#include <string>
#include "token.h"

using namespace std;

class Position {
    private:
        int coluna, linha;
};

class Symbol {
    private:
        Token token;
		string lexema;
		vector<Position> pares;
};

class Lexico {
	private:
		vector<Symbol> *TableSymbols;
		vector<Token> Tokens;

	public:
		Token *tkEOF;
		//Lexico();	// Abre arquivo fonte, e inicializa tabela de token
		void load_tokens();  // Carrega tokens do arquivo
		Token next_token();	 // Pega o proximo token e retorna para o analisador sintatico
		Token search_token(string lexema); // Busca o token na tabela de tokens
		bool insert_symbol_table(Token tk, string lexema, int linha, int coluna);
};

Token Lexico::next_token() {
	return Token("null");
}

void Lexico::load_tokens() {
	//tkEOF = Token("tkEOF");
	// abrir aqurivo tokens
	// para cada linha do arquivo
	// Instanciar Classe token, com o conteudo da linha
	// Adicionar a lista de tokens
}

int main (int argc, char * const argv[]) {
	// Instanciar o lexico
	Lexico lexico;
	
	
	while(lexico.next_token() != *(lexico.tkEOF));

	cout << "YAY"<< endl;

    // 
    return 0;
}
