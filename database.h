#ifndef DATABASE_H
# define DATABASE_H

# include "structs.h"

/*
 * @brief	Function that is responsible for reading a database through a
			binary file.
 * 
 * @param 	filename Name of the file with the database.
 * 
 * @return	Array with all the elements of the database, NULL-terminated.
*/
Element **read_database(char *filename);

/*
 * @brief	Function that deletes a database (NULL-terminated).
 * 
 * @param	database Database to delete.
 * 
 * @return	NULL pointer.
*/
void    *free_database(Element **database);

/*
 * @brief	Function that prints all the information of a database.
 * 
 * @param	database The database with the info.
*/
void    printDatabase(Element **database);

/*
 * @brief	Function that obtains the number of blocks that a database has.
 * 
 * @param	database The database.
 * 
 * @return	The number of elements in the database, or 0 if the database is NULL.
*/
size_t	databaseLength(Element **database);

/*
 * @brief	Function that adds a block to the database.
 * 
 * @param	database Database to add the item to.
 * 
 * @return	A pointer to the database, or NULL if there is a memory problem.
*/
Element	**addDatabaseElement(Element **database, Element *element);

/*
 * @brief	Function that saves the database information in a file.
 * 
 * @param	database Database from which to extract information.
 * @param	filename Name of the file where the information will be saved.
*/
void    save_database(Element **database, char *filename);

#endif
