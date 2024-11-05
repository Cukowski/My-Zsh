#include "my_zsh.h"

int print_paths(char** env) {
    
    char *path_string = get_path(env);

    int num_paths;
    char **path_list = split_paths(path_string, &num_paths);
    
    printf("Number of paths: %d\n", num_paths);

    for (int i = 0; i < num_paths; i++) {
        free(path_list[i]); // Free each path
    }

    free(path_list); // Free the array
    free(path_string);

    return 0;
}

char *get_path(char** env) {
    char *paths = NULL;
    char *sub_string;

    for (int i = 0; env[i]; i++) {
        if (strstr(env[i], "PATH")) {
            sub_string = get_sub_string("PATH=", env[i]);
            if (sub_string != NULL) {
                // Allocate memory for paths
                paths = malloc((strlen(sub_string) + 1) * sizeof(char));
                if (paths == NULL) {
                    perror("Memory allocation failed");
                    exit(EXIT_FAILURE);
                }
                my_strcpy(paths, sub_string);
                free(sub_string); // Free memory allocated by get_sub_string
            }
            break;
        }
    }
    return paths;
}

#define MAX_PATH_LENGTH 1000

char **split_paths(char *paths, int *count) {
    char **result = malloc(sizeof(char*));
    char *token;
    char pathsCopy[MAX_PATH_LENGTH];
    
    // Make a copy of the input string because strtok modifies the string
    strncpy(pathsCopy, paths, sizeof(pathsCopy));
    pathsCopy[sizeof(pathsCopy) - 1] = '\0';  // Ensure null-termination
    
    token = strtok(pathsCopy, ":");
    
    *count = 0;
    while (token != NULL) {
        result = realloc(result, ((*count) + 1) * sizeof(char *));
        if (result == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        result[*count] = strdup(token);
        if (result[*count] == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        (*count)++;
        token = strtok(NULL, ":");
    }
    return result;
}