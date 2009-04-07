
#include <iostream>
#include "lexico.h"

using namespace std;

int main (int argc, char * const argv[]) {
	// Instanciar o lexico
	Lexico lexico;
	
	
	//while(lexico.next_token() != lexico.tkEOF);
	
#ifdef DEBUG
	lexico.insert_symbol(lexico.search_token(";", BY_PADRAO), ";", 0, 1);
	lexico.insert_symbol(lexico.search_token(";", BY_PADRAO), ";", 1, 1);
	lexico.insert_symbol(lexico.search_token(",", BY_PADRAO), ",", 1, 1);
	
	for (int i = 0; i< lexico.table_symbols.size(); i++) {
		cout << lexico.table_symbols[i].to_str() << endl;
	}
#endif

    // 
    return 0;
}
