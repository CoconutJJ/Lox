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
ENVIRONMENT* create_environment();
void         destroy_environment(ENVIRONMENT* env);
ENVIRONMENT* down_scope(ENVIRONMENT* current);
ENVIRONMENT* up_scope(ENVIRONMENT* current);
void  set_value(ENVIRONMENT* current, char* name, void* value, int value_sz);
int   update_value(ENVIRONMENT* current, char* name, void* value, int value_sz);
void* get_value(ENVIRONMENT* current, char* name);
#endif