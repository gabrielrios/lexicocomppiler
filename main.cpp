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
	fstream output;
	vector<Error> errors;
    char format[10];
	int linha = -1;
	
	output.open("output.txt", ios_base::out);
	
	if (argc <= 1) {
		cerr << "Necess√°rio arquivo de entrada" << endl;
		exit(-1);
	}
	
	for (int j = 1; j < argc; j++) {

		lexico = new Lexico(argv[j]);
		
		linha = -1;
		output << "Linha  Token             Cod  Pos lexema" << endl;
		//output << "-----  ----------------  ---  ----------"<< endl;
		do {
			tk = lexico->next_token();
			if (lexico->linha > linha) {
				linha = lexico->linha;
				sprintf(format, "%5d  ", (linha+1));	
			} else {
				sprintf(format, "%5c  ", ' ');
			}
			output << format;
			output << tk.to_str();
			if (tk.alias == "tkIdentificador" || tk.alias == "tkConstante" || tk.alias == "tkLiteral") {
				sprintf(format, "%5d", lexico->last_symbol_position); 
				output << format;
			}
			output << endl;
		} while(tk != lexico->tkEOF) ;

		output << endl << endl << endl;
		
        for(int i = 0; i < lexico->source.size(); i++){
            sprintf(format, "%5d ", i);
            output << format;
            errors = lexico->get_line_errors(i);
            if (errors.empty()) {
                output << lexico->source[i];
            }else{
                for(int j = 0; j < errors.size(); j++){
                    output << lexico->source[i];
                    sprintf(format, "%5c ", ' ');
                    output << format;
                    for (int x = errors[j].coluna-1; x >= 0; x--)
						output << "-";
					output << "^ " << endl;
					output << format;
					output << "Erro lÈxico na linha " << errors[j].linha << " coluna " << errors[j].coluna << ": ";
					if(errors[j].erro==ER_COMENTARIO_NAO_FECHADO){
                        output << "Coment·rio n„o fechado" << endl;
                    }else if(errors[j].erro==ER_LITERAL_NAO_FECHADO){
                        output << "Literal n„o fechado" << endl;
                    }else {
                        output << "Caractere inv·lido `" << errors[j].invalido << "'" <<  endl;
                    }
                }
            }
        }

		output << endl;
		output << "Pos	    Token	          Lexema	                   Pares (LL,CC) onde LL=linha e CC=coluna" << endl;
		output << "-----	----------------  --------------------------   ---------------------------------------" << endl;
		
		for (int i = 0; i < lexico->table_symbols.size(); i++) {
			sprintf(format, "%5d", i);
			output << format << "   " <<  lexico->table_symbols[i].to_str() << endl;
		}
	}
	//system("pause");
    // 
    return 0;
}
