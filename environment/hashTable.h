
#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "../types.h"
HashTable *hashtable_new();
void hashtable_delete(HashTable *t, char *key);
void hashtable_set(HashTable *t, char *key, void *item, int item_sz);
void * hashtable_get(HashTable *t, char * key);
void hashtable_destroy(HashTable * table);
#endif