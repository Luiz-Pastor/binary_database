#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "element.h"
#include "database.h"
#include "loop.h"

enum {
    EXIT = 0,
    ADD,
    UNKNOWN
};

static char  *ft_strdup(char *string)
{
    char *copy;
    int index = 0;

    copy = malloc(strlen(string) + 1);
    if (!copy)
        return (NULL);
    while (string[index])
    {
        copy[index] = string[index];
        index++;
    }
    return (copy);
}

int get_command(char *command)
{
    int index = -1;

    while (command[++index])
        command[index] = tolower(command[index]);

    if (!strcmp(command, "exit"))
        return (EXIT);
    else if (!strcmp(command, "add"))
        return (ADD);
    return UNKNOWN;
}

/*
- Repetido:     add 12345|978-2-12345680-3|El Quijote|catedra
- No repetido:  add 99999|978-2-12345680-3|El Quijote|catedra
_ Bad format:   add 99998|978-2-12345683|El Quijote|catedra
*/
int add_element(char *input, Database *database)
{
    Element *element;
    char    *string;

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

void    take_commands(Database *database)
{
    char    input[1025] = "";
    char    *cmd;
    int     command;
    char    *arguments;

    Element *last_element;

    while (1)
    {
        fgets(input, 1024, stdin);
        input[strlen(input) - 1] = '\0';

        /* Separamos entre comando y argumentos */
        cmd = strtok(input, " ");
        arguments = strtok(NULL, "\0");

        /* Ejecutamos la acción del comando */
        command = get_command(cmd);
        switch (command)
        {
            case EXIT:
                return ;
                break;
            case ADD:
                switch (add_element(arguments, database))
                {
                    case OK:
                        last_element = getLastElement(database);
                        printf("Record with BookID=%d has been added to the database\n", last_element->index.key);
                        break;
                    case REPEATED_ELEMENT:
                        arguments = strtok(arguments, "|");
                        printf("Record with BookID=%s exists\n", arguments);
                        break;
                    case MEMORY_ERROR:
                        printf("Memory error!");
                        return ;
                    case BAD_FORMAT:
                        printf("Bad expression format\n");
                        break;
                }
                break;
            case UNKNOWN:
                printf("Unrecognized command\n");
            default:
                break;
        }
    }
    return ;
}
