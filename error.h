/*
 *  error.h
 *
 *  Created by Gabriel Rios & Hélder Almeida.
 *  Copyright 2009. All rights reserved.
 *
 */

#ifndef ERRO_H
#define ERRO_H
using namespace std;

//tipos de erros encontrados pelo analisador lÈxico
typedef enum eErrorType {
	ER_COMENTARIO_NAO_FECHADO,
	ER_LITERAL_NAO_FECHADO,
	ER_CARACTERE_INVALIDO
}ErrorType;

//classe que guarda as caracterÌsticas
class Error{
    public:
    int linha; //linha em que o erro aconteceu
    int coluna; //coluna em que o erro aconteceu
    ErrorType erro; //tipo de erro
    char invalido; //se for caractere inv·lido, guarda
    
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
