#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "../data/data.h"

#define KW_COUNT 2
#define RULE_COUNT 3
#define TOK_COUNT 18
#define TYPE_COUNT

int ruleProcess(rule_t * rule, scanner_t * scanner, token_t token);

int parseProgram() {

    scanner_t scanner;
    scannerInit(&scanner);

    rule_t * rule = (rule_t *)grammar.syntaxRules->data;
    token_t token = getToken(&scanner);
    if (ruleProcess(rule, &scanner, token))
        return 1;
    return 0;
}

int ruleProcess(rule_t * rule, scanner_t * scanner, token_t token) {
    
    return 1;
}