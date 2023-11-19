##############################################
CC := gcc
CFLAGS := -Wall -Werror -pedantic -ansi -g3
##############################################
NAME=library
HEADER=element.h
SRC=	main.c		\
		element.c	\
		database.c	\
		loop.c
OBJ=$(SRC:%.c=%.o)
##############################################

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $^ -o $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $<

##############################################
run: $(NAME)
	@./$(NAME) best_fit data.db

rune: $(NAME)
	@./$(NAME) best_fit data.db | cat -e

valgrind: $(NAME)
	@valgrind -s --leak-check=full ./$(NAME) best_fit data.db
##############################################

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean $(NAME)

##############################################