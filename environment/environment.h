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