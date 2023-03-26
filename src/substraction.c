#include "substraction.h"

int subtraction(struct number *a, struct number *b, int base)
{
    int carry = 0;

    struct number *result = a;
    struct number *other = b;

    int decimal_part_swapped = 0;

    if (other->decimal_part_size > result->decimal_part_size)
    {
        swap_int_ptr(&(other->decimal_part), &(result->decimal_part));
        swap_size_t(&(other->decimal_part_size), &(result->decimal_part_size));
        decimal_part_swapped = 1;
    }

    if (result->decimal_part_size > 0)
    {
        if (decimal_part_swapped)
        {
            for (size_t i = result->decimal_part_size - 1;
                 i > other->decimal_part_size; i--)
            {
                result->decimal_part[i] =
                    base - result->decimal_part[i] - carry;
                carry = 1;
            }
            result->decimal_part[other->decimal_part_size] =
                base - result->decimal_part[other->decimal_part_size] - carry;
            carry = 1;

            if (other->decimal_part_size > 0)
            {
                for (size_t i = other->decimal_part_size - 1; i > 0; i--)
                {
                    result->decimal_part[i] = other->decimal_part[i]
                        - result->decimal_part[i] - carry;
                    if (result->decimal_part[i] < 0)
                    {
                        carry = 1;
                        result->decimal_part[i] += base;
                    }
                    else
                        carry = 0;
                }
                result->decimal_part[0] =
                    other->decimal_part[0] - result->decimal_part[0] - carry;
                if (result->decimal_part[0] < 0)
                {
                    carry = 1;
                    result->decimal_part[0] += base;
                }
                else
                    carry = 0;
            }
        }
        else if (other->decimal_part_size > 0)
        {
            for (size_t i = other->decimal_part_size - 1; i > 0; i--)
            {
                result->decimal_part[i] -= other->decimal_part[i] + carry;
                if (result->decimal_part[i] < 0)
                {
                    carry = 1;
                    result->decimal_part[i] += base;
                }
                else
                    carry = 0;
            }
            result->decimal_part[0] -= other->decimal_part[0] + carry;
            if (result->decimal_part[0] < 0)
            {
                carry = 1;
                result->decimal_part[0] += base;
            }
            else
                carry = 0;
        }
    }

    for (size_t i = 0; i < other->whole_part_size; i++)
    {
        result->whole_part[i] -= other->whole_part[i] + carry;
        if (result->whole_part[i] < 0)
        {
            carry = 1;
            result->whole_part[i] += base;
        }
        else
            carry = 0;
    }

    size_t i = other->whole_part_size;

    while (carry > 0) // handle carry
    {
        result->whole_part[i] -= 1;
        if (result->whole_part[i] < 0)
            result->whole_part[i] += base;
        else
            carry = 0;
        i++;
    }

    normalize(result);

    return 0;
}
