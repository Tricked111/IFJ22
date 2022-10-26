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

void printToken(token_t tok)
{
    switch (tok.type) {
        case id_tok:
            printf("[id: %s]\n", stringRead(&(tok.textData)));
            break;
        case var_tok:
            printf("[var: %s]\n", stringRead(&(tok.textData)));
            break;
        case int_tok:
            printf("[int: %d]\n", tok.numericData.ivalue);
            break;
        case float_tok:
            printf("[float: %f]\n", tok.numericData.fvalue);
            break;
        case string_tok:
            printf("[string: %s]\n", stringRead(&(tok.textData)));
            break;
        case brack_tok:
            printf("[brack: %d]\n", tok.numericData.ivalue);
            break;
        case c_brack_tok:
            printf("[cbrack: %d]\n", tok.numericData.ivalue);
            break;
        case oper_tok:
            printf("[oper: %s]\n", stringRead(&(tok.textData)));
            break;
        case comma_tok:
            printf("[comma: ]\n");
            break;
        case semicolon_tok:
            printf("[semicolon: ]\n");
            break;
        case colon_tok:
            printf("[colon: ]\n");
            break;
        case assig_tok:
            printf("[assig: ]\n");
            break;
        case eof_tok:
            printf("[eof: ]\n");
            break;
        default:
            break;
    }
}

int main()
{
    scanner_t scan;
    scannerInit(&scan);
    TokenType lastTok = init_tok;
    while (lastTok != eof_tok)
    {
        token_t * tok = readToken(&scan);
        lastTok = tok->type;
        printToken(*tok);
    }
    return 0;
}