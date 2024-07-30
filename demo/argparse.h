#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Add a function to print error
Make error print more nice. and add proper commenting.
*/

// error color defination
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

// error messages
#define INVALID_ARG "invalid argument"
#define MISSING_VALUE_ARG "missing value for argument"
#define ARG_NOT_VALUE "argument is not a type value"
#define ARG_NOT_FLAG "argument is not type flag"

// code to asign dymanic size to the arguments array
#define DA_INIT_CAPACITY 8192;
#define DA_REALLOC(oldptr, oldsz, newsz) realloc(oldptr, newsz);
#define da_append(da, item)                                            \
  do                                                                   \
  {                                                                    \
    if ((da)->count >= (da)->capacity)                                 \
    {                                                                  \
      size_t new_capacity = (da)->capacity * 2;                        \
      if (new_capacity == 0)                                           \
      {                                                                \
        new_capacity = DA_INIT_CAPACITY;                               \
      }                                                                \
      (da)->items = DA_REALLOC((da)->items,                            \
                               (da)->capcity * sizeof((da)->items[0]), \
                               new_capacity * sizeof((da)->items[0])); \
      (da)->capacity = new_capacity;                                   \
    }                                                                  \
    (da)->items[(da)->count++] = (item);                               \
  } while (0);

enum argument_type
{
  ARGUMENT_TYPE_VALUE,
  ARGUMENT_TYPE_FLAG,
};

struct argument
{
  char short_name;
  char *long_name;
  char *description;
  enum argument_type type;
  char *value;
  unsigned int flag;
};

struct argparse_parser
{
  char *name;
  char *description;
  char *version;
  struct argument *items;
  size_t count;
  size_t capacity;
};

// function initialization
struct argparse_parser *argparse_new(char *name, char *description, char *version);
void argparse_add_argument(struct argparse_parser *parser, char short_name, char *long_name, char *description, enum argument_type type);
// function to get flags and values
char *argparse_get_value(struct argparse_parser *parser, char *long_name);
unsigned int argparse_get_flag(struct argparse_parser *parser, char *long_name);
// helper function to print basic stuff
void argparse_print_help(struct argparse_parser *parser);
void argparse_print_version(struct argparse_parser *parser);
void print_errors(char *message, char *string);
void print_warning(char *message, char *string);
// funtions to assign memory to the parser and free up
void argparse_parse(struct argparse_parser *parser, int argc, char *argv[]);
void argparse_free(struct argparse_parser *parser);

// create a program structure
struct argparse_parser *argparse_new(char *name, char *description, char *version)
{
  struct argparse_parser *parser = malloc(sizeof(struct argparse_parser));

  parser->name = name;
  parser->description = description;
  parser->version = version;
  parser->items = NULL;
  parser->count = 0;
  parser->capacity = 0;

  return parser;
}

// function to add new argument
void argparse_add_argument(struct argparse_parser *parser, char short_name, char *long_name, char *description, enum argument_type type)
{
  struct argument arg = {
      .short_name = short_name,
      .long_name = long_name,
      .description = description,
      .type = type,
      .value = NULL,
      .flag = 0,
  };
  da_append(parser, arg);
}

// parse all the argument from command line to program
void argparse_parse(struct argparse_parser *parser, int argc, char *argv[])
{
  for (int i = 1; i < argc; i++)
  {
    char *name = argv[i];
    if (name[0] == '-')
    {
      struct argument *arg = NULL;

      for (size_t j = 0; j < parser->count; j++)
      {
        struct argument *item = &parser->items[j];
        if ((name[1] == '-' && item->long_name != NULL && strcmp(name + 2, item->long_name) == 0) ||
            (name[1] != '-' && item->short_name != '\0' && name[1] == item->short_name))
        {
          arg = item;
          break;
        }
      }

      if (arg == NULL)
      {
        print_errors(INVALID_ARG, name);
      }
      switch (arg->type)
      {
      case ARGUMENT_TYPE_FLAG:
        arg->flag = 1;
        break;
      case ARGUMENT_TYPE_VALUE:
        if (i + 1 >= argc)
        {
          print_warning(MISSING_VALUE_ARG, name);
        }
        arg->value = argv[++i];
        break;
      default:
        fprintf(stderr, "type if not supported for argument: %s\n", name);
        exit(1);
      }
    }
    else
    {
      print_errors(INVALID_ARG, name);
    };
  };
}

// function to get value
char *argparse_get_value(struct argparse_parser *parser, char *long_name)
{
  for (size_t i = 0; i < parser->count; i++)
  {
    struct argument *item = &parser->items[i];
    if (item->long_name != NULL && strcmp(long_name, item->long_name) == 0)
    {
      if (item->type != ARGUMENT_TYPE_VALUE)
      {
        print_warning(ARG_NOT_VALUE, long_name);
      }
      return item->value;
    }
  }
  return NULL;
}

// function to get the flag value
unsigned int argparse_get_flag(struct argparse_parser *parser, char *long_name)
{
  for (size_t i = 0; i < parser->count; i++)
  {
    struct argument *item = &parser->items[i];
    if (item->long_name != NULL && strcmp(long_name, item->long_name) == 0)
    {
      if (item->type != ARGUMENT_TYPE_FLAG)
      {
        print_warning(ARG_NOT_FLAG, long_name);
      }
      return item->flag;
    }
  }
  return 0;
}

// simple function to print help text in a formatted way
void argparse_print_help(struct argparse_parser *parser)
{
  printf("usage:%s [options]\n\n", parser->name);
  printf("%s\n\n", parser->description);
  printf("options:\n");
  for (size_t i = 0; i < parser->count; i++)
  {
    struct argument *item = &parser->items[i];
    printf("  -%c, --%s", item->short_name, item->long_name);
    if (item->type == ARGUMENT_TYPE_VALUE)
    {
      printf("  <value>");
    }
    printf("\n");
    printf("     %s\n", item->description);
    printf("\n");
  }
};

// function to print version when version flag called
void argparse_print_version(struct argparse_parser *parser)
{
  printf(ANSI_COLOR_BLUE "%s %s\n\n" ANSI_COLOR_RESET, parser->name, parser->version);
};

// function to print errors
void print_errors(char *message, char *string)
{
  printf(ANSI_COLOR_RED "error: %s %s \n\n" ANSI_COLOR_RESET, message, string);
  exit(1);
}

// function to print warnings
void print_warning(char *message, char *string)
{
  printf(ANSI_COLOR_YELLOW "warning: %s %s\n\n" ANSI_COLOR_RESET, message, string);
  exit(1);
}

// free the memory of argument parser;
void argparse_free(struct argparse_parser *parser)
{
  free(parser->items);
  free(parser);
}

#endif
