#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "element.h"
#include "database.h"

/* =========================================*/

/*
 * @bried	Function that obtains the length of a number (number of digits). Does
 			not give information about the signs.
 *
 * @param	number Number whose length is to be looked at.
 * 
 * @return	The number of digits of the number.
*/
static int  number_length(int number)
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
static void	*error_reading(Element *element, Database *database, FILE **file)
{
	if (element)
		deleteElement(element);
	if (database)
        free_database(database);
    if (file)
	    fclose(*file);
	return (NULL);
}

/* =========================================*/

static Database *initDatabase()
{
    Database *database;

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
    return (database);
}


/* Function that is responsible for reading a database through a binary file */
Database *read_database(char *filename)
{
    FILE    *file;				/* Archivo del que leer */
    Element *current;			/* Elemento que se crea */
    /*Element **memory;*/			/* Puntero auxiliar para realloc */
    Database    *database;
    /*int     database_size = 1;*/	/* Tamaño de la base de datos + 1 (NULL)*/

    /* Variables auxiliares */
    int     i;					/* Iterador del bucle*/
	int		readed;				/* Caracteres leidos. Usado para ver si falla la lectura*/
    int     offset = 0;			/* Offset */
    char    letter[2];			/* Usado para leer poco a poco el titulo */
    char    text[MAX_LENGTH];	/* Almacenamiento temporal del titulo. Optimización de espacio */
    int     count;				/* Espacio para el campo `printedBy`*/

	/* Abrimos el archivo sobre el que vamos a leer */
    file = fopen(filename, "rb");
    if (!file)
        return (NULL);

    database = initDatabase();
    if (!database)
    {
        fclose(file);
        return (NULL);
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

        /* Fijamos el offset */
        current->index.offset = offset;

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

        /* Calculamos el offset del siguiente elemento */
        offset += current->index.size +  strlen(current->printedBy) + 1;

        /* Añadimos un elemento a la base de datos y lo metemos, añadiendo un NULL al final para iterar */
        /*memory = (Element **) realloc(elements, (database_size + 1) * sizeof(Element *));
        if (!memory)
			return (error_reading(current, elements, &file));
		elements = memory;
        elements[database_size - 1] = current;
    	elements[database_size] = NULL;
        database_size++;*/
        database = addDatabaseElement(database, current);
        if (!database)
            return (error_reading(current, NULL, &file));
    }

    fclose(file);
    return (database);
}

/* Function that deletes a database (NULL-terminated) */
void    *free_database(Database *database)
{
    int index = 0;

	if (!database)
		return (NULL);
    while (database->elements && database->elements[index])
        deleteElement(database->elements[index++]);
    free(database->elements);
    free(database);
    return (NULL);
}

/* Function that prints all the information of a database */
void    printDatabase(Database *database)
{
    int index = 0;

	printf("\n");
    while (database->elements[index])
    {
        printElement(database->elements[index++]);
        printf("\n\n");
    }
}

/* Function that obtains the number of blocks that a database has */
size_t	databaseLength(Database *database)
{
	size_t	size = 0;

	if (!database || !database->elements)
		return (0);
	while (database->elements[size])
		size++;
	return size;
}

/* Function that adds a block to the database */
Database    *addDatabaseElement(Database *database, Element *element)
{
	Element	**memory;
	size_t	size;

    if (!database || !element)
		return (NULL);
	size = databaseLength(database);
    if (size == database->size)
    {
        memory = realloc(database->elements, (database->size * database->size + 1) * sizeof(Database));
        if (!memory)
            return (free_database(database));
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
    
	/*memory = (Element **) realloc(database, size + 2);
	if (!memory)
		return (NULL);
	database = memory;
	database[size + 1] = element;
	database[size + 2] = NULL;
	element->index.offset = database[size]->index.size +  strlen(database[size]->printedBy) + 1;*/
	return database;
}

/* Function that saves the database information in a file */
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
