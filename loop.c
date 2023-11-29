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
    FIND,
    PRINTIND,
    PRINTREC,
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
    return UNKNOWN;
}

/*
- Repetido:     add 12345|978-2-12345680-3|El Quijote|catedra
- No repetido:  add 00001|978-2-12345680-3|El Quijote|catedra
_ Bad format:   add 99998|978-2-12345683|El Quijote|catedra
*/
static int add_element(char *input, Database *database)
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

static Element* findElement(Database *database, char *input)
{
    int bookID;

    bookID = atoi(input);
    return findDatabaseElement(database, bookID);
}

static void printIndex(Database *database)
{
    int index = 0;

    while (database->elements[index])
    {
        printf("Entry #%d\n", index);
        printf("    key: #%d\n", database->elements[index]->index.key);
        printf("    offset: #%ld\n", database->elements[index]->index.offset);
        printf("    size: #%ld\n", database->elements[index]->index.size);
        index++;
    }
}

static void printAllElements(Database *database)
{
    int index = 0;
    Element *current;

    while (database->elements[index])
    {
        current = database->elements[index];
        printf("%d|%s|%s|%s\n", current->index.key, current->isbn, current->title, current->printedBy);
        index++;
    }
}

void    take_commands(Database *database)
{
    char    input[1025] = "";
    char    *cmd;
    int     command;
    char    *arguments;

    Element *element;

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
                switch (add_element(arguments, database))
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
                element = findElement(database, arguments);
                if (element)
                    printf("%d|%s|%s|%s\n", element->index.key, element->isbn, element->title, element->printedBy);
                else
                    printf("Record with bookId=%s does not exist\n", arguments);
                break;

            case PRINTIND:
                printIndex(database);
                break;

            case PRINTREC:
                printAllElements(database);
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
