/*
    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along with
    this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../types.h"

unsigned long hash(char *key) {
        /**
         * djb2 hash method
         */
        unsigned long hash = 5381;
        int           c;

        while ((c = *key++)) hash = ((hash << 5) + hash) ^ c;

        return hash;
}

void initialize_containers(HashTable *t) {
        for (int i = 0; i < t->size; i++) {
                t->table[i].key     = NULL;
                t->table[i].item    = NULL;
                t->table[i].deleted = 0;
        }
}

HashTable *hashtable_new() {
        HashTable *new   = malloc(sizeof(HashTable));
        new->table       = malloc(sizeof(HashContainer));
        new->table->key  = NULL;
        new->table->item = NULL;
        new->filled      = 0;
        new->size        = 1;

        return new;
}

void hashtable_destroy(HashTable *table) {
        for (int i = 0; i < table->size; i++) {
                if (table->table[i].key != NULL) {
                        free(table->table[i].item);
                        free(table->table[i].key);
                }
        }
        free(table->table);
        free(table);
}

void hashtable_find_and_set(HashTable *t, char *key, void *item) {
        assert(t->filled != t->size);

        int index = hash(key) % t->size;

        HashContainer *node;

        while ((node = &(t->table[index]))->key != NULL) {
                /*
                 * If key already exists, destroy old key and item
                 */
                if (strcmp(node->key, key) == 0) {
                        free(node->key);
                        free(node->item);
                        break;
                }

                index = (index + 1) % t->size;
        }

        node->key     = key;
        node->item    = item;
        node->deleted = 0;

        t->filled++;
}

void expand_hashtable(HashTable *t) {
        HashContainer *new_container =
            malloc(2 * t->size * sizeof(HashContainer));

        HashContainer *old_container = t->table;

        t->table  = new_container;
        t->size   = 2 * t->size;
        t->filled = 0;

        initialize_containers(t);

        for (int i = 0; i < t->size / 2; i++) {
                if (old_container[i].deleted || old_container[i].key == NULL)
                        continue;

                hashtable_find_and_set(t, old_container[i].key,
                                       old_container[i].item);
        }

        free(old_container);
}

void shrink_hashtable(HashTable *t) {
        if (t->size == 1) return;

        HashContainer *new_container =
            malloc(t->size / 2 * sizeof(HashContainer));

        HashContainer *old_container = t->table;

        t->table  = new_container;
        t->size   = t->size / 2;
        t->filled = 0;

        initialize_containers(t);

        for (int i = 0; i < t->size * 2; i++) {
                if (old_container[i].deleted || old_container[i].key == NULL)
                        continue;

                hashtable_find_and_set(t, old_container[i].key,
                                       old_container[i].item);
        }
        free(old_container);
}

void hashtable_delete(HashTable *t, char *key) {
        HashContainer *node;
        int            index = hash(key) % t->size, currIndex = index;

        while (strcmp((node = &(t->table[currIndex]))->key, key) != 0) {
                currIndex = (currIndex + 1) % t->size;

                if (currIndex == index) return;
        }

        free(node->key);
        free(node->item);

        node->key     = NULL;
        node->item    = NULL;
        node->deleted = 1;

        t->filled--;

        if (t->filled <= t->size / 4) shrink_hashtable(t);
}

void *hashtable_get(HashTable *t, char *key) {
        int index = hash(key) % t->size, currIndex = index;

        while (1) {
                if (!t->table[currIndex].key) {
                        currIndex = (currIndex + 1) % t->size;
                        if (currIndex == index) return NULL;
                        continue;
                }

                if (t->table[currIndex].deleted) {
                        return NULL;
                }

                if (strcmp(t->table[currIndex].key, key) == 0) {
                        return t->table[currIndex].item;
                }

                currIndex = (currIndex + 1) % t->size;

                if (currIndex == index) return NULL;
        }
}

void hashtable_set(HashTable *t, char *key, void *item, int item_sz) {
        if (t->filled == t->size) expand_hashtable(t);

        char *memkey  = malloc(strlen(key) + 1);
        void *memitem = malloc(item_sz);

        strcpy(memkey, key);
        memcpy(memitem, item, item_sz);

        hashtable_find_and_set(t, memkey, memitem);
}

int hashtable_has(HashTable *t, char *key) {
        return hashtable_get(t, key) != NULL;
}
