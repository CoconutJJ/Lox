#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>

/**
 * Copy n bytes from src to dest and append a null terminator at the end.
 * Returns the number of bytes copied from src.
 */
int nstrcp(char *dest, char *src, int str_len);



#endif