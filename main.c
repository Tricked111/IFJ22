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
#include "src/bst/bst.h"
#include "src/data/data.h"


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
            printf("[oper: %lld]\n", tok.numericData.ivalue);
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
        case KW:
            printf("[KW: %lld]\n", tok.numericData.ivalue);
            break;
        case TYPE:
            printf("[type: %lld]\n", tok.numericData.ivalue);
            break;
        case FUN:
            printf("[funID: %s]\n", stringRead(&(tok.textData)));
            break;
        case QUEST:
            printf("[question: ]\n");
            break;
        default:
            printf("error\n");
            break;
    }
}

void printTreeInt(bst_t * bst, int level) {
    if (bst != NULL) {
        printf("%d", *((int *)(bst->data)));
        putchar('\t');
        printTreeInt(bst->right, level + 1);
        putchar('\n');
        for (int i = 0; i < level; i++)
            putchar('\t');
        printTreeInt(bst->left, level + 1);
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
    grammar_t grammar;
    if (grammarInit(&grammar))
        return INTERN_ERR;

    scanner_t scan;
    scannerInit(&scan);
    TokenType lastTok = tok_init;
    while (lastTok != END && lastTok != PHP_END)
    {
        token_t token = getToken(&scan, grammar);
        //token_t token = tokRet();
        lastTok = token.type;
        printToken(token);
        tokenFree(&token);
    }
    
    //parseProgram();
    return 0;
}