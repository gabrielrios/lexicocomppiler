/*
 *  error.h
 *
 *  Created by Gabriel Rios & H�lder Almeida.
 *  Copyright 2009. All rights reserved.
 *
 */

#ifndef ERRO_H
#define ERRO_H
using namespace std;

//tipos de erros encontrados pelo analisador l�xico
typedef enum eErrorType {
	ER_COMENTARIO_NAO_FECHADO,
	ER_LITERAL_NAO_FECHADO,
	ER_CARACTERE_INVALIDO
}ErrorType;

//classe que guarda as caracter�sticas
class Error{
    public:
    
    //linha em que o erro aconteceu
    int linha; 
    
    //coluna em que o erro aconteceu
    int coluna; 
    
    //tipo de erro
    ErrorType erro;
    
    //se for caractere inv�lido, guarda
    char invalido; 
    
    Error(int _linha, int _coluna, ErrorType _erro){
        linha = _linha;
        coluna = _coluna;
        erro = _erro;
    }
    
    Error(int _linha, int _coluna, ErrorType _erro, char _invalido){
        linha = _linha;
        coluna = _coluna;
        erro = _erro;
        invalido = _invalido;
    }
};
#endif
