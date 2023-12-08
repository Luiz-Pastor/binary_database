#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "element.h"
#include "database.h"
#include "loop.h"

/* TODO: añadir y comentar el comando printLst */
enum {
	EXIT = 0,
	ADD,
	FIND,
	PRINTIND,
	PRINTREC,
	DEL,
	UNKNOWN
};

static char  *ft_strdup(char *string)
{
	char	*copy;
	int		index = 0;

	copy = malloc(strlen(string) + 1);
	if (!copy)
		return (NULL);
	while (string[index])
	{
		copy[index] = string[index];
		index++;
	}
	copy[index] = '\0';
	return (copy);
}

static int get_command(char *command)
{
	if (!strcmp(command, "exit"))
		return (EXIT);
	else if (!strcmp(command, "add"))
		return (ADD);
	else if(!strcmp(command, "find"))
		return (FIND);
	else if (!strcmp(command, "printInd"))
		return (PRINTIND);
	else if (!strcmp(command, "printRec"))
		return (PRINTREC);
	else if (!strcmp(command, "del"))
		return (DEL);
	return UNKNOWN;
}

static int command_add(char *input, Database *database)
{
	/*
	- Repetido:     add 12345|978-2-12345680-3|El Quijote|catedra
	- No repetido:  add 00001|978-2-12345680-3|El Quijote|catedra
	- Bad format:   add 99998|978-2-12345683|El Quijote|catedra
	*/

	Element	*element;
	char	*string;

	element = createElement();
	if (!element)
		return (MEMORY_ERROR);
	
	/* bookID */
	string = strtok(input, "|");
	element->index.key = atoi(string);

	/* ISBN */
	string = strtok(NULL, "|");
	if (strlen(string) != ISBN_LENGTH)
	{
		deleteElement(element);
		return BAD_FORMAT;
	}
	strcpy(element->isbn, string);

	/* Title */
	string = strtok(NULL, "|");
	if (strlen(string) > MAX_LENGTH)
	{
		deleteElement(element);
		return BAD_FORMAT;
	}
	element->title = ft_strdup(string);

	/* printedBy */
	string = strtok(NULL, "\0");
	if (strlen(string) > MAX_LENGTH)
	{
		deleteElement(element);
		return BAD_FORMAT;
	}
	element->printedBy = ft_strdup(string);
	element->index.size = 20 + strlen(element->title) + strlen(element->printedBy) + 1;

	if (addDatabaseElement(database, element))
		return REPEATED_ELEMENT;

	return (OK);
}

static Element* command_find(Database *database, char *input)
{
	int	bookID;

	bookID = atoi(input);
	return findDatabaseElement(database, bookID);
}

static void command_printind(Database *database)
{
	int		index;
	Element	**order;

	/* Copiamos los elementos de la tabla original en la nueva */
	order = copyElements(database->elements);
	if (!order)
		return ;

	/* Ordenamos, aplicando SelectorSort*/
	shortElements(order);
	
	/* Imprimimos los elementos de la tabla ordenada */
	index = 0;
	while (order[index])
	{
		if (order[index]->using)
		{
			printf("Entry #%d\n", index);
			printf("    key: #%d\n", order[index]->index.key);
			printf("    offset: #%ld\n", order[index]->index.offset);
			printf("    size: #%ld\n", order[index]->index.size);
		}
		index++;
	}

	/* Eliminamos la memoria de la tabla auxiliar */
	free(order);
}

static void command_printrec(Database *database)
{
	int		index;
	Element	*current, **order;

	/* Copiamos los elementos de la tabla original en la nueva */
	order = copyElements(database->elements);
	if (!order)
		return ;

	/* Ordenamos, aplicando SelectorSort */
	shortElements(order);

	/* Imprimimos los elementos de la tabla ordenada */
	index = 0;
	while (order[index])
	{
		current = order[index];
		if (current->using)
			printf("%d|%s|%s|%s\n", current->index.key, current->isbn, current->title, current->printedBy);
		index++;
	}

	/* Eliminamos la memoria de la tabla auxiliar */
	free(order);
}

static int command_del(Database *database, char *command)
{
	int		bookID, index = 0;
	char	*copy, *first, *second;
	Element	*element;

	copy = ft_strdup(command);

	first = strtok(copy, " ");
	second = strtok(NULL, "\0");

	if (second)
	{
		free(copy);
		return BAD_FORMAT;
	}

	while (first[index])
	{
		if (!isdigit(first[index]))
		{
			free(copy);
			return BAD_FORMAT;
		}
		index++;
	}

	bookID = atoi(first);
	element = findDatabaseElement(database, bookID);
	if (!element)
	{
		free(copy);
		return ERROR;
	}
	
	cleanElement(element);

	

	free(copy);
	return OK;
}

void    take_commands(Database *database)
{
	char	input[1025] = "";
	char	*cmd;
	int		command;
	char	*arguments;

	Element	*element;

	printf("Type command and argument/s.\nexit\n");

	while (1)
	{
		fgets(input, 1024, stdin);
		
		if (input[0] != '\n')
		{
			input[strlen(input) - 1] = '\0';

			/* Separamos entre comando y argumentos */
			cmd = strtok(input, " ");
			arguments = strtok(NULL, "\0");

			/* Ejecutamos la acción del comando */
			command = get_command(cmd);
		}
		else
			command = UNKNOWN;
		switch (command)
		{
			case EXIT:
				return ;
				break;

			case ADD:
				switch (command_add(arguments, database))
				{
					case OK:
						element = getLastElement(database);
						printf("Record with BookID=%d has been added to the database\n", element->index.key);
						break;
					case REPEATED_ELEMENT:
						arguments = strtok(arguments, "|");
						printf("Record with BookID=%s exists\n", arguments);
						break;
					case MEMORY_ERROR:
						printf("Memory error!\n");
						return ;
					case BAD_FORMAT:
						printf("Bad expression format\n");
						break;
				}
				break;

			case FIND:
				element = command_find(database, arguments);
				if (element)
					printf("%d|%s|%s|%s\n", element->index.key, element->isbn, element->title, element->printedBy);
				else
					printf("Record with bookId=%s does not exist\n", arguments);
				break;

			case PRINTIND:
				command_printind(database);
				break;

			case PRINTREC:
				command_printrec(database);
				break;

			case DEL:
				switch (command_del(database, arguments))
				{
					case OK:
						printf("Record with BookID=%d has been deleted\n", atoi(arguments));
						break;
					case BAD_FORMAT:
						printf("Bad expression format\n");
						break;
					case ERROR:
						printf("Record with BookID=%d does not exist\n", atoi(arguments));
						break;
				}
				break;

			case UNKNOWN:
				printf("Unrecognized command\n");
			default:
				break;
		}
		printf("exit\n");
	}
	return ;
}
