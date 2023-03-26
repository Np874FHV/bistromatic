#include "base_converter.h"

int incorrect_base(char *base)
{
    if (base == NULL || strlen(base) < 2)
        return 1;

    char operators[] = "+-*/%()^!";

    for (size_t i = 0; i < strlen(base); i++)
        for (size_t j = 0; j < strlen(operators); j++)
            if (base[i] == operators[j])
                return 1;
    return 0;
}

ssize_t get_position(char a, char *model)
{
    for (size_t i = 0; i < strlen(model); i++)
        if (model[i] == a)
            return (ssize_t)i;
    return -1;
}
