#include "my_zsh.h"

char **input_parser(char *input) {
    size_t buffer_size = MAX_INPUT;
    int position = 0;
    char **tokens = malloc(buffer_size * sizeof(char*));
    char *token_start = NULL;
    int token_length = 0;

    if (!tokens) {
        printf("Allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Loop through each character in the input string
    for (int i = 0; input[i] != '\0'; i++) {
        // Skip leading whitespace characters
        // \t	Horizontal Tab, \r	Carriage Return, \n	New Line, \a	Alarm or Beep
        while (input[i] == ' ' || input[i] == '\t' || input[i] == '\r' || input[i] == '\n' || input[i] == '\a') {
            i++;
        }

        // Check if end of string is reached
        if (input[i] == '\0') {
            break;
        }

        // Store the start position of the token
        token_start = &input[i];

        // Calculate the length of the token
        while (input[i] != '\0' && input[i] != ' ' && input[i] != '\t' && input[i] != '\r' && input[i] != '\n' && input[i] != '\a') {
            token_length++;
            i++;
        }

        // Allocate memory for the token
        tokens[position] = malloc((token_length + 1) * sizeof(char));
        if (!tokens[position]) {
            printf("Allocation error\n");
            exit(EXIT_FAILURE);
        }

        // Copy the token into the array
        for (int j = 0; j < token_length; j++) {
            tokens[position][j] = token_start[j];
        }
        tokens[position][token_length] = '\0'; // Null-terminate the token
        position++;
        token_length = 0; // Reset token length for the next token
    }

    tokens[position] = NULL; // Terminate the array with NULL
    return tokens;
}

// Free memory allocated for tokens
void free_tokens(char **tokens) {
    if (tokens == NULL) {
        return; // No memory allocated
    }
    
    // Iterate through each element of the tokens array
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]); // Free memory for each token
    }
    
    free(tokens); // Free memory for the tokens array itself
}
