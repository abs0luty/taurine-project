#include "argparse/include/argparse.h"
#include "arty/include/utf8.h"
#include "lunarity/include/lexer.h"
#include <stdio.h>
#include <string.h>

int cmd_lex(size_t argc, const char *argv[]) {
  const char *filepath = NULL;
  struct argparse_option options[] = {
      OPT_HELP(),
      OPT_STRING('f', "filepath", &filepath, "input file path", NULL, 0, 0),
      OPT_END(),
  };
  struct argparse argparse;
  argparse_init(&argparse, options, NULL, 0);
  argc = argparse_parse(&argparse, argc, argv);
  printf("filepath: %s\n", filepath);
}

struct cmd {
  const char *name;
  int (*func)(size_t argc, const char *argv[]);
};

static struct cmd commands[] = {{"lex", cmd_lex}};
static const char *const usages[] = {"subcommands [options] [cmd] [args]",
                                     NULL};

int main(size_t argc, const char *argv[]) {
  struct argparse argparse;
  struct argparse_option options[] = {
      OPT_HELP(),
      OPT_END(),
  };
  argparse_init(&argparse, options, usages, ARGPARSE_STOP_AT_NON_OPTION);
  if (argc < 2) {
    argparse_usage(&argparse);
    return -1;
  }

  struct cmd *cmd = NULL;
  for (register size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
    if (!strcmp(argv[1], commands[i].name)) {
      cmd = &commands[i];
    }
  }

  if (cmd) {
    return cmd->func(argc, argv);
  } else {
    argparse_usage(&argparse);
    return -1;
  }

  return 0;
}
