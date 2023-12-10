#include <stdio.h>
#include <string.h>
#include "../inc/structs.h"
#include "../inc/database.h"
#include "../inc/loop.h"
#include "../inc/element.h"

/*
TODO:
	- Guardar toda la información del archivo en estructuras
	- Funcion para leer los comandos del usuario
	- Guardar todos los cambios que se hayan hecho en el programa
	- Liberar memoria
*/

/*static int  print_help(char *program)
{
	printf("Error. Usage: %s <insertion_mode> <database_file>.\n", program);
	printf("Posible insertion modes:\n\t·best_fit\n\t·first_mode\n\t·worst_mode\n");
	return (1);
}*/

static int  check_arguments(int argc, char **argv)
{

	if (argc != 3)
	{
		printf("Missing argument\n");
		return (0);
	}

	if (strcmp(argv[1], "best_fit") && strcmp(argv[1], "first_fit") && strcmp(argv[1], "worst_fit"))
	{
		printf("Unknown search strategy unknown_search_strategy\n");
		return 0;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	int			status;
	Database	*database;

	/* Check arguments */
	if (!check_arguments(argc, argv))
		return (0);

	/* Read all the info in the database */
	database = read_database(argv[1], argv[2]);
	if (!database)
	{
		printf("Error reading the database.\n");
		return 1;
	}

	/* Read the user commands */
	take_commands(database);

	/* Save all the information in the database file */
	status = 0;
	if (save_database(database, argv[2]))
	{
		printf("Error while saving the database\n");
		status = 1;
	}

	/* Free the memory */
	free_database(database); /* Una alternativa puede ser usar exit */

	return (status);
}
