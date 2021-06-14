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
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "../types.h"
struct environment* create_environment();
void destroy_environment(struct environment* env);
struct environment* down_scope(struct environment* current);
struct environment* up_scope(struct environment* current);
void set_value(struct environment* current, char* name, void* value,
               int value_sz);
int update_value(struct environment* current, char* name, void* value,
                 int value_sz);
void* get_value(struct environment* current, char* name);
#endif