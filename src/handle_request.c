#include "handle_request.h"

#include "evaluation.h"
#include "lexer.h"
#include "parser.h"
#include "shunting_yard.h"

int handle_request(FILE *input, FILE *output, const char *base,
                   size_t decimal_precision)
{
    if (!input || !output || !base)
        return 1;
    struct token_list *tl = get_tokens(input, base);
    if (!tl)
    {
        fprintf(stderr, "Incorrect input!\n");
        return 1;
    }
    if (parser(tl))
    {
        destroy_token_list(tl);
        return 1;
    }

    struct token_list *sy = shunting_yard(tl);
    if (!sy)
        return 1;
    struct number *result = evaluate(sy, strlen(base), decimal_precision);
    if (!result)
        return 1;
    print_number(result, base, output);
    destroy_number(result);
    return 0;
}

struct number *get_result(FILE *input, const char *base, size_t decimal_precision)
{
    if (!input || !base)
        return NULL;
    struct token_list *tl = get_tokens(input, base);
    if (!tl)
    {
        fprintf(stderr, "Incorrect input!\n");
        return NULL;
    }
    if (parser(tl))
    {
        destroy_token_list(tl);
        return NULL;
    }
    struct token_list *sy = shunting_yard(tl);
    if (!sy)
        return NULL;
    return evaluate(sy, strlen(base), decimal_precision);
}
