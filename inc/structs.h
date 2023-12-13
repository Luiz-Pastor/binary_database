#ifndef STRUCTS_H
# define STRUCTS_H

#include <stdio.h>
# include <stdlib.h>

# define ISBN_LENGTH 16
# define MAX_LENGTH 128

/* Estructura de los índices */
typedef struct {
	int			key;    /* Book bookID */
	long int	offset; /*Book is stored in disk at this position */
	size_t		size;   /* Book record size. This is a redundant field that helps in the implementation */
} indexbook;

/* Estructura de los elementos */
typedef struct {
	int			using;
	indexbook	index;
	char		isbn[ISBN_LENGTH + 1];
	char		*title;        /* Tamaño variable, máximo de `MAX_LENGTH`*/
	char		*printedBy;    /* Tamaño variable, máximo de `MAX_LENGTH`*/
} Element;

/* Estructura de la base de datos */
typedef struct {
	int		size;		/* Tamaño máximo de los arrays */
	int		type;		/* Tipo de inserción */
	Element	**elements;	/* Elementos de la bsae de dats */
	Element **deleted;	/* Elementos borrados */
} Database;

/* Tipo de inserción en la base de datos */
enum {
	BEST_FIT,
	WORST_FIT,
	FIRST_FIT
};

/* Posibles errores de algunas funciones en específico */
enum {
	OK = 0,
	REPEATED_ELEMENT,
	MEMORY_ERROR,
	BAD_FORMAT,
	ERROR
};

#endif
