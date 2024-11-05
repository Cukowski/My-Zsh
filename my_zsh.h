#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_INPUT 1024

/* Public Functions */

// Parser
char **input_parser(char *input);           // inside of input_parser.c
void free_tokens(char **tokens);            // inside of input_parser.c

// External command executor
int executor(char **args, char** env);
int child_process(char **args, char **env);

// shell_builts function to handle built-in commands
int shell_builts(char **args, char** env, char *init_dir);

// Built-in function implementations        inside of shell_builts.c
int command_echo(char **args, char** env);
int command_cd(char **args,  char *init_dir);
char **command_setenv(char **args, char** env);
char **command_unsetenv(char **args, char** env);
int command_env(char **env);
int command_pwd();
int command_which(char **args, char **env);

// paths.c
char *get_path(char** env);
char **split_paths(char *paths, int *count);
int print_paths(char** env);

/* Helper Functions */

int my_strlen(const char *input);
char *my_strdup(const char *str);
char *my_strcpy(char *dest, const char *src);
char *my_strtok(char *input_string, const char *delimiter);
char *cpy_var(const char *args);
char *get_sub_string(char *arg, char *env);