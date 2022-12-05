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

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_2(History, const char*, unsigned int)
MAKE_DEL_1(History, const char*)

int IMPLEMENT(History_init)(History *history, const char *filename, unsigned int length)
{
    return provided_History_init(history, filename, length);
}

void IMPLEMENT(History_finalize)(History *history, const char *filename)
{
    provided_History_finalize(history, filename);
}

void IMPLEMENT(History_clear)(History *history)
{
    provided_History_clear(history);
}

void IMPLEMENT(History_add)(History *history, const char *cmd)
{
    provided_History_add(history, cmd);
}

const char* IMPLEMENT(History_up)(History *history)
{
    return provided_History_up(history);
}

const char* IMPLEMENT(History_down)(History *history)
{
    return provided_History_down(history);
}
