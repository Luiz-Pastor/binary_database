#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loop.h"

enum {
    EXIT = 0,
    ADD,
    UNKNOWN
};

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

char    *add_element(char *input)
{
    printf("Añadiendo elemento...\n");
    return (NULL);
}

void    take_commands(Element **database)
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
                add_element(arguments);
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
