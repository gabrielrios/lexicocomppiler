//#define DEBUG
#include <iostream>
#include "lexico.h"

using namespace std;

int main (int argc, char * const argv[]) {
	// Instanciar o lexico
	Lexico *lexico;
	
	if (argc <= 1) {
		cerr << "Necessário arquivo de entrada" << endl;
		exit(-1);
	}
	
	lexico = new Lexico(argv[1]);


	
	for (int i =0; i < 6; i++) {
		cout << lexico->next_token().to_str() << endl;	
	}
	
	//while(lexico.next_token() != lexico.tkEOF);


    // 
    return 0;
}
