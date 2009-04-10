//#define DEBUG
#include <iostream>
#include "lexico.h"

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
	
	lexico = new Lexico(argv[1]);
	
	do {
		tk = lexico->next_token();
		cout << tk.to_str() << endl;
	} while(tk != lexico->tkEOF) ;


    // 
    return 0;
}
