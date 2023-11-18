#include <stdio.h>
#include <string.h>
#include "structs.h"
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

int  number_length(int number)
{
    int count = 0;
    if (number == 0)
        return (1);
    while (number != 0)
    {
        count++;
        number /= 10;
    }
    return (count);
}

void    printDatabase(Element **database)
{
    int index = 0;

    while (database[index])
    {
        printElement(database[index++]);
        printf("\n=================\n\n");
    }
}

void    free_database(Element **database)
{
    int index = 0;

    while (database[index])
        deleteElement(database[index++]);
    free(database);
    return ;
}

Element **read_database(char *filename)
{
    FILE    *file;
    Element *element;
    Element **database;
    /*Element **memory;*/
    int     database_size = 1;

    /* Variables auxiliares */
    int     i;
    int     offset = 0;
    char    letter[2];
    char    text[MAX_LENGTH];
    int     count;

    file = fopen(filename, "rb");
    if (!file)
        return (printf("No open\n"), NULL);

    database = (Element **) malloc(sizeof(Element *));
    if (!database)
    {
        fclose(file);
        return (NULL);
    }

    while (!feof(file))
    {
        /* Reiniciamos la variable donde pondremos texto variable */
        for (i = 0; i < MAX_LENGTH; i++)
            text[i] = '\0';
        /* Creamos el nodo del elemento */
        element = createElement();
        if (!element)
        {
            free_database(database);
            fclose(file);
            return (NULL);
        }

        /* Fijamos el offset */
        element->index.offset = offset;

        /* Guardamos el tamaño del bloque */
        fread(&(element->index.size), sizeof(size_t), 1, file);
        if (feof(file))
        {
            deleteElement(element);
            break;
        }

        /* Guardamos el bookId */
        fread(&(element->index.key), sizeof(int), 1, file);

        /* Guardamos el ISBN */
        fread(element->isbn, sizeof(char), ISBN_LENGTH, file);
        element->isbn[ISBN_LENGTH] = '\0';

        /*
            Guardamos todo en una variable auxiliar, hasta encontrar un '|'.
            Después, reservamos el espacio necesario para almacenarlo en el nombre
            del titulo. Así, no desperdiciamos espacio/memoria
        */
        do{
            fread(letter, sizeof(char), 1, file);
            letter[1] = '\0';
            if (letter[0] != '|')
                strcat(text, letter);
        } while (letter[0] != '|');

        element->title = calloc(strlen(text) + 1, sizeof(char));
        if (!(element->title))
            return (NULL);
        strcpy(element->title, text);
    
        /*
            Calculamos cuantos caracteres nos quedan por leer, y leemos esa cantidad.
            Todo ello forma parte del campo 'printedBy'
        */
        count = (element->index.size) - number_length(element->index.key) - strlen(element->isbn) - strlen(element->title);
        element->printedBy = calloc(count + 1, sizeof(char));
        if (!(element->printedBy))
            return NULL;
        fread(element->printedBy, sizeof(char), count, file);

        /* Calculamos el offset del siguiente elemento */
        offset += element->index.size +  strlen(element->printedBy) + 1;

        /* Añadimos un elemento a la base de datos y lo metemos */
        database = (Element **) realloc(database, (database_size + 1) * sizeof(Element *));
        if (!database)
        {
            deleteElement(element);
            free_database(database);
            fclose(file);
            return (NULL);
        }
        database[database_size - 1] = element;
        database_size++;
    }
    database[database_size - 1] = NULL;

    fclose(file);
    return (database);
}


void    take_commands(Element **database)
{
    return ;
}

void    save_database(Element **database)
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

    printDatabase(database);

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