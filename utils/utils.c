#include <stdio.h>

/**
 * Copy n bytes from src to dest and append a null terminator at the end.
 * Returns the number of bytes copied from src.
 */
int nstrcp(char *dest, char *src, int str_len)
{

    int init = str_len;
    while (str_len > 0)
    {

        if (!(*src))
            break;

        *dest = *src;
        dest++;
        src++;
        str_len--;
    }
    *dest = '\0';

    return init - str_len;
}
