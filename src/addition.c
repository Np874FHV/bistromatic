#include "addition.h"

static int rearrange(struct number *a, struct number *b)
{
    if (b->whole_part_size > a->whole_part_size)
    {
        swap_int_ptr(&(a->whole_part), &(b->whole_part));
        swap_size_t(&(a->whole_part_size), &(b->whole_part_size));
    }
    if (b->decimal_part_size > a->decimal_part_size)
    {
        swap_int_ptr(&(a->decimal_part), &(b->decimal_part));
        swap_size_t(&(a->decimal_part_size), &(b->decimal_part_size));
    }
    a->whole_part_size += 1;
    int *tmp = realloc(a->whole_part, a->whole_part_size * sizeof(int));
    if (!tmp)
        return 1;
    a->whole_part = tmp;
    a->whole_part[a->whole_part_size - 1] = 0;
    return 0;
}

int addition(struct number *a, struct number *b, int base)
{
    // TEST SIGN
    int carry = 0;

    struct number *result = a;
    struct number *other = b;
    if (rearrange(result, other))
        return 1;
    if (other->decimal_part_size > 0)
    {
        for (size_t i = other->decimal_part_size - 1; i > 0; i--)
        {
            result->decimal_part[i] += other->decimal_part[i] + carry;
            carry = result->decimal_part[i] / base;
            result->decimal_part[i] %= base;
        }
        result->decimal_part[0] += other->decimal_part[0] + carry;
        carry = result->decimal_part[0] / base;
        result->decimal_part[0] %= base;
    }

    for (size_t i = 0; i < other->whole_part_size; i++)
    {
        result->whole_part[i] += other->whole_part[i] + carry;
        carry = result->whole_part[i] / base;
        result->whole_part[i] %= base;
    }
    size_t i = other->whole_part_size;
    while (carry > 0) // handle carry
    {
        result->whole_part[i] += 1;
        carry = result->whole_part[i] / base;
        result->whole_part[i] %= base;
        i++;
    }
    normalize(result);
    return 0;
}