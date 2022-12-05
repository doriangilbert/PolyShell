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

int IMPLEMENT(Input_init)(Input *input)
{
    input->current = NULL;
	input->pos = 0;
	return 0;
	//return provided_Input_init(input);
}

void IMPLEMENT(Input_finalize)(Input *input)
{
    Input_clear(input);
	//provided_Input_finalize(input);
}

void IMPLEMENT(Input_clear)(Input *input)
{
    provided_Input_clear(input);
}

size_t IMPLEMENT(Input_size)(const Input *input)
{
	size_t s = 0;
	if (input->current) {
		for(Cell *c = input->current ; c != NULL ; c = c->next) {
			s += Bucket_size(&c->bucket);
		}
		for(Cell *c = input->current->previous ; c != NULL ; c = c->previous) {
			s += Bucket_size(&c->bucket);
		}
	}
	return s;
	//return provided_Input_size(input);
}

char IMPLEMENT(Input_get)(const Input *input)
{
	// A REVOIR
    if (input!=NULL && input->current != NULL && !Bucket_empty(&input->current->bucket) && input->pos<(input->current->bucket.top+1)){
		return input->current->bucket.content[input->pos];
	}
	return '\0';
	//return provided_Input_get(input);
}

int IMPLEMENT(Input_insert)(Input *input, char c)
{
    //cas liste vide
	if (input->current == NULL) {
		input->current = Cell_new(); //allocation dyn + init WARNING : Ne pas oublier de désallouer -> FUITE MEMOIRE
		if (input->current == NULL) {
			return 1;
		}
		Bucket_insert(&input->current->bucket, 0, c);
		input->pos = 1;
	}
	else {
		//cas particulier 2 : bucket plein
		if (Bucket_full(&input->current->bucket)) {
			Cell* newCell = Cell_new();
			if (newCell == NULL) {
				return 1;
			}
			Cell_insertAfter(input->current, newCell);
			if (input->pos > input->current->bucket.top) {
				//impossible de déplacer ce qui est à droite de pos car pos == top + 1
				input->current = newCell;
				input->pos = 0;
			}
			else {
				//on déplace du contenu dans la nouvelle cellule pour faire de la place
				Bucket_move(&input->current->bucket, input->pos, &newCell->bucket);
			}
			//dans tous les cas, il y a maintenant de la place dans current->bucket et OK pour insérer à la position pos
		}
		//cas général
		Bucket_insert(&input->current->bucket, input->pos, c);
		Input_moveRight(input);
	}
	return 0;
	//return provided_Input_insert(input, c);
}

int IMPLEMENT(Input_backspace)(Input *input)
{
    return provided_Input_backspace(input);
}

int IMPLEMENT(Input_del)(Input *input)
{
    return provided_Input_del(input);
}

int IMPLEMENT(Input_moveLeft)(Input *input)
{
    if (input->current != NULL) {
		if (input->pos == 0) {
			if (input->current->previous == NULL) {
				return 1;
			}
			else {
				input->current = input->current->previous;
				input->pos = input->current->bucket.top;
				return 0;
			}
		}
		else {
			input->pos = input->pos-1;
			return 0;
		}
	}
	return 1;
	//return provided_Input_moveLeft(input);
}

int IMPLEMENT(Input_moveRight)(Input *input)
{
	//A FAIRE
	/*if (input != NULL && input->current!=NULL && !Bucket_empty(&input->current->bucket)){
		if (input->pos==input->current->bucket.top){
			if (input->current->next!=NULL){
				input->current=input->current->next;
				input->pos=0;
				return 0;
			}	
			else{
				return 1;
			}
		}
		else{
			input->pos++;
			return 0;
		}
	}
	else{
		return 1;
	}*/
	return provided_Input_moveRight(input);
}

char* IMPLEMENT(Input_toString)(const Input *input)
{
    return provided_Input_toString(input);
}

/* POUR TESTER : (typedef Input InputIterator)
 * InputIterator it;
 * for (InputIterator_init(&it) ; !InputIterator_isOver(&it) ; InputIterator_next(&it)) {
 * 		printf("%c", InputIterator_get(&it);
 * }
*/

void IMPLEMENT(InputIterator_initIterator)(const Input *input, InputIterator *inputIterator)
{
	//TODO : Gérer le cas où input est vide
    /*inputIterator->pos = 0;
	inputIterator->current = input->current;
	while (inputIterator->current->previous != NULL) {
		inputIterator->current = inputIterator->current->previous;
	}
	*/
	provided_InputIterator_initIterator(input, inputIterator);
}

int IMPLEMENT(InputIterator_equals)(const InputIterator *x, const InputIterator *other)
{
    return provided_InputIterator_equals(x, other);
}

int IMPLEMENT(InputIterator_isOver)(const InputIterator *inputIterator)
{
    //à top+1 (après le dernier caractere)
	return provided_InputIterator_isOver(inputIterator);
}

void IMPLEMENT(InputIterator_next)(InputIterator *inputIterator)
{
    Input_moveRight(inputIterator);
	//provided_InputIterator_next(inputIterator);
}

char IMPLEMENT(InputIterator_get)(const InputIterator *inputIterator)
{
    return Input_get(inputIterator);
	//return provided_InputIterator_get(inputIterator);
}

int IMPLEMENT(Input_load)(Input *input, const char *cmd)
{
    if (cmd==NULL) return 1;
	Input_clear(input);
	for (size_t i=0; i<=stringLength(cmd);i++) Input_insert(input,cmd[i]);
	return 0;
	//return provided_Input_load(input, cmd);
}

char* IMPLEMENT(Input_getEditedWord)(const Input *input)
{
    return provided_Input_getEditedWord(input);
}
