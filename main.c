#include <stdio.h>
#include <string.h>
#include "structs.h"
#include "database.h"
#include "loop.h"
#include "element.h"

/*
TODO:
    - Guardar toda la información del archivo en estructuras
    - Funcion para leer los comandos del usuario
    - Guardar todos los cambios que se hayan hecho en el programa
    - Liberar memoria
*/
static int  exit_error(char *program)
{
    printf("Error. Usage: %s <insertion_mode> <database_file>.\n", program);
    printf("Posible insertion modes:\n\t·best_fit\n\t·first_mode\n\t·worst_mode\n");
    return (1);
}

static int  check_arguments(int argc, char **argv)
{
    if (argc != 3)
        return 0;

    if (strcmp(argv[1], "best_fit") && strcmp(argv[1], "first_fit") && strcmp(argv[1], "worst_fit"))
        return 0;
    return 1;
}

int main(int argc, char *argv[])
{
    Database    *database;
    char        filename[1024];

    /* Guardamos el nombre del archivo que leer y escribir */
    sprintf(filename, "%s.db", argv[2]);

    /* Check arguments */
    if (!check_arguments(argc, argv))
        return exit_error(argv[0]);

    /* Read all the info in the database */
    database = read_database(filename);
    if (!database)
    {
        printf("Error reading the database.\n");
        return 1;
    }

    /* Read the user commands */
    take_commands(database);

    /* printDatabase(database); */

    /* Save all the information in the database file */
    save_database(database, filename);

    /* Free the memory */
    free_database(database); /* Una alternativa puede ser usar exit */

    return (0);
}
