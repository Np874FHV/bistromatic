#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../src/declarations.h"
#include "../src/handle_request.h"

#define DB "0123456789"

Test(ZeroDivision, zerodiv)
{
    const char *filename = "zerodiv.txt";
    const char *command = "1/(1-1.0)\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(ZeroDivision, zeroeucldiv)
{
    const char *filename = "zeroeucldiv.txt";
    const char *command = "1//0\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(ZeroDivision, zeromod)
{
    const char *filename = "zeromod.txt";
    const char *command = "1%0\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(DecimalDivision, decimod1)
{
    const char *filename = "decimod1.txt";
    const char *command = "1.1%7\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(DecimalDivision, decimod2)
{
    const char *filename = "decimod2.txt";
    const char *command = "1%7.2\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(DecimalDivision, eucl1)
{
    const char *filename = "eucl1.txt";
    const char *command = "11//7.2\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(Power, decimalpow)
{
    const char *filename = "decimalpow.txt";
    const char *command = "2.3^2.2\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
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
    cr_assert_eq(1, handle_request(file, stdout, DB, 100));
    fclose(file);
    remove(filename);
}

Test(Base, wrongbase1)
{
    const char *filename = "wrongbase1.txt";
    const char *command = "1+1\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, "1", 100));
    fclose(file);
    remove(filename);
}

Test(Base, wrongbase2)
{
    const char *filename = "wrongbase2.txt";
    const char *command = "1+1\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, "01231", 100));
    fclose(file);
    remove(filename);
}

Test(Base, wrongbase3)
{
    const char *filename = "wrongbase3.txt";
    const char *command = "1+1\n";
    FILE *file = fopen(filename, "w+");
    if (!file)
        return;
    fwrite(command, sizeof(char), strlen(command), file);
    fseek(file, 0, SEEK_SET);
    cr_assert_eq(1, handle_request(file, stdout, "0123+", 100));
    fclose(file);
    remove(filename);
}
