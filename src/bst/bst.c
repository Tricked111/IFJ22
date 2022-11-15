#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

void bstInit(bst_t ** bst) {
    *bst = NULL;
}
void bstDestroy(bst_t ** bst) { 
    if (*bst != NULL) {
        bstDestroy(&((*bst)->left));
        bstDestroy(&((*bst)->right));
        free((*bst)->data);
        free(*bst);
        *bst = NULL;
    }
}
bstData_t bstSearch(bst_t * bst, hash_t key) {
    if (bst == NULL)
        return NULL;
    if (key > bst->key)
        return bstSearch(bst->right, key);
    if (key < bst->key)
        return bstSearch(&bst->left, key);
    return bst->data;
}
int bstInsert(bst_t ** bst, hash_t key, bstData_t data) {
    if (*bst == NULL) {
        if ((*bst = malloc(sizeof(bst_t))) == NULL)
            return 1;
        (*bst)->data = data;
        (*bst)->key = key;
        (*bst)->right = NULL;
        (*bst)->left = NULL;
        return 0;
    }
    if (key > (*bst)->key)
        return bstInsert(&((*bst)->right), key, data);
    if (key < (*bst)->key)
        return bstInsert(&((*bst)->left), key, data);
    return 1;
}


//Source: https://stackoverflow.com/questions/7666509/hash-function-for-string
hash_t hash(char *str)
{
    hash_t hash = 5381;
    int c;
    c = *str++;
    do {
        c = *str++;
        hash = ((hash << 5) + hash) + c;
    }
    while (c);
    return hash;
}