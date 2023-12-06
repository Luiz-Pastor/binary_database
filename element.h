#ifndef ELEMENT_H
# define ELEMENT_H

# include "structs.h"

Element *createElement();
void	cleanElement(Element *element);
void	deleteElement(Element *element);
void	printElement(Element *element);

#endif
