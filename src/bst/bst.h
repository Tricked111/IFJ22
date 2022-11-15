#ifndef IFJ_BST
#define IFJ_BST

typedef unsigned long hash_t;
typedef void * bstData_t;

typedef struct BST {
    hash_t key;
    bstData_t data;
    struct BST * left;
    struct BST * right;
} bst_t;

void bstInit(bst_t ** bst);
void bstDestroy(bst_t ** bst);
bstData_t bstSearch(bst_t * bst, hash_t key);
int bstInsert(bst_t ** bst, hash_t key, bstData_t data);
hash_t hash(char *str);

#endif