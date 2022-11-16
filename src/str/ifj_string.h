
/******************************************************************************
 *                                  IFJ22
 *                              ifj_string.h
 * 
 *      Authors: Nikita Kotvitskiy (xkotvi01)
 *      Purpose: Declaration of string structure and operations on them
 * 
 *                        Last change: 24. 10. 2022
 *****************************************************************************/

#ifndef IFJ_STRING_H
#define IFJ_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_STRING_LENGTH 16    //The number of byte allocated for string during initiation.

typedef struct string {
    char * str;
    size_t len;
    size_t allocated;   //Number of currently allocated bytes.
} string_t;

//Initiates string on str address.
int stringInit(string_t * str);
//Free string on str address.
void stringFree(string_t * str);
//Appends new symbol newChar to the end of string on str address.
int stringAppend(string_t * str, char newChar);
//Erases a string on str address.
int stringClear(string_t * str);
//Copies string on strSource address to string on strDest address.
void stringCopy(string_t * strDest, string_t * strSource);
//Returns pointer to the first char of string in str address.
char * stringRead(string_t * str);
//Returns length of string on str address.
int stringLength(string_t * str);

#endif