#include "my_zsh.h"

int my_strlen(const char *input);
char *my_strdup(const char *str);
char *my_strcpy(char *dest, const char *src);

int my_strlen(const char *input) {
    int length = 0;
    while(*input != '\0'){
        length++;
        input++;
    }
    return length;
}

char *my_strdup(const char *str) {
    if (str == NULL)
        return NULL;
    
    size_t len = my_strlen(str);

    char *dup_str = (char *)malloc((len + 1) * sizeof(char));

    if (dup_str == NULL)
        return NULL;

    my_strcpy(dup_str, str);

    return dup_str;
}

char *my_strcpy(char *dest, const char *src) {
    char *old_dest = dest;
   while(*src != '\0'){
       *dest = *src;
       dest++;
       src++;
   }
   *dest = '\0';
   return old_dest;
}

char *my_strchr(const char *str, int target) {
    while (*str != '\0') {
        if (*str == target) {
            return (char *)str; // Cast const away
        }
        str++;
    }
    return NULL;
}

char *my_strtok(char *input_string, const char *delimiter) {
    static char *next_token = NULL;

    // If input_string is NULL, continue from the last token
    if (input_string == NULL) {
        input_string = next_token;
    }

    // Skip leading delimiters
    while (*input_string && my_strchr(delimiter, *input_string)) {
        input_string++;
    }

    // If no more tokens, return NULL
    if (*input_string == '\0') {
        next_token = NULL;
        return NULL;
    }

    // Find the end of the token
    char *token = input_string;
    while (*input_string && !my_strchr(delimiter, *input_string)) {
        input_string++;
    }

    // If delimiter found, replace it with null character
    if (*input_string) {
        *input_string = '\0';
        next_token = input_string + 1;
    } else {
        next_token = NULL;
    }

    return token;
}

// Function to copy the arg without the first char
char *cpy_var(const char *args) {
    int arg_len = my_strlen(args) - 1;  // Exclude the first character
    char *arg = malloc((arg_len + 1) * sizeof(char));  // Allocate memory
    if (arg == NULL) {
        perror("malloc");
        return NULL;
    }

    for (int i = 1; args[i]; i++) {
        arg[i - 1] = args[i];
    }
    arg[arg_len] = '\0';  // Null-terminate the string

    return arg;
}

// Function to get substr, removing remove and '=' from full
char *get_sub_string(char *remove, char *full) {
    int remove_len = my_strlen(remove);
    int full_len = my_strlen(full);
    int ret_len = full_len - remove_len - 1;
    char *ret = malloc((ret_len + 1) * sizeof(char));  // Allocate memory
    if (ret == NULL) {
        perror("malloc");
        return NULL;
    }

    for (int i = remove_len+1; full[i]; i++) {
        ret[i - (remove_len+1)] = full[i];
    }
    ret[ret_len] = '\0';  // Null-terminate the string

    return ret;
}