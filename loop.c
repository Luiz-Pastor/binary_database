#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "element.h"
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

/* add 12345|978-2-12345680-3|El Quijote|catedra */
char    *add_element(char *input, Database *database)
{
    Element *element;
    char    *string;

    element = createElement();
    if (!element)
        return (NULL);
    
    /* bookID */
    string = strtok(input, "|");
    element->index.key = atoi(string);

    /* ISBN */
    string = strtok(NULL, "|");
    if (strlen(string) > ISBN_LENGTH)
    {
        printf("Error de formato.\n");
        deleteElement(element);
        return NULL;
    }
    strcpy(element->isbn, string);

    /* Title */
    string = strtok(NULL, "|");
    if (strlen(string) > MAX_LENGTH)
    {
        printf("Error de formato.\n");
        deleteElement(element);
        return NULL;
    }
    element->title = ft_strdup(string);

    /* printedBy */
    string = strtok(NULL, "\0");
    if (strlen(string) > MAX_LENGTH)
    {
        printf("Error de formato.\n");
        deleteElement(element);
        return NULL;
    }
    element->printedBy = ft_strdup(string);
    element->index.size = 20 + strlen(element->title) + strlen(element->printedBy) + 1;

    printElement(element);

    return (NULL);
}

void    take_commands(Database *database)
{
    char    input[1025] = "";
    char    *cmd;
    char    *arguments;
    int command;

    while (1)
    {
        fgets(input, 1024, stdin);
        input[strlen(input) - 1] = '\0';

        /* Separamos entre comando y argumentos */
        cmd = strtok(input, " ");
        arguments = strtok(NULL, "\0");

        command = get_command(cmd);
        switch (command)
        {
            case EXIT:
                return ;
                break;
            case ADD:
                add_element(arguments, database);
                break;
            case UNKNOWN:
                printf("Unrecognized command\n");
                break;
            default:
                break;
        }
    }
    return ;
}
