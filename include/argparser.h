#ifndef SJPSLIB_ARGPARSER_H

  #define SJPSLIB_ARGPARSER_H

  #ifndef _INC_STDIO
    #include <stdio.h>
  #endif

  #ifndef _INC_STDLIB
    #include <stdlib.h>
  #endif

  #ifndef _INC_STRING
    #include <string.h>
  #endif

  #ifndef __ASSERT_H_
    #include <assert.h>
  #endif

  #include "types.h"

  enum ap_OptionType {
    BOOL_OPT,
    INT_OPT,
    STR_OPT,
  };

  // TODO: Ignore as_bool?
  // Isn't is_set enough?
  union ap_OptionValue {
    bool as_bool;
    int as_int;
    char *as_str;
  };

  typedef struct Option {
    char *name;
    enum ap_OptionType type;
    union ap_OptionValue value;
    bool is_set;
  } Option;

  Option* ap_new_option(enum ap_OptionType type, char *name) {
    Option *o = (Option*) malloc(sizeof(Option));
    if (o == NULL) {
      fprintf(stderr, "Memory allocation failed!\n");
      exit(EXIT_FAILURE);
    }
    o->name = name;
    o->type = type;
    o->is_set = false;
    return o;
  }

  Option* ap_new_bool_option(char *name) {
    return ap_new_option(BOOL_OPT, name);
  }

  Option* ap_new_int_option(char *name) {
    return ap_new_option(INT_OPT, name);
  }

  Option* ap_new_string_option(char *name) {
    return ap_new_option(STR_OPT, name);
  }

  struct Command {
    char *name;
    char *description;

    void (*run)(struct Command *command, void *data);

    struct Command **commands;
    size_t command_count;
    size_t command_capacity;

    Option**options;
    size_t option_count;
    size_t option_capacity;

    char **args;
    size_t arg_count;
    size_t arg_capacity;
  };

  struct Command *ap_command(char *name, void (*run)(struct Command *command, void *data)) {
    struct Command *c = (struct Command*) malloc(sizeof(struct Command));

    if (c == NULL) {
      fprintf(stderr, "Memory allocation failed!\n");
      exit(EXIT_FAILURE);
    }

    c->name = name;
    c->description = NULL;
    c->run = run;
    c->commands = NULL;
    c->command_count= 0;
    c->command_capacity = 10;
    c->options = NULL;
    c->option_count = 0;
    c->option_capacity = 10;
    c->args = NULL;
    c->arg_count = 0;
    c->arg_capacity = 10;

    return c;
  }

  void ap_add_sub(struct Command *cmd, struct Command *sub) {
    assert(cmd != NULL);
    assert(sub != NULL);

    if (cmd->command_count == 0) {
      cmd->commands = (struct Command**) malloc(sizeof(struct Command*) * cmd->command_capacity);
      if (cmd->commands == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
      }
    }

    cmd->commands[cmd->command_count++] = sub;

    if (cmd->command_count == cmd->command_capacity) {
      cmd->command_capacity *= 2;
      cmd->commands = (struct Command**) realloc(cmd->commands, sizeof(struct Command*) * cmd->command_capacity);
      if (cmd->commands == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  void ap_add_opt(struct Command *c, enum ap_OptionType type, char *name) {
    assert(c != NULL);
    assert(type >= BOOL_OPT && type <= STR_OPT);
    assert(name != NULL);

    if (c->option_count == 0) {
      c->options = (Option**) malloc(sizeof(struct Option*) * c->option_capacity);
      if (c->options == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
      }
    }

    c->options[c->option_count++] = ap_new_option(type, name);

    if (c->option_count == c->option_capacity) {
      c->option_capacity *= 2;
      c->options = (Option**) realloc(c->options, sizeof(Option*) * c->option_capacity);
      if (c->options == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  void ap_bool_opt(struct Command *c, char *name) {
    ap_add_opt(c, BOOL_OPT, name);
  }

  void ap_int_opt(struct Command *c, char *name) {
    ap_add_opt(c, INT_OPT, name);
  }

  void ap_string_opt(struct Command *c, char *name) {
    ap_add_opt(c, STR_OPT, name);
  }

  Option* ap_get_opt(struct Command *c, const char *name) {
    assert(c != NULL);
    assert(name != NULL);

    for (size_t i = 0; i < c->option_count; i++) {
      Option*o = c->options[i];
      if (strcmp(o->name, name) == 0) {
        return o;
      }
    }
    return NULL;
  }

  void ap_add_arg(struct Command *c, char *arg) {
    assert(c != NULL);
    assert(arg != NULL);

    if (c->arg_count == 0) {
      c->args = (char**) malloc(sizeof(char*) * c->arg_capacity);
      if (c->args == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
      }
    }

    c->args[c->arg_count++] = arg;

    if (c->arg_count == c->arg_capacity) {
      c->arg_capacity *= 2;
      c->args = (char**) realloc(c->args, sizeof(char*) * c->arg_capacity);
      if (c->args == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  void ap_parse(int argc, char **argv, struct Command *cmd, void *data) {
    assert(argc > 0);
    assert(argv != NULL);
    assert(cmd != NULL);

    struct Command *tmp = cmd;
    Option* read_option = NULL;
    bool check_for_command = true;
    bool read_option_value = false;

    for (int i = 1; i < argc; i++) {
      char *arg = argv[i];

      if (read_option_value) {
        read_option_value = false;
        if (read_option->type == INT_OPT) {
          read_option->value.as_int = atoi(arg);
        } else if (read_option->type == STR_OPT) {
          read_option->value.as_str = arg;
        }
        read_option->is_set = true;
        read_option = NULL;
        continue;
      }

      bool matched_option = false;
      bool matched_command = false;

      for (size_t j = 0; j < tmp->option_count; j++) {
        Option*o = tmp->options[j];
        if (strcmp(o->name, arg) == 0) {
          matched_option = true;
          read_option = o;
          if (o->type != BOOL_OPT) {
            read_option_value = true;
          } else {
            o->is_set = true;
          }
          break;
        }
      }

      if (matched_option) {
        continue;
      }

      if (check_for_command) for (size_t j = 0; j < tmp->command_count; j++) {
        if (strcmp(tmp->commands[j]->name, arg) == 0) {
          tmp = tmp->commands[j];
          matched_command = true;
          break;
        }
      }

      if (!matched_command) {
        check_for_command = false;
        ap_add_arg(tmp, arg);
      }

    }

    if (tmp->run != NULL) {
      tmp->run(tmp, data);
    }
  }

  typedef struct Command Command;

#endif