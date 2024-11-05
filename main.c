#include <stdio.h>
#include <stdlib.h>
#include "my_zsh.h"

// shell_builts function to handle built-in commands
int shell_builts(char **args, char** env, char *init_dir) {

    if (strcmp(args[0], "echo") == 0) {
        return command_echo(args, env);
    } else if (strcmp(args[0], "cd") == 0) {
        return command_cd(args, init_dir);
    } else if (strcmp(args[0], "env") == 0) {
        return command_env(env);
    } else if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "quit") == 0) {
        exit(EXIT_SUCCESS);
    } else if (strcmp(args[0], "pwd") == 0) {
        return command_pwd();
    } else if (strcmp(args[0], "which") == 0) {
        return command_which(args, env);
    } else {
        // Not a built-in command, execute as external command
        return executor(args, env);
    }
    return EXIT_SUCCESS;
}

int main(int argc, char** argv, char** env) {
    if(argc && argv){} // For Werror
    char *input = NULL;
    size_t input_size = 0;

    char** args;

    char *initial_directory = getcwd(NULL, 0); // for cd -

    while (1) {
        // Functions return -1 on failure to read a line (including end-of-file condition)
        if(getline(&input, &input_size, stdin) == -1) {
            break;
        }
            
        // You will have to parse the command line and format it in order to execute it.
        args = input_parser(input);
        if(args == NULL) return -1;

        if (args[0] == NULL) { // Empty command has entered
            return 1;
        } else if (strcmp(args[0], "setenv") == 0) {
            env = command_setenv(args, env);
        } else if (strcmp(args[0], "unsetenv") == 0) {
            env = command_unsetenv(args, env);
        } else {
            shell_builts(args, env, initial_directory); // If args are not built-ins, it will call executor
        }

        free_tokens(args); // Free args, also free inside of a loop
    }
    
    return EXIT_SUCCESS;
}