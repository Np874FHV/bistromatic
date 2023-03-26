#include <stdio.h>
#include <unistd.h>

#include "src/handle_request.h"
#define DEFAULT_BASE "0123456789"

size_t string_to_size_t(const char *str)
{
    size_t res = 0;
    if (!str)
        return res;
    size_t pos = 0;
    while (str[pos] != '\0')
    {
        char n = str[pos] - '0';
        if (n < 0 || n > 9)
            return 0;
        res *= 10;
        res += n;
        pos++;
    }
    return res;
}

static int interactive(void)
{
    printf("Entering interactive mode.\nAny wrong request will properly close "
           "the program.\n");
    int result = 0;
    while (!result)
    {
        printf("bistro$ ");
        fflush(stdout);
        result = handle_request(stdin, stdout, DEFAULT_BASE, 100);
        if (!result)
            printf("\n");
    }
    return result;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
        return interactive();
    if (isatty(STDIN_FILENO) == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr,
                    "Incorrect input format. Expected:\n ./main output_name "
                    "base decimal_precision\n");
            return 2;
        }
        size_t decimal_precision = string_to_size_t(argv[3]);
        if (!decimal_precision)
        {
            fprintf(stderr, "Incorrect decimal precision.\n");
            return 2;
        }
        FILE *output = fopen(argv[1], "w");
        if (!output)
        {
            fprintf(stderr, "Cannot open output.\n");
            return 2;
        }
        int result = handle_request(stdin, output, argv[2], decimal_precision);
        fclose(output);
        return result;
    }
    if (argc != 5)
    {
        fprintf(stderr,
                "Incorrect input format. Expected:\n ./main input_name "
                "output_name base decimal_precision\n");
        return 2;
    }
    FILE *input = fopen(argv[1], "r+");
    if (input == NULL)
    {
        fprintf(stderr, "Could not open input file.\n");
        return 2;
    }
    FILE *output = fopen(argv[2], "w+");
    if (output == NULL)
    {
        fclose(input);
        fprintf(stderr, "Could not open output file.\n");
        return 2;
    }
    size_t decimal_precision = string_to_size_t(argv[4]);
    if (!decimal_precision)
    {
        fclose(input);
        fclose(output);
        fprintf(stderr, "Incorrect decimal precision.\n");
        return 2;
    }
    int result = handle_request(input, output, argv[3], decimal_precision);
    fclose(input);
    fclose(output);
    return result;
}
