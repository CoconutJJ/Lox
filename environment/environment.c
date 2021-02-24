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
        current->next = create_environment();

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
