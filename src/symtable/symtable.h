/******************************************************************************
 *                                  IFJ22
 *                                symtable.c
 * 
 *      Authors: Daniil Kniazkin (xkniaz00)
 *      Purpose: Definition of operations on scanner and token structures
 * 
 *                        Last change: 23. 11. 2022
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
 * @brief Data structure for placing function attributes
 */
typedef struct Func_dtype_t{
    TypesInd retype;
    TypesInd *func_params;
    size_t lenght;
    size_t size;
}func_dtype_t;

/**
 * @brief Data structure for SymtableData
 */
typedef struct sym_data {
    sym_type_t type;
    union D_type{
        TypesInd var_type;
        func_dtype_t func_type;
    }dtype;
}sym_data_t;

typedef sym_data_t SymtableData;


/**
 * @brief Data structure for Symtable
 */
typedef struct TreeNode{
    uint32_t key;
    sym_data_t data;
    struct TreeNode *left;
    struct TreeNode *right;
}TreeNode_t;

typedef TreeNode_t * Symtable;


/**
 * @brief Adds a variable and its type to the structure SymtableData
 * @param data pointers to structures SymtableData
 * @param type the type that will be assigned
 */ 
void add_var(SymtableData *data,TypesInd type);


/**
 * @brief Adds a func to the structure SymtableData
 * @param data pointers to structures SymtableData
 */ 
void add_func(SymtableData *data);



/**
 * @brief Adds a param for func to the structure SymtableData
 * @param data pointers to structures SymtableData
 * @param param the param that will be assigned
 */ 
void add_func_param(SymtableData *data,TypesInd param);



/**
 * @brief Adds a return type for func to the structure SymtableData
 * @param data pointers to structures SymtableData
 * @param param the param that will be assigned
 */ 
void add_retype(SymtableData *data,TypesInd param);



/**
 * @brief Inserts a new element into existing symbol table or updates existing node
 * @param table symbol table in which should be searching executed
 * @param key key of element that should be found
 * @param data pointers to structures SymtableData
 * @return Pointer to found symbol or NULL
 */ 
int insertSymtable(Symtable *table,uint32_t key,SymtableData *data);



/**
 * @brief Returns a pointer to the parameter list
 * @param data structures SymtableData
 * @return Pointer to the parameter list
 */ 
TypesInd * return_param_func(SymtableData data);

/**
 * @brief clears parameters 
 * @param data double pointers to structures SymtableData
 */ 
void freeParams(SymtableData **data);



/**
 * @brief Returns the structure depending on the key
 * @param table symbol table in which should be searching executed
 * @param key key of element that should be found
 * @return Pointer to SymtableData
 */ 
SymtableData *symtableGet(Symtable *table,uint32_t key); 



/**
 * @brief Checking for availability in the table
 * @param table symbol table in which should be searching executed
 * @param key key of element that should be found
 * @return bool type
 */ 
bool symtableSearch(Symtable *table,uint32_t key);



/**
 * @brief Replaces the type with a variable depending on the key
 * @param table symbol table in which should be searching executed
 * @param key key of element that should be found
 * @param type the type that will be assigned
 */ 
void replaceType(Symtable *table,uint32_t key,TypesInd type); //for var


#endif