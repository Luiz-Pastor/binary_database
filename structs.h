#ifndef STRUCTS_H
# define STRUCTS_H

#include <stdio.h>
# include <stdlib.h>

# define ISBN_LENGTH 16
# define MAX_LENGTH 128

typedef struct {
	int			key;    /* Book bookID */
	long int	offset; /*Book is stored in disk at this position */
	size_t		size;   /* Book record size. This is a redundant field that helps in the implementation */
} indexbook;


typedef struct {
	int			using;
	indexbook	index;
	char		isbn[ISBN_LENGTH + 1];
	char		*title;        /* Tamaño variable, máximo de `MAX_LENGTH`*/
	char		*printedBy;    /* Tamaño variable, máximo de `MAX_LENGTH`*/
} Element;

typedef struct {
	int		size;
	int		type;		/* Tipo de inserción */
	Element	**elements;
} Database;

enum {
	FIRST_FIT,
	WORST_FIT,
	BEST_FIT
};

enum {
	OK = 0,
	REPEATED_ELEMENT,
	MEMORY_ERROR,
	BAD_FORMAT,
	ERROR
};

#endif
