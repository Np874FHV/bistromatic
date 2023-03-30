#ifndef UNTITLED_EUCLIDEAN_DIVISION_H
#define UNTITLED_EUCLIDEAN_DIVISION_H

#include "declarations.h"
#include "multiplication.h"

int euclidean_division(struct number *a, struct number *b, int base);
struct number *euclidean_division_with_remainder(struct number *a,
                                                 struct number *b, int base);
int brute_euclidean_division(struct number *a, struct number *b, int base);

#endif // UNTITLED_EUCLIDEAN_DIVISION_H
