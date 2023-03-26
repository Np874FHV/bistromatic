#include "shunting_yard.h"

static void pop_oper(struct token_list **tl);
static int precedence(enum oper oper);
static void plus_minus(struct token_list **input, struct token_list **output,
                       struct token_list **operators);
static void mult_div_eucl(struct token_list **input, struct token_list **output,
                          struct token_list **operators);
static void power(struct token_list **input, struct token_list **output,
                  struct token_list **operators);
static void open_par(struct token_list **input, struct token_list **operators);
static int close_par(struct token_list **input, struct token_list **output,
                     struct token_list **operators);

static void pop_oper(struct token_list **tl)
{
    if (!tl || !*tl)
        return;
    struct token_list *head = *tl;
    *tl = head->next;
    free(head);
}

struct token_list *shunting_yard(struct token_list *inp)
{
    if (!inp)
        return NULL;
    struct token_list *input = inp;
    struct token_list *operators = NULL;
    struct token_list *output = NULL;
    enum oper last = NONE;
    while (input)
    {
        if (input->oper == NONE)
        {
            transfer(&input, &output);
            last = NONE;
            continue;
        }
        if ((input->oper != NONE && input->oper != OPEN)
            && ((last != NONE && last != CLOSE)
                || (!operators && !output))) // unary operator
        {
            enum oper op = input->oper;
            if (op != PLUS && op != MINUS)
            {
                goto error_shunting_yard;
            }
            if (input->oper == PLUS)
            {
                input->oper = POS;
                last = POS;
            }
            if (input->oper == MINUS)
            {
                input->oper = NEG;
                last = NEG;
            }
            transfer(&input, &operators);
            continue;
        }
        switch (input->oper)
        {
        case PLUS:
            plus_minus(&input, &output, &operators);
            last = PLUS;
            break;
        case MINUS:
            plus_minus(&input, &output, &operators);
            last = MINUS;
            break;
        case MULT:
            mult_div_eucl(&input, &output, &operators);
            last = MINUS;
            break;
        case DIV:
            mult_div_eucl(&input, &output, &operators);
            last = DIV;
            break;
        case EUCLDIV:
            mult_div_eucl(&input, &output, &operators);
            last = EUCLDIV;
            break;
        case MOD:
            mult_div_eucl(&input, &output, &operators);
            last = MOD;
            break;
        case POW:
            power(&input, &output, &operators);
            last = POW;
            break;
        case OPEN:
            open_par(&input, &operators);
            last = OPEN;
            break;
        case CLOSE:
            if (close_par(&input, &output, &operators))
                goto error_shunting_yard;
            last = CLOSE;
            break;
        default:
            goto error_shunting_yard;
        }
    }
    while (operators)
    {
        if (operators->oper == OPEN)
        {
            fprintf(stderr, "Unmatched parenthesis\n");
            goto error_shunting_yard;
        }
        transfer(&operators, &output);
    }

    return reverse_token_list(output);

error_shunting_yard:
    destroy_token_list(input);
    destroy_token_list(output);
    destroy_token_list(operators);
    fprintf(stderr, "Shunting-Yard failed\n");
    return NULL;
}

static int precedence(enum oper oper)
{
    switch (oper)
    {
    case POS:
        return 4;
    case NEG:
        return 4;
    case PLUS:
        return 1;
    case MINUS:
        return 1;
    case MULT:
        return 2;
    case DIV:
        return 2;
    case EUCLDIV:
        return 2;
    case MOD:
        return 2;
    case POW:
        return 3;
    case OPEN:
        return -1;
    case CLOSE:
        return -2;
    default:
        return 0;
    }
}

static void plus_minus(struct token_list **input, struct token_list **output,
                       struct token_list **operators)
{
    while (*operators && precedence((*operators)->oper) > 0)
    {
        transfer(operators, output);
    }
    transfer(input, operators);
}

static void mult_div_eucl(struct token_list **input, struct token_list **output,
                          struct token_list **operators)
{
    while (*operators && precedence((*operators)->oper) > 1)
    {
        transfer(operators, output);
    }
    transfer(input, operators);
}

static void power(struct token_list **input, struct token_list **output,
                  struct token_list **operators)
{
    while (*operators && precedence((*operators)->oper) >= 3)
    {
        transfer(operators, output);
    }
    transfer(input, operators);
}

static void open_par(struct token_list **input, struct token_list **operators)
{
    transfer(input, operators);
}

static int close_par(struct token_list **input, struct token_list **output,
                     struct token_list **operators)
{
    while (*operators && (*operators)->oper != OPEN)
    {
        transfer(operators, output);
    }
    if (*operators)
    {
        pop_oper(input);
        pop_oper(operators);
        return 0;
    }
    return 1;
}
