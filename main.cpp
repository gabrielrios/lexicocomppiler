//#define DEBUG
#include <iostream>
#include "lexico.h"
#include <fstream>
#include <sstream>
//#include "transtable.h"

using namespace std;

int main (int argc, char * const argv[]) {
	Lexico *lexico; //inst�ncia da classe L�xico, que cont�m o m�todo do analisador l�xico
	Token tk; //inst�ncia do objeto token
	fstream file_token; //usado para abrir o arquivo com o c�digo fonte
    char format[10], //para formatar os dados a serem impressos no arquivo
         source_name = '\0'; //guarda o nome do arquivo com o c�digo fonte, quando n�o passado por linha de comando
	int linha = -1; //auxilia na contagem das linhas do arquivo
	stringstream output_name; //guarda o nome do arquivo de sa�da com a lista de tokens
	
	//caso n�o seja passado nome de arquivo por linha de comando
	if (argc <= 1) {
	   cout << "Digite o nome do arquivo de entrada:";
	   gets(&source_name);;
	}
	
	//caso seja passado nome de arquivo por linha de comando, estes ser�o analisados
	//a partir deste la�o, que chama o analisador l�xico at� o fim de cada arquivo
	for(int j = 1; j < argc || source_name != '\0'; j++){
        
    	if (argc > 1){lexico = new Lexico(argv[j]);}
    	else{lexico = new Lexico(&source_name);}
        
        //definindo o nome do arquivo de sa�da
    	output_name << lexico->file_name.str() << "_tokens.txt";
    	
    	//abrindo o arquivo para escrita
    	file_token.open(output_name.str().c_str(), ios_base::out);
    	
    	linha = -1;
    	file_token << "Lista de tokens por linha" << endl << endl;
    	file_token << "Arquivo de entrada: " << lexico->file_name.str() << endl << endl << endl;
    	file_token << "Linha  Token             Cod  Pos lexema" << endl;
    	file_token << "-----  ----------------  ---  ----------" << endl;
    	do {
    		tk = lexico->next_token();//chamando o analisador l�xico para que devolva um token
    		if (lexico->linha > linha) {
    			linha = lexico->linha;
    			sprintf(format, "%5d  ", (linha+1));
    		} else {
    			sprintf(format, "%5c  ", ' ');
    		}
    		file_token << format; //colocando o n�mero da linha ou espa�o em branco
    		file_token << tk.to_str(); //imprimindo o token retornado com o c�digo
    		if (tk.alias == "tkIdentificador" || tk.alias == "tkConstante" || tk.alias == "tkLiteral") {
    			sprintf(format, "%5d", lexico->last_symbol_position); 
    			file_token << format; //imprimindo a posi��o do token na tabela de s�mbolos
    		}
    		file_token << endl;
    	} while(tk != lexico->tkEOF) ;
    	
    	file_token << endl;
    	file_token.close(); //fechando o arquivo de sa�da
    	
    	lexico->print_file_errors(); //imprimindo o arquivo com os erros no c�digo
    	lexico->print_file_symbol(); //imprimindo a tabela de s�mbolos
    	
    	output_name.str(""); //limpando o nome do arquivo de sa�da para o pr�ximo
    }
    
    cout << "\tOs arquivos:\n";
    for(int j = 1; j < argc; j++){
        cout << "\t\t" << argv[j] << endl;
    }
    cout << "\tforam tratados pelo analisador l�xico." << endl << endl;
    
    system("pause");
    return 0;
}
