//#define DEBUG
#include <iostream>
#include "lexico.h"
#include <fstream>
#include <sstream>
//#include "transtable.h"

using namespace std;

int main (int argc, char * const argv[]) {
	Lexico *lexico; //instância da classe Léxico, que contém o método do analisador léxico
	Token tk; //instância do objeto token
	fstream file_token; //usado para abrir o arquivo com o código fonte
    char format[10], //para formatar os dados a serem impressos no arquivo
         source_name = '\0'; //guarda o nome do arquivo com o código fonte, quando não passado por linha de comando
	int linha = -1; //auxilia na contagem das linhas do arquivo
	stringstream output_name; //guarda o nome do arquivo de saída com a lista de tokens
	
	//caso não seja passado nome de arquivo por linha de comando
	if (argc <= 1) {
	   cout << "Digite o nome do arquivo de entrada:";
	   gets(&source_name);;
	}
	
	//caso seja passado nome de arquivo por linha de comando, estes serão analisados
	//a partir deste laço, que chama o analisador léxico até o fim de cada arquivo
	for(int j = 1; j < argc || source_name != '\0'; j++){
        
    	if (argc > 1){lexico = new Lexico(argv[j]);}
    	else{lexico = new Lexico(&source_name);}
        
        //definindo o nome do arquivo de saída
    	output_name << lexico->file_name.str() << "_tokens.txt";
    	
    	//abrindo o arquivo para escrita
    	file_token.open(output_name.str().c_str(), ios_base::out);
    	
    	linha = -1;
    	file_token << "Lista de tokens por linha" << endl << endl;
    	file_token << "Arquivo de entrada: " << lexico->file_name.str() << endl << endl << endl;
    	file_token << "Linha  Token             Cod  Pos lexema" << endl;
    	file_token << "-----  ----------------  ---  ----------" << endl;
    	do {
    		tk = lexico->next_token();//chamando o analisador léxico para que devolva um token
    		if (lexico->linha > linha) {
    			linha = lexico->linha;
    			sprintf(format, "%5d  ", (linha+1));
    		} else {
    			sprintf(format, "%5c  ", ' ');
    		}
    		file_token << format; //colocando o número da linha ou espaço em branco
    		file_token << tk.to_str(); //imprimindo o token retornado com o código
    		if (tk.alias == "tkIdentificador" || tk.alias == "tkConstante" || tk.alias == "tkLiteral") {
    			sprintf(format, "%5d", lexico->last_symbol_position); 
    			file_token << format; //imprimindo a posição do token na tabela de símbolos
    		}
    		file_token << endl;
    	} while(tk != lexico->tkEOF) ;
    	
    	file_token << endl;
    	file_token.close(); //fechando o arquivo de saída
    	
    	lexico->print_file_errors(); //imprimindo o arquivo com os erros no código
    	lexico->print_file_symbol(); //imprimindo a tabela de símbolos
    	
    	output_name.str(""); //limpando o nome do arquivo de saída para o próximo
    }
    
    cout << "\tOs arquivos:\n";
    for(int j = 1; j < argc; j++){
        cout << "\t\t" << argv[j] << endl;
    }
    cout << "\tforam tratados pelo analisador léxico." << endl << endl;
    
    system("pause");
    return 0;
}
