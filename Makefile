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
SRC=	main.c		\
		element.c	\
		database.c	\
		loop.c
OBJ=$(SRC:%.c=obj/%.o)
##############################################
TEST_FILE=test
##############################################

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(C_BLUE)Compiling proyect...$(C_CLEAR)"
	@$(CC) $(OBJ) -o $(NAME)
	@echo "$(C_GREEN)Compiled!$(C_CLEAR)"

obj/%.o: src/%.c
	@mkdir -p obj/
	@echo "$(C_YELLOW)\t> Compiling $< $(C_CLEAR)"
	@$(CC) $(CFLAGS) -c $< -o $@

##############################################

first_fit: $(NAME)
	./$(NAME) first_fit $(TEST_FILE)

worst_fit: $(NAME)
	./$(NAME) worst_fit $(TEST_FILE)

best_fit: $(NAME)
	./$(NAME) best_fit $(TEST_FILE)

valgrind: $(NAME)
	./$(NAME) first_fit $(TEST_FILE)

##############################################

test: all
	@rm -rf tests/test.db tests/test.ind tests/test.lst
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

# Cuarto test
	@echo "\n\n\t\t$(C_GREEN)#########################$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)# $(C_YELLOW)add_delete_test_01.sh $(C_GREEN)#$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)#########################$(C_CLEAR)"
	@cd tests; ./add_delete_test_01.sh

# Quinto test
	@echo "\n\n\t\t$(C_GREEN)#########################$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)# $(C_YELLOW)add_delete_test_02.sh $(C_GREEN)#$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)#########################$(C_CLEAR)"
	@cd tests; ./add_delete_test_02.sh

# Sexto test
	@echo "\n\n\t\t$(C_GREEN)#########################$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)# $(C_YELLOW)add_delete_test_03.sh $(C_GREEN)#$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)#########################$(C_CLEAR)"
	@cd tests; ./add_delete_test_03.sh

# Séptimo test
	@echo "\n\n\t\t$(C_GREEN)#########################$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)# $(C_YELLOW)add_delete_test_04.sh $(C_GREEN)#$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)#########################$(C_CLEAR)"
	@cd tests; ./add_delete_test_04.sh

# Octavo test
	@echo "\n\n\t\t$(C_GREEN)#####################$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)# $(C_YELLOW)add_index_test.sh $(C_GREEN)#$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)#####################$(C_CLEAR)"
	@cd tests; ./add_index_test.sh

# Noveno test
	@echo "\n\n\t\t$(C_GREEN)###################$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)# $(C_YELLOW)reload_index.sh $(C_GREEN)#$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)###################$(C_CLEAR)"
	@cd tests; ./reload_index.sh

	@echo "\n$(C_RED)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@$(C_CLEAR)\n"
	@rm -rf tests/test.db tests/test.ind tests/test.lst

personal_test: $(NAME)
	@rm -rf tests/test.db tests/test.ind tests/test.lst
	@echo "\n$(C_RED)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@$(C_CLEAR)"

	@echo "\n\n\t\t$(C_GREEN)###################$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)# $(C_YELLOW)test_use_deleted_books.sh $(C_GREEN)#$(C_CLEAR)"
	@echo "\t\t$(C_GREEN)###################$(C_CLEAR)"
	@cd tests; ./test_use_deleted_books.sh

	@echo "\n$(C_RED)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@$(C_CLEAR)\n"
	@rm -rf tests/test.db tests/test.ind tests/test.lst

##############################################

clean:
	@rm -rf obj/

fclean: clean
	@rm -rf $(NAME)

re: fclean $(NAME)

.PHONY: clean fclean re restart

##############################################
