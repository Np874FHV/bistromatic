#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/redirect.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../src/declarations.h"
#include "../src/handle_request.h"

#define DB "0123456789"

void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

int compare_output(FILE *output, char *expected)
{
    size_t pos = 0;
    char c = fgetc(output);
    while (c == expected[pos] && expected[pos] != '\0')
    {
        pos++;
        c = fgetc(output);
    }
    if (expected[pos] == '\0' && c == EOF)
        return 0;
    return 1;
}

Test(Correct_result, correctres1)
{
    const char *filename = "correctres1.txt";
    const char *filenameout = "outcorrectres1.txt";
    const char *command = "2+5*4\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    FILE *output = fopen(filenameout, "w+");
    if (!output)
    {
        fclose(file);
        return;
    }
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    handle_request(file, output, DB, 5);
    fseek(output, 0, SEEK_SET);
    char expected[] = "22";
    cr_assert_eq(0, compare_output(output, expected));
    fclose(file);
    fclose(output);
    remove(filename);
    remove(filenameout);
}

Test(Correct_result, correctres2)
{
    const char *filename = "correctres2.txt";
    const char *filenameout = "outcorrectres2.txt";
    const char *command = "--++-4*5.5\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    FILE *output = fopen(filenameout, "w+");
    if (!output)
    {
        fclose(file);
        return;
    }
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    handle_request(file, output, DB, 5);
    fseek(output, 0, SEEK_SET);
    char expected[] = "-22";
    cr_assert_eq(0, compare_output(output, expected));
    fclose(file);
    fclose(output);
    remove(filename);
    remove(filenameout);
}

Test(Correct_result, correctres3)
{
    const char *filename = "correctres3.txt";
    const char *filenameout = "outcorrectres3.txt";
    const char *command = "10^10\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    FILE *output = fopen(filenameout, "w+");
    if (!output)
    {
        fclose(file);
        return;
    }
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    handle_request(file, output, "01", 5);
    fseek(output, 0, SEEK_SET);
    char expected[] = "100";
    cr_assert_eq(0, compare_output(output, expected));
    fclose(file);
    fclose(output);
    remove(filename);
    remove(filenameout);
}

Test(Correct_result, correctres4)
{
    const char *filename = "correctres4.txt";
    const char *filenameout = "outcorrectres4.txt";
    const char *command = "1/3\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    FILE *output = fopen(filenameout, "w+");
    if (!output)
    {
        fclose(file);
        return;
    }
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    handle_request(file, output, DB, 10);
    fseek(output, 0, SEEK_SET);
    char expected[] = "0.3333333333";
    cr_assert_eq(0, compare_output(output, expected));
    fclose(file);
    fclose(output);
    remove(filename);
    remove(filenameout);
}

Test(Correct_result, correctres5)
{
    const char *filename = "correctres5.txt";
    const char *filenameout = "outcorrectres5.txt";
    const char *command = "1.1*(1.0000-8^0)\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    FILE *output = fopen(filenameout, "w+");
    if (!output)
    {
        fclose(file);
        return;
    }
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    handle_request(file, output, DB, 10);
    fseek(output, 0, SEEK_SET);
    char expected[] = "0";
    cr_assert_eq(0, compare_output(output, expected));
    fclose(file);
    fclose(output);
    remove(filename);
    remove(filenameout);
}

Test(Correct_result, correctres6)
{
    const char *filename = "correctres6.txt";
    const char *filenameout = "outcorrectres6.txt";
    const char *command = "100//10%3.0\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    FILE *output = fopen(filenameout, "w+");
    if (!output)
    {
        fclose(file);
        return;
    }
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    handle_request(file, output, DB, 10);
    fseek(output, 0, SEEK_SET);
    char expected[] = "1";
    cr_assert_eq(0, compare_output(output, expected));
    fclose(file);
    fclose(output);
    remove(filename);
    remove(filenameout);
}

Test(Power, negativepow)
{
    const char *filename = "negativepow.txt";
    const char *command = "2.3^-2\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(0, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}
