#include <stdio.h>
#include <stdlib.h>
#include "my_zsh.h"

// ADDED SEARCH WITH '$'
// Built-in function implementations
int command_echo(char **args, char** env) {
    if (args[1] == NULL) {
        printf("\n");
        return 0;
    } else if (args[1][0] == '$') { // if starts with '$' search env for the values
        char *arg = cpy_var(args[1]);

        for (int i = 0; env[i]; i++) {
            if (strstr(env[i], arg)) {
                char *val = get_sub_string(arg, env[i]);
                printf("%s\n", val);
                free(val);
                break;
            }
        }
        free(arg);
    } else if ((args[1][0] == '"') && (args[1][my_strlen(args[1])-1] == '"')){ // Might need fixing for the " "
        for (int i = 1; args[i]; i++) {
            for (int j = 1; j < my_strlen(args[i])-1; j++) {
                printf("%c", args[i][j]);
            }
            printf(" ");
        }
        printf("\n");
    } else {
        for (int i = 1; args[i]; i++) {
            for (int j = 0; args[i][j]; j++) {
                printf("%c", args[i][j]);
            }
            printf(" ");
        }
        printf("\n");
    }
    
    return 0;
}

// The command cd - is used in a shell environment to switch back to the previous working directory.
int command_cd(char **args, char *init_dir) {

    if (args[1] == NULL) {
        printf("cd: expected argument to \"cd\"\n");
    } else if (strcmp(args[1], "-") == 0) {
        if (init_dir) {
            chdir(init_dir);
            printf("%s\n", init_dir);
        } else {
            printf("No previous directory available.\n");
        }
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
    }

    return 0;
}

char** command_setenv_args(char **args, char** env);
char** command_setenv_arg(char **args, char** env);

// Function to set an environment variable
char** command_setenv(char **args, char** env) {
    // Check if argument is provided correctly
    if (args[1] == NULL) {
        fprintf(stderr, "Usage:  setenv VAR=value\nor\tsetenv <variable> <value>\n");
        return env; // Return the original environment
    }

    // check the use setnv format
    int env_count = 0;
    while (env[env_count] != NULL) {
        env_count++;
    }

    // Allocate memory for the new environment variables
    char **new_env = malloc((env_count + 2) * sizeof(char *));
    if (new_env == NULL) {
        perror("malloc");
        return env; // Return the original environment
    }
    
    if (args[2] == NULL) { // case setenv VAR=val
        new_env = command_setenv_arg(args, env);
    } else { // case setenv VAR val
        new_env = command_setenv_args(args, env);
    }

    return new_env;
}

char** command_setenv_args(char **args, char** env) {

    // Count the number of environment variables
    int env_count = 0;
    while (env[env_count] != NULL) {
        env_count++;
    }

    // Allocate memory for the new environment variables
    char **new_env = malloc((env_count + 2) * sizeof(char *));
    if (new_env == NULL) {
        perror("malloc");
        return env; // Return the original environment
    }

    // Copy existing environment variables to the new array
    for (int i = 0; i < env_count; i++) {
        new_env[i] = my_strdup(env[i]);
        if (new_env[i] == NULL) {
            perror("strdup");
            // Free memory allocated so far
            for (int j = 0; j < i; j++) {
                free(new_env[j]);
            }
            free(new_env);
            return env; // Return the original environment
        }
    }

    // Construct the new environment variable string
    char *new_var = malloc(strlen(args[1]) + my_strlen(args[2]) + 2);
    if (new_var == NULL) {
        perror("malloc");
        // Free memory allocated so far
        for (int i = 0; i < env_count; i++) {
            free(new_env[i]);
        }
        free(new_env);
        return env; // Return the original environment
    }
    sprintf(new_var, "%s=%s", args[1], args[2]);

    // Add the new environment variable to the end
    new_env[env_count] = new_var;
    new_env[env_count + 1] = NULL;

    // Return the new environment variables array
    return new_env;
}

char** command_setenv_arg(char **args, char** env) {

    // Count the number of environment variables
    int env_count = 0;
    while (env[env_count] != NULL) {
        env_count++;
    }

    // Allocate memory for the new environment variables
    char **new_env = malloc((env_count + 2) * sizeof(char *));
    if (new_env == NULL) {
        perror("malloc");
        return env; // Return the original environment
    }

    // Copy existing environment variables to the new array
    for (int i = 0; i < env_count; i++) {
        new_env[i] = strdup(env[i]);
        if (new_env[i] == NULL) {
            perror("strdup");
            // Free memory allocated so far
            for (int j = 0; j < i; j++) {
                free(new_env[j]);
            }
            free(new_env);
            return env; // Return the original environment
        }
    }

    // Find the position of '=' in the argument
    char *equal_pos = strchr(args[1], '=');
    if (equal_pos == NULL) {
        fprintf(stderr, "Invalid argument format: setenv VAR=value\n");
        // Free memory allocated so far
        for (int i = 0; i < env_count; i++) {
            free(new_env[i]);
        }
        free(new_env);
        return env; // Return the original environment
    }

    // Extract variable and value from the args[1]ument
    int var_len = equal_pos - args[1];
    char *var = malloc((var_len + 1) * sizeof(char));
    if (var == NULL) {
        perror("malloc");
        // Free memory allocated so far
        for (int i = 0; i < env_count; i++) {
            free(new_env[i]);
        }
        free(new_env);
        return env; // Return the original environment
    }
    strncpy(var, args[1], var_len);
    var[var_len] = '\0'; // Null-terminate the variable string

    // Extract value from the argument
    char *val = strdup(equal_pos + 1);
    if (val == NULL) {
        perror("strdup");
        free(var);
        // Free memory allocated so far
        for (int i = 0; i < env_count; i++) {
            free(new_env[i]);
        }
        free(new_env);
        return env; // Return the original environment
    }

    // Construct the new environment variable string
    char *new_var = malloc(strlen(var) + strlen(val) + 2);
    if (new_var == NULL) {
        perror("malloc");
        free(var);
        free(val);
        // Free memory allocated so far
        for (int i = 0; i < env_count; i++) {
            free(new_env[i]);
        }
        free(new_env);
        return env; // Return the original environment
    }
    sprintf(new_var, "%s=%s", var, val);

    // Add the new environment variable to the end
    new_env[env_count] = new_var;
    new_env[env_count + 1] = NULL;

    // Free allocated memory for variable and value
    free(var);
    free(val);

    // Return the new environment variables array
    return new_env;
}


char** command_unsetenv(char **args, char** env) {
    // Check if arguments are provided correctly
    if (args[1] == NULL || args[2] != NULL) {
        fprintf(stderr, "Usage: unsetenv <variable>\n");
        return env; // Return the original environment
    }

    // Count the number of environment variables
    int env_count = 0;
    while (env[env_count] != NULL) {
        env_count++;
    }

    // Allocate memory for the new environment variables
    char **new_env = malloc((env_count + 1) * sizeof(char *));
    if (new_env == NULL) {
        perror("malloc");
        return env; // Return the original environment
    }

    int removed = 0; // Flag to track if the variable was removed

    // Copy existing environment variables to the new array, omitting the variable to be removed
    for (int i = 0, j = 0; i < env_count; i++) {
        // Check if the variable matches the one to be removed
        if (strncmp(args[1], env[i], strlen(args[1])) != 0 || env[i][strlen(args[1])] != '=') {
            new_env[j++] = my_strdup(env[i]); // Copy the variable
            if (new_env[j - 1] == NULL) {
                perror("strdup");
                // Free memory allocated so far
                for (int k = 0; k < j - 1; k++) {
                    free(new_env[k]);
                }
                free(new_env);
                return env; // Return the original environment
            }
        } else {
            removed = 1; // Set the flag indicating removal
        }
    }

    // If the variable was not found, return the original environment
    if (!removed) {
        fprintf(stderr, "Variable %s not found in environment\n", args[1]);
        free(new_env);
        return env;
    }

    // Terminate the new environment variables array with NULL
    new_env[env_count - 1] = NULL;

    // Free the original environment variables array
    for (int i = 0; env[i] != NULL; i++) {
        free(env[i]);
    }
    free(env);

    // Return the new environment variables array
    return new_env;
}

int command_env(char **env) {
    int index = 0;
    while (env[index]) {
        printf("%s\n", env[index]);
        index++;
    }
    return 1;
}

int command_pwd(char **args) {
    if(args[0] == NULL) return 0;
    char cwd[MAX_INPUT];
    if (getcwd(cwd, sizeof(cwd)) != NULL) { // getcwd is allowed
        printf("%s\n", cwd);
    } else {
        perror("getcwd"); // perror is allowed
    }
    return 1;
}

char *process(char *arg, char **env) ;

int command_which(char **args, char **env) {
    if (args[1] == NULL) {
        printf("which: expected argument\n");
        return 1;
    }

    char *built_in_cmds[] = {"echo", "cd", "setenv", "unsetenv", "env", "exit", "pwd", "which", NULL};
    for (int i = 0; built_in_cmds[i]; i++) {
        if (strcmp(args[1], built_in_cmds[i]) == 0) {
            printf("%s: shell built-in command\n", args[1]);
            return 0;
        }
    }

    // If not a built-in command, check if it's an external executable
    char *full_path = process(args[1], env);
    if (full_path != NULL) {
        printf("%s\n", full_path);
        free(full_path);
        return 0;
    } else {
        printf("which: %s: command not found\n", args[1]);
        return 1;
    }
}

char *process(char *arg, char **env) {
    char *path_string = get_path(env);
    int num_paths;
    char **path_list = split_paths(path_string, &num_paths);

    char *full_path = malloc(MAX_INPUT * sizeof(char));

    for (int i = 0; i < num_paths; i++) {
        char *temp_path = path_list[i];

        // Construct the full path
        snprintf(full_path, MAX_INPUT, "%s/%s", temp_path, arg);

        // Use stat to check if the file exists and is executable
        struct stat file_stat;
        if (stat(full_path, &file_stat) == 0 && S_ISREG(file_stat.st_mode) && (file_stat.st_mode & S_IXUSR)) { // stat is allowed
            // Found executable, return the path
            free(path_string);
            free(path_list);
            return full_path;
        }

        // Free memory allocated by split_paths
        free(temp_path);
    }

    // Clean up memory
    free(path_string);
    free(path_list);
    free(full_path);

    return NULL; // No executable found
}
