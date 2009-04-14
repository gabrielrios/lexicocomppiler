/*
 *  main.cpp
 *
 *  Created by Gabriel Rios & Hélder Almeida.
 *  Copyright 2009 Home. All rights reserved.
 *
 */
 
#include <iostream>
#include "lexico.h"
#include <fstream>
#include <sstream>

using namespace std;

int main (int argc, char * const argv[]) {
  //instância da classe Léxico, que contém o método do analisador léxico
	Lexico *lexico; 
	
	//instância do objeto token
	Token tk; 
	
	//usado para abrir o arquivo com o código fonte
	fstream file_token; 
	
	//para formatar os dados a serem impressos no arquivo
  char format[10]; 
  
  //guarda o nome do arquivo com o código fonte, quando não passado por linha de comando
  string source_name; 
  
  //auxilia na contagem das linhas do arquivo
	int linha = -1, versao; 
	
	//nome do arquivo de saída para os erros
	stringstream output_name; 
	
	
	// Tratamento dos parametros de linha de comando.
	// Escolhendo a versão do analisador que vai ser usada.
	if (argc > 1 && ( string(argv[1]) == "-v1" || string(argv[1]) == "-v2")) { 
		versao = (string(argv[1]) == "-v1") ? 1 : 2;
		// Ajuda, mostra sintaxe da chamada do programa e os comandos disponívis
	} else if (string(argv[1]) == "--help" || string(argv[1]) == "-h") {	
		cout << "Analisador Lexico de Portugol. Por Gabriel Rios e Hélder Almeida" << endl;
		cout << "Uso: lexicocompiler [options] arquivo" << endl;
		cout << "Opções:"<< endl;
		cout << "\t -v1 \t\t Executa a versão 1 do analaisador"<< endl;
		cout << "\t -v2 \t\t Executa a versão 2 do analaisador"<< endl;
		return 0;
	} else {
		cout << "lexicocompiler: Parâmetros inválidos" << endl;
		cout << "lexicocompiler: lexicocompiler --help para mais informações" << endl;
		return 0;
	}
	
	//caso não seja passado nome de arquivo por linha de comando
	if (argc < 3) {
		cout << "Digite o nome do arquivo de entrada:";
		cin >> source_name;
	}
	
	//caso seja passado nome de arquivo por linha de comando, estes serão analisados
	//a partir deste laço, que chama o analisador léxico até o fim de cada arquivo
	for(int j = 2; j < argc || !source_name.empty(); j++, source_name = ""){
        
    	if (argc > 2) {
			lexico = new Lexico(argv[j], versao);
		} else {
			lexico = new Lexico(source_name, versao);
		}
        
        //definindo o nome do arquivo de saída
    	output_name << lexico->file_name.str() << "_tokens.txt";
    	
    	//abrindo o arquivo para escrita
    	file_token.open(output_name.str().c_str(), ios_base::out);
    	
    	linha = -1;
    	file_token << "Lista de tokens por linha" << endl << endl;
    	file_token << "Arquivo de entrada (versão " << versao << "): " << lexico->file_name.str() << endl << endl << endl;
    	file_token << "Linha  Token             Cod  Pos lexema" << endl;
    	file_token << "-----  ----------------  ---  ----------" << endl;
    	do {
        //chamando o analisador léxico para que devolva um token
    		tk = lexico->next_token();
    		if (lexico->linha > linha) {
    			linha = lexico->linha;
    			sprintf(format, "%5d  ", (linha+1));
    		} else {
    			sprintf(format, "%5c  ", ' ');
    		}
    		//colocando o número da linha ou espaço em branco
    		file_token << format; 
    		//imprimindo o token retornado com o código
    		file_token << tk.to_str(); 
    		if (tk.alias == "tkIdentificador" || tk.alias == "tkConstante" || tk.alias == "tkLiteral") {
    			sprintf(format, "%5d", lexico->last_symbol_position); 
    			//imprimindo a posição do token na tabela de símbolos
    			file_token << format; 
    		}
    		file_token << endl;
    	} while(tk != lexico->tkEOF) ;
    	
    	file_token << endl;
    	//fechando o arquivo de saída
    	file_token.close(); 
    	
    	//imprimindo o arquivo com os erros no código
    	lexico->print_file_errors(); 
    	//imprimindo a tabela de símbolos
    	lexico->print_file_symbol(); 
    	
    	//limpando o nome do arquivo de saída para o próximo
    	output_name.str(""); 
    }
    
    cout << "\tOs arquivos:\n";
    for(int j = 2; j < argc; j++){
        cout << "\t\t" << argv[j] << endl;
    }
    cout << "\tforam tratados pelo analisador léxico." << endl << endl;
    
    system("pause");
    return 0;
}
