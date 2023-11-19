#ifndef DATABASE_H
# define DATABASE_H

# include "structs.h"

Element **read_database(char *filename);
void    *free_database(Element **database);
void    printDatabase(Element **database);
size_t	databaseLength(Element **database);
Element	**addDatabaseElement(Element **database, Element *element);
void    save_database(Element **database, char *filename);


#endif