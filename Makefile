##############################################
C_CLEAR	=	\033[0m
C_RED	=	\033[91;1m
C_GREEN	=	\033[92;1m
C_BLUE	=	\033[94;1m
C_PINK	=	\033[95;1m
C_YELLOW	=	\033[93;1m
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

$(NAME): $(OBJ) restart
	@echo "$(C_BLUE)Compiling proyect...$(C_CLEAR)"
	@$(CC) $(OBJ) -o $(NAME)
	@echo "$(C_GREEN)Compiled!$(C_CLEAR)"

%.o: %.c
	@echo "$(C_YELLOW)\t> Compiling $< $(C_CLEAR)"
	@$(CC) $(CFLAGS) -c $<

##############################################
DB_FILE=test

run: $(NAME)
	@./$(NAME) best_fit $(DB_FILE)

rune: $(NAME)
	@./$(NAME) best_fit $(DB_FILE) | cat -e

valgrind: $(NAME)
	@valgrind -s --leak-check=full ./$(NAME) best_fit data.db
##############################################

restart:
	@cp tests/data.db .

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean $(NAME)

.PHONY: clean fclean re restart
##############################################