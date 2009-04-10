//#define DEBUG
#include <iostream>
#include "lexico.h"
//#include "transtable.h"

// Criar arquivo de teste com literal e comentario não fechado

using namespace std;

int main (int argc, char * const argv[]) {
	// Instanciar o lexico
	Lexico *lexico;
	Token tk;
	
	if (argc <= 1) {
		cerr << "Necessário arquivo de entrada" << endl;
		exit(-1);
	}
	
	for (int j = 1; j < argc; j++) {

		lexico = new Lexico(argv[j]);
		
		do {
			tk = lexico->next_token();
		} while(tk != lexico->tkEOF) ;

		cout << endl << "######################################################"<< endl;
		cout << "#\t\t\t\t\tTabela de Simbolos\t\t\t\t\t  #"<< endl;
		cout << "######################################################"<< endl << endl;
		
		for (int i = 0; i < lexico->table_symbols.size(); i++) {
			cout << i << " | "<<  lexico->table_symbols[i].to_str() << endl;
		}
	}
	system("pause");
    // 
    return 0;
}
