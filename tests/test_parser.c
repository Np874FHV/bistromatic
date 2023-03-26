#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../src/declarations.h"
#include "../src/handle_request.h"

#define DB "0123456789"

Test(Parenthesis, unmatched1)
{
    const char *filename = "unmatched1.txt";
    const char *command = "(1+1\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(Parenthesis, unmatched2)
{
    const char *filename = "unmatched2.txt";
    const char *command = "1+1)\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(Parenthesis, unmatched3)
{
    const char *filename = "unmatched3.txt";
    const char *command = "1+((2+5)*2\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(operators, incorrect1)
{
    const char *filename = "incorrect1.txt";
    const char *command = "1**2\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(operators, incorrect2)
{
    const char *filename = "incorrect2.txt";
    const char *command = "1-*2\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(operators, incorrect3)
{
    const char *filename = "incorrect3.txt";
    const char *command = "1**2\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(operators, incorrect4)
{
    const char *filename = "incorrect4.txt";
    const char *command = "1+(1*)\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(operators, correct1)
{
    const char *filename = "correct1.txt";
    const char *command = "--++-1*-2/++---++1\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(0, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(general, gcorrect1)
{
    const char *filename = "gcorrect1.txt";
    const char *command = "1\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(0, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(general, gcorrect2)
{
    const char *filename = "gcorrect2.txt";
    const char *command = "(1+-1)^2\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(0, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(general, gcorrect3)
{
    const char *filename = "gcorrect3.txt";
    const char *command = "3*-(4+5)\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(0, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}
