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

#include "misc/pattern.h"

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_1(Pattern, const char *)
MAKE_DEL_0(Pattern)

/*
Les fonctions Pattern permettent de vérifier si une chaîne de caractères str vérifie ou non un motif pattern donné.
*/

int IMPLEMENT(Pattern_init)(Pattern *preg, const char *pattern)
{
    // TODO Pattern_init
    return provided_Pattern_init(preg, pattern);
}

void IMPLEMENT(Pattern_finalize)(Pattern *preg)
{
    // TODO Pattern_finalize
    provided_Pattern_finalize(preg);
}

int IMPLEMENT(Pattern_match)(const Pattern *preg, const char *str)
{
    // TODO Pattern_match
    return provided_Pattern_match(preg, str);
}
