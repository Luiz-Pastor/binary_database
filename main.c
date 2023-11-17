#include <stdio.h>
#include <string.h>
#include "structs.h"

/*
TODO:
    - Guardar toda la información del archivo en estructuras
    - Funcion para leer los comandos del usuario
    - Guardar todos los cambios que se hayan hecho en el programa
    - Liberar memoria
*/
static int  exit_error(char *program)
{
    printf("Error. Usage: %s <insertion_mode> <databse_file>.\n", program);
    printf("Posible insertion modes:\n\t·best_fit\n\t·first_mode\n\t·worst_mode\n");
    return (1);
}

static int  check_arguments(int argc, char **argv)
{
    FILE *file;

    if (argc != 3)
        return 0;
    if (strcmp(argv[1], "best_fit") && strcmp(argv[1], "first_fit") && strcmp(argv[1], "worst_fit"))
        return 0;

    file = fopen(argv[2], "rb");
    if (!file)
        return (0);
    fclose(file);
    return 1;
}


Element **read_database(char *filename)
{
    size_t  size;
    int     key;
    char    isbn[ISBN_LENGTH];
    char    *title;

    int     length; /* Aux */
    // char    *printedBy;
    // char aux;

    // Element *node;
    
    FILE    *file;

    file = fopen(filename, "rb");
    if (!file)
        return (printf("No open\n"), NULL);

    fread(&size, sizeof(size_t), 1, file);
    printf("> %ld\n", size);

    fread(&key, sizeof(int), 1, file);
    printf("> %d\n", key);

    fread(isbn, sizeof(char), ISBN_LENGTH, file);
    isbn[ISBN_LENGTH] = '\0';
    printf("> %s\n", isbn);

    title = calloc(1, 1);
    if (!title)
    {
        fclose(file);
        return (NULL);
    }


    fclose(file);
    return (NULL);
}


void    take_commands(Element **database)
{
    return ;
}

void    save_database(Element **database)
{
    return ;
}

void    free_database(Element **database)
{
    return ;
}

int main(int argc, char *argv[])
{
    Element **database;

    /* Check arguments */
    if (!check_arguments(argc, argv))
        return exit_error(argv[0]);

    /* Read all the info in the database */
    database = read_database(argv[2]);
    if (!database)
    {
        printf("Database file error.\n");
        return 1;
    }

    /* Read the user commands */
    take_commands(database);

    /* Save all the information in the database file */
    save_database(database);

    /* Free the memory */
    free_database(database); /* Una alternativa puede ser usar exit */

    return (0);
}

/*
OP 1:
    - Leer todo el archivo primero --> guardamos en estructuras
    - Trabajar con estructuras, y cuando se cierre el programa escribir todo
    - Función `loop`, que vaya leyendo los comandos
*/