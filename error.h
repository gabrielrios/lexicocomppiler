/*
 *  lexico.h
 *  lexicocompiler
 *
 *  Created by Gabriel Rios on 07/04/09.
 *  Copyright 2009 Home. All rights reserved.
 *
 */

#ifndef ERRO_H
#define ERRO_H

using namespace std;

typedef enum eErrorType {
	ER_COMENTARIO_NAO_FECHADO,
	ER_LITERAL_NAO_FECHADO,
	ER_CARACTERE_INVALIDO
}ErrorType;

class Error{
    public:
    int linha;
    int coluna;
    ErrorType erro;
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
