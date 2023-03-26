#include "division.h"

static void handle_carry_whole(struct result *result, int base);
static int subs(struct number *a, struct number *b, int base);
static int resize_result_whole(struct result *result);
static struct number *convert_from_result(struct result *result);
static struct result *init_result(size_t whole_part_capacity,
                                  size_t decimal_part_capacity);
static void destroy_result(struct result *result);
static int absolute_greater_with_power(struct number *a, struct number *b,
                                       size_t negative_power);
static int subs_with_neg_pow(struct number *a, struct number *b,
                             size_t negative_power, int base);
static void handle_carry_decimal(struct result *result, size_t i, int base);
static void normalize_whole_part(struct number *num);

/*
void print_result(struct result *num, const char *base)
{
    printf("WHOLE: %ld ; DECIMAL : %ld\n", num->whole_part_size,
num->decimal_part_size); for (size_t i = num->whole_part_size-1; i > 0; i--)
        printf("%c", base[num->whole_part[i]]);
    printf("%c", base[num->whole_part[0]]);
    if (num->decimal_part_size > 0)
        printf(".");
    for (size_t i = 0; i < num->decimal_part_size; i++)
        printf("%d ", num->decimal_part[i]);
    printf("\n");
}
*/

static size_t maximum(size_t a, size_t b)
{
    if (a > b)
        return a;
    return b;
}

static int absolute_positive(struct number *num)
{
    return !(num->decimal_part_size == 0 && num->whole_part_size == 1
             && num->whole_part[0] == 0);
}

int add_one(struct result *result, int base)
{
    if (result->whole_part_size == result->whole_part_capacity
        && resize_result_whole(result))
        return 1;
    result->whole_part[0] += 1;
    handle_carry_whole(result, base);
    if (result->whole_part[result->whole_part_size] > 0)
        result->whole_part_size = result->whole_part_size + 1;
    return 0;
}

int add_one_pow(struct result *result, int base, size_t pos)
{
    result->decimal_part[pos - 1] += 1;
    handle_carry_decimal(result, pos - 1, base);
    result->decimal_part_size = pos;
    return 0;
}

static int equals_zero(struct number *a)
{
    for (size_t i = 0; i < a->whole_part_size; i++)
    {
        if (a->whole_part[i] != 0)
            return 0;
    }
    for (size_t i = 0; i < a->decimal_part_size; i++)
    {
        if (a->decimal_part[i] != 0)
            return 0;
    }
    return 1;
}

int division(struct number *a, struct number *b, int base,
             size_t decimal_precision)
{
    if (!absolute_positive(b)) // if b == 0
    {
        fprintf(stderr, "Cannot divide by 0!\n");
        return 1;
    }
    struct result *result = init_result(10, decimal_precision); // to sub
    if (!result)
        goto error_division;
    result->whole_part_size = 1;
    if (a->decimal_part_size < b->decimal_part_size + decimal_precision)
    {
        size_t n = maximum(a->decimal_part_size,
                           b->decimal_part_size + decimal_precision);
        int *tmp = realloc(a->decimal_part, n * sizeof(int));
        if (!tmp)
            goto error_division;
        memset(tmp + a->decimal_part_size, 0,
               (n - a->decimal_part_size) * sizeof(int));
        a->decimal_part = tmp;
        a->decimal_part_size = n;
    }

    while (!equals_zero(a) && absolute_greater(a, b) >= 0)
    {
        add_one(result, base);
        subs(a, b, base);
    }

    size_t negative_power = 1;
    while (!equals_zero(a) && absolute_positive(a)
           && negative_power <= decimal_precision)
    {
        while (!equals_zero(a)
               && absolute_greater_with_power(a, b, negative_power) >= 0)
        {
            subs_with_neg_pow(a, b, negative_power, base);
            add_one_pow(result, base, negative_power);
        }

        negative_power += 1;
    }

    struct number *res = convert_from_result(result);
    if ((a->positive && b->positive) || (!a->positive && !b->positive))
        res->positive = 1;
    else
        res->positive = 0;
    swap_numbers_in_place(a, res);
    destroy_number(res);
    return 0;

error_division:
    fprintf(stderr, "Error in division\n");
    if (result)
        destroy_result(result);
    return 1;
}

static void handle_carry_whole(struct result *result, int base)
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

static void handle_carry_decimal(struct result *result, size_t i, int base)
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
    while (carry > 0)
    {
        result->whole_part[k] += carry;
        carry = result->whole_part[k] / base;
        result->whole_part[k] %= base;
        k++;
    }
}

static int
subs(struct number *a, struct number *b,
     int base) // a > b and a->decimal_part_size >= b->decimal_part_size
{
    int carry = 0;

    struct number *result = a;
    struct number *other = b;

    if (result->decimal_part_size > 0)
    {
        if (other->decimal_part_size > 0)
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
    normalize_whole_part(a);

    return 0;
}

static int resize_result_whole(struct result *result)
{
    result->whole_part_capacity = 2 * result->whole_part_capacity;
    int *tmp =
        realloc(result->whole_part, result->whole_part_capacity * sizeof(int));
    if (!tmp)
        return 1;
    result->whole_part = tmp;
    return 0;
}

static struct number *convert_from_result(struct result *result)
{
    struct number *num = malloc(sizeof(struct number));
    if (!num)
        return NULL;
    num->whole_part = result->whole_part;
    num->whole_part_size = result->whole_part_size;
    num->decimal_part = result->decimal_part;
    num->decimal_part_size = result->decimal_part_size;
    num->positive = 1;
    normalize(num);
    free(result);
    return num;
}

static struct result *init_result(size_t whole_part_capacity,
                                  size_t decimal_part_capacity)
{
    struct result *res = malloc(sizeof(struct result));
    if (!res)
        return NULL;
    res->whole_part_capacity = whole_part_capacity;
    res->whole_part_size = 0;
    res->decimal_part_capacity = decimal_part_capacity;
    res->decimal_part_size = 0;
    res->whole_part = calloc(whole_part_capacity, sizeof(int));
    if (!res->whole_part)
        goto error_init_temporary_number;
    res->decimal_part = calloc(decimal_part_capacity, sizeof(int));
    if (decimal_part_capacity > 0 && !res->decimal_part)
        goto error_init_temporary_number;
    return res;

error_init_temporary_number:
    if (res->whole_part)
        free(res->whole_part);
    if (res->decimal_part_capacity > 0 && res->decimal_part)
        free(res->decimal_part);
    destroy_result(res);
    return NULL;
}

static void destroy_result(struct result *result)
{
    if (result->whole_part)
        free(result->whole_part);
    if (result->decimal_part)
        free(result->decimal_part);
    free(result);
}

static int get_at_whole_position(struct number *a, size_t pos,
                                 size_t negative_power)
{
    if (pos >= negative_power && pos - negative_power < a->whole_part_size)
        return a->whole_part[pos - negative_power];
    if (negative_power - pos - 1 < a->decimal_part_size)
        return a->decimal_part[negative_power - pos - 1];
    return 0;
}

static int get_at_decimal_position(struct number *a, size_t pos,
                                   size_t negative_power)
{
    if (pos + negative_power < a->decimal_part_size)
        return a->decimal_part[pos + negative_power];
    return 0;
}

static int absolute_greater_with_power(
    struct number *a, struct number *b,
    size_t negative_power) //>0 = True, <0 = False, 0 =equal
                           // a < b
{
    size_t wpa = a->whole_part_size;
    if (wpa == 1 && a->whole_part[0] == 0)
        wpa = 0;
    if (b->whole_part_size > negative_power)
    {
        if (wpa > b->whole_part_size - negative_power)
            return 1;
        if (wpa < b->whole_part_size - negative_power)
            return -1;
    }
    if (b->whole_part_size <= negative_power)
    {
        for (size_t i = 0; i < a->whole_part_size; i++)
        {
            if (a->whole_part[i] != 0)
                return 1;
        }
        for (size_t i = 0; i < negative_power - b->whole_part_size; i++)
        {
            if (a->decimal_part[i] != 0)
                return 1;
        }
    }

    for (size_t i = b->whole_part_size - 1; i > 0; i--)
    {
        if (b->whole_part[i] != get_at_whole_position(a, i, negative_power))
            return get_at_whole_position(a, i, negative_power)
                - b->whole_part[i];
    }
    if (b->whole_part[0] != get_at_whole_position(a, 0, negative_power))
        return get_at_whole_position(a, 0, negative_power) - b->whole_part[0];
    for (size_t i = 0; i < b->decimal_part_size; i++)
    {
        if (b->decimal_part[i] != get_at_decimal_position(a, i, negative_power))
            return get_at_decimal_position(a, i, negative_power)
                - b->decimal_part[i];
    }
    return 0;
}

static int subs_with_neg_pow(
    struct number *a, struct number *b, size_t negative_power,
    int base) // a > b and a->decimal_part_size >= b->decimal_part_size
{
    for (size_t i = 0; i < b->whole_part_size; i++)
    {
        if (i >= negative_power)
            a->whole_part[i - negative_power] -= b->whole_part[i];
        else
            a->decimal_part[negative_power - i - 1] -= b->whole_part[i];
    }
    for (size_t i = 0; i < b->decimal_part_size; i++)
    {
        a->decimal_part[negative_power + i] -= b->decimal_part[i];
    }

    int carry = 0;
    for (size_t i = b->decimal_part_size + negative_power - 1; i > 0; i--)
    {
        a->decimal_part[i] -= carry;
        if (a->decimal_part[i] < 0)
        {
            a->decimal_part[i] += base;
            carry = 1;
        }
        else
            carry = 0;
    }
    a->decimal_part[0] -= carry;
    if (a->decimal_part[0] < 0)
    {
        a->decimal_part[0] += base;
        carry = 1;
    }
    else
        carry = 0;

    size_t k = 0;
    if (b->whole_part_size > negative_power)
        k = b->whole_part_size - negative_power;

    for (size_t i = 0; i < k; i++)
    {
        a->whole_part[i] -= carry;
        if (a->whole_part[i] < 0)
        {
            a->whole_part[i] += base;
            carry = 1;
        }
        else
            carry = 0;
    }

    while (carry > 0)
    {
        a->whole_part[k] -= carry;
        if (a->whole_part[k] < 0)
        {
            a->whole_part[k] += base;
            carry = 1;
        }
        else
            carry = 0;
        k++;
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
    num->whole_part =
        realloc(num->whole_part, num->whole_part_size * sizeof(int));
}
