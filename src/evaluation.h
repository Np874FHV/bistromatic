#ifndef UNTITLED_EVALUATION_H
#define UNTITLED_EVALUATION_H
#include "addition.h"
#include "declarations.h"
#include "division.h"
#include "euclidean_division.h"
#include "modulo.h"
#include "multiplication.h"
#include "power.h"
#include "substraction.h"

struct number *evaluate(struct token_list *input, int base_length,
                        size_t decimal_precision);

#endif // UNTITLED_EVALUATION_H
