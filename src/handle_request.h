#ifndef UNTITLED_HANDLE_REQUEST_H
#define UNTITLED_HANDLE_REQUEST_H

#include <stdio.h>

int handle_request(FILE *input, FILE *output, const char *base,
                   size_t decimal_precision);

struct number *get_result(FILE *input, const char *base,
                          size_t decimal_precision);

#endif // UNTITLED_HANDLE_REQUEST_H
