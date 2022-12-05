/*-------------------------------------------------------------------------*
 | Copyright (C) 2018 Département Informatique de PolyTech Tours.          |
 |                                                                         |
 | This file is part of PolyShell, yet another shell.                      |
 |                                                                         |
 | PolyShell is free software; you can redistribute it and/or modify       |
 | it under the terms of the GNU General Public License as published by    |
 | the Free Software Foundation; either version 3 of the License,          |
 | or (at your option) any later version.                                  |
 |                                                                         |
 | PolyShell is distributed in the hope that it will be useful,            |
 | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
 | GNU General Public License for more details.                            |
 |                                                                         |
 | You should have received a copy of the GNU General Public License       |
 | along with this program. If not, see <http://www.gnu.org/licenses/>.    |
 |                                                                         |
 | Additional permission under GNU GPL version 3 section 7 ---- If you     |
 | modify PolyShell, or any covered work, by linking or combining it with  |
 | libprovided (or a modified version of that library), containing parts   |
 | covered by the terms of the Creative Commons BY-NC-ND 4.0 International |
 | License, the licensors of  PolyShell grant you additional permission    |
 | to convey the resulting work.                                           |
 *-------------------------------------------------------------------------*/

#include "misc/fifo.h"
#include <stdlib.h>
#include "misc/string.h"
// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_2(Fifo, unsigned int, FifoMode)
MAKE_DEL_0(Fifo)

//WARNING : allocation dynamique malloc -> ne pas oublier de libérer
//fifo->capacity = capacitySaisieUtilisateur + 1
int IMPLEMENT(Fifo_init)(Fifo *fifo, unsigned int capacity, FifoMode mode)
{
    if (capacity == 0) {
		return 1;
	}
	fifo->head = 0;
	fifo->tail = 0;
	fifo->capacity = capacity + 1;
	fifo->mode = mode;
	fifo->storage = malloc(fifo->capacity * sizeof(char*)); //MALLOC : NE PAS OUBLIER DE FREE
	if(fifo->storage == NULL){
		return 1;
	}
	return 0;
	//return provided_Fifo_init(fifo, capacity, mode);
}

void IMPLEMENT(Fifo_finalize)(Fifo *fifo)
{
    if (fifo->storage != NULL){
		free(fifo->storage); //FREE
	}
	//provided_Fifo_finalize(fifo);
}

void IMPLEMENT(Fifo_clear)(Fifo *fifo)
{
    provided_Fifo_clear(fifo);
}

// On ajoute au niveau de tail et on incrémente tail
//Aggrégation (mode AGGREGATE) : Contenu dépendant du conteneur : si on détruit le conteneur, on détruit le contenu (mettre str au niveau de tail == passage par référence)
//Composition (mode COMPOSE) : Contenu indépendant du conteneur : si on détruit le conteneur, le contenu existe toujours (créer une copie de str à mettre au niveau de tail == passage par valeur)
int IMPLEMENT(Fifo_push)(Fifo *fifo, const char *str)
{
	//Si la file est pleine -> ERREUR
    if (Fifo_full(fifo)) 
		return 1;
	//Si on est en mode AGGREGATE
	if (fifo->mode == AGGREGATE) {
		fifo->storage[fifo->tail] = (char*)str; //Storage est un tableau de pointeurs de chaine de caractères et pas un tableau de caractères
	}
	//Si on est en mode COMPOSE
	else {
		char* copy_str = duplicateString(str); //MALLOC : NE PAS OUBLIER DE FREE (dans finalize ou pop)
		fifo->storage[fifo->tail] = copy_str;
	}
	//tail + 1 si tail = capacity alors tail = 0
	fifo->tail = (fifo->tail+1) % fifo->capacity;
	return 0;
	//return provided_Fifo_push(fifo, str);
}

const char* IMPLEMENT(Fifo_front)(const Fifo *fifo)
{
    return provided_Fifo_front(fifo);
}

//On enlève au niveau de head et on incrémente head
int IMPLEMENT(Fifo_pop)(Fifo *fifo)
{
    return provided_Fifo_pop(fifo);
}

//On va parcourir la file de head à tail donc pas possible d'incrémenter simplement car tableau circulaire (revenir au début une fois à la fin

//File pleine : Nombre de cases allouées = fifo->capacity - 1
int IMPLEMENT(Fifo_full)(const Fifo *fifo)
{
	if (fifo->head == 0){
		return (fifo->tail == fifo->capacity - 1);
	}
	else{
		return (fifo->tail == fifo->head - 1);
	}
	/* Correction Prof : Plein si un ajout ramenerait au cas vide
	return fifo->head == (fifo->tail+1)%fifo->capacity;
	*/
	//return provided_Fifo_full(fifo);
}

//Si tail vaut head
int IMPLEMENT(Fifo_empty)(const Fifo *fifo)
{
    return (fifo->head == fifo->tail);
	//return provided_Fifo_empty(fifo);
}
