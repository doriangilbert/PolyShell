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

#include "interactive/cell.h"

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_0(Cell)
MAKE_DEL_0(Cell)

/*
int Cell_init(Cell *cell) :

Initialise une cellule vide isolée.
*/

int IMPLEMENT(Cell_init)(Cell *cell)
{
	cell->next = cell->previous = NULL;
	if (Bucket_init(&cell->bucket))
	{
		return 1; // init a echoué
	}
	return 0;
	// return provided_Cell_init(cell);
}

/*
void Cell_finalize(Cell *cell) :

Libère les ressources allouées par cell.
*/

void IMPLEMENT(Cell_finalize)(Cell *cell)
{
	Bucket_finalize(&cell->bucket);
	// provided_Cell_finalize(cell);
}

/*
void Cell_insertAfter(Cell *cell, Cell *newCell) :

Insère newCell après cell. On suppose que newCell vient d'être créée.
*/

void IMPLEMENT(Cell_insertAfter)(Cell *cell, Cell *newCell)
{
	newCell->next = (cell->next);
	newCell->previous = cell;
	if (newCell->next != NULL)
	{
		(newCell->next)->previous = newCell;
	}
	cell->next = newCell;

	/*Correction Prof :
	newCell->previous=cell;
	newCell->next=cell->next;
	if (cell->next) {
		cell->next->previous=newCell;
	}
	cell->next=newCell;*/

	// provided_Cell_insertAfter(cell, newCell);
}
