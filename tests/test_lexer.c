#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../src/declarations.h"
#include "../src/handle_request.h"

#define DB "0123456789"

Test(Dot, dot1)
{
    const char *filename = "dot1.txt";
    const char *command = "1.1.1+1\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(Dot, dot2)
{
    const char *filename = "dot2.txt";
    const char *command = "0.+1\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(0, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(Dot, dot3)
{
    const char *filename = "dot3.txt";
    const char *command = ".0+1\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(Numbers, numbers1)
{
    const char *filename = "numbers1.txt";
    const char *command = "1 1\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(Operators, operators1)
{
    const char *filename = "loperators1.txt";
    const char *command = "+ 2\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(0, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(Parenthesis, par1)
{
    const char *filename = "par1.txt";
    const char *command = "2(3)\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(Parenthesis, par2)
{
    const char *filename = "par2.txt";
    const char *command = "2(--+)1\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(Char, IncorrectChar1)
{
    const char *filename = "IncorrectChar1.txt";
    const char *command = "1+1+;\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}
