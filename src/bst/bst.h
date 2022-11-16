#ifndef IFJ_BST
#define IFJ_BST

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t key_t;
typedef void * bstData_t;

typedef struct BST {
    key_t key;
    bstData_t data;
    struct BST * left;
    struct BST * right;
} bst_t;

void bstInit(bst_t ** bst);
void bstDestroy(bst_t ** bst);
bool bstSearch(bst_t * bst, key_t key);
bstData_t bstGet(bst_t * bst, key_t key);
int bstInsert(bst_t ** bst, key_t key, bstData_t data);
key_t get_key(const char *str);

#endif