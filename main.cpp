#include <iostream>
#include "lexico.h"

using namespace std;

int main (int argc, char * const argv[]) {
	// Instanciar o lexico
	Lexico lexico;
	
	
	//while(lexico.next_token() != lexico.tkEOF);
	
	cout << lexico.search_token(";", BY_PADRAO).to_str() << endl;
	

    // 
    return 0;
}
