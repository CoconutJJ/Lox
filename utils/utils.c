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
/**
 * Copy str_len bytes from src to dest and append a null terminator at the end.
 * Returns the number of bytes copied from src.
 */
int nstrcp(char *dest, char *src, int str_len) {
        int init = str_len;
        while (str_len > 0) {
                if (!(*src)) break;

                *dest = *src;
                dest++;
                src++;
                str_len--;
        }
        *dest = '\0';

        return init - str_len;
}
/**
 * Prints out msg to stderr and exits program with non-zero exit code when
 * called.
 */
void UNREACHABLE(char *msg) {
        if (msg) fprintf(stderr, msg);

        exit(EXIT_FAILURE);
}
