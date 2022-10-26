
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

//Adds data from scanner on scanner address to read token on token address.
void finishToken(scanner_t * scanner, token_t * token);
//Ruturns new state of scanner on scan address and set type of token on tok address based on new-read symbol on ch.
FSMSTate charProcessing(scanner_t * scan, token_t * tok, int ch);

void scannerInit(scanner_t * scanner) {
    stringInit(&(scanner->tokenString));
    scanner->charToProcess = '\0';              //Symbol '\0' means that there is not cuurently sybmol to process on scanner.
    scanner->state = Start;
    scanner->doNotWriteCurrentChar = false;
}

void scannerFree(scanner_t * scanner) {
    stringFree(&(scanner->tokenString));
}

void tokenInit(token_t * token) {
    token->type = init_tok;
    stringInit(&(token->textData));
}

void tokenReinit(token_t * token) {
    tokenFree(token);
    tokenInit(token);
}

void tokenFree(token_t * token) {
    stringFree(&(token->textData));
}

token_t * readToken(scanner_t * scanner) {
    token_t * newTok;
    if ((newTok = malloc(sizeof(token_t))) == NULL)
    {
        //TODO ERROR EXIT
        fprintf(stderr, "New tok allocation error\n");
        exit(1);
    }
    while (scanner->state != EndOfTok)
    {
        //If there is not symbol to process in scanner, new symbol will be read from stdin.
        if (scanner->charToProcess == '\0')
            scanner->charToProcess = getchar();
        scanner->state = charProcessing(scanner, newTok, scanner->charToProcess);
        if (scanner->state == Error)
        {
            //TODO ERROR EXIT
            fprintf(stderr, "Lexical errror\n");
            exit(1);
        }
        //If we are not going to process symbol which is currently in scanner again, we will append him to tokenString and remove him from scanner.
        if (!scanner->doNotWriteCurrentChar)
        {
            stringAppend(&(scanner->tokenString), scanner->charToProcess);
            scanner->charToProcess = '\0';
        }
        scanner->doNotWriteCurrentChar = false;
    }
    finishToken(scanner, newTok);
    scanner->state = Start;
    stringClear(&(scanner->tokenString));
    return newTok;
}

void finishToken(scanner_t * scanner, token_t * token)
{
    //Data will be added to token based on token type.
    switch (token->type) {
        case id_tok:
            stringCopy(&(token->textData), &(scanner->tokenString));
            break;
        case var_tok:
            stringCopy(&(token->textData), &(scanner->tokenString));
            break;
        case int_tok:
            token->numericData.ivalue = atoi(scanner->tokenString.str);
            break;
        case float_tok:
            token->numericData.fvalue = atof(scanner->tokenString.str);
            break;
        case string_tok:
            stringCopy(&(token->textData), &(scanner->tokenString));
            break;
        case brack_tok:
            token->numericData.ivalue = scanner->tokenString.str[0] == '(' ? 0 : 1;
            break;
        case c_brack_tok:
            token->numericData.ivalue = scanner->tokenString.str[0] == '{' ? 0 : 1;
            break;   
        case oper_tok:
            stringCopy(&(token->textData), &(scanner->tokenString));
            break;
        default:
            break;
    }
}

FSMSTate charProcessing(scanner_t * scan, token_t * tok, int ch)
{
    switch (scan->state)
    {
        case Start:
            if (isdigit(ch)) {
                tok->type = int_tok;
                return Num;
            }
            if (ch == '"') {
                tok->type = string_tok;
                return StringStart;
            }
            if (ch == '$') {
                tok->type = var_tok;
                return VarStart;
            }
            if (isalpha(ch) || ch == '_') {
                tok->type = id_tok;
                return Identifier;
            }
            if (ch == ':') {
                tok->type = colon_tok;
                return Colon;
            }
            if (ch == ';') {
                tok->type = semicolon_tok;
                return Semicolon;
            }
            if (ch == ',') {
                tok->type = comma_tok;
                return Comma;
            }
            if (ch == '=') {
                tok->type = assig_tok;
                return Assig;
            }
            if (ch == '!') {
                tok->type = oper_tok;
                return OperInter3;
            }
            if (ch == '<' || ch == '>') {
                tok->type = oper_tok;
                return OperInter2;
            }
            if (ch == '*' || ch == '-' || ch == '+' || ch == '.') {
                tok->type = oper_tok;
                return Operator;
            }
            if (ch == '{' || ch == '}') {
                tok->type = c_brack_tok;
                return CBrack;
            }
            if (ch == '(' || ch == ')') {
                tok->type = brack_tok;
                return Brack;
            }
            //'/' can be operator or the beginning of comment, so type of token will be defined lately.
            if (ch == '/') {
                return Slash;
            }
            //We will ignore all white sybmols.
            if (isspace(ch)) {
                //Setting doNotWriteCurrentChar on true and assignment '\0' to charToProcess means that the symbol will not be appended at tokenString,
                //and also will not be processed again. So the symbol will be skipped.
                scan->doNotWriteCurrentChar = true;
                scan->charToProcess = '\0';
                return Start;
            }
            if (ch == EOF)
            {
                tok->type = eof_tok;
                scan->doNotWriteCurrentChar = true;
                return EndOfTok;
            }
            return Error;
        case Num:
            if (isdigit(ch))
                return Num;
            if (ch == '.') {
                tok->type = float_tok;
                return FloatDot;
            }
            if (ch == 'e' || ch == 'E') {
                tok->type = float_tok;
                return FloatE;
            }
            scan->doNotWriteCurrentChar = true;
            return EndOfTok;
        case FloatE:
            if (isdigit(ch))
                return Float;
            if (ch == '+' || ch == '-')
                return FloatDot;
            return Error;
        case FloatDot:
            if (isdigit(ch))
                return Float;
            return Error;
        case Float:
            if (isdigit(ch))
                return Float;
            scan->doNotWriteCurrentChar = true;
            return EndOfTok;
        case StringStart:
            if (ch != '/' && ch != '"')
                return StringStart;
            if (ch == '/')
                return EsqSeq;
            if (ch == '"')
                return EndOfTok;
            return Error;
        case EsqSeq:
            return StringStart;
        case StringEnd:
            scan->doNotWriteCurrentChar = true;
            return EndOfTok;
        case VarStart:
            if (isalpha(ch) || ch == '_')
                return VarName;
            return Error;
        case VarName:
            if (isalnum(ch) || ch == '_')
                return VarName;
            scan->doNotWriteCurrentChar = true;
            return EndOfTok;
        case Identifier:
            if (isalnum(ch) || ch == '_')
                return Identifier;
            scan->doNotWriteCurrentChar = true;
            return EndOfTok;
        case Assig:
            if (ch == '=')
                return OperInter1;
            scan->doNotWriteCurrentChar = true;
            return EndOfTok;
        case OperInter1:
            if (ch == '=')
            {
                tok->type = oper_tok;
                return Operator;
            }
            return Error;
        case OperInter2:
            if (ch == '=')
                return Operator;
            scan->doNotWriteCurrentChar = true;
            return Operator;
        case OperInter3:
            if (ch == '=')
                return OperInter1;
            return Error;
        /* case Slash:
            if (ch == '/')
                return LineCom;
            if (ch == '*')
            {
                tok->type = LineCom;
                return BlockCom;
            }
            tok->type = oper_tok;
            scan->doNotWriteCurrentChar = true;
            return Operator;
        case LineCom:
            if (ch == '\n') {
                //After comment end the whole tokenString will be erased and scanner will be returned to it's initial state.
                scan->doNotWriteCurrentChar = true;
                scan->charToProcess = '\0';
                stringClear(&(scan->tokenString));
                return Start;
            }
            if (ch == EOF)
            {
                tok->type = eof_tok;
                scan->doNotWriteCurrentChar;
                return EndOfTok;
            }
            return LineCom; */
        case BlockCom:
            if (ch == '*')
                return BlockComEnd;
            //Block comment must be closed before the program's text end.
            if (ch == EOF)
                return Error;
            return BlockCom;
        case BlockComEnd:
            if (ch == '/') {
                scan->doNotWriteCurrentChar = true;
                scan->charToProcess = '\0';
                stringClear(&(scan->tokenString));
                return Start;
            }
            if (ch == EOF)
                return Error;
            return BlockCom;
        default:
            scan->doNotWriteCurrentChar = true;
            return EndOfTok;
    }
}