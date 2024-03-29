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

/*
int Fifo_init(Fifo *fifo, unsigned int capacity, FifoMode mode) :

Initialise une file vide permettant de stocker capacity éléments au plus. Ces derniers seront composés ou agrégés selon le mode spécifié.
*/

// WARNING : allocation dynamique malloc -> ne pas oublier de libérer
// fifo->capacity = capacitySaisieUtilisateur + 1
int IMPLEMENT(Fifo_init)(Fifo *fifo, unsigned int capacity, FifoMode mode)
{
	if (capacity == 0)
	{
		return 1;
	}
	fifo->head = 0;
	fifo->tail = 0;
	fifo->capacity = capacity + 1;
	fifo->mode = mode;
	fifo->storage = malloc(fifo->capacity * sizeof(char *)); // MALLOC : NE PAS OUBLIER DE FREE
	if (fifo->storage == NULL)
	{
		return 1;
	}
	return 0;
	// return provided_Fifo_init(fifo, capacity, mode);
}

/*
void Fifo_finalize(Fifo *fifo) :

Libère toutes les ressources allouées par la file fifo.
*/

void IMPLEMENT(Fifo_finalize)(Fifo *fifo)
{
	Fifo_clear(fifo);	 // On vide fifo
	free(fifo->storage); // On libère fifo->storage
						 // provided_Fifo_finalize(fifo);
}

/*
void Fifo_clear(Fifo *fifo) :

Vide fifo.
*/

void IMPLEMENT(Fifo_clear)(Fifo *fifo)
{
	while (!Fifo_empty(fifo)) // Tant que la fifo n'est pas vide
		Fifo_pop(fifo);		  // On retire de la file
							  // provided_Fifo_clear(fifo);
}

/*
int Fifo_push(Fifo *fifo, const char *str) :

Rajoute la chaîne de caractères str à la fin de la file (attention au mode choisi !) et retourne un code d'erreur (0 en cas de succès).
*/

// On ajoute au niveau de tail et on incrémente tail
// Aggrégation (mode AGGREGATE) : Contenu dépendant du conteneur : si on détruit le conteneur, on détruit le contenu (mettre str au niveau de tail == passage par référence)
// Composition (mode COMPOSE) : Contenu indépendant du conteneur : si on détruit le conteneur, le contenu existe toujours (créer une copie de str à mettre au niveau de tail == passage par valeur)
int IMPLEMENT(Fifo_push)(Fifo *fifo, const char *str)
{
	// Si la file est pleine -> ERREUR
	if (Fifo_full(fifo)) // Si fifo est plein
		return 1;
	// Si on est en mode AGGREGATE
	if (fifo->mode == AGGREGATE)
	{
		fifo->storage[fifo->tail] = (char *)str; // On met str à la position tail // Storage est un tableau de pointeurs de chaine de caractères et pas un tableau de caractères
	}
	// Si on est en mode COMPOSE
	else
	{
		fifo->storage[fifo->tail] = duplicateString(str); // On met str à la position tail // MALLOC : NE PAS OUBLIER DE FREE (dans finalize ou pop)
	}
	// tail + 1 si tail = capacity alors tail = 0
	fifo->tail = (fifo->tail + 1) % fifo->capacity; // On incrémente tail
	return 0;
	// return provided_Fifo_push(fifo, str);
}

/*
const char* Fifo_front(const Fifo *fifo) :

Retourne un pointeur constant sur le plus vieil élément de la file ou NULL si celle-ci est vide.
*/

const char *IMPLEMENT(Fifo_front)(const Fifo *fifo)
{
	if (Fifo_empty(fifo)) // Si fifo est vide
	{
		return NULL;
	}
	else
	{
		const char *pt = &fifo->storage[fifo->head][0]; // On retourne le pointeur sur head
		return pt;
	}
	// return provided_Fifo_front(fifo);
}

/*
int Fifo_pop(Fifo *fifo) :

Supprime le plus vieil élément de fifo et retourne un code d'erreur.
*/

// On enlève au niveau de head et on incrémente head
int IMPLEMENT(Fifo_pop)(Fifo *fifo)
{
	if (Fifo_empty(fifo)) // Si fifo est vide
	{
		return 1;
	}
	if (&fifo->storage[fifo->head] != NULL && fifo->mode == COMPOSE)
	{
		free(fifo->storage[fifo->head]); // On libère le caractère à head
	}
	fifo->head = (fifo->head + 1) % fifo->capacity; // On incrémente head
	return 0;
	// return provided_Fifo_pop(fifo);
}

/*
int Fifo_full(const Fifo *fifo) :

Retourne vrai si fifo est pleine et faux sinon.
*/

// On va parcourir la file de head à tail donc pas possible d'incrémenter simplement car tableau circulaire (revenir au début une fois à la fin

// File pleine : Nombre de cases allouées = fifo->capacity - 1
int IMPLEMENT(Fifo_full)(const Fifo *fifo)
{
	if (fifo->head == 0)
	{
		return (fifo->tail == fifo->capacity - 1);
	}
	else
	{
		return (fifo->tail == fifo->head - 1);
	}
	/* Correction Prof : Plein si un ajout ramenerait au cas vide
	return fifo->head == (fifo->tail+1)%fifo->capacity;
	*/
	// return provided_Fifo_full(fifo);
}

/*
int Fifo_empty(const Fifo *fifo) :

Retourne vrai (1) si fifo est vide et faux (0) sinon.
*/

// Si tail vaut head
int IMPLEMENT(Fifo_empty)(const Fifo *fifo)
{
	return (fifo->head == fifo->tail);
	// return provided_Fifo_empty(fifo);
}
