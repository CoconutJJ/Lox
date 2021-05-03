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
#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "../types.h"
HashTable *hashtable_new();
void       hashtable_delete(HashTable *t, char *key);
void       hashtable_set(HashTable *t, char *key, void *item, int item_sz);
void *     hashtable_get(HashTable *t, char *key);
int        hashtable_has(HashTable *t, char *key);
void       hashtable_destroy(HashTable *table);
#endif