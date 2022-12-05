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
    /*if (input->current!=NULL){
		if (input->current->bucket!=NULL){
			if (input->current->bucket->content[input->pos]!=""){
				return input->current->bucket[input->pos];
			}
		}
	}
	return '\0';*/
	return provided_Input_get(input);
}

int IMPLEMENT(Input_insert)(Input *input, char c)
{
    return provided_Input_insert(input, c);
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
    return provided_Input_moveLeft(input);
}

int IMPLEMENT(Input_moveRight)(Input *input)
{
    return provided_Input_moveRight(input);
}

char* IMPLEMENT(Input_toString)(const Input *input)
{
    return provided_Input_toString(input);
}

void IMPLEMENT(InputIterator_initIterator)(const Input *input, InputIterator *inputIterator)
{
    provided_InputIterator_initIterator(input, inputIterator);
}

int IMPLEMENT(InputIterator_equals)(const InputIterator *x, const InputIterator *other)
{
    return provided_InputIterator_equals(x, other);
}

int IMPLEMENT(InputIterator_isOver)(const InputIterator *inputIterator)
{
    return provided_InputIterator_isOver(inputIterator);
}

void IMPLEMENT(InputIterator_next)(InputIterator *inputIterator)
{
    provided_InputIterator_next(inputIterator);
}

char IMPLEMENT(InputIterator_get)(const InputIterator *inputIterator)
{
    return provided_InputIterator_get(inputIterator);
}

int IMPLEMENT(Input_load)(Input *input, const char *cmd)
{
    return provided_Input_load(input, cmd);
}

char* IMPLEMENT(Input_getEditedWord)(const Input *input)
{
    return provided_Input_getEditedWord(input);
}
