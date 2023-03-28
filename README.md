
# Bistromatic

This project is a calculator inspired from an EPITECH academic project that handles infinite numbers in any base. The main goal is performance.




## Installation

A ```bistro.a``` static library is given.

Otherwise, ```src``` and ```Makefile``` can be downloaded and ```bistro.a``` can be created with the following command:

```bash
    $ make bistro
    $ rm *.o  # To remove all created object files
```

Please refer to the Usage/Examples section.
## Run Locally

Clone the project

```bash
    $ git clone https://github.com/Np874FHV/bistromatic.git
```

Go to the project directory

```bash
    $ cd bistromatic
```

Create main binary or use the given one. Be warned, a `bistro.a` and `.o` object files will also be created.

```bash
    $ make main
```

You can delete all `make`-created files by using:

```bash
    $ make clean
```

You can use the `main` binary in three ways

- Without any argument

The base is 10 and the maximum decimal precision is 100 by default.
Run the main binary:

```bash
    $ ./main
    Entering interactive mode.
    Any wrong request will properly close the program.
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

The division by `0` is of course forbidden and makes the program to fail.

Unmatched parenthesis, incorrect bases, incorrect syntax will also create errors.

As indicated, an incorrect request will properly terminate the program.

For example:

```bash
    $ ./main
    Entering interactive mode.
    Any wrong request will properly close the program.
    bistro$ -+-2
    -2
    bistro$ 10+6 *7
    52
    bistro$ hello
    Incorrect request! # stderr
    $
```


- Using `stdin` as input

Enter your request in stdin, then run main with exactly three arguments:

-> The name of the output file

-> The base (`()+-*/%^ .` characters are reserved)

-> The maximal decimal precision

For example:

```bash
    $ ls
    main
    $ echo " 2^2+   1/3" | ./main out.txt 0123456789 4
    $ ls
    main    out.txt
    $ cat out.txt
    4.3333$ 
```

Be careful, if the output file already exists, it will be overrided.

- Using a file as input

Write a request in a file. Only the first line will be computed.

Run main with exactly four arguments:

-> The name of the input file

-> The name of the output file

-> The base (`()+-*/%^ .` characters are reserved)

-> The maximal decimal precision

For example:

```bash
    $ ls
    input.txt    main
    $ cat input.txt | cat -e
    10^10 + 1.001$
    This is a test.$
    $ ./main input.txt out.txt 01 0
    $ ls
    input.txt    main    out.txt
    $ cat out.txt
    101$ 
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
## Running Tests

To run tests, run the following command

```bash
    $ make testsuite
```

A ```testsuite``` binary will be generated and automatically run once.

To add custom tests, add them in a ```.c``` file in the ```tests``` folder.

A future release may add an easier way to create tests, just by creating a file containing a command line.

You can also create the `debug`binay file:

```bash
    $ make debug
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

- Unmatched parenthesis, incorrect bases, incorrect syntax will also create errors.

- Incorrect sequence (`1**2`, `-*2`, `1 2`, `1(*2)`, ...)

- Division, euclidean division and modulo by 0

- Power by a decimal number

- `main` can be run in three ways (plese refer to Run Locally)

- Testsuite

- Custom base and decimal precision

- Fast power: `a ^ 5 = a * ((a ^ 2) ^ 2)`

- Only one number allocation by operation. The usual division was the hardest function to implement. How to compute `13.78 / 4458.455455 = a / b` efficienly? 

Consider `n` the length of the base.
I implemented a function that tests if `b * n ^ (-i) > a`. If it is true, then substract `b * n ^ (-i)` to the result, without creating any temporary number. The brute-force solution was, in the previous example, to do 

```
13.78 -> 137.8
137.8 -> 1378
1378 -> 13780
13780 >= 4458.455455 then substract...
```

Which is very slow.
## Roadmap

- More efficients division algorithms. The regular division was the hardest part of the project, as I managed to allocate only one number (refer to Features). The currect algorithm is brute force: Substract while the dividend is greater than the divisor. Consequeltly, 1 000 000 000 / 1 is way too slow. 

- Handle bugs, but I didn't find any in my tests for the moment.

- Implement power by a negative whole number : `a ^ (-b) = (1 / a) ^ b`
## License

Please refer to the LICENCE file.

