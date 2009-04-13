/*
 *  main.cpp
 *
 *  Created by Gabriel Rios & Hélder Almeida.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

//#define DEBUG
#include <iostream>
#include "lexico.h"
#include <fstream>
#include <sstream>

using namespace std;

int main (int argc, char * const argv[]) {
	Lexico *lexico; //inst‚ncia da classe LÈxico, que contÈm o mÈtodo do analisador lÈxico
	Token tk; //inst‚ncia do objeto token
	fstream file_token; //usado para abrir o arquivo com o cÛdigo fonte
    char format[10]; //para formatar os dados a serem impressos no arquivo
    string source_name; //guarda o nome do arquivo com o cÛdigo fonte, quando n„o passado por linha de comando
	int linha = -1, versao; //auxilia na contagem das linhas do arquivo
	stringstream output_name; //guarda o nome do arquivo de saÌda com a lista de tokens
	
	//caso n„o seja passado nome de arquivo por linha de comando
	if (argc <= 1) {
	   cout << "Digite o nome do arquivo de entrada:";
	   cin >> source_name;
	}
	
	do {
		cout << "Qual versão do analisador deseja utilizar?";
		cin >> versao;
	} while (versao != 1 && versao != 2);
	
	//caso seja passado nome de arquivo por linha de comando, estes ser„o analisados
	//a partir deste laÁo, que chama o analisador lÈxico atÈ o fim de cada arquivo
	for(int j = 1; j < argc || !source_name.empty(); j++, source_name = ""){
        
    	if (argc > 1) {
			lexico = new Lexico(argv[j], versao);
		} else {
			lexico = new Lexico(source_name, versao);
		}
        
        //definindo o nome do arquivo de saÌda
    	output_name << lexico->file_name.str() << "_tokens.txt";
    	
    	//abrindo o arquivo para escrita
    	file_token.open(output_name.str().c_str(), ios_base::out);
    	
    	linha = -1;
    	file_token << "Lista de tokens por linha" << endl << endl;
    	file_token << "Arquivo de entrada (versão " << versao << "): " << lexico->file_name.str() << endl << endl << endl;
    	file_token << "Linha  Token             Cod  Pos lexema" << endl;
    	file_token << "-----  ----------------  ---  ----------" << endl;
    	do {
    		tk = lexico->next_token();//chamando o analisador lÈxico para que devolva um token
    		if (lexico->linha > linha) {
    			linha = lexico->linha;
    			sprintf(format, "%5d  ", (linha+1));
    		} else {
    			sprintf(format, "%5c  ", ' ');
    		}
    		file_token << format; //colocando o n˙mero da linha ou espaÁo em branco
    		file_token << tk.to_str(); //imprimindo o token retornado com o cÛdigo
    		if (tk.alias == "tkIdentificador" || tk.alias == "tkConstante" || tk.alias == "tkLiteral") {
    			sprintf(format, "%5d", lexico->last_symbol_position); 
    			file_token << format; //imprimindo a posiÁ„o do token na tabela de sÌmbolos
    		}
    		file_token << endl;
    	} while(tk != lexico->tkEOF) ;
    	
    	file_token << endl;
    	file_token.close(); //fechando o arquivo de saÌda
    	
    	lexico->print_file_errors(); //imprimindo o arquivo com os erros no cÛdigo
    	lexico->print_file_symbol(); //imprimindo a tabela de sÌmbolos
    	
    	output_name.str(""); //limpando o nome do arquivo de saÌda para o prÛximo
    }
    
    cout << "\tOs arquivos:\n";
    for(int j = 1; j < argc; j++){
        cout << "\t\t" << argv[j] << endl;
    }
    cout << "\tforam tratados pelo analisador lÈxico." << endl << endl;
    
    system("pause");
    return 0;
}
