#include "my_zsh.h"

int executor(char **args, char **env) {
    pid_t pid; 
    int status;
    
    pid = fork();
    if (pid == -1) { // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        // Get the possible paths for child process and run
        // Execute the command
        if (child_process(args, env)) {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    } else { 
        // Parent process
        // Wait for the child process to terminate
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        // Parent process
        // Check if the child process terminated due to a segmentation fault
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV) {
            perror("segmentation fault");
        }
    }

    return 1;
}

int child_process(char **args, char **env) {
    // Get and check each path if they lead to the correct directory
    char *full_path = malloc((MAX_INPUT) * sizeof(char));
    int cmd_length = my_strlen(args[0]);; // Current command
    
    char *path_string = get_path(env);
    int num_paths;
    char **path_list = split_paths(path_string, &num_paths);

    // Checks if the command is executable on PATH
    for (int i = 0; i < num_paths; i++) {
        int path_length = my_strlen(path_list[i]); // Current testing path
        char *temp_path = path_list[i];
        
        // Ensure that the combined length does not exceed the buffer size
        if (path_length + cmd_length + 2 > MAX_INPUT) {
            fprintf(stderr, "Path too long\n");
            exit(EXIT_FAILURE);
        }

        // Concatenate the current testing directory and command with a '/'
        my_strcpy(full_path, temp_path);
        full_path[path_length] = '/';
        my_strcpy(full_path + path_length + 1, args[0]);
        
        if(execve(full_path, args, env) != -1) return 0;
        
        free(temp_path);
    }
    // Checks if the command is executable on CWD
    {
        char cwd[MAX_INPUT];
        int cwd_length; // Current working directory

        // Child process
        // Get the current working directory
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");
            exit(EXIT_FAILURE);
        }

        // Calculate the lengths of current working directory and command
        cwd_length = my_strlen(cwd);
        cmd_length = my_strlen(args[0]);

        // Ensure that the combined length does not exceed the buffer size
        if (cwd_length + cmd_length + 2 > MAX_INPUT) {
            fprintf(stderr, "Path too long\n");
            exit(EXIT_FAILURE);
        }

        // Concatenate the current working directory and command with a '/'
        my_strcpy(full_path, cwd);
        full_path[cwd_length] = '/';
        my_strcpy(full_path + cwd_length + 1, args[0]);

        if(execve(full_path, args, env) != -1) return 0;
    }

    free(path_string);
    free(path_list); // Free the array
    free(full_path);
    
    return 1;
}
