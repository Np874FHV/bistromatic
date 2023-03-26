#include "lexer.h"

static int incorrect_base(const char *base)
{
    size_t i = 0;
    while (base[i] != '\0')
    {
        if (strchr("+-*/%^(). ", base[i]) != NULL)
            return 1;
        if (strchr(base + i + 1, base[i]) != NULL)
            return 1;
        i++;
    }
    return strlen(base) < 2;
}

static int is_operator(int t)
{
    return strchr("+-*/%^()", (char)t) != NULL;
}

static int convert_to_base(const char *base, int digit)
{
    char c = (char)digit;
    int n = 0;
    while (base[n] != '\0')
    {
        if (base[n] == c)
            return n;
        n++;
    }
    return -1;
}

static enum oper convert_to_oper(int t)
{
    switch (t)
    {
    case '+': // PLUS/POS conflict is solved later
        return PLUS;
    case '-':
        return MINUS;
    case '*':
        return MULT;
    case '/':
        return DIV; // DIV/EUCLDIV conflict is solved later
    case '%':
        return MOD;
    case '^':
        return POW;
    case '(':
        return OPEN;
    case ')':
        return CLOSE;
    default:
        return NONE;
    }
}

static struct token_list *get_next_token(FILE *input, const char *base,
                                         int *shared_char, int *error_occurred)
{
    struct token_list *res = calloc(1, sizeof(struct token_list));
    struct temporary_number *tempnum = NULL;
    if (!res)
        goto error_get_next_token;
    int t = *shared_char;
    if (t == '\0')
        t = fgetc(input);
    while (t == ' ')
        t = fgetc(input);
    if (t == EOF || t == '\n')
    {
        free(res);
        return NULL;
    }
    if (is_operator(t))
    {
        res->oper = convert_to_oper(t);
        if (res->oper == DIV)
        {
            t = fgetc(input);
            if (t == '/')
            {
                res->oper = EUCLDIV;
                t = fgetc(input);
            }
            *shared_char = t;
            return res;
        }
        *shared_char = fgetc(input);
        return res;
    }
    int converted = convert_to_base(base, t);
    if (converted == -1) // Incorrect character
        goto error_get_next_token;
    fflush(stdout);
    res->oper = NONE;
    tempnum = init_temporary_number(2, 2);
    if (!tempnum)
        goto error_get_next_token;
    while (converted >= 0)
    {
        if (tempnum->whole_part_size + 1 == tempnum->whole_part_capacity
            && resize_temporary_number_whole(tempnum))
            goto error_get_next_token;
        tempnum->whole_part[tempnum->whole_part_size] = converted;
        tempnum->whole_part_size += 1;
        t = fgetc(input);
        converted = convert_to_base(base, t);
    }
    if (t != '.')
    {
        *shared_char = t;
        free(tempnum->decimal_part);
        tempnum->decimal_part = NULL;
        struct number *num = convert_from_temporary(tempnum);
        if (!num)
            goto error_get_next_token;
        res->number = num;
        return res;
    }
    t = fgetc(input);
    converted = convert_to_base(base, t);
    while (converted >= 0)
    {
        if (tempnum->decimal_part_size + 1 == tempnum->decimal_part_capacity
            && resize_temporary_number_decimal(tempnum))
            goto error_get_next_token;
        tempnum->decimal_part[tempnum->decimal_part_size] = converted;
        tempnum->decimal_part_size += 1;
        t = fgetc(input);
        converted = convert_to_base(base, t);
    }
    if (t == '.')
        goto error_get_next_token;
    *shared_char = t;
    struct number *num = convert_from_temporary(tempnum);
    if (!num)
        goto error_get_next_token;
    res->number = num;
    return res;

error_get_next_token:
    *error_occurred = 1;
    destroy_temporary_number(tempnum);
    if (!res)
        return NULL;
    destroy_token_list(res);
    return NULL;
}

struct token_list *get_tokens(FILE *input, const char *base)
{
    if (incorrect_base(base))
    {
        fprintf(stderr, "Incorrect base\n");
        return NULL;
    }
    int error_occurred = 0;
    int shared_char = '\0';
    struct token_list *head =
        get_next_token(input, base, &shared_char, &error_occurred);
    if (!head)
        return NULL;
    struct token_list *actual = head;
    while (actual != NULL)
    {
        actual->next =
            get_next_token(input, base, &shared_char, &error_occurred);
        actual = actual->next;
    }
    if (error_occurred)
    {
        destroy_token_list(head);
        return NULL;
    }
    return head;
}
