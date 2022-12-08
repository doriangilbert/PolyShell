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

#include "interactive/bucket.h"
#include <assert.h>

// #########################################################################
// #########################################################################
// #########################################################################

// MAKE_NEW_0(Bucket)
// MAKE_DEL_0(Bucket)

/*
Bucket* Bucket_new(void) :

Retourne un bucket vide alloué sur le tas (“allocation” et “construction”), ou NULL en cas d'erreur.
*/

Bucket *IMPLEMENT(Bucket_new)(void)
{
	Bucket *b = malloc(sizeof(Bucket)); // MALLOC : NE PAS OUBLIER DE FREE
	if (b != NULL)
	{
		if (Bucket_init(b))
		{
			// erreur init
			free(b);
			b = NULL;
		}
	}
	return b;
	// return provided_Bucket_new();
}

/*
int Bucket_init(Bucket *bucket) :

Initialise (“construit”) un bucket vide et renvoie un code d'erreur (0 en cas succès, 1 en cas d'erreur).
*/

int IMPLEMENT(Bucket_init)(Bucket *bucket)
{
	bucket->top = -1;
	return 0;
	// return provided_Bucket_init(bucket);
}

/*
void Bucket_delete(Bucket *bucket) :

Désalloue bucket et ses éventuelles ressources (“destruction” et “désallocation”).
*/

void IMPLEMENT(Bucket_delete)(Bucket *bucket)
{
	if (bucket != NULL)
	{
		Bucket_finalize(bucket);
		free(bucket); // FREE
	}
	// provided_Bucket_delete(bucket);
}

/*
void Bucket_finalize(Bucket *bucket) :

Libère les ressources allouées par (“détruit”) bucket.
*/

void IMPLEMENT(Bucket_finalize)(Bucket *bucket)
{
	(void)bucket; // ne fait rien mais enlève le warning "unused variable"
				  // provided_Bucket_finalize(bucket);
}

/*
size_t Bucket_size(const Bucket *bucket) :

Retourne le nombre de caractères contenus dans bucket.
*/

size_t IMPLEMENT(Bucket_size)(const Bucket *bucket)
{
	return bucket->top + 1;
	// return provided_Bucket_size(bucket);
}

/*
void Bucket_remove(Bucket *bucket, int position) :

Supprime le position-ième caractère de bucket.
*/

void IMPLEMENT(Bucket_remove)(Bucket *bucket, int position)
{
	assert(!Bucket_empty(bucket));
	assert(position >= 0 && position <= bucket->top);
	for (int i = position; i < (bucket->top); i++)
	{
		bucket->content[i] = bucket->content[i + 1];
	}
	bucket->top = (bucket->top) - 1;
	/* Correction prof :
	while(position<bucket->top) {
		bucket->content[position]=bucket->content[position+1];
		position++;
	}
	(bucket->top)--;
	*/
	// provided_Bucket_remove(bucket, position);
}

/*
void Bucket_insert(Bucket *bucket, int position, char c) :

Insère c en position "position" dans bucket.
*/

void IMPLEMENT(Bucket_insert)(Bucket *bucket, int position, char c)
{
	assert(!Bucket_full(bucket));
	assert(position >= 0 && position <= bucket->top + 1);
	if (position == bucket->top + 1)
	{
		bucket->content[position] = c;
	}
	else
	{
		char temp = bucket->content[position];
		char temp2;
		for (int i = position + 1; i <= (bucket->top) + 1; i++)
		{
			temp2 = bucket->content[i];
			bucket->content[i] = temp;
			temp = temp2;
		}
		bucket->content[position] = c;
	}
	bucket->top = bucket->top + 1;
	// provided_Bucket_insert(bucket, position, c);
}

/*
void Bucket_move(Bucket *from, int position, Bucket *to) :

Déplace le position-ième caractère et ceux qui suivent de from à to. On suppose que to est vide.
*/

void IMPLEMENT(Bucket_move)(Bucket *from, int position, Bucket *to)
{
	char temp;
	int top = from->top;
	for (int i = 0; i <= top - position; i++)
	{
		temp = from->content[position];
		Bucket_remove(from, position);
		Bucket_insert(to, i, temp);
	}
	// provided_Bucket_move(from, position, to);
}

/*
int Bucket_empty(const Bucket *bucket) :

Retourne vrai (1) si bucket est vide et faux (0) sinon.
*/

int IMPLEMENT(Bucket_empty)(const Bucket *bucket)
{
	return bucket->top == -1;
	// return provided_Bucket_empty(bucket);
}

/*
int Bucket_full(const Bucket *bucket) :

Retourne vrai si bucket est plein et faux sinon.
*/

int IMPLEMENT(Bucket_full)(const Bucket *bucket)
{
	return (bucket->top + 1) == BUCKET_SIZE;
	// return provided_Bucket_full(bucket);
}
