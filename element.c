#include <stdio.h>
#include <stdlib.h>
#include "element.h"

/*
 * Function that created an empty element
*/
Element *createElement()
{
	int		i;
	Element	*element;

	element = (Element*) malloc(sizeof(Element));
	if (!element)
		return (NULL);

	element->using = 1;

	element->title = NULL;
	element->printedBy = NULL;

	element->index.key = 0;
	element->index.offset = 0;
	element->index.size = 0;

	for (i = 0; i < ISBN_LENGTH + 1; i++)
		element->isbn[i] = '\0';

	return (element);
}

/*
 * Function that cleans the information of an element
*/
void	cleanElement(Element *element)
{
	if (!element)
		return ;

	element->using = 0;

	element->index.key = 0;

	if (element->title)
		free(element->title);
	element->title = NULL;
	if (element->printedBy)
		free(element->printedBy);
	element->printedBy = NULL;
}

/*
 * Function that deletes an element
*/
void	deleteElement(Element *element)
{
	if (!element)
		return;

	cleanElement(element);
	free(element);
}

/*
 * Function that prints the information of an element
*/
void	printElement(Element *element)
{
	printf("Tamano del bloque: %ld (offset: %ld)\n", element->index.size, element->index.offset);
	printf("\tBookID:\t\t%d\n", element->index.key);
	printf("\tIDBN:\t\t%s\n", element->isbn);
	printf("\tTitle:\t\t%s\n", element->title);
	printf("\tEditorial:\t%s\n", element->printedBy);
}
