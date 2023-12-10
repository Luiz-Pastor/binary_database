#include <stdio.h>
#include <stdlib.h>
#include "../inc/element.h"

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
	int i;

	if (!element)
		return ;

	element->using = 0;
	element->index.key = 0;

	for (i = 0; i < 16; i++)
		element->isbn[i] = '\0';

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

Element	**copyElements(Element **elements)
{
	Element **copy;
	int		index;

	if (!elements)
		return NULL;

	index = 0;
	while (elements[index])
		index++;

	copy = malloc((index + 1) * sizeof(Element *));
	if (!copy)
		return NULL;

	index = 0;
	while (elements[index])
	{
		copy[index] = elements[index];
		index++;
	}
	copy[index] = NULL;
	return (copy);
}

void	shortElements(Element **elements)
{
	int		i, j, index;
	Element	*aux;

	if (!elements)
		return ;

	i = 0;
	while (elements[i])
	{
		index = i;
		j = i + 1;
		while (elements[j])
		{
			if (elements[index]->index.key > elements[j]->index.key)
				index = j;
			j++;
		}

		if (i != index)
		{
			aux = elements[i];
			elements[i] = elements[index];
			elements[index] = aux;
		}
		i++;
	}
}

/* ================================================================================ */
/* ================================================================================ */
/* ================================================================================ */

static void	shortMode(Element **deleted, int mode)
{
	int		i, j, index;
	Element	*aux;

	i = 0;
	while (deleted[i])
	{
		index = i;
		j = i + 1;
		while (deleted[j])
		{
			if (mode == BEST_FIT && deleted[index]->index.size > deleted[j]->index.size)
				index = j;
			else if (mode == WORST_FIT && deleted[index]->index.size < deleted[j]->index.size)
				index = j;
			j++;
		}
			if (i != index)
		{
			aux = deleted[i];
			deleted[i] = deleted[index];
			deleted[index] = aux;
		}
		i++;
	}
}

void	shortElementsDeleted(Element **deleted, int type)
{
	if (!deleted || type == FIRST_FIT)
		return ;

	shortMode(deleted, type);
}