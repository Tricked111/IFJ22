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
    Var,
    Error
} ScannerStates;

//Types of tokens.
typedef enum {
    tok_init,   //Default value of new inicialized token, does not denote any real type of token.
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

//Types of action on processed symbol.
typedef enum {
    WRITE,
    SKIP,
    NEXT,
    CLEAN
} ScannerActions;

//Lexical unit of program.
typedef struct Token {
    TokenType type;
    union TokenData {
        long long ivalue;
        float fvalue; 
    } numericData;
    string_t textData;
} token_t;

typedef struct Scanner {
    int symbol;                 //Processed symbol.
    ScannerActions action;      //Denote an action on symbol.
    ScannerStates state;        //Actual state of scanner.
    bool endOfToken;            //Indicates whether we have reached the end of token.
} scanner_t;

void scannerInit(scanner_t * scanner);
//Initiates token on token address.
void tokenInit(token_t * token);
//Returns token on token address to it's initial state.
void tokenClean(token_t * token);
//Free token on token address.
void tokenFree(token_t * token);
//Reads next token from stdin with scanner on scanner address and returns pointer to new token.
token_t getToken(scanner_t * scanner);

#endif