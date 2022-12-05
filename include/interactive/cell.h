#ifndef CELL_H

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

#define CELL_H

//
#include "misc/new.h"

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include "bucket.h"

/**
 *
 */
typedef struct __Cell__ {

    //
    Bucket bucket;

    //
    struct __Cell__ *previous;

    //
    struct __Cell__ *next;

} Cell;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
LIB Cell* OVERRIDABLE(Cell_new)(void);

/**
 *
 */
LIB int OVERRIDABLE(Cell_init)(Cell *cell);

/**
 *
 */
LIB void OVERRIDABLE(Cell_delete)(Cell *cell);

/**
 *
 */
LIB void OVERRIDABLE(Cell_finalize)(Cell *cell);

/**
 *
 */
LIB void OVERRIDABLE(Cell_insertAfter)(Cell *cell, Cell *newCell);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

//
#include "impl/cell.h"

#endif
