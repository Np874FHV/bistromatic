#ifndef BISTROMATIC_LEXER_H
#define BISTROMATIC_LEXER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "addition.h"
#include "declarations.h"

struct token_list *get_tokens(FILE *input, const char *base);

#endif // BISTROMATIC_LEXER_H
