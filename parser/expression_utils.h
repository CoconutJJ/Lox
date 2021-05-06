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
#ifndef EXPR_UTIL_H
#define EXPR_UTIL_H

EXPR_BIN_OP *create_expr_bin_op(EXPR_V op, int line);
EXPR_UNR_OP *create_expr_unr_op(EXPR_V op, int line);
EXPR_STR *create_expr_str(char *str, int line);
EXPR_NUM *create_expr_num(double num, int line);
EXPR_BOOL *create_expr_bool(int b, int line);
EXPR_VAR *create_expr_var(char *name, int line);
void free_expr(EXPR_OP *head);
#endif