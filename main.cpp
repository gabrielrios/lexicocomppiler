//#define DEBUG
#include <iostream>
#include "lexico.h"
#include <fstream>
#include <sstream>
//#include "transtable.h"

// Criar arquivo de teste com literal e comentario n√£o fechado

using namespace std;

int main (int argc, char * const argv[]) {
	Lexico *lexico;
	Token tk;
	fstream file_token;
    char format[10];
	int linha = -1;
	stringstream output_name;
	
	if (argc <= 1) {
		cerr << "Necess�rio arquivo de entrada" << endl;
		exit(-1);
	}
	
	for(int j = 1; j < argc; j++){
        
    	lexico = new Lexico(argv[j]);
    	
    	output_name << lexico->file_name.str() << "_tokens.txt";
    	file_token.open(output_name.str().c_str(), ios_base::out);
    	
    	cout << output_name.str() << endl;
    	
    	linha = -1;
    	file_token << "Lista de tokens por linha" << endl << endl;
    	file_token << "Arquivo de entrada: " << lexico->file_name.str() << endl << endl << endl;
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
        output_name.str("");
    	file_token << endl;
    	
    	lexico->print_file_errors();
    	lexico->print_file_symbol();
    }
	//system("pause");
    // 
    return 0;
}
