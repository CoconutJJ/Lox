#include <stdlib.h>

#include "../types.h"
#include "hashTable.h"

ENVIRONMENT* create_environment() {
        ENVIRONMENT* new_env = malloc(sizeof(ENVIRONMENT));

        new_env->next  = NULL;
        new_env->prev  = NULL;
        new_env->table = hashtable_new();

        return new_env;
}

void destroy_environment_scope(ENVIRONMENT* env) {
        hashtable_destroy(env->table);
        free(env);
}

void destroy_environment(ENVIRONMENT* env) {
        while (env) {
                ENVIRONMENT* prev = env->prev;

                destroy_environment_scope(env);

                env = prev;
        }
}

ENVIRONMENT* down_scope(ENVIRONMENT* current) {
        current->next       = create_environment();
        current->next->prev = current;

        return current->next;
}

ENVIRONMENT* up_scope(ENVIRONMENT* current) {
        ENVIRONMENT* prev = current->prev;

        if (!prev) return current;

        prev->next = NULL;

        destroy_environment_scope(current);

        return prev;
}

void set_value(ENVIRONMENT* current, char* name, void* value, int value_sz) {
        hashtable_set(current->table, name, value, value_sz);
}

int update_value(ENVIRONMENT* current, char* name, void* value, int value_sz) {
        while (current) {
                if (hashtable_has(current, name)) {
                        set_value(current, name, value, value_sz);
                        return 1;
                }

                current = current->prev;
        }

        return 0;
}

void* get_value(ENVIRONMENT* current, char* name) {
        while (current) {
                void* obj = hashtable_get(current->table, name);

                if (obj) return obj;

                current = current->prev;
        }

        return NULL;
}