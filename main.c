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
        case BR_O:
            printf("[open_br: ]\n");
            break;
        case BR_C:
            printf("[close_br: ]\n");
            break;
        case CB_O:
            printf("[open_cb: ]\n");
            break;
        case CB_C:
            printf("[close_cb: ]\n");
            break;
        case IDEN:
            printf("[id: %s]\n", stringRead(&(tok.textData)));
            break;
        case KW:
            printf("[key_word: %s]\n", tok.textData.str);
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

void printRules(bst_t * tree) {
    if (tree == NULL)
        return;

    printf("RULE %u:\n", tree->key);
    rule_t * rule = (rule_t *)(tree->data);
    for (int i = 0; i < rule->variantsCount; i++) {
        printf("\tRule variant: ");
        ruleJoint_t * rj = rule->ruleVariants[i];
        while (rj != NULL) {
            printf("%d ", rj->type);
            rj = rj->next;
        }
        putchar('\n');
    }

    printRules(tree->left);
    printRules(tree->right);
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

    /* parseProgram(); */
    /* if (grammarInit())
        return INTERN_ERR;
    
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
    } */
    
    parseProgram();
    return 0;
}