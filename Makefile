##############################################
CC := gcc
CFLAGS := -Wall -Werror -g3
##############################################
NAME=library
SRC=	main.c
OBJ=$(SRC:%.c=%.o)
##############################################

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $^ -o $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $<

##############################################
run: re
	@./$(NAME) best_fit data.db | cat -e

valgrind: re
	@valgrind --leak-check=full ./$(NAME) best_fit data.db
##############################################

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean $(NAME)

##############################################