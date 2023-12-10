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

	/* Reservamos memoria para la estructura principal */
	database = malloc(sizeof(Database));
	if (!database)
		return (NULL);

	/* Reservamos memoria para la tabla de elementos y para la de elementos eliminados */
	database->elements = calloc(3, sizeof(Element *));
	database->deleted = calloc(3, sizeof(Element *));
	if (!database->elements || !database->deleted)
	{
		free_database(database);
		return (NULL);
	}
	
	/* Fijamos el espacio inicial del array de elementos */
	database->size = 2;
	
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
		/* offset += database->elements[i]->index.size + strlen(database->elements[i]->printedBy) + 1; */
		offset += database->elements[i]->index.size + 8;
		i++;
	}
}

/*
 * Function that is responsible for reading a database through a binary file
*/
Database        *read_database(char *insertion, char *filename)
{
	char		*path;
	FILE		*file;				/* Archivo del que leer */
	Element		*current;			/* Elemento que se crea */
	Database	*database;			/* Base de datos */

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

	path = calloc(strlen(filename) + 4, sizeof(char));
	if (!path)
	{
		free_database(database);
		return (NULL);
	}

	sprintf(path, "%s.db", filename);

	/* Abrimos el archivo sobre el que vamos a leer */
	file = fopen(path, "rb");
	if (!file)
	{
		/* No existe el archivo; se crea un archivo para poder escribir después */
		file = fopen(path, "w");
		fclose(file);
		free(path);
		return (database);
	}
	free(path);

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
			return (error_reading(current, NULL, &file));
		}
	}
	setOffset(database);
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

	/* Eliminamos todos los registros guardados, tanto los guardados como los no guardados */
	while (database->elements && database->elements[index])
		deleteElement(database->elements[index++]);

	/* Eliminamos los punteros en los que habíamos reservado lso arrays */
	free(database->elements);
	free(database->deleted);
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

size_t	databaseDeletedLength(Database *database)
{
	size_t	size = 0;

	if (!database || !database->deleted)
		return (0);
	while (database->deleted[size])
		size++;
	return size;
}

/* =================================================================================== */
/* =================================================================================== */
/* =================================================================================== */

static Database	*expand_database(Database *database, int count)
{
	size_t		size_elements, size_deleted;
	Element		**memory;

	if (!database)
		return NULL;

	size_elements = databaseLength(database);
	size_deleted = databaseDeletedLength(database);
	if ((size_elements + count) >= database->size || (size_deleted + count) >= database->size)
	{
		memory = realloc(database->elements, (database->size * database->size + 1) * sizeof(Element*));
		if (!memory)
		{
			free_database(database);
			return NULL;
		}
		database->elements = memory;
		memory = realloc(database->deleted, (database->size * database->size + 1) * sizeof(Element*));
		if (!memory)
		{
			free_database(database);
			return NULL;
		}
		database->deleted = memory;

		database->size *= database->size;
	}
	return database;
}

static Database	*replace_empty_element(int index, Element *element, Database *database)
{
	int	new_size, i;
	Element	*empty, *new_empty, *aux;

	/* Miramos si el elemento que hay que sustituir es NULL; en ese caso se trata del final de la base de datos */
	empty = database->elements[index];
	if (empty == NULL)
	{
		/* Se inserta el elemento al final de la base de datos. Expandimos la tabla un elemento */
		database = expand_database(database, 1);
		if (!database)
			return NULL;

		/* Ponemos el elemento en ultima posicion, y detras de él un NULL */
		database->elements[index] = element;
		database->elements[index + 1] = NULL;

		if (index == 0)
			element->index.offset = 0;
		else
			element->index.offset = database->elements[index - 1]->index.offset + database->elements[index - 1]->index.size + 8;
			/* element->index.offset = database->elements[index - 1]->index.offset + database->elements[index - 1]->index.size + strlen(database->elements[index - 1]->printedBy) + 1; */

		return database;
	}

	/* Calculamos el espacio que va a tener el elemento sobrante que no se va a usar */
	new_size = database->elements[index]->index.size - element->index.size;
	if (new_size == 0)
	{		
		/* El elemento coincide justo con el espacio en blanco. Ponemos el elemento a meter */
		database->elements[index] = element;

		/* Cambiamos el offset al que tenia el elemento vacío */
		element->index.offset = empty->index.offset;

		/* Buscar el elemento en el array de eliminados */
		i = 0;
		while (database->deleted[i])
		{
			if (database->deleted[i] == empty)
				break ;
			i++;
		}

		/* Movemos toda el array de eliminados una posición a la izquierda, a partir del elemento eliminado */
		database->deleted[i] = NULL;
		while (database->deleted[i + 1])
		{
			database->deleted[i] = database->deleted[i + 1];
			database->deleted[i + 1] = NULL;
			i++;
		}

		/* Eliminamos el elemento */
		deleteElement(empty);
	}
	else
	{
		/* Va a haber sobrante sin usar. Expandimos la tabla un elemento*/
		database = expand_database(database, 1);
		if (!database)
			return NULL;
		
		/* Ponemos el offset al elemento a añadir */
		element->index.offset = database->elements[index]->index.offset;

		/* Creamos el bloque con el espacio sobrante */
		new_empty = createElement();
		if (!new_empty)
			return NULL;

		/* Le ponemos tamaño y offset al elemento vacío */
		new_empty->using = 0;
		new_empty->index.size = new_size;
		new_empty->index.offset = element->index.offset + element->index.size + 8;
		/* new_empty->index.offset = database->elements[index]->index.size + strlen(database->elements[index]->printedBy) + 1; */
		/* new_empty->index.offset = database->elements[index]->index.size + strlen(database->elements[index]->printedBy) + 1; */
		
		/* Reemplazamos el elemento vacío por el que hay que insertar */
		database->elements[index] = element;

		/* Buscamos el elemento a eliminar definitivamente del array de eliminados */
		i = 0;
		while (database->deleted[i])
		{
			if (database->deleted[i] == empty)
				break ;
			i++;
		}

		/* Sustituimos este elemento por el nuevo elemento vacío*/
		database->deleted[i] = new_empty;

		/* Eliminamos el elemento */
		deleteElement(empty);

		/* Iteramos moviendo todos los elementos existentes una posición a la derecha, para colocar el vacío sobrante */
		index++;
		while (database->elements[index])
		{
			aux = database->elements[index];
			database->elements[index] = new_empty;
			new_empty = aux;
			index++;
		}
		database->elements[index] = new_empty;
		database->elements[index + 1] = NULL;

	}
	return database;
}

/*
 * Function that adds a block to the database
*/

int         addDatabaseElement(Database *database, Element *element)
{
	int		index, select;
	/*size_t	size;*/
	/* Element	**memory;*/

	if (!database || !element)
		return (MEMORY_ERROR);

	if (findDatabaseElement(database, element->index.key))
		return (REPEATED_ELEMENT);

	if (database->type == FIRST_FIT)
	{
		index = 0;
		while (database->elements[index])
		{
			if (database->elements[index]->using == 0 && element->index.size <= database->elements[index]->index.size)
				break;
			index++;
		}
		database = replace_empty_element(index, element, database);
		if (!database)
			return MEMORY_ERROR;
	}

	else if (database->type == WORST_FIT)
	{
		/* Si la array está vacía, insertamos elemento al principio*/
		if (!database->elements[0])
		{
			database = replace_empty_element(0, element, database);
			if (!database)
				return MEMORY_ERROR;
			return OK;
		}
		
		/* Buscamos el hueco más grande, entre los que están libres */
		select = -1;
		index = 0;
		while (database->elements[index])
		{
			if (database->elements[index]->using == 0 && element->index.size <= database->elements[index]->index.size
			&& (select == -1 || database->elements[select]->index.size < database->elements[index]->index.size))
				select = index;
			index++;
		}

		if (select == -1)
			select = index;

		database = replace_empty_element(select, element, database);
		if (!database)
			return MEMORY_ERROR;
	}

	else if (database->type == BEST_FIT)
	{
		/* Si la array está vacía, insertamos elemento al principio*/
		if (!database->elements[0])
		{
			database = replace_empty_element(0, element, database);
			if (!database)
				return MEMORY_ERROR;
			return OK;
		}
		
		/* Buscamos el hueco más pequeño, entre los que están libres */
		select = -1;
		index = 0;
		while (database->elements[index])
		{
			if (database->elements[index]->using == 0 && (select == -1 || database->elements[select]->index.size > database->elements[index]->index.size))
				select = index;
			index++;
		}

		if (select == -1)
			select = index;

		database = replace_empty_element(select, element, database);
		if (!database)
			return MEMORY_ERROR;
	}

	return OK;
}

/* =================================================================================== */
/* =================================================================================== */
/* =================================================================================== */

/*
 * Function that saves the database information in a file
*/
int    save_database(Database *database, char *filename)
{
	char	*path;
	FILE	*file;
	Element	*current;
	size_t	index = 0;

	if (!database)
		return (1);

	path = calloc(strlen(filename) + 4, sizeof(char));
	if (!path)
		return (1);
	
	sprintf(path, "%s.db", filename);

	file = fopen(path, "w");
	if (!file)
	{
		free(path);
		return (1);
	}
	free(path);

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
	return (0);
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
static Element* find_recursive(Element **elements, int start, int end, int key)
{
	int	mid;

	if (start > end)
		return NULL;

	if (start == end)
	{
		if (elements[start]->index.key == key)
			return (elements[start]);
		else
			return NULL;
	}

	mid = (end + start) / 2;
	if (elements[mid]->index.key == key)
		return elements[mid];

	if (elements[mid]->index.key < key)
		return find_recursive(elements, mid + 1, end, key);
	else
		return find_recursive(elements, start, mid, key);
	return NULL;
}

/*
 * Function that find a register with a specific bookID
*/
Element         *findDatabaseElement(Database *database, int key)
{
	Element	**copy, *result;

	if (!database)
		return (NULL);

	/* La busqueda binaria solo funciona con datos ordenados. Copiamos la tabla y la ordenamos */
	copy = copyElements(database->elements);
	if (!copy)
		return NULL;
	shortElements(copy);

	/* Aplciamos la busqueda binaria */
	result = find_recursive(copy, 0, databaseLength(database) - 1, key);

	/* Eliminamos la memoria auxiliar */
	free(copy);

	return result;
}

/* =================================================================================== */

int				delDatabaseElement(Database *database, Element *element)
{
	int	index;

	if (!database || !element)
		return (MEMORY_ERROR);

	cleanElement(element);

	database = expand_database(database, 1);
	if (!database)
		return (MEMORY_ERROR);

	index = 0;
	while (database->deleted[index])
		index++;
	
	database->deleted[index] = element;
	database->deleted[index + 1] = NULL;

	shortElementsDeleted(database->deleted, database->type);

	return OK;
}
