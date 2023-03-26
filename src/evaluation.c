#include "evaluation.h"

static int smart_subtraction(struct number *a, struct number *b,
                             int base_length);
static int smart_addition(struct number *a, struct number *b, int base_length);

static int smart_addition(struct number *a, struct number *b, int base_length)
{
    if (a->positive)
    {
        if (b->positive)
            return addition(a, b, base_length);
        b->positive = 1;
        return smart_subtraction(a, b, base_length); // x + (-y) = x - y
    }
    if (b->positive)
    {
        a->positive = 1;
        swap_numbers_in_place(a, b);
        return smart_subtraction(a, b, base_length); // (-x)+y = y-x
    }
    return addition(a, b, base_length); // (-x) + (-y) = -(x+y)
}

static int smart_subtraction(struct number *a, struct number *b,
                             int base_length)
{
    if (a->positive)
    {
        if (b->positive)
        {
            if (absolute_greater(a, b)
                >= 0) // The subtraction must be authorized
                return subtraction(a, b, base_length);
            swap_numbers_in_place(a, b);
            a->positive = 0;
            return subtraction(a, b, base_length); // if x < y, x-y -> - (y-x)
        }
        return addition(a, b, base_length); // x - (-y) = x+y
    }
    if (b->positive) // (-x)-y = -(x+y)
        return addition(a, b, base_length);
    b->positive = 1;
    swap_numbers_in_place(a, b);
    return smart_subtraction(a, b, base_length); // (-x) - (-y) = (-x) + y = y-x
}

static struct token_list *
eval(struct token_list *input, struct token_list *output, int base_length,
     size_t decimal_precision) // The parser ensures the request is correct
{
    if (!input)
        return output;
    struct token_list *new_output = output;
    struct token_list *new_input = input;
    switch (input->oper)
    {
    case POS:
        break;
    case NEG:
        output->number->positive = !output->number->positive;
        break;
    case PLUS:
        if (smart_addition(output->next->number, output->number, base_length))
            goto error_eval;
        pop_token_list(&new_output);
        break;
    case MINUS:
        if (smart_subtraction(output->next->number, output->number,
                              base_length))
            goto error_eval;
        pop_token_list(&new_output);
        break;
    case MULT:
        if (multiplication(output->next->number, output->number, base_length,
                           1))
            goto error_eval;
        pop_token_list(&new_output);
        break;
    case DIV:
        if (division(output->next->number, output->number, base_length,
                     decimal_precision))
            goto error_eval;
        pop_token_list(&new_output);
        break;
    case EUCLDIV:
        if (euclidean_division(output->next->number, output->number,
                               base_length))
            goto error_eval;
        pop_token_list(&new_output);
        break;
    case MOD:
        if (modulo(output->next->number, output->number, base_length))
            goto error_eval;
        pop_token_list(&new_output);
        break;
    case POW:

        if (power(output->next->number, output->number, base_length))
            goto error_eval;
        pop_token_list(&new_output);
        break;
    default: // input is a number
        transfer(&new_input, &new_output);
        return eval(new_input, new_output, base_length, decimal_precision);
    }
    pop_token_list(&new_input);
    return eval(new_input, new_output, base_length, decimal_precision);

error_eval:
    destroy_token_list(new_input);
    destroy_token_list(new_output);
    fprintf(stderr, "The evaluation failed\n");
    return NULL;
}

struct number *evaluate(struct token_list *input, int base_length,
                        size_t decimal_precision)
{
    if (!input)
        return NULL;
    struct token_list *result_list =
        eval(input, NULL, base_length, decimal_precision);
    if (!result_list)
        return NULL;
    struct number *result_number = result_list->number;
    free(result_list);
    return result_number;
}
