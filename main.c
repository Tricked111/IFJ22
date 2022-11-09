/******************************************************************************
 *                                  IFJ22
 *                                  main.c
 * 
 *      Authors: Kniazkin Daniil   (xkniaz00)
                 Erik Hrubý        (xhruby30)
                 Alžbeta Hricovová (xhrico00)
                 Nikita Kotvitskiy (xkotvi01)
 *                    Purpose: Main function of compiler
 * 
 *                        Last change: 2. 10. 2022
 *****************************************************************************/

#include <stdio.h>
#include "src/scanner/scanner.h"
#include "src/parser/parser.h"


/* 
test main func changed

 */

void printToken(token_t tok)
{
    switch (tok.type) {
        case FLOAT:
            printf("[float: %f]\n", tok.numericData.fvalue);
            break;
        case INT:
            printf("[int: %lld]\n", tok.numericData.ivalue);
            break;
        case PHP_END:
            printf("[php_end: ]\n");
            break;
        case PHP_START:
            printf("[php_start: ]\n");
            break;
        case OPER:
            printf("[oper: %s]\n", stringRead(&(tok.textData)));
            break;
        case ASSIG:
            printf("[assig: ]\n");
            break;
        case VAR:
            printf("[var: %s]\n", stringRead(&(tok.textData)));
            break;
        case STRING:
            printf("[string: %s]\n", stringRead(&(tok.textData)));
            break;
        case END:
            printf("[end: ]\n");
            break;
        case COLON:
            printf("[colon: ]\n");
            break;
        case SEMICOLON:
            printf("[semicolon: ]\n");
            break;
        case COMMA:
            printf("[comma: ]\n");
            break;
        case BRACK:
            printf("[brack: %lld]\n", tok.numericData.ivalue);
            break;
        case CBRACK:
            printf("[cbrack: %lld]\n", tok.numericData.ivalue);
            break;
        case IDEN:
            printf("[id: %s]\n", stringRead(&(tok.textData)));
            break;
        default:
            printf("error\n");
            break;
    }
}

token_t tokRet()
{
    token_t tok;
    tokenInit(&tok);
    tok.type = END;
    return tok;
}

int main()
{
    scanner_t scan;
    scannerInit(&scan);
    TokenType lastTok = tok_init;
    while (lastTok != END && lastTok != PHP_END)
    {
        token_t token = getToken(&scan);
        //token_t token = tokRet();
        lastTok = token.type;
        printToken(token);
        tokenFree(&token);
    }
    return 0;
}