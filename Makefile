TARGET = my_zsh
OBJ    = main.c input_parser.c shell_builts.c exec.c helper_functions.c paths.c
CFLAGS = -Wall -Wextra -Werror 
CC     = gcc

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)
clean:
	rm -f *.o
fclean: clean
	rm -f $(TARGET)
re: fclean all
