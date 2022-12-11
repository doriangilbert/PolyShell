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

#include "interactive/all.h"
#include "misc/string.h"

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_0(Input)
MAKE_DEL_0(Input)

/*
int Input_init(Input *input) :

Initialise une ligne de commande vide.
*/

int IMPLEMENT(Input_init)(Input *input)
{
	input->current = NULL;
	input->pos = 0;
	return 0;
	// return provided_Input_init(input);
}

/*
void Input_finalize(Input *input) :

Libère les ressources allouées par input.
*/

void IMPLEMENT(Input_finalize)(Input *input)
{
	Input_clear(input);
	// provided_Input_finalize(input);
}

/*
void Input_clear(Input *input) :

Vide la ligne de commande.
*/

void IMPLEMENT(Input_clear)(Input *input)
{
	 if (input != NULL && input->current != NULL)
	{
		while (Input_moveRight(input) == 0);
		Input_del(input);
		while (Input_moveLeft(input) == 0)
		{
			Input_del(input);
		}
	}
	//provided_Input_clear(input);
}

/*
size_t Input_size(const Input *input) :

Retourne la longueur de input (le nombre total de caractères dans cette ligne de commande).
*/

size_t IMPLEMENT(Input_size)(const Input *input)
{
	size_t s = 0;
	if (input->current)
	{
		for (Cell *c = input->current; c != NULL; c = c->next)
		{
			s += Bucket_size(&c->bucket);
		}
		for (Cell *c = input->current->previous; c != NULL; c = c->previous)
		{
			s += Bucket_size(&c->bucket);
		}
	}
	return s;
	// return provided_Input_size(input);
}

/*
char Input_get(const Input *input) :

Retourne le caractère pointé par le curseur. Si la commande est vide ou si le curseur est placé en bout de chaîne (après le dernier caractère), la fonction renvoie '\0'.
*/

char IMPLEMENT(Input_get)(const Input *input)
{
	// A REVOIR
	if (input != NULL && input->current != NULL && !Bucket_empty(&input->current->bucket) && input->pos < (input->current->bucket.top + 1))
	{
		return input->current->bucket.content[input->pos];
	}
	return '\0';
	// return provided_Input_get(input);
}

/*
int Input_insert(Input *input, char c) :

Insère un caractère puis met le curseur à jour.
*/

int IMPLEMENT(Input_insert)(Input *input, char c)
{
	// cas liste vide
	if (input->current == NULL)
	{
		input->current = Cell_new(); // allocation dyn + init WARNING : Ne pas oublier de désallouer -> FUITE MEMOIRE
		if (input->current == NULL)
		{
			return 1;
		}
		Bucket_insert(&input->current->bucket, 0, c);
		input->pos = 1;
	}
	else
	{
		// cas particulier 2 : bucket plein
		if (Bucket_full(&input->current->bucket))
		{
			Cell *newCell = Cell_new();
			if (newCell == NULL)
			{
				return 1;
			}
			Cell_insertAfter(input->current, newCell);
			if (input->pos > input->current->bucket.top)
			{
				// impossible de déplacer ce qui est à droite de pos car pos == top + 1
				input->current = newCell;
				input->pos = 0;
			}
			else
			{
				// on déplace du contenu dans la nouvelle cellule pour faire de la place
				Bucket_move(&input->current->bucket, input->pos, &newCell->bucket);
			}
			// dans tous les cas, il y a maintenant de la place dans current->bucket et OK pour insérer à la position pos
		}
		// cas général
		Bucket_insert(&input->current->bucket, input->pos, c);
		Input_moveRight(input);
	}
	return 0;
	// return provided_Input_insert(input, c);
}

/*
int Input_backspace(Input *input) :

Efface un caractère puis met le curseur à jour (bouton backspace).
*/

int IMPLEMENT(Input_backspace)(Input *input)
{
	if(!Input_moveLeft(input)){
		return Input_del(input);
	}
	return 1;

	//return provided_Input_backspace(input);
}

/*
int Input_del(Input *input) :

Efface un caractère puis met le curseur à jour (bouton suppr).
*/

int IMPLEMENT(Input_del)(Input *input)
{
	// TODO Input_del
	/*if (input!=NULL && input->current !=NULL)
	{
		if (input->pos < input->current->bucket.top +1 && !Bucket_empty(&input->current->bucket))
		{
			Bucket_remove(&input->current->bucket,input->pos);
			if (Bucket_empty(&input->current->bucket) && input->current->next !=NULL){
				input->current->next->previous = input->current->previous;
				input->current=input->current->next;
				input->pos = 0;

				if (input->current->previous !=NULL){
					input->current->previous->next =input->current;
				}
			}
			return 0;
		}
	}
	return 1;*/

	return provided_Input_del(input);
}

/*
int Input_moveLeft(Input *input) :

Déplace le curseur d'un caractère vers la gauche.
*/

int IMPLEMENT(Input_moveLeft)(Input *input)
{
	if (input->current != NULL)
	{
		if (input->pos == 0)
		{
			if (input->current->previous == NULL)
			{
				return 1;
			}
			else
			{
				input->current = input->current->previous;
				input->pos = input->current->bucket.top;
				return 0;
			}
		}
		else
		{
			input->pos = input->pos - 1;
			return 0;
		}
	}
	return 1;
	// return provided_Input_moveLeft(input);
}

/*
int Input_moveRight(Input *input) :

Déplace le curseur vers la droite.
*/

int IMPLEMENT(Input_moveRight)(Input *input)
{

	if (input != NULL && input->current != NULL && !Bucket_empty(&input->current->bucket) && input->pos != input->current->bucket.top + 1)
	{
		if (input->pos == input->current->bucket.top)
		{
			if (input->current->next != NULL)
			{
				input->current = input->current->next;
				input->pos = 0;
				return 0;
			}
			else
			{
				if (!Bucket_full(&input->current->bucket))
				{
					input->pos++;
					return 0;
				}
				else
				{
					input->current->next = Cell_new();
					input->current->next->previous = input->current;
					input->current = input->current->next;

					input->pos = 0;
					return 0;
				}
			}
		}
		else
		{
			input->pos++;
			return 0;
		}
	}
	else
	{
		return 1;
	}

	// return provided_Input_moveRight(input);
}

/*
char* Input_toString(const Input *input) :

Retourne une nouvelle chaîne de caractères initialisée à partir de input.
*/

char *IMPLEMENT(Input_toString)(const Input *input)
{
	InputIterator it;
	size_t l = Input_size(input);
	char *chaine = malloc(sizeof(char *) * l+1);
	int i = 0;
	for (InputIterator_initIterator(input, &it); !InputIterator_isOver(&it); InputIterator_next(&it))
	{
		chaine[i] = InputIterator_get(&it);
		i++;
	}
	chaine[i] = '\0';
	return chaine;
	// return provided_Input_toString(input);
}

/* POUR TESTER : (typedef Input InputIterator)
 * InputIterator it;
 * for (InputIterator_init(&it) ; !InputIterator_isOver(&it) ; InputIterator_next(&it)) {
 * 		printf("%c", InputIterator_get(&it);
 * }
 */

void IMPLEMENT(InputIterator_initIterator)(const Input *input, InputIterator *inputIterator)
{
	// TODO InputIterator_initIterator
	//  A faire : Gérer le cas où input est vide
	if (input != NULL && input->current != NULL)
	{
		inputIterator->pos = 0;
		inputIterator->current = input->current;
		while (inputIterator->current->previous != NULL)
		{
			inputIterator->current = inputIterator->current->previous;
		}
	}
	provided_InputIterator_initIterator(input, inputIterator);
}

/*
int InputIterator_equals(const InputIterator *x, const InputIterator *other) :

Retourne vrai si et seulement si x et other pointent sur le même caractère de la même commande.
*/

int IMPLEMENT(InputIterator_equals)(const InputIterator *x, const InputIterator *other)
{
	if (&x->current->bucket.content[x->pos] == &other->current->bucket.content[other->pos])
	{
		return 1;
	}
	return 0;
	// return provided_InputIterator_equals(x, other);
}

int IMPLEMENT(InputIterator_isOver)(const InputIterator *inputIterator)
{
	// TODO InputIterator_isOver
	//  à top+1 (après le dernier caractere)
	return provided_InputIterator_isOver(inputIterator);
}

void IMPLEMENT(InputIterator_next)(InputIterator *inputIterator)
{
	Input_moveRight(inputIterator);
	// provided_InputIterator_next(inputIterator);
}

char IMPLEMENT(InputIterator_get)(const InputIterator *inputIterator)
{
	return Input_get(inputIterator);
	// return provided_InputIterator_get(inputIterator);
}

/*
int Input_load(Input *input, const char *cmd) :

Réinitialise input à partir de cmd (input et cmd doivent correspondre à la même séquence de caractères après opération). La fonction renvoie une erreur si cmd est NULL.
*/

int IMPLEMENT(Input_load)(Input *input, const char *cmd)
{
	if (cmd == NULL)
		return 1;
	Input_clear(input);
	for (size_t i = 0; i <= stringLength(cmd); i++)
		Input_insert(input, cmd[i]);
	return 0;
	// return provided_Input_load(input, cmd);
}

/*
char* Input_getEditedWord(const Input *input) :

Qui retourne le mot en cours de saisie (placé juste avant le curseur). La chaîne est allouée sur le tas.
*/

// Prendre tous les caractères jusqu'à espace
char *IMPLEMENT(Input_getEditedWord)(const Input *input)
{
	// TODO Input_getEditedWord
	return provided_Input_getEditedWord(input);
}
