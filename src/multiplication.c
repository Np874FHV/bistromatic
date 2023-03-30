#include "multiplication.h"

static void handle_carry_whole(struct number *result, size_t i, int base)
{
    int carry = result->whole_part[i] / base;
    result->whole_part[i] %= base;
    size_t k = i + 1;
    while (carry > 0)
    {
        result->whole_part[k] += carry;
        carry = result->whole_part[k] / base;
        result->whole_part[k] %= base;
        k++;
    }
    if (k > result->whole_part_size)
        result->whole_part_size = k;
}

static void handle_carry_decimal(struct number *result, size_t i, int base)
{
    int carry = result->decimal_part[i] / base;
    result->decimal_part[i] %= base;
    size_t k = 0;
    if (i > 0)
    {
        k = i - 1;
        while (k >= 1 && carry > 0)
        {
            result->decimal_part[k] += carry;
            carry = result->decimal_part[k] / base;
            result->decimal_part[k] %= base;
            k--;
        }
        result->decimal_part[0] += carry;
        carry = result->decimal_part[0] / base;
        result->decimal_part[0] %= base;
    }
    if (carry == 0)
        return;
    while (carry > 0)
    {
        result->whole_part[k] += carry;
        carry = result->whole_part[k] / base;
        result->whole_part[k] %= base;
        k++;
    }
    if (k > result->whole_part_size)
        result->whole_part_size = k;
}

int multiplication(struct number *a, struct number *b, int base, int destroy)
{
    size_t max_whole_part_size = a->whole_part_size + b->whole_part_size;
    size_t max_decimal_part_size = a->decimal_part_size + b->decimal_part_size;
    struct number *result =
        init_number(max_whole_part_size, max_decimal_part_size);
    if (!result)
        return 1;
    result->positive =
        (a->positive && b->positive) || (!a->positive && !b->positive);
    size_t i;
    size_t j;
    for (i = 0; i < a->whole_part_size; i++)
    {
        for (j = 0; j < b->whole_part_size; j++)
        {
            result->whole_part[i + j] += a->whole_part[i] * b->whole_part[j];
            handle_carry_whole(result, i + j, base);
        }

        for (j = 0; j < b->decimal_part_size; j++)
        {
            if (i > j)
            {
                result->whole_part[i - j - 1] +=
                    a->whole_part[i] * b->decimal_part[j];
                handle_carry_whole(result, i - j - 1, base);
            }
            else
            {
                result->decimal_part[j - i] +=
                    a->whole_part[i] * b->decimal_part[j];
                handle_carry_decimal(result, j - i, base);
            }
        }
    }
    for (i = 0; i < a->decimal_part_size; i++)
    {
        for (j = 0; j < b->decimal_part_size; j++)
        {
            result->decimal_part[i + j + 1] +=
                a->decimal_part[i] * b->decimal_part[j];
            handle_carry_decimal(result, i + j + 1, base);
        }

        for (j = 0; j < b->whole_part_size; j++)
        {
            if (j > i)
            {
                result->whole_part[j - i - 1] +=
                    a->decimal_part[i] * b->whole_part[j];
                handle_carry_whole(result, j - i - 1, base);
            }
            else
            {
                result->decimal_part[i - j] +=
                    a->decimal_part[i] * b->whole_part[j];
                handle_carry_decimal(result, i - j, base);
            }
        }
    }
    normalize(result);
    swap_numbers_in_place(a, result);
    if (result != b && destroy)
        destroy_number(result);
    return 0;
}

struct number *multiplication_return_result(struct number *a, struct number *b,
                                            int base)
{
    size_t max_whole_part_size = a->whole_part_size + b->whole_part_size;
    size_t max_decimal_part_size = a->decimal_part_size + b->decimal_part_size;
    struct number *result =
        init_number(max_whole_part_size, max_decimal_part_size);
    if (!result)
        return NULL;
    result->positive =
        (a->positive && b->positive) || (!a->positive && !b->positive);
    size_t i;
    size_t j;
    for (i = 0; i < a->whole_part_size; i++)
    {
        for (j = 0; j < b->whole_part_size; j++)
        {
            result->whole_part[i + j] += a->whole_part[i] * b->whole_part[j];
            handle_carry_whole(result, i + j, base);
        }

        for (j = 0; j < b->decimal_part_size; j++)
        {
            if (i > j)
            {
                result->whole_part[i - j - 1] +=
                    a->whole_part[i] * b->decimal_part[j];
                handle_carry_whole(result, i - j - 1, base);
            }
            else
            {
                result->decimal_part[j - i] +=
                    a->whole_part[i] * b->decimal_part[j];
                handle_carry_decimal(result, j - i, base);
            }
        }
    }
    for (i = 0; i < a->decimal_part_size; i++)
    {
        for (j = 0; j < b->decimal_part_size; j++)
        {
            result->decimal_part[i + j + 1] +=
                a->decimal_part[i] * b->decimal_part[j];
            handle_carry_decimal(result, i + j + 1, base);
        }

        for (j = 0; j < b->whole_part_size; j++)
        {
            if (j > i)
            {
                result->whole_part[j - i - 1] +=
                    a->decimal_part[i] * b->whole_part[j];
                handle_carry_whole(result, j - i - 1, base);
            }
            else
            {
                result->decimal_part[i - j] +=
                    a->decimal_part[i] * b->whole_part[j];
                handle_carry_decimal(result, i - j, base);
            }
        }
    }
    normalize(result);
    return result;
}

struct number *multiply_by_base(struct number *b)
{
    struct number *result = init_number(b->whole_part_size + 1, 0);
    if (!result)
        return NULL;
    for (size_t i = 0; i < b->whole_part_size; i++)
    {
        result->whole_part[i + 1] = b->whole_part[i];
    }
    result->whole_part[0] = 0;
    return result;
}
