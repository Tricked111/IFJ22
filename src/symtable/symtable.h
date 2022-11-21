/******************************************************************************
 *                                  IFJ22
 *                                scanner.c
 * 
 *      Authors: Daniil Kniazkin (xkniaz00)
 *      Purpose: Definition of operations on scanner and token structures
 * 
 *                        Last change: 18. 11. 2022
 *****************************************************************************/


/**
 * @file symtable.c
 * @brief Symbol table
 */

#ifndef SYMTABLE_IFJ22
#define SYMTABLE_IFJ22

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../str/ifj_string.h"
#include "../data/data.h"


/**
 * @brief Types of characters in the table
 */
typedef enum sym_type{
    S_VAR,S_FUNC
}sym_type_t;

/**
 * @brief Type of variabel or Return type in function
 */
typedef enum sym_dtype{
    S_NIL,S_INT,S_FLOAT,S_BOOl,S_STRING
}sym_dtype_t;


/**
 * @brief Struct with all data
 */

//var: type
//func: param type ~ array type in param func


//func: vehledat symbol,zmenit_type(var),pridat paramtert(func),smazat(),

typedef struct sym_data {
    //string_t name;
    TypesInd type; //bez Void 
    sym_type_t typee;
    //sym_dtype_t dtype;
    //bool is_used;
}sym_data_t;


/**
 * @brief Struct BST for sym_data_t
 */

typedef struct sym_tree{
    char *key;
    sym_data_t data;
    struct sym_tree *lptr;
    struct sym_tree *rptr;
}sym_tree_t;








/**
 * @brief Pointer for tree
 */
typedef sym_tree_t * symtable;



typedef struct symtable{
    sym_tree_t *t;
}symtabel_t;


//symtable first = NULL;  =    symtable_t first;  first->t


/**
 * @brief Initializes symbol table
 * @param 
 */
void init_table(symtable *table);

/**
 * @brief Initializes data struct symbol
 */
int init_data(sym_data_t *data);


void data_free(sym_data_t *data);


void insert_symbol(symtable *table,const char *key,sym_data_t data);

int mem_for_key(char **str,const char *str2,size_t len);


#endif