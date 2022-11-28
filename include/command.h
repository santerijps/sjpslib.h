#ifndef SJPSLIB_COMMAND_H

  #define SJPSLIB_COMMAND_H

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

  typedef enum OptionType {
    OptionTypeBoolean,
    OptionTypeInteger,
    OptionTypeString,
  } OptionType;

  typedef union OptionValue {
    bool as_boolean;
    i64 as_integer;
    string as_string;
  } OptionValue;

  typedef struct OptionValues {
    u64 len;
    OptionValue items[32];
  } OptionValues;

  typedef struct Option {
    char name[32];
    bool exists;
    OptionType type;
    OptionValues values;
  } Option;

  typedef struct CommandOptions {
    u64 len;
    Option items[32];
  } CommandOptions;

  CommandOptions command_options_create() {
    CommandOptions co;
    co.len = 0;
    return co;
  }

  typedef struct CommandArgs {
    u64 len;
    char items[32][32];
  } CommandArgs;

  CommandArgs command_args_create() {
    CommandArgs ca;
    ca.len = 0;
    return ca;
  }

  typedef struct Command {
    char name[32];
    bool is_base;
    void (*handler)(struct Command *c, void *data);
    struct Command *subs;
    u64 sub_cap, sub_len;
    CommandOptions options;
    CommandArgs args;
  } Command;

  Command* command_create(string name, bool is_base, void (*handler)(struct Command *c, void *data)) {
    Command *c = (Command*) calloc(1, sizeof(Command));
    memset(c->name, 0, 32);
    if (name != NULL) strcpy(c->name, name);
    c->is_base = is_base;
    c->handler = handler;
    c->subs = NULL;
    c->sub_cap = 8;
    c->sub_len = 0;
    c->options = command_options_create();
    c->args = command_args_create();
    return c;
  }

  Command* command_create_base(void (*handler)(Command*, void*)) {
    return command_create(NULL, true, handler);
  }

  Command* command_create_sub(string name, Command *base, void (*handler)(Command*, void*)) {
    Command *c = command_create(name, false, handler);
    if (base->subs == NULL) base->subs = (Command*) malloc(sizeof(Command) * 8);
    base->subs[base->sub_len++] = *c;
    if (base->sub_len == base->sub_cap) {
      base->sub_cap *= 2;
      base->subs = (Command*) realloc(base->subs, sizeof(Command) * base->sub_cap);
    }
    return &base->subs[base->sub_len - 1];
  }

  void command_add_option(Command *c, string name, OptionType type) {
    Option o;
    memset(o.name, 0, 32);
    strcpy(o.name, name);
    o.type = type;
    o.exists = false;
    o.values.len = 0;
    c->options.items[c->options.len++] = o;
  }

  void command_add_boolean_option(Command *c, string name) {
    command_add_option(c, name, OptionTypeBoolean);
  }

  void command_add_integer_option(Command *c, string name) {
    command_add_option(c, name, OptionTypeInteger);
  }

  void command_add_string_option(Command *c, string name) {
    command_add_option(c, name, OptionTypeString);
  }

  Option* command_get_option(Command *c, string name) {
    for (u64 i = 0; i < c->options.len; i++) {
      if (!strcmp(c->options.items[i].name, name)) {
        return &(c->options.items[i]);
      }
    }
    return NULL;
  }

  void command_parse_args(i32 argc, string argv[], Command *c, void *data) {

    assert(argc > 0);
    assert(c != NULL);
    Command *runner = c;

    bool jump_to_reading_options = false;
    bool jump_to_reading_args = false;
    bool read_next_arg_to_option = false;
    u64 read_arg_option_index = 0;

    for (i32 arg_index = 1; arg_index < argc; arg_index++) {

      if (jump_to_reading_args) {
        goto READ_ARG;
      }

      if (jump_to_reading_options) {
        goto READ_OPT;
      }

      // Does the arg match a subcommand?
      // If yes, move the command pointer forwards.
      if (!read_next_arg_to_option) {
        for (u64 i = 0; i < runner->sub_len; i++) {
          if (!strcmp(runner->subs[i].name, argv[arg_index])) {
            runner = &runner->subs[i];
            goto END_OF_OUTER_LOOP;
          }
        }
      }

  READ_OPT:
      // Should we be reading an option value?
      // If yes, then read the arg into an option.
      if (read_next_arg_to_option) {
        OptionValue ov;
        switch (runner->options.items[read_arg_option_index].type) {
          case OptionTypeBoolean:
            ov.as_boolean = true;
            break;
          case OptionTypeInteger:
            ov.as_integer = atoi(argv[arg_index]);
            break;
          case OptionTypeString:
            ov.as_string = argv[arg_index];
            break;
        }
        runner->options.items[read_arg_option_index].values.items[runner->options.items[read_arg_option_index].values.len++] = ov;
        read_next_arg_to_option = false;
        goto END_OF_OUTER_LOOP;
      }

      // Does the arg match an option?
      // If yes, read the option value.
      for (u64 i = 0; i < runner->options.len; i++) {
        if (!strcmp(runner->options.items[i].name, argv[arg_index])) {
          runner->options.items[i].exists = true;
          jump_to_reading_options = true;
          if (runner->options.items[i].type != OptionTypeBoolean) {
            read_next_arg_to_option = true;
            read_arg_option_index = i;
          }
          goto END_OF_OUTER_LOOP;
        }
      }

  READ_ARG:
      // Otherwise, add the arg to the commands args.
      memset(runner->args.items[runner->args.len], 0, 32);
      strcpy(runner->args.items[runner->args.len], argv[arg_index]);
      runner->args.len += 1;
      jump_to_reading_args = true;

  END_OF_OUTER_LOOP:
      ; // Do nothing
    }

    assert(runner != NULL);
    runner->handler(runner, data);

  }

#endif