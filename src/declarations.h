#ifndef BISTROMATIC_DECLARATIONS_H
#define BISTROMATIC_DECLARATIONS_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct number
{
    int positive; // 0 = negative, positive otherwise
    int *whole_part;
    size_t whole_part_size;
    int *decimal_part;
    size_t decimal_part_size;
};

struct temporary_number
{
    int *whole_part;
    size_t whole_part_size;
    size_t whole_part_capacity;
    int *decimal_part;
    size_t decimal_part_size;
    size_t decimal_part_capacity;
};

enum oper
{
    NONE,
    POS, //+
    NEG, //-
    PLUS, // +
    MINUS, // -
    MULT, // *
    DIV, // /
    EUCLDIV, // //
    MOD, // %
    POW, // ^
    OPEN, // (
    CLOSE // )
};

struct token_list // linked list
{
    struct token_list *next;
    struct number *number;
    enum oper oper;
};
void pop_token_list(struct token_list **tl);
struct number *init_number(size_t whole_part_size, size_t decimal_part_size);
struct temporary_number *init_temporary_number(size_t whole_part_capacity,
                                               size_t decimal_part_capacity);
int resize_temporary_number_whole(struct temporary_number *tempnum);
int resize_temporary_number_decimal(struct temporary_number *tempnum);
void destroy_token_list(struct token_list *tl);
void destroy_temporary_number(struct temporary_number *tempnum);
void destroy_number(struct number *num);
struct number *convert_from_temporary(struct temporary_number *tempnum);
void normalize(struct number *num);
void print_number(struct number *num, const char *base, FILE *output);
void print_token_list(struct token_list *list, const char *base);
struct token_list *reverse_token_list(struct token_list *tl);
void transfer(struct token_list **sender, struct token_list **receiver);
void swap_size_t(size_t *a, size_t *b);
void swap_int_ptr(int **a, int **b);
void swap_numbers_in_place(struct number *a, struct number *b);
int absolute_greater(struct number *a, struct number *b);
int is_whole(struct number *num);

#endif // BISTROMATIC_DECLARATIONS_H
