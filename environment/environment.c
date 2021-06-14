
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
#include <stdio.h>
#include <stdlib.h>

#include "../types.h"
#include "hash_table.h"

/**
 * Creates a new environment ready for storage
 */
struct environment* create_environment() {
        struct environment* new_env = malloc(sizeof(struct environment));

        if (!new_env) {
                perror("malloc");
        }

        new_env->next = NULL;
        new_env->prev = NULL;
        new_env->table = hashtable_new();

        return new_env;
}

/**
 * Destroys the current environment scope
 */
void destroy_environment_scope(struct environment* env) {
        hashtable_destroy(env->table);
        free(env);
}

/**
 * Destroy the environment from env and up (including root)
 */
void destroy_environment(struct environment* env) {
        while (env) {
                struct environment* prev = env->prev;
                destroy_environment_scope(env);
                env = prev;
        }
}

/**
 * Enter a scope block
 */
struct environment* down_scope(struct environment* current) {
        current->next = create_environment();
        current->next->prev = current;

        return current->next;
}

/**
 * Leave the current scope block, silently ignores if current scope block is
 * also root scope. Will never destroy root environment
 */
struct environment* up_scope(struct environment* current) {
        struct environment* prev = current->prev;

        /**
         * Prevent root environment from being destroyed.
         */
        if (!prev) return current;

        prev->next = NULL;

        destroy_environment_scope(current);

        return prev;
}

/**
 * Set the name value keypair in the current environment.
 */
void set_value(struct environment* current, char* name, void* value,
               int value_sz) {
        hashtable_set(current->table, name, value, value_sz);
}

/**
 * Search current environment and higher scope environments for name value
 * keypair and update with new values
 */
int update_value(struct environment* current, char* name, void* value,
                 int value_sz) {
        while (current) {
                if (hashtable_has(current->table, name)) {
                        set_value(current, name, value, value_sz);
                        return 1;
                }

                current = current->prev;
        }

        return 0;
}

/**
 * Get the value associated with name from current environment or higher scoped
 * environments. Returns the value in the lowest environment containing such
 * name.
 */
void* get_value(struct environment* current, char* name) {
        while (current) {
                void* obj = hashtable_get(current->table, name);

                if (obj) return obj;

                current = current->prev;
        }

        return NULL;
}