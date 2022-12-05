#ifndef GENERATOR_H
#define GENERATOR_H

#include "../scanner/scanner.h"
#include "../semantic/semantic.h"
#include "../symtable/symtable.h"
#include "../bst/bst.h"
#include <stdbool.h>

#define PROG_START 8
#define OUTPUT_FILE_NAME "out.ifjcode"

#define IFJ_PROG_START      ".IFJcode22"
#define TMP1                "GF@__TMP1"
#define TMP2                "GF@__TMP2"
#define TMP3                "GF@__TMP3"

#define CREATEFRAME         "CREATEFRAME"
#define PUSHFRAME           "PUSHFRAME"
#define POPFRAME            "POPFRAME"
#define LABEL               "LABEL"
#define JUMP                "JUMP"
#define IFJ_TYPE            "TYPE"

#define DEFVAR          "DEFVAR"
#define LOCAL_VAR       "LF@"
#define POPS            "POPS"
#define PUSHS           "PUSHS"
#define IFJ_INTEGER     "int@"
#define IFJ_FLOAT       "float@"
#define ADDS            "ADDS"
#define SUBS            "SUBS"
#define MULS            "MULS"
#define DIVS            "DIVS"
#define EQS             "EQS"

#define NUMERIC_CONVERTER "\
POPS GF@__TMP2\n\
POPS GF@__TMP1\n\
TYPE GF@__TMP3 GF@__TMP2\n\
PUSHS GF@__TMP3\n\
TYPE GF@__TMP3 GF@__TMP1\n\
PUSHS GF@__TMP3\n\
EQS\n\
PUSHS bool@false\n\
JUMPIFEQS __CONV_%d_1\n\
PUSHS GF@__TMP1\n\
PUSHS GF@__TMP2\n\
JUMP __CONV_%d_2\n\
LABEL __CONV_%d_1\n\
PUSHS GF@__TMP3\n\
PUSHS string@float\n\
EQS\n\
PUSHS bool@false\n\
JUMPIFEQS __CONV_%d_3\n\
PUSHS GF@__TMP1\n\
PUSHS GF@__TMP2\n\
INT2FLOATS\n\
JUMP __CONV_%d_2\n\
LABEL __CONV_%d_3\n\
PUSHS GF@__TMP1\n\
INT2FLOATS\n\
PUSHS GF@__TMP2\n\
LABEL __CONV_%d_2\n"

#define FLOAT_CONVERTER "\
POPS GF@__TMP2\n\
POPS GF@__TMP1\n\
TYPE GF@__TMP3 GF@__TMP1\n\
PUSHS GF@__TMP3\n\
PUSHS string@float\n\
EQS\n\
PUSHS bool@false\n\
JUMPIFEQS __CONV_%d_1\n\
PUSHS GF@__TMP1\n\
LABEL __CONV_%d_2\n\
TYPE GF@__TMP3 GF@__TMP2\n\
PUSHS GF@__TMP3\n\
PUSHS string@float\n\
EQS\n\
PUSHS bool@false\n\
JUMPIFEQS __CONV_%d_3\n\
PUSHS GF@__TMP2\n\
JUMP __CONV_%d_4\n\
LABEL __CONV_%d_1\n\
PUSHS GF@__TMP1\n\
INT2FLOATS\n\
JUMP __CONV_%d_2\n\
LABEL __CONV_%d_3\n\
PUSHS GF@__TMP2\n\
INT2FLOATS\n\
JUMP __CONV_%d_4\n\
LABEL __CONV_%d_4\n"

#define CONCATENATION "\
POPS GF@__TMP2\n\
POPS GF@__TMP1\n\
TYPE GF@__TMP3 GF@__TMP1\n\
PUSHS GF@__TMP3\n\
PUSHS string@string\n\
EQS\n\
PUSHS bool@false\n\
JUMPIFNEQS __CONV_%d_1\n\
MOVE GF@__TMP1 string@\n\
LABEL __CONV_%d_1\n\
TYPE GF@__TMP3 GF@__TMP2\n\
PUSHS GF@__TMP3\n\
PUSHS string@string\n\
EQS\n\
PUSHS bool@false\n\
JUMPIFNEQS __CONV_%d_2\n\
MOVE GF@__TMP2 string@\n\
LABEL __CONV_%d_2\n\
CONCAT GF@__TMP1 GF@__TMP1 GF@__TMP2\n\
PUSHS GF@__TMP1\n"

typedef struct Generator {
    int counter;
    int currentPosition;
    int subPosition;
    bool inWhile;
    bool global;
    bst_t * globalVars;
    bst_t * localVars;
    bst_t * functions;
} generator_t;

int generateProgram(program_t prog);

#endif