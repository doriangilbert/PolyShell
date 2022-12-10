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

#include "interactive/history.h"
#include "interactive/autocomplete.h"
#include "misc/string.h"
#include "misc/filesystem.h"

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_2(History, const char *, unsigned int)
MAKE_DEL_1(History, const char *)

// Utiliser les fonctions de fifo

/*
int History_init(History *history, const char *filename, unsigned int length) :

Initialise un historique vide de longueur maximale length, puis le remplit à partir des commandes lues dans le fichier filename (une commande par ligne) (sauf si filename est NULL).
*/

int IMPLEMENT(History_init)(History *history, const char *filename, unsigned int length)
{
	// Init
	if (Fifo_init(&history->storage, length, COMPOSE))
	{ // Fifo_init = MALLOC, Ne pas oublier le & car Fifo_init attend Fifo *
		return 1;
	}
	history->position = history->storage.tail;

	// Lecture du fichier
	if (filename)
	{
		char *path = prependHomeDir(duplicateString(filename)); // Gérer le ~ qui symbolise le répertoire utilisateur (/home/user)
		if (path)
		{
			FILE *fichier = fopen(path, "r");
			if (fichier)
			{
				FileIterator *iterator = FileIterator_new(fichier);
				if (iterator)
				{
					while (!FileIterator_isOver(iterator))
					{
						History_add(history, FileIterator_get(iterator));
						FileIterator_next(iterator);
					}
					FileIterator_delete(iterator);
				}
				fclose(fichier);
			}
			free(path);
		}
	}
	return 0;
	// return provided_History_init(history, filename, length);
}

/*
void History_finalize(History *history, const char *filename) :

Libère les ressources allouées par history. Son contenu devra avoir été sauvegardé dans le fichier filename au préalable (sauf bien sûr si le pointeur fourni est NULL).
*/

// Ecriture : Meme structure que la lecture (voir init, changer uniquement le contenu de if(fichier) ), pas de FileIterator, on utilise fprintf avec un parcours normal
// On veut conserver les commentaires dans l'historique (débute par #) en utilisant getProtString, on n'écrit pas la chaine dans le fichier mais getProtString(chaine)

void IMPLEMENT(History_finalize)(History *history, const char *filename)
{
	if (filename)
	{
		char *path = prependHomeDir(duplicateString(filename)); // Gérer le ~ qui symbolise le répertoire utilisateur (/home/user)
		if (path)
		{
			FILE *fichier = fopen(path, "w");
			if (fichier)
			{
				char *chaine;
				while (!Fifo_empty(&history->storage))
				{
					chaine = duplicateString(history->storage.storage[history->storage.head]);
					fputs(getProtString(chaine, '#'), fichier);
					fputs("\n", fichier);
					Fifo_pop(&history->storage);
				}
				fclose(fichier);
			}
			free(path);
		}
	}
	history->position = history->storage.head;
	Fifo_finalize(&history->storage);
	// Fifo_finalize à la fin
	// provided_History_finalize(history, filename);
}

/*
void History_clear(History *history) :

Vide l'historique.
*/

void IMPLEMENT(History_clear)(History *history)
{
	Fifo_clear(&history->storage);
	history->position = history->storage.tail;
	//provided_History_clear(history);
}

/*
void History_add(History *history, const char *cmd) :

Ajoute une chaîne de caractères (commande) (si celle-ci est non-vide) à l'historique. La position de l'utilisateur dans l'historique est réinitialisée.
*/

void IMPLEMENT(History_add)(History *history, const char *cmd)
{
	// TODO History_add
	/*if (cmd!=NULL){
		if (Fifo_full(&history->storage) == 1){
			Fifo_pop(&history->storage);
		}
		Fifo_push(&history->storage,cmd);
		history->position = history->storage.tail;
	};*/
	provided_History_add(history, cmd);
}

/*
const char* History_up(History *history) :

Retourne la dernière commande saisie par l'utilisateur. Il doit être possible de remonter dans l'historique en appelant plusieurs fois cette fonction. La fonction renvoie NULL si l'utilisateur a déjà consulté la plus vieille commande contenue dans l'historique.
*/

const char *IMPLEMENT(History_up)(History *history)
{
	if (history->position == history->storage.head)
		return NULL;
	else
	{
		history->position = (history->position - 1) % history->storage.capacity;
		return (const char *)history->storage.storage[history->position];
	}
	// return provided_History_up(history);
}

/*
const char* History_down(History *history) :

Retourne une commande plus récente si possible ou NULL en cas d'erreur. La fonction retourne la chaîne de caractère vide “” si cette action remet position dans son état initial (== history->storage.tail).
*/

const char *IMPLEMENT(History_down)(History *history)
{
	if (history->position == history->storage.tail)
		return NULL;
	else if (history->position == history->storage.tail - 1 % history->storage.capacity)
	{
		history->position = history->storage.tail;
		return "";
	}
	else
	{
		history->position = (history->position + 1) % history->storage.capacity;
		return (const char *)history->storage.storage[history->position];
	}
	// return provided_History_down(history);
}
