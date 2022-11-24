/******************************************************************************
 *                                  IFJ22
 *                                symtable.c
 * 
 *      Authors: Daniil Kniazkin (xkniaz00)
 *      Purpose: Symtable
 * 
 *                        Last change: 23. 11. 2022
 *****************************************************************************/
#include "symtable.h"

#define BASE_SIZE_ARRAY 5

/**
 * @brief Adds a variable and its type to the structure SymtableData
 * @param data pointers to structures SymtableData
 * @param type the type that will be assigned
 */ 

void add_var(SymtableData *data,TypesInd varaibale_type){
    data->type = S_VAR;
    data->dtype.var_type = varaibale_type;
}



/**
 * @brief Adds a func to the structure SymtableData
 * @param data pointers to structures SymtableData
 */ 

void add_func(SymtableData *data){
    data->type = S_FUNC;
    data->dtype.func_type.size = BASE_SIZE_ARRAY;
    data->dtype.func_type.lenght = 0;
    data->dtype.func_type.func_params = (TypesInd*) malloc(sizeof(TypesInd) * data->dtype.func_type.size);
}



/**
 * @brief Adds a param for func to the structure SymtableData
 * @param data pointers to structures SymtableData
 * @param param the param that will be assigned
 */ 

void add_func_param(SymtableData *data,TypesInd param){
    if(data->dtype.func_type.lenght == data->dtype.func_type.size){
        data->dtype.func_type.size *= 2;
        data->dtype.func_type.func_params = realloc(data->dtype.func_type.func_params,data->dtype.func_type.size * (sizeof(TypesInd)));
    }
    data->dtype.func_type.func_params[data->dtype.func_type.lenght++] = param;
}



/**
 * @brief Adds a return type for func to the structure SymtableData
 * @param data pointers to structures SymtableData
 * @param param the param that will be assigned
 */ 

void add_retype(SymtableData *data,TypesInd param){
    data->dtype.func_type.retype = param;
}



/**
 * @brief Inserts a new element into existing symbol table or updates existing node
 * @param table symbol table in which should be searching executed
 * @param key key of element that should be found
 * @param data pointers to structures SymtableData
 * @return correct insert
 */ 

int insertSymtable(Symtable *table,uint32_t key,SymtableData * data){
    if (*table == NULL) {
        //printf("NULL\n");
        if ((*table = (TreeNode_t*)malloc(sizeof(TreeNode_t))) == NULL)
            return 1;
        (*table)->data = *data;
        (*table)->key = key;
        (*table)->right = NULL;
        (*table)->left = NULL;
        return 0;
    }
    if(key == (*table)->key){
        (*table)->data = *data;
        return 0;
    }
    if (key > (*table)->key)
        return insertSymtable(&((*table)->right), key, &(*data));
    if (key < (*table)->key)
        return insertSymtable(&((*table)->left), key, &(*data));
    return 1;
}



/**
 * @brief Returns a pointer to the parameter list
 * @param data structures SymtableData
 * @return Pointer to the parameter list
 */ 

TypesInd * return_param_func(SymtableData data){
    return data.dtype.func_type.func_params;
}



/**
 * @brief clears parameters 
 * @param data double pointers to structures SymtableData
 */ 

void freeParams(SymtableData **data){
    free((*data)->dtype.func_type.func_params);
}



/**
 * @brief Returns the structure depending on the key
 * @param table symbol table in which should be searching executed
 * @param key key of element that should be found
 * @return Pointer to SymtableData
 */ 

SymtableData *symtableGet(Symtable *table,uint32_t key){
    if(*table == NULL) return NULL;
    if(key > (*table)->key) 
        return symtableGet(&(*table)->right,key);
    if(key < (*table)->key)
        return symtableGet(&(*table)->left,key);
    return &(*table)->data;
}



/**
 * @brief Checking for availability in the table
 * @param table symbol table in which should be searching executed
 * @param key key of element that should be found
 * @return bool type
 */

bool symtableSearch(Symtable *table,uint32_t key){
    if(*table == NULL) return false;
    if(key > (*table)->key) 
        return symtableSearch(&(*table)->right,key);
    if(key < (*table)->key)
        return symtableSearch(&(*table)->left,key);
    return true;
}



/**
 * @brief Replaces the type with a variable depending on the key
 * @param table symbol table in which should be searching executed
 * @param key key of element that should be found
 * @param type the type that will be assigned
 */ 

void replaceType(Symtable * table,uint32_t key,TypesInd new_type){
    SymtableData * data = symtableGet(&(*table),key);
    data->dtype.var_type = new_type;
    insertSymtable(&(*table),key,data);
}

