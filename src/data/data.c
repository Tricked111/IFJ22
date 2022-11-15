#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "../bst/bst.h"

bst_t * getKeyWordTree();
bst_t * getOperatorsTree();
bst_t * getTypesTree();

static const char * KEY_WORDS[] = {
    "else",
    "function",
    "if",
    "return",
    "while",
    "declare",
    "strict_types"
};

static const char * OPERATORS[] = {
    "+",
    "-",
    "*",
    "/",
    ">",
    "<",
    ">=",
    "<=",
    "===",
    "!==",
    "."
};

static const char * TYPES[] = {
    "int",
    "float",
    "string",
    "null",
    "void"
};

bst_t * getKeyWordTree() {
    bst_t * tree;
    bstInit(&tree);
    key_t key;
    lexData_t data;

    for (int i = KW_ELSE_IND; i <= KW_ST_IND; i++) {
        key = get_key(KEY_WORDS[i]);
        if ((data = malloc(sizeof(int))) == NULL)
            return NULL;
        *data = i;
        bstInsert(&tree, key, (bstData_t)data);
    }

    return tree;
}

bst_t * getOperatorsTree() {
    bst_t * tree;
    bstInit(&tree);
    key_t key;
    lexData_t data;

    for (int i = ADD_IND; i <= CON_IND; i++) {
        key = get_key(OPERATORS[i]);
        if ((data = malloc(sizeof(int))) == NULL)
            return NULL;
        *data = i;
        bstInsert(&tree, key, (bstData_t)data);
    }

    return tree;
}

bst_t * getTypesTree() {
    bst_t * tree;
    bstInit(&tree);
    key_t key;
    lexData_t data;

    for (int i = INT_IND; i <= VOID_IND; i++) {
        key = get_key(TYPES[i]);
        if ((data = malloc(sizeof(int))) == NULL)
            return NULL;
        *data = i;
        bstInsert(&tree, key, (bstData_t)data);
    }

    return tree;
}

int grammarInit(grammar_t * grammar) {
    if (    (grammar->keyWords = getKeyWordTree())      == NULL ||
            (grammar->operators = getOperatorsTree())   == NULL ||
            (grammar->types = getTypesTree())           == NULL
        )
        return 1;
    return 0;
}