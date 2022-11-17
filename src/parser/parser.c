#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "../data/data.h"

bool ruleProcess(rule_t * rule, int tokIndex, const program_t * program);
bool checRulePart(ruleJoint_t * part, const program_t * program, int tokIndex);
bool checkVariant(ruleJoint_t * rule, const program_t * program, int firstTokIndex);
bool ruleProcess(rule_t * rule, int tokIndex, const program_t * program);

int readPogram(program_t * program, scanner_t * scanner) {
    program->tokenCount = 0;
    if ((program->tokens = malloc(0)) == NULL)
        return INTERN_ERR;
    TokenType tokType;
    token_t token;
    do {
        token = getToken(scanner);
        tokType = token.type;
        program->tokenCount++;
        if ((program->tokens = realloc(program->tokens, sizeof(token_t) * program->tokenCount)))
            return INTERN_ERR;
        program->tokens[program->tokenCount - 1] = token;
        if (token.type == ERROR)
            return LEXICAL_ERR;
    } while (tokType != END && tokType != PHP_END);
    return 0;
}

int parseProgram() {
    scanner_t scanner;
    scannerInit(&scanner);
    program_t program;
    
    int programReaderReturn = readPogram(&program, &scanner);
    if (programReaderReturn)
        return programReaderReturn;

    rule_t * rule = (rule_t *)grammar.syntaxRules->data;
    if (ruleProcess(rule, 0, &program))
        return SYNTAX_ERR;
    return 0;
}


bool ruleProcess(rule_t * rule, int tokIndex, const program_t * program) {
    bool observed = false;
    for (int i = 0; i < rule->variantsCount; i++)
        if (checkVariant(rule->ruleVariants[i], program, tokIndex))
            observed = true;
    if (observed)
        return true;
    return false;
}

bool checkVariant(ruleJoint_t * rule, const program_t * program, int firstTokIndex) {
    do {
        if (!checRulePart(rule, program, firstTokIndex))
            return false;
        rule = rule->next;
        firstTokIndex++;
    } while (rule != NULL);
    return true;
}

bool checRulePart(ruleJoint_t * part, const program_t * program, int tokIndex) {
    if (tokIndex >= program->tokenCount)
        return false;
    switch (part->type) {
        case KEY_WORDS_J:
            if (program->tokens[tokIndex].type == KW && program->tokens[tokIndex].numericData.ivalue == part->RuleJointData.kwIndex)
                return true;
            return false;
        case TYPE_J:
            if (program->tokens[tokIndex].type == TYPE && program->tokens[tokIndex].numericData.ivalue == part->RuleJointData.typeIndex)
                return true;
            return false;
        case TOK_J:
            if (program->tokens[tokIndex].type == (TokenType)part->RuleJointData.TokenData.tokenType) {
                if (part->RuleJointData.TokenData.tokenType == TOK_OPER_MIN || part->RuleJointData.TokenData.tokenType == TOK_ONE)
                    if (program->tokens[tokIndex].numericData.ivalue != part->RuleJointData.TokenData.tokenAtribute)
                        return false;
                return true;
            }
            return false;
        case RULE_J:
            rule_t * rule = (rule_t *)bstGet(grammar.syntaxRules, part->RuleJointData.ruleKey);
            return ruleProcess(rule, tokIndex, program);
        case CONST_J:
            if (program->tokens[tokIndex].type == INT || program->tokens[tokIndex].type == FLOAT || program->tokens[tokIndex].type == STRING)
                return true;
            return false;
    }
    return false;
}