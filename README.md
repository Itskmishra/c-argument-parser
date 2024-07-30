# Simple Argument Parser

A lightweight, easy-to-use argument parser library implemented in C. This library provides a simple interface for parsing command-line arguments in C programs.

## Features

- Support for short and long argument names
- Handling of flag (boolean) and value arguments
- Dynamic allocation of argument storage
- Colored error and warning messages
- Automatic generation of help and version information

## Usage

### Including the Library

To use the argument parser in your project, include the `argparse.h` header file:

```c
#include "argparse.h"

```

### Creating a Parser

Create a new argument parser using `argparse_new()`:

```c
struct argparse_parser *parser = argparse_new("program_name", "program description", "version");

```

### Adding Arguments

Add arguments to the parser using `argparse_add_argument()`:

```c
argparse_add_argument(parser, 'v', "version", "print version", ARGUMENT_TYPE_FLAG);
argparse_add_argument(parser, 'f', "file", "input file", ARGUMENT_TYPE_VALUE);

```

### Parsing Arguments

Parse the command-line arguments:

```c
argparse_parse(parser, argc, argv);

```

### Retrieving Values

Retrieve parsed values using `argparse_get_flag()` for flags and `argparse_get_value()` for value arguments:

```c
unsigned int version_flag = argparse_get_flag(parser, "version");
char *file_name = argparse_get_value(parser, "file");

```

### Printing Help and Version

Print help and version information:

```c
argparse_print_help(parser);
argparse_print_version(parser);

```

### Cleaning Up

Free the parser's memory when done:

```c
argparse_free(parser);

```

## API Reference

### Functions

- `struct argparse_parser *argparse_new(char *name, char *description, char *version)`
- `void argparse_add_argument(struct argparse_parser *parser, char short_name, char *long_name, char *description, enum argument_type type)`
- `void argparse_parse(struct argparse_parser *parser, int argc, char *argv[])`
- `char *argparse_get_value(struct argparse_parser *parser, char *long_name)`
- `unsigned int argparse_get_flag(struct argparse_parser *parser, char *long_name)`
- `void argparse_print_help(struct argparse_parser *parser)`
- `void argparse_print_version(struct argparse_parser *parser)`
- `void argparse_free(struct argparse_parser *parser)`

### Enums

```c
enum argument_type
{
  ARGUMENT_TYPE_VALUE,
  ARGUMENT_TYPE_FLAG
};

```

## Error Handling

The library provides colored error and warning messages for various scenarios:

- Invalid arguments
- Missing values for arguments
- Type mismatches (e.g., using a flag as a value argument)

## Example

Here's a simple example demonstrating the usage of the argument parser:

```c
#include <stdio.h>
#include "argparse.h"

int main(int argc, char *argv[])
{
  struct argparse_parser *parser = argparse_new("test", "test description", "0.0.1");

  argparse_add_argument(parser, 'v', "version", "print version", ARGUMENT_TYPE_FLAG);
  argparse_add_argument(parser, 'h', "help", "print help", ARGUMENT_TYPE_FLAG);
  argparse_add_argument(parser, 'f', "file", "file name", ARGUMENT_TYPE_VALUE);

  argparse_parse(parser, argc, argv);

  unsigned int version = argparse_get_flag(parser, "version");
  unsigned int help = argparse_get_flag(parser, "help");
  char *file = argparse_get_value(parser, "file");

  if (version)
  {
    argparse_print_version(parser);
    return 0;
  }
  if (help)
  {
    argparse_print_help(parser);
    return 0;
  }
  if (file){
    printf("file: %s\n", file);
  }

  argparse_free(parser);
  return 0;
}

```

## Limitations

- The library uses dynamic memory allocation, so proper memory management is crucial.
- Error handling is basic and terminates the program on errors.
- The maximum number of arguments is limited by available memory.

## Contributing

Contributions to improve the library are welcome. Please submit pull requests or open issues on the project's repository.
