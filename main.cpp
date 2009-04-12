//#define DEBUG
#include <iostream>
#include "lexico.h"
#include <fstream>
//#include "transtable.h"

// Criar arquivo de teste com literal e comentario nâˆšÂ£o fechado

using namespace std;

int main (int argc, char * const argv[]) {
	// Instanciar o lexico
	Lexico *lexico;
	Token tk;
	fstream file_token, file_symbols, file_errors;
	vector<Error> errors;
    char format[10];
	int linha = -1;
	
	file_token.open("file_token.txt", ios_base::out);
	
	file_symbols.open("file_symbols.txt", ios_base::out);
	
	file_errors.open("file_errors.txt", ios_base::out);
	
	if (argc <= 1) {
		cerr << "Necessário arquivo de entrada" << endl;
		exit(-1);
	}
	
	for (int j = 1; j < argc; j++) {

		lexico = new Lexico(argv[j]);
		
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
		
        for(int i = 0; i < lexico->source.size(); i++){
            sprintf(format, "%5d ", i);
            file_errors << format;
            errors = lexico->get_line_errors(i);
            if (errors.empty()) {
                file_errors << lexico->source[i];
            }else{
                for(int j = 0; j < errors.size(); j++){
                    file_errors << lexico->source[i];
                    sprintf(format, "%5c ", ' ');
                    file_errors << format;
                    for (int x = errors[j].coluna-1; x >= 0; x--)
						file_errors << "-";
					file_errors << "^ " << endl;
					file_errors << format;
					file_errors << "Erro léxico na linha " << errors[j].linha << " coluna " << errors[j].coluna << ": ";
					if(errors[j].erro==ER_COMENTARIO_NAO_FECHADO){
                        file_errors << "Comentário não fechado" << endl;
                    }else if(errors[j].erro==ER_LITERAL_NAO_FECHADO){
                        file_errors << "Literal não fechado" << endl;
                    }else {
                        file_errors << "Caractere inválido `" << errors[j].invalido << "'" <<  endl;
                    }
                }
            }
        }

		file_errors << endl;
		
		
		file_symbols << "Pos	    Token	          Lexema	                   Pares (LL,CC) onde LL=linha e CC=coluna" << endl;
		file_symbols << "-----	----------------  --------------------------   ---------------------------------------" << endl;
		
		for (int i = 0; i < lexico->table_symbols.size(); i++) {
			sprintf(format, "%5d", i);
			file_symbols << format << "   " <<  lexico->table_symbols[i].to_str() << endl;
		}
	}
	//system("pause");
    // 
    return 0;
}
