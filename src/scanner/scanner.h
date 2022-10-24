
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

#include "ifj_string.h"
#include <ctype.h>
#include <stdbool.h>

//States of scanner FSM.
typedef enum {
    Start,
    Num,
    FloatDot,
    FloatE,
    Float,
    StringStart,
    EsqSeq,
    StringEnd,
    VarStart,
    VarName,
    Identifier,
    Colon,
    Semicolon,
    Comma,
    Brack,
    CBrack,
    Operator,
    Assig,
    OperInter1,
    OperInter2,
    Slash,
    LineCom,
    BlockCom,
    BlockComEnd,
    Error,
    EndOfTok,
    EndOfFile
} FSMSTate;

//Types of tokens.
typedef enum {
    init_tok,
    id_tok,
    var_tok,
    int_tok,
    float_tok, 
    string_tok,
    brack_tok,
    c_brack_tok,
    oper_tok,
    comma_tok,
    semicolon_tok,
    colon_tok,
    assig_tok,
    eof_tok
} TokenType;


typedef struct Token {
    TokenType type;
    union TokenData {
        int ivalue;
        float fvalue; 
    } numericData;
    string_t textData;
} token_t;

typedef struct Scanner {
    int charToProcess;             //Char which is currently processed.
    string_t tokenString;
    TokenType state;
    bool doNotWriteCurrentChar;     //If true, charToProcess will not be appended at the end of tokenString.
                                    //It is useful if we read the first symbol of next token.
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
