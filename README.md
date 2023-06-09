# Bistromatic

This project is a calculator inspired from an EPITECH academic project that handles infinite numbers in any base. The main goal is performance.


## Installation

Clone the project

```bash
    git clone https://github.com/Np874FHV/bistromatic.git
```

Go to the project directory

```bash
    cd bistromatic
```

Create `bistro.a` static library to include it in a project. Be warned: `.o` object files will also be created.

```bash
    make bistro
    rm *.o  # To remove all created object files
```

## Run Locally

Create main binary or use the given one. Be warned, a `bistro.a` and `.o` object files will also be created.

```bash
    make main
```

You can delete all `make`-created files by using:

```bash
    make clean
```

You can use the `main` binary in three ways:

- Without any argument

The base is 10 and the maximum decimal precision is 100 by default. You can easily modify them through the main.c file.


Run the main binary:

```bash
    ./main
    Entering interactive mode.
    Any incorrect request will properly close the program.
    bistro$ 
```

Enter a request and press Enter to get the result.
The following features are implemented:

-> Any number of spaces anywhere

-> `.` to add a decimal part to a number. No spaces are allowed around a point or inside a same number.

-> `+`, `-`, the unary operators to specify the sign of a number. There is no usage limit.

-> `+`, `-`, `*`, `/`, the usual operators.

-> `//`,`%`, the quotient and remainder of two whole numbers.

-> `^`, the power operator. The second number must be whole (and positive for the moment).

-> `(`,`)`, the parenthesis, allowing to force priorities.

-> A maximum decimal precision is implemented. It is taken into account only when dividing.

The division by `0` is of course forbidden and makes the program to fail.

Unmatched parenthesis, incorrect bases, incorrect syntax will also create errors.

As indicated, an incorrect request will properly terminate the program.

For example:

```bash
    ./main
    Entering interactive mode.
    Any incorrect request will properly close the program.
    bistro$ -+-2
    -2
    bistro$ 10+6 *7
    52
    bistro$ hello
    Incorrect input! # stderr
```


- Using `stdin` as input

Enter your request in stdin, then run main with exactly three arguments:

-> The name of the output file

-> The base (`()+-*/%^ .` characters are reserved)

-> The maximal decimal precision

For example:

```bash
    ls
    main
    echo " 2^2+   1/3" | ./main out.txt 0123456789 4
    ls
    main    out.txt
    cat out.txt
    4.3333 
```

Be careful: if the output file already exists, it will be overrided.

- Using a file as input

Write a request in a file. Only the first line will be computed.

Run main with exactly four arguments:

-> The name of the input file

-> The name of the output file

-> The base (`()+-*/%^ .` characters are reserved)

-> The maximal decimal precision

For example:

```bash
    ls
    input.txt    main
    cat input.txt | cat -e
    10^10 + 1.001$
    This is a test.$
    ./main input.txt out.txt 01 0
    ls
    input.txt    main    out.txt
    cat out.txt
    101.001 
```



## Usage/Examples

To use the `bistro.a` static library, include the `handle_request.h` file to your project to include the `handle_request` function.

```c
    #include <stdio.h>
    #include "src/handle_request.h"
    // int handle_request(FILE *input, FILE *output, const char *base, size_t decimal_precision);

    int main(int argc, char **argv)
    {
        const char *input_filename = "input.txt";                           // Create an input file
        const char *command = "1+1.1\n";                                    // The command you want to execute
        FILE *input = fopen(input_filename, "w+");                          // Create the stream 
        if (!input)
            return 1;
        fwrite(command, sizeof(char), strlen(command), input);              // Write the command in the file
        fseek(input, 0, SEEK_SET);                                          // Go back to the beginning of the file
        int result = handle_request(input, stdout, "0123456789", 100);      // Prints "2.1" to stdout
        fclose(input);                                                      // Close the stream
        remove(input_filename);                                             // Delete the file
        return result;                                                      // Returns 0 as the request is valid
    }
```

You can use any input and output, as long as they are of `FILE *` type.

You can also use the `get_result` function, that directly returns a number:

```
struct number
{
    int positive;                                                                       // 0 = negative, positive otherwise
    int *whole_part;
    size_t whole_part_size;
    int *decimal_part;
    size_t decimal_part_size;
};                                                                                      // src/declarations.h

struct number *get_result(FILE *input, const char *base, size_t decimal_precision);     // src/handle_request.h
```

## Running Tests

To run tests, run the following command

```bash
    make testsuite
```

A ```testsuite``` binary will be generated and automatically run once.

To add custom tests, add them in a ```.c``` file in the ```tests``` folder.

A future release may add an easier way to create tests, just by creating a file containing a command line.

You can also create the `debug`binay file:

```bash
    make debug
```

You can use the `debug` binary the same way as `main` (please refer to the Usage/Examples section), but `debug` is compiled with the `-g` flag, allowing you to use `gdb` and `valgrind` for example.

## Features

- Any number of spaces anywhere

- `.` to add a decimal part to a number. No spaces are allowed around a point or inside a same number.

- `+`, `-`, the unary operators to specify the sign of a number. There is no usage limit.

- `+`, `-`, `*`, `/`, the usual operators.

- `//`,`%`, the quotient and remainder of two whole numbers.

- `^`, the power operator. The second number must be whole (and positive for the moment).

- `(`,`)`, the parenthesis, allowing to force priorities.

- A maximum decimal precision is implemented. It is taken into account only when dividing.

- Unmatched parenthesis, incorrect bases, incorrect syntax will also create errors.

- Incorrect sequence (`1**2`, `-*2`, `1 2`, `1(*2)`, ...)

- Division, euclidean division and modulo by 0

- Power by a negative whole number : `a ^ (-b) = (1 / a) ^ b`

- Power by a decimal number

- `main` can be run in three ways (please refer to Run Locally)

- Testsuite

- Custom base and decimal precision

- Fast power: `a ^ 5 = a * ((a ^ 2) ^ 2)`

- No more than one number allocation by operation.

- Euclidean division and modulo algorithms are based on the fast exponentiation: their complexity is logarithmic in the length of the number.

## Roadmap

- Optimize the usual division.
 
- Handle bugs, but I didn't find any in my tests for the moment.


## License

Please refer to the LICENCE file.

