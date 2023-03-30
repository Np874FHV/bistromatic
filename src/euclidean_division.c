#include "euclidean_division.h"

static void handle_carry_whole(struct number *result, int base);
static void add_one(struct number *result, int base);
static int absolute_greater_whole(struct number *a, struct number *b);
static int subs_whole(struct number *a, struct number *b, int base); // a > b
static void normalize_whole_part(struct number *num);
static int equals_zero(struct number *a);
static int subs_whole_with_power(struct number *a, struct number *b, int base,
                                 size_t pow);

static int absolute_greater_with_power(struct number *a, struct number *b,
                                       size_t pow)
{
    if (a->whole_part_size < b->whole_part_size + pow)
        return -1;
    if (a->whole_part_size > b->whole_part_size + pow)
        return 1;

    for (size_t i = b->whole_part_size - 1; i > 0; i--)
    {
        if (a->whole_part[i + pow] != b->whole_part[i])
            return a->whole_part[i + pow] - b->whole_part[i];
    }
    return a->whole_part[pow] - b->whole_part[0];
}

struct number *euclidean_division_aux(struct number *a, struct number *b,
                                      int base, size_t pow)
{
    if (absolute_greater_with_power(a, b, pow) < 0)
        return init_number(a->whole_part_size, 0);

    struct number *result = euclidean_division_aux(a, b, base, pow + 1);
    if (!result)
        return NULL;

    while (absolute_greater_with_power(a, b, pow) >= 0)
    {
        result->whole_part[pow] += 1;
        subs_whole_with_power(a, b, base, pow);
    }
    return result;
}

int euclidean_division(struct number *a, struct number *b, int base)
{
    if (!is_whole(b))
    {
        fprintf(stderr,
                "Cannot compute Euclidean division by a decimal number.\n");
        return 1;
    }

    if (equals_zero(b))
    {
        fprintf(stderr, "Cannot compute Euclidean division by 0!\n");
        return 1;
    }
    if (a->decimal_part_size > 0)
    {
        free(a->decimal_part);
        a->decimal_part_size = 0;
        a->decimal_part = NULL;
    }

    if (b->whole_part_size > a->whole_part_size) // result is 0
    {
        a->whole_part_size = 1;
        a->whole_part[0] = 0;
        return 0;
    }
    struct number *result = euclidean_division_aux(a, b, base, 0);
    if (!result)
        return 1;
    if ((a->positive && b->positive) || (!a->positive && !b->positive))
        result->positive = 1;
    else
        result->positive = 0;

    normalize_whole_part(result);

    swap_numbers_in_place(result, a);
    destroy_number(result);

    return 0;
}

static int subs_whole_with_power(struct number *a, struct number *b, int base,
                                 size_t pow)
{
    int carry = 0;

    size_t pos = pow;

    for (size_t i = 0; i < b->whole_part_size; i++)
    {
        a->whole_part[pos] -= b->whole_part[i] + carry;
        if (a->whole_part[pos] < 0)
        {
            carry = 1;
            a->whole_part[pos] += base;
        }
        else
            carry = 0;
        pos++;
    }

    while (carry > 0) // handle carry
    {
        a->whole_part[pos] -= 1;
        if (a->whole_part[pos] < 0)
            a->whole_part[pos] += base;
        else
            carry = 0;
        pos++;
    }
    normalize_whole_part(a);
    return 0;
}

int brute_euclidean_division(struct number *a, struct number *b, int base)
{
    if (!is_whole(a) || !is_whole(b))
    {
        fprintf(stderr,
                "Cannot compute Euclidean division of decimal numbers.\n");
        return 1;
    }

    if (equals_zero(b))
    {
        fprintf(stderr, "Cannot compute Euclidean division by 0!\n");
        return 1;
    }

    if (b->whole_part_size > a->whole_part_size) // result is 0
    {
        a->whole_part_size = 1;
        a->whole_part[0] = 0;
        return 0;
    }
    struct number *result =
        init_number(a->whole_part_size - b->whole_part_size + 1, 0);
    if (!result)
        return 1;
    if ((a->positive && b->positive) || (!a->positive && !b->positive))
        result->positive = 1;
    else
        result->positive = 0;

    while (absolute_greater_whole(a, b) >= 0)
    {
        subs_whole(a, b, base);
        add_one(result, base);
    }

    normalize_whole_part(result);

    swap_numbers_in_place(result, a);
    destroy_number(result);

    return 0;
}

static void handle_carry_whole(struct number *result, int base)
{
    int carry = result->whole_part[0] / base;
    result->whole_part[0] %= base;
    size_t k = 1;
    while (carry > 0)
    {
        result->whole_part[k] += carry;
        carry = result->whole_part[k] / base;
        result->whole_part[k] %= base;
        k++;
    }
}

static void add_one(struct number *result, int base)
{
    result->whole_part[0] += 1;
    handle_carry_whole(result, base);
}

static int
absolute_greater_whole(struct number *a,
                       struct number *b) //>0 = True, <0 = False, 0 = equal
{
    if (a->whole_part_size > b->whole_part_size)
        return 1;
    if (a->whole_part_size < b->whole_part_size)
        return -1;
    size_t i = a->whole_part_size - 1;
    while (i > 0)
    {
        if (a->whole_part[i] != b->whole_part[i])
            return a->whole_part[i] - b->whole_part[i];
        i--;
    }
    return a->whole_part[0] - b->whole_part[0];
}

static int
subs_whole(struct number *a, struct number *b,
           int base) // a > b and a->decimal_part_size >= b->decimal_part_size
{
    int carry = 0;

    struct number *result = a;
    struct number *other = b;

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
    normalize_whole_part(a);

    return 0;
}

static void normalize_whole_part(struct number *num)
{
    size_t i = num->whole_part_size - 1;
    while (i > 0 && num->whole_part[i] == 0)
    {
        i--;
    }
    num->whole_part_size = i + 1;
}

static int equals_zero(struct number *a)
{
    for (size_t i = 0; i < a->whole_part_size; i++)
    {
        if (a->whole_part[i] != 0)
            return 0;
    }
    return 1;
}

struct number *euclidean_division_with_remainder(struct number *a,
                                                 struct number *b,
                                                 int base) // a gets the result
{
    struct number *result;
    if (b->whole_part_size > a->whole_part_size)
    {
        result = init_number(1, 0);
        swap_numbers_in_place(a, result);
        return result;
    }
    result = init_number(a->whole_part_size - b->whole_part_size + 1, 0);

    if (!result)
        return NULL;

    while (absolute_greater_whole(a, b) >= 0)
    {
        subs_whole(a, b, base);
        add_one(result, base);
    }

    normalize_whole_part(result);
    swap_numbers_in_place(a, result);
    return result;
}
