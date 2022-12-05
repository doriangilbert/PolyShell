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

#include "misc/string.h"

// #########################################################################
// #########################################################################
// #########################################################################

char IMPLEMENT(toLowerCase)(char c)
{
	if (c >= 'A' && c <='Z') { //Si c fait partie de l'alphabet majuscule
		c = (char)(c - 'A' + 'a'); //On retire l'écart entre les majuscules et minuscules dans la table ASCII
	}
	return c;
    /*switch (c) {
		case 'A' : return 'a'; break;
		case 'B' : return 'b'; break;
		case 'C' : return 'c'; break;
		case 'D' : return 'd'; break;
		case 'E' : return 'e'; break;
		case 'F' : return 'f'; break;
		case 'G' : return 'g'; break;
		case 'H' : return 'h'; break;
		case 'I' : return 'i'; break;
		case 'J' : return 'j'; break;
		case 'K' : return 'k'; break;
		case 'L' : return 'l'; break;
		case 'M' : return 'm'; break;
		case 'N' : return 'n'; break;
		case 'O' : return 'o'; break;
		case 'P' : return 'p'; break;
		case 'Q' : return 'q'; break;
		case 'R' : return 'r'; break;
		case 'S' : return 's'; break;
		case 'T' : return 't'; break;
		case 'U' : return 'u'; break;
		case 'V' : return 'v'; break;
		case 'W' : return 'w'; break;
		case 'X' : return 'x'; break;
		case 'Y' : return 'y'; break;
		case 'Z' : return 'z'; break;
		default : return c;
	};*/
	//return provided_toLowerCase(c);
}

char IMPLEMENT(toUpperCase)(char c)
{
	if (c >= 'a' && c <='z') { //Si c fait partie de l'alphabet minuscule
		c = (char)(c + 'A' - 'a'); //On ajoute l'écart entre les majuscules et minuscules dans la table ASCII
	}
	return c;		
    //return provided_toUpperCase(c);
}

size_t IMPLEMENT(stringLength)(const char *str)
{
	int i = 0;
	while (str[i] != '\0') { //Tant que le caractère courant est différent du caractère de fin de chaine '\0'
		i++; //On incrémente le compteur i de 1
	}
	return i;
    //return provided_stringLength(str);
}

char* IMPLEMENT(duplicateString)(const char *str)
{
    return provided_duplicateString(str);
}

const char* IMPLEMENT(findFirst)(const char *str, const char *separators)
{
    return provided_findFirst(str, separators);
}

char* IMPLEMENT(findLast)(char *str, char c)
{
    return provided_findLast(str, c);
}

int IMPLEMENT(stringCompare)(const char *str1, const char *str2)
{
	/*int i = 0;
	while (str1[i] != '\0' || str2[i] != '\0') {
		if (str1[i] != str2[i]) {
			if (str1[i] < str2[i]) {
				return -1;
			}
			else if (str1[i] > str2[i]) {
				return 1;
			}
		}
	}*/
	/*return -1;
	return 0;
	return 1;*/
	//return provided_stringCompare(str1, str2);
}

const char* IMPLEMENT(indexOfString)(const char *foin, const char *aiguille, int csensitive)
{
    return provided_indexOfString(foin, aiguille, csensitive);
}

char* IMPLEMENT(concatenateStrings)(const char *str1, const char *str2, size_t minDestSize)
{
    return provided_concatenateStrings(str1, str2, minDestSize);
}

void IMPLEMENT(copyStringWithLength)(char *dest, const char * src, size_t destSize)
{
    provided_copyStringWithLength(dest, src, destSize);
}

char* IMPLEMENT(mkReverse)(char *str)
{
    return provided_mkReverse(str);
}

const char* IMPLEMENT(startWith)(const char *str, const char *prefix, int csensitive)
{
    return provided_startWith(str, prefix, csensitive);
}

int IMPLEMENT(belongs)(char c, const char *str)
{
    return provided_belongs(c, str);
}

char* IMPLEMENT(subString)(const char *start, size_t length)
{
    return provided_subString(start, length);
}

void IMPLEMENT(mkCommon)(char *result, const char *str)
{
    provided_mkCommon(result, str);
}

int IMPLEMENT(isNotEmpty)(const char *str)
{
    return provided_isNotEmpty(str);
}

char* IMPLEMENT(getProtString)(const char *str, char c)
{
    return provided_getProtString(str, c);
}

char* IMPLEMENT(getRealString)(const char *str, char c, char **firstNotEscaped)
{
    return provided_getRealString(str, c, firstNotEscaped);
}

MAKE_NEW_2(Tokenizer, const char*, const char*)
MAKE_DEL_0(Tokenizer)

int IMPLEMENT(Tokenizer_init)(Tokenizer *tokenizer, const char *str, const char *separators)
{
    return provided_Tokenizer_init(tokenizer, str, separators);
}

void IMPLEMENT(Tokenizer_finalize)(Tokenizer *tokenizer)
{
    provided_Tokenizer_finalize(tokenizer);
}

int IMPLEMENT(Tokenizer_isOver)(const Tokenizer *tokenizer)
{
    return provided_Tokenizer_isOver(tokenizer);
}

char* IMPLEMENT(Tokenizer_get)(const Tokenizer *tokenizer)
{
    return provided_Tokenizer_get(tokenizer);
}

void IMPLEMENT(Tokenizer_next)(Tokenizer *tokenizer)
{
    provided_Tokenizer_next(tokenizer);
}
