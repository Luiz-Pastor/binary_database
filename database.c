#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "element.h"
#include "database.h"

/* =================================================================================== */
/* =================================================================================== */
/* =================================================================================== */

/*
 * @bried	Function that obtains the length of a number (number of digits). Does
 			not give information about the signs.
 *
 * @param	number Number whose length is to be looked at.
 * 
 * @return	The number of digits of the number.
*/
static int      number_length(int number)
{
	int	count = 0;

	if (number == 0)
		return (1);
	while (number != 0)
	{
		count++;
		number /= 10;
	}
	return (count);
}

/*
 * @bried	Function that is responsible for eliminating all memory and closing
 			the reading file if there is any problem in the process of reading
			the database.
 *
 * @param	element Block that was being created to add to the database.
 * @param	database Database that was being created with the file information.
 * #param	file Pointer to the file from which the information was being read.
 * 
 * @return	NULL.
*/
static void     *error_reading(Element *element, Database *database, FILE **file)
{
	if (element)
		deleteElement(element);
	if (database)
		free_database(database);
	if (file)
		fclose(*file);
	return (NULL);
}

/*
 * @brief	Function to initialize a empty database
 *
 * @param	insertion Way to insert elements into the database.
 * 
 * @return	An empty database, or NULL if there is a memory problem
*/
static Database *initDatabase(char *insertion)
{
	Database	*database;

	database = malloc(sizeof(Database));
	if (!database)
		return (NULL);
	database->size = 2;
	database->elements = calloc(3, sizeof(Element*));
	if (!database->elements)
	{
		free(database);
		return (NULL);
	}

	/* Ponemos el tipo de inserción a la hora de meter datos */
	if (!strcmp(insertion, "first_fit"))
		database->type = FIRST_FIT;
	else if (!strcmp(insertion, "worst_fit"))
		database->type = WORST_FIT;
	else
		database->type = BEST_FIT;

	return (database);
}

/*
 * Function that is responsible for reading a database through a binary file
*/
Database        *read_database(char *insertion, char *filename)
{
	FILE		*file;				/* Archivo del que leer */
	Element		*current;			/* Elemento que se crea */
	Database	*database;

	/* Variables auxiliares */
	int			i;					/* Iterador del bucle*/
	int			readed;				/* Caracteres leidos. Usado para ver si falla la lectura*/
	char		letter[2];			/* Usado para leer poco a poco el titulo */
	char		text[MAX_LENGTH];	/* Almacenamiento temporal del titulo. Optimización de espacio */
	int			count;				/* Espacio para el campo `printedBy`*/
	int			exists;             /* El elemento existe */

	database = initDatabase(insertion);
	if (!database)
		return (NULL);

	/* Abrimos el archivo sobre el que vamos a leer */
	file = fopen(filename, "rb");
	if (!file)
	{
		file = fopen(filename, "w");
		fclose(file);
		return (database);
	}

	/* Vamos guardando cada elemento */
	while (!feof(file))
	{
		/* Reiniciamos la variable donde pondremos texto variable */
		for (i = 0; i < MAX_LENGTH; i++)
			text[i] = '\0';

		/* Creamos el nodo del elemento */
		current = createElement();
		if (!current)
			return (error_reading(current, database, &file));

		/* Guardamos el tamaño del bloque */
		readed = fread(&(current->index.size), sizeof(size_t), 1, file);
		if (feof(file))
		{
			deleteElement(current);
			break;
		}
		if (!readed)
			return (error_reading(current, database, &file));

		/* Guardamos el bookId */
		readed = fread(&(current->index.key), sizeof(int), 1, file);
		if (!readed)
			return (error_reading(current, database, &file));

		/* Guardamos el ISBN */
		readed = fread(current->isbn, sizeof(char), ISBN_LENGTH, file);
		if (!readed)
			return (error_reading(current, database, &file));
		current->isbn[ISBN_LENGTH] = '\0';

		/*
			Guardamos todo en una variable auxiliar, hasta encontrar un '|' (fin del titulo).
			Después, reservamos el espacio necesario para almacenarlo en el nombre
			del titulo. Así, no desperdiciamos espacio/memoria (optimización).
		*/
		do{
			readed = fread(letter, sizeof(char), 1, file);
			if (!readed)
				return (error_reading(current, database, &file));
			letter[1] = '\0';
			if (letter[0] != '|')
				strcat(text, letter);
		} while (letter[0] != '|');

		current->title = calloc(strlen(text) + 1, sizeof(char));
		if (!(current->title))
			return (error_reading(current, database, &file));
		strcpy(current->title, text);
	
		/*
			Calculamos cuantos caracteres nos quedan por leer, y leemos esa cantidad.
			Todo ello forma parte del campo 'printedBy'
		*/
		count = (current->index.size) - number_length(current->index.key) - strlen(current->isbn) - strlen(current->title);
		current->printedBy = calloc(count + 1, sizeof(char));
		if (!(current->printedBy))
			return (error_reading(current, database, &file));
		readed = fread(current->printedBy, sizeof(char), count, file);
		if (!readed)
			return (error_reading(current, database, &file));

		/* Añadimos un elemento a la base de datos y lo metemos, añadiendo un NULL al final para iterar */
		exists = addDatabaseElement(database, current);
		if (exists == REPEATED_ELEMENT)
		{
			printf("There is some elements with the same bookID!\n");
			return (error_reading(current, NULL, &file));
		}
		else if (exists == MEMORY_ERROR)
		{
			printf("Memory Error\n");
			exit(0);
		}
	}
	fclose(file);
	return (database);
}

/* =================================================================================== */
/* =================================================================================== */
/* =================================================================================== */

/*
 * Function that deletes a database (NULL-terminated)
*/
void    *free_database(Database *database)
{
	int	index = 0;

	if (!database)
		return (NULL);
	while (database->elements && database->elements[index])
		deleteElement(database->elements[index++]);
	free(database->elements);
	free(database);
	return (NULL);
}

/* =================================================================================== */
/* =================================================================================== */
/* =================================================================================== */

/*
 * Function that prints all the information of a database
*/
void    printDatabase(Database *database)
{
	int	index = 0;

	printf("\n");
	printf("Método de inserción: \n");
	if (database->type == FIRST_FIT)
		printf("FIRST_FIT\n");
	else if (database->type == WORST_FIT)
		printf("WORST_FIT\n");
	else
		printf("BEST_FIT\n");


	while (database->elements[index])
	{
		printElement(database->elements[index++]);
		printf("\n\n");
	}
}

/* =================================================================================== */
/* =================================================================================== */
/* =================================================================================== */

/*
 * Function that obtains the number of blocks that a database has
*/
size_t  databaseLength(Database *database)
{
	size_t	size = 0;

	if (!database || !database->elements)
		return (0);
	while (database->elements[size])
		size++;
	return size;
}

/* =================================================================================== */
/* =================================================================================== */
/* =================================================================================== */

/*
 * @brief	Function that sets the offset corresponding to each element.
 * 
 * @param	database The database to change.
*/
static void setOffset(Database *database)
{
	int	i = 0, offset = 0;

	if (!database)
		return ;
	while (database->elements[i])
	{
		database->elements[i]->index.offset = offset;
		offset += database->elements[i]->index.size + strlen(database->elements[i]->printedBy) + 1;
		i++;
	}
}

/*
 * @brief	Function to sort a database.
 * 
 * @param	database Database to sort.
*/
static void shortDatabase(Database *database)
{
	int		i, j, min, length;
	Element	*aux;

	if (!database)
		return ;
	
	length = databaseLength(database);
	for (i = 0; i < length; i++)
	{
		min = i;
		for (j = i + 1; j < length; j++)
		{
			if (database->elements[j]->index.key < database->elements[min]->index.key)
				min = j;
		}
		if (min != i)
		{
			aux = database->elements[i];
			database->elements[i] = database->elements[min];
			database->elements[min] = aux;
		}
	}
	setOffset(database);
}

/*
 * Function that adds a block to the database
*/
int         addDatabaseElement(Database *database, Element *element)
{
	Element	**memory;
	size_t	size;

	if (!database || !element)
		return (MEMORY_ERROR);

	if (findDatabaseElement(database, element->index.key))
	{
		return (REPEATED_ELEMENT);
	}

	size = databaseLength(database);
	if (size == database->size)
	{
		memory = realloc(database->elements, (database->size * database->size + 1) * sizeof(Database));
		if (!memory)
		{
			printf("Memory error!\n");
			exit(MEMORY_ERROR);
		}
		database->elements = memory;
		database->size *= database->size;
		database->elements[size++] = element;
		while (size < database->size)
			database->elements[size++] = NULL;
	}
	else
	{
		database->elements[size] = element;
		database->elements[size + 1] = NULL;
	}

	/* Ordenamod la base de datos , y fijamos nuevos offsets */
	shortDatabase(database);

	return OK;
}

/* =================================================================================== */
/* =================================================================================== */
/* =================================================================================== */

/*
 * Function that saves the database information in a file
*/
void    save_database(Database *database, char *filename)
{
	FILE	*file;
	Element	*current;
	size_t	index = 0;

	if (!database)
		return ;
	
	file = fopen(filename, "w");
	if (!file)
		return ;

	while (database->elements[index])
	{
		if (!database->elements[index]->using)
		{
			index++;
			continue ;
		}
		current = database->elements[index];
		fwrite(&(current->index.size),	sizeof(size_t),	1,							file);
		fwrite(&(current->index.key),	sizeof(int),	1,							file);
		fwrite(&(current->isbn),		sizeof(char),	ISBN_LENGTH,				file);
		fwrite(current->title,			sizeof(char),	strlen(current->title),		file);
		fwrite("|",						sizeof(char),	1,							file);
		fwrite(current->printedBy,		sizeof(char),	strlen(current->printedBy),	file);
		index++;
	}
	fclose(file);
}

/* =================================================================================== */
/* =================================================================================== */
/* =================================================================================== */

/*
 * Function that returns the last element of the database
*/
Element *getLastElement(Database *database)
{
	int	index = 0;

	if (!database || !database->elements)
		return NULL;
	while (database->elements[index])
		index++;
	if (index - 1 < 0)
		return NULL;
	return database->elements[index - 1];
}

/* =================================================================================== */
/* =================================================================================== */
/* =================================================================================== */

/*
 * @brief	Function finds an element on a database, recursively.
 *          Apply the binary search algorithm.
 * 
 * @param	database Database to apply the search to.
 * @param	start Index of the first item in the database.
 * @param   end Index of the last item in the database
 * @param   key ID to search
 * 
 * @return	The element, or NULL if it does not exist.
*/
static Element* find_recursive(Database *database, int start, int end, int key)
{
	int	mid;

	if (start > end)
		return NULL;

	if (start == end)
	{
		if (database->elements[start]->index.key == key)
			return (database->elements[start]);
		else
			return NULL;
	}

	mid = (end + start) / 2;
	if (database->elements[mid]->index.key == key)
		return database->elements[mid];

	if (database->elements[mid]->index.key < key)
		return find_recursive(database, mid + 1, end, key);
	else
		return find_recursive(database, start, mid, key);
	return NULL;
}

/*
 * Function that find a register with a specific bookID
*/
Element         *findDatabaseElement(Database *database, int key)
{
	if (!database)
		return (NULL);

	return find_recursive(database, 0, databaseLength(database) - 1, key);
}

/* =================================================================================== */