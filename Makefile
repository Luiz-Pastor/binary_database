##############################################
CC := gcc
CFLAGS := -Wall -Werror -ansi -pedantic
##############################################
NAME=library
SRC=	main.c
OBJ=$(SRC:%.c=%.o)
##############################################

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $^ -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

##############################################
run:
	./library best_fit test.db
##############################################

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean $(NAME)

##############################################