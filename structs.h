#ifndef STRUCTS_H
# define STRUCTS_H

#include <stdio.h>
# include <stdlib.h>

# define ISBN_LENGTH 16
# define MAX_LENGTH 128

typedef struct{
    int         key;    /* Book bookID */
    long int    offset; /*Book is stored in disk at this position */
    size_t      size;   /* Book record size. This is a redundant field that helps in the implementation */
} indexbook;


typedef struct{
    indexbook   index;
    char        isbn[ISBN_LENGTH + 1];
    char        *title;        /* Tamaño variable, máximo de `MAX_LENGTH`*/
    char        *printedBy;    /* Tamaño variable, máximo de `MAX_LENGTH`*/
} Element;

#endif
