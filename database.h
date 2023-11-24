#ifndef DATABASE_H
# define DATABASE_H

# include "structs.h"

/*
 * @brief	Function that is responsible for reading a database through a
			binary file.
 * 
 * @param 	filename Name of the file with the database.
 * 
 * @return	Database struct, with an array with all the elements of the database,
 * 			NULL-terminated.
*/
Database *read_database(char *filename);

/*
 * @brief	Function that deletes a database (NULL-terminated).
 * 
 * @param	database Database to delete.
 * 
 * @return	NULL pointer.
*/
void    *free_database(Database *database);

/*
 * @brief	Function that prints all the information of a database.
 * 
 * @param	database The database with the info.
*/
void    printDatabase(Database *database);

/*
 * @brief	Function that obtains the number of blocks that a database has.
 * 
 * @param	database The database.
 * 
 * @return	The number of elements in the database, or 0 if the database is NULL.
*/
size_t	databaseLength(Database *database);

/*
 * @brief	Function that adds a block to the database.
 * 
 * @param	database Database to add the item to.
 * 
 * @return	1 if `element` has been added correctly, 0 if the element
 * 			(bookID) exists
*/
int addDatabaseElement(Database *database, Element *element);

/*
 * @brief	Function that saves the database information in a file.
 * 
 * @param	database Database from which to extract information.
 * @param	filename Name of the file where the information will be saved.
*/
void    save_database(Database *database, char *filename);

Element *getLastElement(Database *database);
Element *findDatabaseElement(Database *database, int key);

#endif
