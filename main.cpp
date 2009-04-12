//#define DEBUG
#include <iostream>
#include "lexico.h"
#include <fstream>
//#include "transtable.h"

// Criar arquivo de teste com literal e comentario n√£o fechado

using namespace std;

int main (int argc, char * const argv[]) {
	// Instanciar o lexico
	Lexico *lexico;
	Token tk;
	fstream file_token;
	vector<Error> errors;
    char format[10];
	int linha = -1;
	
	file_token.open("file_token.txt", ios_base::out);
	
	if (argc <= 1) {
		cerr << "Necess�rio arquivo de entrada" << endl;
		exit(-1);
	}
	
	lexico = new Lexico(argv[1]);
	
	linha = -1;
	file_token << "Linha  Token             Cod  Pos lexema" << endl;
	file_token << "-----  ----------------  ---  ----------" << endl;
	do {
		tk = lexico->next_token();
		if (lexico->linha > linha) {
			linha = lexico->linha;
			sprintf(format, "%5d  ", (linha+1));	
		} else {
			sprintf(format, "%5c  ", ' ');
		}
		file_token << format;
		file_token << tk.to_str();
		if (tk.alias == "tkIdentificador" || tk.alias == "tkConstante" || tk.alias == "tkLiteral") {
			sprintf(format, "%5d", lexico->last_symbol_position); 
			file_token << format;
		}
		file_token << endl;
	} while(tk != lexico->tkEOF) ;

	file_token << endl;
	
	lexico->print_file_errors();
	lexico->print_file_symbol();

	//system("pause");
    // 
    return 0;
}
