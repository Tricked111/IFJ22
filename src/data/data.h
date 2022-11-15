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

typedef int * lexData_t;
typedef struct Grammar {
    bst_t * keyWords;
    bst_t * operators;
    bst_t * types;
} grammar_t;

int grammarInit(grammar_t * grammar);

#endif