#ifndef UNTITLED_DIVISION_H
#define UNTITLED_DIVISION_H

#include "declarations.h"

struct result
{
    int *whole_part;
    size_t whole_part_size;
    size_t whole_part_capacity;
    int *decimal_part;
    size_t decimal_part_size;
    size_t decimal_part_capacity;
};

int division(struct number *a, struct number *b, int base,
             size_t decimal_precision);

#endif // UNTITLED_DIVISION_H
