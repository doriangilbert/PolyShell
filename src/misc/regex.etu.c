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
    if (pattern)
    {
        int valeur = regcomp(&preg->preg, pattern, REG_EXTENDED); // On compile pattern
        if (valeur == 0) // Si regcomp a réussi
        {
            return 0;
        }
    }
    return 1; // Si il n'y a pas de pattern ou si regcomp echoue
    // return provided_Pattern_init(preg, pattern);
}

void IMPLEMENT(Pattern_finalize)(Pattern *preg)
{
    regfree(&preg->preg); // On libère la mémoire allouée par regcomp
    // provided_Pattern_finalize(preg);
}

int IMPLEMENT(Pattern_match)(const Pattern *preg, const char *str)
{
    int valeur = regexec(&preg->preg, str, 0, NULL, 0);
    if (valeur == 0)
    { // Si str vérifie le pattern
        return 1;
    }
    return 0;
    // return provided_Pattern_match(preg, str);
}
