#include "parser.h"

static void eat_signs(struct token_list **token_list);
static int eat_number(struct token_list **token_list);
static int eat_open_parenthesis(struct token_list **token_list);
static int eat_operator(struct token_list **token_list);
static struct token_list *aux_parser(struct token_list *token_list);

static void eat_signs(struct token_list **token_list)
{
    struct token_list *tl = *token_list;
    while (tl && (tl->oper == PLUS || tl->oper == MINUS))
    {
        tl = tl->next;
    }
    *token_list = tl;
}

static int eat_number(struct token_list **token_list)
{
    if (*token_list && (*token_list)->number != NULL)
    {
        *token_list = (*token_list)->next;
        return 0;
    }
    return 1;
}

static int eat_open_parenthesis(struct token_list **token_list)
{
    if (*token_list && (*token_list)->oper == OPEN)
    {
        *token_list = (*token_list)->next;
        return 0;
    }
    return 1;
}

static int eat_close_parenthesis(struct token_list **token_list)
{
    if (*token_list && (*token_list)->oper == CLOSE)
    {
        *token_list = (*token_list)->next;
        return 0;
    }
    return 1;
}

static int eat_operator(struct token_list **token_list)
{
    if (!*token_list)
        return 1;
    switch ((*token_list)->oper)
    {
    case OPEN:
        return 1;
    case CLOSE:
        return 1;
    case NONE:
        return 1;
    default:
        *token_list = (*token_list)->next;
        return 0;
    }
}

int parser(struct token_list *token_list)
{
    return aux_parser(token_list) != NULL;
}

static struct token_list *aux_parser(struct token_list *token_list)
{
    struct token_list *tl = token_list;
    while (tl)
    {
        if (tl->oper == CLOSE)
            return tl;
        eat_signs(&tl);
        if (eat_open_parenthesis(&tl) == 0)
        {
            if (eat_close_parenthesis(&tl) == 0)
            {
                fprintf(stderr, "Empty parenthesis\n");
                goto error_parser;
            }
            tl = aux_parser(tl);
            if (eat_close_parenthesis(&tl))
            {
                fprintf(stderr, "Unmatched parenthesis\n");
                goto error_parser;
            }
        }
        else if (eat_number(&tl))
            goto error_parser;
        if (!tl || tl->oper == CLOSE)
            return tl;
        if (eat_operator(&tl))
        {
            fprintf(stderr, "Incorrect expression\n");
            goto error_parser;
        }
        if (!tl)
            goto error_parser;
    }
    return NULL;

error_parser:
    fprintf(stderr, "Error in parser\n");
    return token_list;
}
