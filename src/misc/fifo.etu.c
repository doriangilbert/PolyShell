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

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_2(Fifo, unsigned int, FifoMode)
MAKE_DEL_0(Fifo)

int IMPLEMENT(Fifo_init)(Fifo *fifo, unsigned int capacity, FifoMode mode)
{
    return provided_Fifo_init(fifo, capacity, mode);
}

void IMPLEMENT(Fifo_finalize)(Fifo *fifo)
{
    provided_Fifo_finalize(fifo);
}

void IMPLEMENT(Fifo_clear)(Fifo *fifo)
{
    provided_Fifo_clear(fifo);
}

int IMPLEMENT(Fifo_push)(Fifo *fifo, const char *str)
{
    return provided_Fifo_push(fifo, str);
}

const char* IMPLEMENT(Fifo_front)(const Fifo *fifo)
{
    return provided_Fifo_front(fifo);
}

int IMPLEMENT(Fifo_pop)(Fifo *fifo)
{
    return provided_Fifo_pop(fifo);
}

int IMPLEMENT(Fifo_full)(const Fifo *fifo)
{
    return provided_Fifo_full(fifo);
}

int IMPLEMENT(Fifo_empty)(const Fifo *fifo)
{
    return provided_Fifo_empty(fifo);
}
