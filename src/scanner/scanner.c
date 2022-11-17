
/******************************************************************************
 *                                  IFJ22
 *                                scanner.c
 * 
 *      Authors: Nikita Kotvitskiy (xkotvi01)
 *      Purpose: Definition of operations on scanner and token structures
 * 
 *                        Last change: 24. 10. 2022
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "../data/data.h"

//Processes symbol in scanner->symbol, defines new scanner state and decides, which type of action should be performed with the symbol.
ScannerStates processChar(scanner_t * scanner);
//Performs action with the symbol.
void charAction(scanner_t * scanner, token_t * token);
//Fills token structure with data.
void finishToken(scanner_t * scanner, token_t * token);

//TEMPORARY!!!!
void error() {
    fprintf(stderr, "ERROR!!!\n");
    exit(1);
}

void scannerInit(scanner_t * scanner) {
    scanner->state = Start;
    scanner->symbol = 0;
    scanner->endOfToken = false;
}

void tokenInit(token_t * token) {
    token->type = tok_init;
    stringInit(&(token->textData));
}

void tokenClean(token_t * token) {
    tokenFree(token);
    tokenInit(token);
}

void tokenFree(token_t * token) {
    stringFree(&(token->textData));
}

token_t getToken(scanner_t * scanner) {
    token_t token;
    tokenInit(&token);
    while (!scanner->endOfToken) {
        if (scanner->symbol == 0)               //If there is not any symbol to process in scanner, read new one.
            scanner->symbol = getchar();
        scanner->state = processChar(scanner);  //Processing the symbol.
        if (scanner->state == Error)            //Lexical error handling.
            error();
        charAction(scanner, &token);            //Performing action on symbol.
    }
    finishToken(scanner, &token);               //Filling token structure.
    scanner->endOfToken = false;                //Scanner cleaning.
    scanner->state = Start;
    return token;
}

ScannerStates processChar(scanner_t * scanner) {
    int c = scanner->symbol;
    switch (scanner->state) {
        case Start:
            if (isdigit(c)) {
                scanner->action = WRITE;
                return Num;
            }
            if (c == '?') {
                scanner->action = SKIP;
                return Question;
            }
            if (c == '<') {
                scanner->action = WRITE;
                return Less;
            }
            if (c == '>') {
                scanner->action = WRITE;
                return More;
            }
            if (c == '*' || c == '+' || c == '.') {
                scanner->action = WRITE;
                scanner->endOfToken = true;
                return Oper;
            }
            if (c == '-') {
                scanner->action = WRITE;
                return OperMinus;
            }
            if (c == '/') {
                scanner->action = WRITE;
                return Slash;
            }
            if (c == '!') {
                scanner->action = WRITE;
                return OperInter1;
            }
            if (c == '=') {
                scanner->action = WRITE;
                return Assig;
            }
            if (c == '$') {
                scanner->action = SKIP;
                return VarStart;
            }
            if (c == '"') {
                scanner->action = SKIP;
                return String;
            }
            if (c == EOF) {
                scanner->action = SKIP;
                scanner->endOfToken = true;
                return End;
            }
            if (c == ':') {
                scanner->action = SKIP;
                scanner->endOfToken = true;
                return Colon;
            }
            if (c == ';') {
                scanner->action = SKIP;
                scanner->endOfToken = true;
                return Semicolon;
            }
            if (c == ',') {
                scanner->action = SKIP;
                scanner->endOfToken = true;
                return Comma;
            }
            if (c == '(' || c == ')') {
                scanner->action = WRITE;
                scanner->endOfToken = true;
                return Bracket;
            }
            if (c == '{' || c == '}') {
                scanner->action = WRITE;
                scanner->endOfToken = true;
                return CBracket;
            }
            if (isalpha(c) || c == '_') {
                scanner->action = WRITE;
                return ID;
            }
            if (isspace(c)) {
                scanner->action = SKIP;
                return Start;
            }
            return Error;
        case Num:
            if (isdigit(c)) {
                scanner->action = WRITE;
                return Num;
            }
            if (c == '.') {
                scanner ->action = WRITE;
                return FloatInter1;
            }
            if (c == 'e' || c == 'E') {
                scanner->action = WRITE;
                return FloatInter2;
            }
            scanner->action = NEXT;
            scanner->endOfToken = true;
            return Num;
        case FloatInter1:
            if (isdigit(c)) {
                scanner->action = WRITE;
                return Float;
            }
            return Error;
        case FloatInter2:
            if (c == '+' || c == '-') {
                scanner->action = WRITE;
                return FloatInter1;
            }
            if (isdigit(c)) {
                scanner->action = WRITE;
                return Float;
            }
            return Error;
        case Float:
            if (isdigit(c)) {
                scanner->action = WRITE;
                return Float;
            }
            scanner->action = NEXT;
            scanner->endOfToken = true;
            return Float;
        case Question:
            if (c == '>') {
                scanner->action = SKIP;
                scanner->endOfToken = true;
                return PhpEnd;
            }
            scanner->action = NEXT;
            scanner->endOfToken = true;
            return Question;
        case Less:
            if (c == '?') {
                scanner->action = SKIP;
                return LQ;
            }
            if (c == '=') {
                scanner->action = WRITE;
                scanner->endOfToken = true;
                return Oper;
            }
            scanner->action = NEXT;
            scanner->endOfToken = true;
            return Oper;
        case More:
            if (c == '=') {
                scanner->action = WRITE;
                scanner->endOfToken = true;
                return Oper;
            }
            scanner->action = NEXT;
            scanner->endOfToken = true;
            return Oper;
        case LQ:
            if (c == 'p') {
                scanner->action = SKIP;
                return P;
            }
            return Error;
        case P:
            if (c == 'h') {
                scanner->action = SKIP;
                return PH;
            }
            return Error;
        case PH:
            if (c == 'p') {
                scanner->action = SKIP;
                scanner->endOfToken = true;
                return PhpStart;
            }
            return Error;
        case Slash:
            if (c == '*') {
                scanner->action = CLEAN;
                return BlockCom;
            }
            if (c == '/') {
                scanner->action = CLEAN;
                return Comment;
            }
            scanner->action = NEXT;
            scanner->endOfToken = true;
            return Oper;
        case Comment:
            if (c == '\n') {
                scanner->action = SKIP;
                return Start;
            }
            if (c == EOF) {
                scanner->action = NEXT;
                return Start;
            }
            scanner->action = SKIP;
            return Comment;
        case BlockCom:
            if (c == '*') {
                scanner->action = SKIP;
                return BCInter;
            }
            if (c != EOF) {
                scanner->action = SKIP;
                return BlockCom;
            }
            return Error;
        case BCInter:
            if (c == '/') {
                scanner->action = SKIP;
                return Start;
            }
            if (c != EOF) {
                scanner->action = SKIP;
                return BlockCom;
            }
            return Error;
        case OperInter1:
            if (c == '=') {
                scanner->action = WRITE;
                return OperInter2;
            }
            return Error;
        case OperInter2:
            if (c == '=') {
                scanner->action = WRITE;
                scanner->endOfToken = true;
                return Oper;
            }
            return Error;
        case OperMinus:
            if (isdigit(c)) {
                scanner->action = WRITE;
                return Num;
            }
            if (isspace(c)) {
                scanner->action = SKIP;
                return OperMinus;
            }
            scanner->action = NEXT;
            scanner->endOfToken = true;
            return Oper;
        case Assig:
            if (c == '=') {
                scanner->action = WRITE;
                return OperInter2;
            }
            scanner->action = NEXT;
            scanner->endOfToken = true;
            return Assig;
        case VarStart:
            if (isalpha(c) || c == '_') {
                scanner->action = WRITE;
                return Var;
            }
            return Error;
        case Var:
            if (isalnum(c) || c == '_') {
                scanner->action = WRITE;
                return Var;
            }
            scanner->action = NEXT;
            scanner->endOfToken = true;
            return Var;
        case String:
            if (c == '\\') {
                scanner->action = WRITE;
                return EsqSeq;
            }
            if (c == '"') {
                scanner->action = SKIP;
                scanner->endOfToken = true;
                return StringEnd;
            }
            if (c != EOF) {
                scanner->action = WRITE;
                return String;
            }
            return Error;
        case EsqSeq:
            if (c != EOF) {
                scanner->action = WRITE;
                return String;
            }
            return Error;
        case ID:
            if (isalnum(c) || c == '_') {
                scanner->action = WRITE;
                return ID;
            }
            scanner->action = NEXT;
            scanner->endOfToken = true;
            return ID;
        default:
            return Error;
    }
}

void charAction(scanner_t * scanner, token_t * token) {
    switch (scanner->action) {
        case SKIP:
            scanner->symbol = 0;
            break;
        case WRITE:
            stringAppend(&(token->textData), scanner->symbol);
            scanner->symbol = 0;
            break;
        case CLEAN:
            tokenClean(token);
            scanner->symbol = 0;
            break;
        default:
            break;
    }
}

void finishToken(scanner_t * scanner, token_t * token) {
    key_t key;
    switch (scanner->state)
    {
        case Num:
            token->type = INT;
            token->numericData.ivalue = atoll(token->textData.str);
            break;
        case Float:
            token->type = FLOAT;
            token->numericData.fvalue = atof(token->textData.str);
            break;
        case PhpEnd:
            token->type = PHP_END;
            break;
        case PhpStart:
            token->type = PHP_START;
            break;
        case Oper:
            token->type = OPER;
            key = get_key(stringRead(&(token->textData)));
            token->numericData.ivalue = *(long long *)bstGet(grammar.operators, key);
            break;
        case Assig:
            token->type = ASSIG;
            break;
        case Var:
            token->type = VAR;
            break;
        case StringEnd:
            token->type = STRING;
            break;
        case End:
            token->type = END;
            break;
        case Colon:
            token->type = COLON;
            break;
        case Semicolon:
            token->type = SEMICOLON;
            break;
        case Comma:
            token->type = COMMA;
            break;
        case Bracket:
            token->type = BRACK;
            if (*stringRead(&(token->textData)) == '(') {
                token->type = BR_O;
                token->numericData.ivalue = 0;
            }
            else {
                token->type = BR_C;
                token->numericData.ivalue = 1;
            }
            break;
        case CBracket:
            token->type = BRACK;
            if (*stringRead(&(token->textData)) == '[') {
                token->type = CB_O;
                token->numericData.ivalue = 0;
            }
            else {
                token->type = CB_C;
                token->numericData.ivalue = 1;
            }
            break;
        case ID:
            key = get_key(stringRead(&(token->textData)));
            void * searchResult = bstGet(grammar.keyWords, key);
            if (searchResult != NULL) {
                token->type = FUN;
                token->numericData.ivalue = *(long long *)searchResult;
                break;
            }
            searchResult = bstGet(grammar.types, key);
            if (searchResult != NULL) {
                token->type = TYPE;
                token->numericData.ivalue = *(long long *)searchResult;
                break;
            }
            token->type = KW;
            break;
        case Question:
            token->type = QUEST;
            break;
        default:
            error();
    }
}