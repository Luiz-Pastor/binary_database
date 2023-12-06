#ifndef ELEMENT_H
# define ELEMENT_H

# include "structs.h"

/*
 * @brief	Function that created an empty element
 * 
 * @return	The empty element created, or NULL if there is a memory problem
*/
Element	*createElement();

/*
 * @brief	Function that performs all the information about an element,
 *			except its size and offset
 * 
 * @param	element Register to clean
*/
void	cleanElement(Element *element);

/*
 * @brief	Function that delete an element.
 * 
 * @param	element Element to delete/free
*/
void	deleteElement(Element *element);

/*
 * @brief	Function that prints all the information of an element.
 * 
 * @param	element Element to print
*/
void	printElement(Element *element);

#endif
