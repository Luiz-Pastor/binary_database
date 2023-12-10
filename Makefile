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

$(NAME): $(OBJ)
	@echo "$(C_BLUE)Compiling proyect...$(C_CLEAR)"
	@$(CC) $(OBJ) -o $(NAME)
	@echo "$(C_GREEN)Compiled!$(C_CLEAR)"

%.o: src/%.c
	@echo "$(C_YELLOW)\t> Compiling $< $(C_CLEAR)"
	@$(CC) $(CFLAGS) -c $<

##############################################

test: all
	@rm -rf tests/test.db
	@echo "\n$(C_RED)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@$(C_CLEAR)"

# Primer test
	@echo "\n\t\t$(C_GREEN)####################$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)# $(C_YELLOW)add_data_test.sh $(C_GREEN)#$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)####################$(C_CLEAR)"
	@cd tests; ./add_data_test.sh

# Segundo test
	@echo "\n\n\t\t$(C_GREEN)################$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)# $(C_YELLOW)cli_tests.sh $(C_GREEN)#$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)################$(C_CLEAR)"
	@cd tests; ./cli_tests.sh

# Tercer test
	@echo "\n\n\t\t$(C_GREEN)###################$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)# $(C_YELLOW)many_entries.sh $(C_GREEN)#$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)###################$(C_CLEAR)"
	@cd tests; ./many_entries.sh

	@echo "\n$(C_RED)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@$(C_CLEAR)\n"
	@rm -rf tests/test.db
##############################################

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean $(NAME)

.PHONY: clean fclean re restart

##############################################

env:
	@cp tests/test_control.db .

run: all
	./library first_fit test_control

valgrind: all
	valgrind --leak-check=full ./library first_fit test_control