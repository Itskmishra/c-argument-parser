#include <stdio.h>
#include "argparse.h"
#include "argparse.h"

int main(int argc, char *argv[])
{
  // a new program struct
  struct argparse_parser *parser = argparse_new("test", "test description", "0.0.1");

  // adding arguments
  argparse_add_argument(parser, 'v', "version", "print version", ARGUMENT_TYPE_FLAG);
  argparse_add_argument(parser, 'h', "help", "print help", ARGUMENT_TYPE_FLAG);
  argparse_add_argument(parser, 'f', "file", "file name", ARGUMENT_TYPE_VALUE);

  // parsing arguments
  argparse_parse(parser, argc, argv);

  // getting values from the program 
  unsigned int version = argparse_get_flag(parser, "version");
  unsigned int help = argparse_get_flag(parser, "help");
  char *file = argparse_get_value(parser, "file");

  // printing stuff
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
};
