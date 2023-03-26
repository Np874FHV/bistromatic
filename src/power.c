#include "power.h"

static int equals_zero(struct number *a)
{
    for (size_t i = 0; i < a->whole_part_size; i++)
    {
        if (a->whole_part[i] != 0)
            return 0;
    }
    return 1;
}

static int power_aux(struct number *a, struct number *b,
                     struct number *const_two, int base) // a^b
{
    if (equals_zero(b))
    {
        b->whole_part[0] = 1;
        return 0;
    }
    struct number *result =
        euclidean_division_with_remainder(b, const_two, base);
    if (!result)
        return 1;
    if (power_aux(a, b, const_two, base))
        goto error_power_aux;
    if (multiplication(b, b, base, 1))
        goto error_power_aux;
    if (result->whole_part[0] == 1 && multiplication(b, a, base, 1))
        goto error_power_aux;
    destroy_number(result);
    return 0;

error_power_aux:
    destroy_number(result);
    return 1;
}

int power(struct number *a, struct number *b, int base)
{
    if (b->decimal_part_size > 0 || b->positive == 0)
    {
        fprintf(stderr, "Cannot compute a decimal or negative power\n");
        return 1;
    }

    struct number *const_two;
    if (base == 2)
    {
        const_two = init_number(2, 0);
        if (!const_two)
            return 1;
        const_two->whole_part[1] = 1;
    }
    else
    {
        const_two = init_number(1, 0);
        if (!const_two)
            return 1;
        const_two->whole_part[0] = 2;
    }

    int result = power_aux(a, b, const_two, base);
    swap_numbers_in_place(a, b);
    destroy_number(const_two);
    return result;
}
