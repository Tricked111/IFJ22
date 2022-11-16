#ifndef DATA_IFJ
#define DATA_IFJ

#include "../bst/bst.h"

typedef enum {
    KW_ELSE_IND,
    KW_FUNC_IND,
    KW_IF_IND,
    KW_RET_IND,
    KW_WHILE_IND,
    KW_DEC_IND,
    KW_ST_IND
} KeyWordsInd;

typedef enum {
    ADD_IND,
    SUB_IND,
    MUL_IND,
    DIV_IND,
    GT_IND,
    LT_IND,
    GE_IND,
    LE_IND,
    EQ_IND,
    NE_IND,
    CON_IND
} OperatorsInd;

typedef enum {
    INT_IND,
    FLOAT_IND,
    STRING_IND,
    NULL_IND,
    VOID_IND
} TypesInd;

typedef enum {
    TOK_END,
    TOK_START,
    TOK_OPER,
    TOK_OPER_MIN,
    TOK_ASSIG,
    TOK_FUN,
    TOK_TYPE,
    TOK_BRO,
    TOK_BRC,
    TOK_CBO,
    TOK_CBC,
    TOK_COL,
    TOK_SEMICOL,
    TOK_EOF,
    TOK_CONST,
    TOK_VAR,
    TOK_QUEST,
    TOK_ONE,
    TOK_COMMA
} TokenInd;

typedef enum {
    KEY_WORDS_J,
    TYPE_J,
    TOK_J,
    RULE_J,
    CONST_J
} RuleJointType;

typedef struct RuleJoint {
    RuleJointType type;
    union
    {
        KeyWordsInd kwIndex;
        TypesInd typeIndex;
        struct
        {
            TokenInd tokenType;
            long long tokenAtribute;
        } TokenData;
        key_t ruleKey;
    } RuleJointData;
    struct RuleJoint * next;
} ruleJoint_t;

typedef struct Rule {
    int variantsCount;
    ruleJoint_t ** ruleVariants;
} rule_t;

typedef int * lexData_t;
typedef struct Grammar {
    bst_t * keyWords;
    bst_t * operators;
    bst_t * types;
    bst_t * syntaxRules;
} grammar_t;

extern grammar_t grammar;

int grammarInit();

#endif