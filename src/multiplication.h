#ifndef UNTITLED_MULTIPLICATION_H
#define UNTITLED_MULTIPLICATION_H

#include "declarations.h"

int multiplication(struct number *a, struct number *b, int base, int destroy);
struct number* multiplication_return_result(struct number *a, struct number *b, int base);
struct number *multiply_by_base(struct number *a);

#endif // UNTITLED_MULTIPLICATION_H
