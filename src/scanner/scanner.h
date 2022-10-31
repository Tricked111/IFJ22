/******************************************************************************
 *                                  IFJ22
 *                                scanner.h
 * 
 *      Authors: Nikita Kotvitskiy (xkotvi01)
 *      Purpose: Declaration of scanner and token structures and operations on them
 * 
 *                        Last change: 24. 10. 2022
 *****************************************************************************/

#ifndef SCANNER_H
#define SCANNER_H

#include "../str/ifj_string.h"
#include <ctype.h>
#include <stdbool.h>

//States of scanner FSM.
typedef enum {
    Start,
    Num,
    FloatInter1,
    FloatInter2,
    Float,
    Question,
    PhpEnd,
    Less,
    LQ,
    P,
    PH,
    PhpStart,
    More,
    Slash,
    Comment,
    BlockCom,
    BCInter,
    Oper,
    OperInter1,
    OperInter2,
    ID,
    Assig,
    CBracket,
    Bracket,
    Comma,
    Semicolon,
    Colon,
    End,
    String,
    EsqSeq,
    StringEnd,
    VarStart,
    Var
} ScannerStates;

//Types of tokens.
typedef enum {
    FLOAT,
    INT,
    PHP_END,
    PHP_START,
    OPER, 
    ASSIG,
    IDEN,
    CBRACK,
    BRACK,
    COMMA,
    SEMICOLON,
    COLON,
    END,
    STRING,
    VAR
} TokenType;

typedef enum {
    WRITE,
    SKIP,
    NEXT,
    CLEAN
} ScannerActions;


typedef struct Token {
    TokenType type;
    union TokenData {
        int ivalue;
        float fvalue; 
    } numericData;
    string_t textData;
} token_t;

typedef struct Scanner {
    int symbol;
    ScannerActions action;
    ScannerStates state;
    token_t * token;
} scanner_t;

//Initiates scanner on scanner address.
void scannerInit(scanner_t * scanner);
//Free scanner on scanner address.
void scannerFree(scanner_t * scanner);
//Initiates token on token address.
void tokenInit(token_t * token);
//Returns token on token address to it's initial state.
void tokenReinit(token_t * token);
//Free token on token address.
void tokenFree(token_t * token);
//Reads next token from stdin with scanner on scanner address and returns pointer to new token.
token_t * readToken(scanner_t * scanner);

#endif