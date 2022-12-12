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
#include "misc/ferror.h"

// #########################################################################
// #########################################################################
// #########################################################################

/*
char toLowerCase(char c) :

Convertit c en minuscule si cela est possible. La fonction retourne c si la conversion n'est pas possible.
*/

char IMPLEMENT(toLowerCase)(char c)
{
	if (c >= 'A' && c <= 'Z') // Si c fait partie de l'alphabet majuscule
	{
		c = (char)(c - 'A' + 'a'); // On retire l'écart entre les majuscules et minuscules dans la table ASCII
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

	// return provided_toLowerCase(c);
}

/*
char toUpperCase(char c) :

Convertit le caractère c en majuscule lorsque cela est possible.
*/

char IMPLEMENT(toUpperCase)(char c)
{
	if (c >= 'a' && c <= 'z') // Si c fait partie de l'alphabet minuscule
	{
		c = (char)(c + 'A' - 'a'); // On ajoute l'écart entre les majuscules et minuscules dans la table ASCII
	}
	return c;
	// return provided_toUpperCase(c);
}

/*
size_t stringLength(const char *str) :

Retourne la longueur d'une chaîne de caractères (le marqueur de fin de chaîne '\0' ne compte pas).
*/

size_t IMPLEMENT(stringLength)(const char *str)
{
	size_t i = 0;
	while (str[i] != '\0') // Tant que le caractère courant est différent du caractère de fin de chaine '\0'
	{
		i++; // On incrémente le compteur i de 1
	}
	return i;
	// return provided_stringLength(str);
}

/*
char* duplicateString(const char *str) :

Retourne une nouvelle chaîne allouée sur le tas contenant une copie des caractères de str.
*/

char *IMPLEMENT(duplicateString)(const char *str)
{
	size_t size = stringLength(str) + 1;
	char *r = malloc(size); // MALLOC : NE PAS OUBLIER DE FREE
	if (r == NULL)
	{
		return NULL;
	}
	copyStringWithLength(r, str, size);
	return r;
	// return provided_duplicateString(str);
}

/*
const char* findFirst(const char *str, const char *separators) :

Retourne un pointeur sur la première occurrence de n'importe quel caractère de separators dans str, ou NULL si la chaîne ne contient pas l'un des caractères recherchés.
*/

const char *IMPLEMENT(findFirst)(const char *str, const char *separators)
{
	int i = 0;				   // Initialisation de la variable de boucle i
	const char *ptChar = NULL; // Initialisation du pointeur constant ptChar vers un char à NULL
	while (str[i] != '\0')	   // Tant que le caractère courant est différent du caractère de fin de chaine '\0'
	{
		int j = 0;					  // Initialisation de la variable de boucle j
		while (separators[j] != '\0') // Tant que le caractère courant est différent du caractère de fin de chaine '\0'
		{
			if (str[i] == separators[j]) // Si le caractère courant est dans separators
			{
				ptChar = &str[i]; // Faire pointer ptChar sur le caractère courant
				return ptChar;	  // Retourner le pointeur ptChar
			}
			j++; // On incrémente le compteur j de 1
		}
		i++; // On incrémente le compteur i de 1
	}
	return ptChar; // Retourner le pointeur ptChar

	/* Correction prof :
	size_t len_sep = stringLength(separators);
	while (*str != '\0') {
		for (size_t i = 0; i < len_sep; ++i) {
			if (*str == separators[i]) {
				return str;
			}
		}
		++str;
	}
	return 0; */

	// return provided_findFirst(str, separators);
}

/*
char* findLast(char *str, char c) :

Retourne un pointeur sur la dernière occurrence de c dans la chaîne.
*/

char *IMPLEMENT(findLast)(char *str, char c)
{
	int i = 0;			   // Initialisation de la variable de boucle i
	char *ptChar = NULL;   // Initialisation du pointeur ptChar vers un char à NULL
	while (str[i] != '\0') // Tant que le caractère courant est différent du caractère de fin de chaine '\0'
	{
		if (str[i] == c) // Si le caractère courant est égal à c
		{
			ptChar = &str[i]; // Faire pointer ptChar sur le caractère courant
		}
		i++; // On incrémente le compteur i de 1
	}
	return ptChar;
	// return provided_findLast(str, c);
}

/*
int stringCompare(const char *str1, const char *str2) :

Compare deux chaînes str1 et str2 selon l'ordre lexicographique (alphabétique). La fonction retourne un nombre négatif, 0 ou un nombre positif, si la première chaîne est, respectivement, avant, égale ou après la seconde chaîne dans l'ordre lexicographique (ordre des caractères dans la table ASCII).
*/

int IMPLEMENT(stringCompare)(const char *str1, const char *str2)
{
	int i = 0;								   // Initialisation de la variable de boucle i
	while (str1[i] != '\0' || str2[i] != '\0') // Tant que l'on n'est pas arrivé au bout des deux chaines
	{
		if (str1[i] != str2[i]) // Si les caractères à une position i sont différents
		{
			if (str1[i] < str2[i])
			{
				return -1; // Retourner -1 si le caratère de str1 est plus petit que celui de str2
			}
			else if (str1[i] > str2[i])
			{
				return 1; // Retourner 1 si le caratère de str1 est plus grand que celui de str2
			}
		}
		i++; // Incrément de la variable de boucle i
	}
	return 0; // Retourner 0 si les chaines sont égales

	/* Correction prof :
	while(*str1 != '\0' && *str1 == *str2) {
		++str1;
		++str2;
	}
	return *str1 - *str2; */

	// return provided_stringCompare(str1, str2);
}

/*
const char* indexOfString(const char *foin, const char *aiguille, int csensitive) :

Retourne un pointeur sur la première occurrence de aiguille dans foin, ou bien NULL si la chaîne foin ne contient pas la chaîne recherchée. Le paramètre csensitive indique si cette recherche doit être sensible à la casse ou non.
*/

const char *IMPLEMENT(indexOfString)(const char *foin, const char *aiguille, int csensitive)
{
	size_t TailleFoin, TailleAiguille;
	TailleFoin = stringLength(foin);
	TailleAiguille = stringLength(aiguille);
	if (TailleFoin < TailleAiguille)
	{
		return NULL;
	}
	size_t i;
	int Booleen;
	for (i = 0; i <= (TailleFoin - TailleAiguille); i++)
	{
		Booleen = (startWith(foin, aiguille, csensitive) == NULL);
		if (Booleen == 0)
		{
			return foin;
		}
		foin++;
	}
	return NULL;

	// return provided_indexOfString(foin, aiguille, csensitive);
}

/*
char* concatenateStrings(const char *str1, const char *str2, size_t minDestSize) :

Retourne une nouvelle chaîne allouée sur le tas (de capacité minimale minDestSize). La nouvelle chaîne est le résultat de la concaténation des deux chaînes str1 et str2.
*/

char *IMPLEMENT(concatenateStrings)(const char *str1, const char *str2, size_t minDestSize)
{
	size_t taille = (stringLength(str1) + stringLength(str2)) + 1;
	if (taille < minDestSize)
	{
		taille = minDestSize;
	}
	char *str = malloc(sizeof(char) * taille);					  // MALLOC : NE PAS OUBLIER DE FREE
	copyStringWithLength(str, str1, taille);					  // On copie str1 dans str
	copyStringWithLength(str + stringLength(str1), str2, taille); // On copie str2 dans str
	return str;
	// return provided_concatenateStrings(str1, str2, minDestSize);
}

/*
void copyStringWithLength(char *dest, const char *src, size_t destSize) :

Recopie la chaîne de caractères src dans le tableau dest, de taille destSize, en tronquant si besoin (dest doit être une chaîne de caractères valide après cette opération).
*/

void IMPLEMENT(copyStringWithLength)(char *dest, const char *src, size_t destSize)
{
	if (destSize == 0)
	{
		fatalError("destSize = 0");
	}
	size_t i = 0;
	while (*src && i < destSize - 1)
	{
		dest[i] = *src; // On met src dans dest
		src++;
		i++;
	}
	dest[i] = '\0';
	// provided_copyStringWithLength(dest, src, destSize);
}

/*
char* mkReverse(char *str) :

Renvoie str après avoir inversé l'ordre des caractères (“abc” devient “cba”).
*/

char *IMPLEMENT(mkReverse)(char *str)
{
	char temp;
	size_t TailleSTR = stringLength(str);
	if (TailleSTR > 0)
	{
		for (size_t i = 0; i <= (TailleSTR / 2) - 1; i++)
		{
			temp = str[i];
			str[i] = str[TailleSTR - i - 1];
			str[TailleSTR - i - 1] = temp;
		}
	}
	return str;
	// return provided_mkReverse(str);
}

/*
const char* startWith(const char *str, const char *prefix, int csensitive) :

Retourne un pointeur sur le caractère qui suit prefix dans str si prefix est préfixe de str ou NULL si str ne commence pas par la chaîne recherchée. Le paramètre csensitive indique si cette recherche doit être sensible à la casse ou non.
*/

const char *IMPLEMENT(startWith)(const char *str, const char *prefix, int csensitive)
{
	while (*prefix)
	{
		if ((csensitive != 0 && *prefix != *str) || (csensitive == 0 && toLowerCase(*str) != toLowerCase(*prefix)))
		{
			return NULL;
		}
		prefix++;
		str++;
	}
	return str;
	// return provided_startWith(str, prefix, csensitive);
}

/*
int belongs(char c, const char *str) :

Retourne vrai (1) si la chaîne de caractères str contient le caractère c et faux (0) sinon.
*/

int IMPLEMENT(belongs)(char c, const char *str)
{
	int i = 0;			   // Initialisation de la variable de boucle i
	while (str[i] != '\0') // Tant que le caractère courant est différent du caractère de fin de chaine '\0'
	{
		if (str[i] == c) // Si le caractère courant est égal à c
		{
			return 1;
		}
		i++; // Incrément de la variable de boucle i
	}
	return 0;
	// return provided_belongs(c, str);
}

/*
char* subString(const char *start, size_t length) :

Retourne une nouvelle chaîne allouée sur le tas. La nouvelle chaîne contient les length premiers caractères, au plus, de la chaîne de caractères start.
*/

char *IMPLEMENT(subString)(const char *start, size_t length)
{
	size_t taille = length + 1;
	char *str = malloc(sizeof(char) * taille); // MALLOC : NE PAS OUBLIER DE FREE
	copyStringWithLength(str, start, taille);  // On copie start dans str
	return str;
	// return provided_subString(start, length);
}

/*
void mkCommon(char *result, const char *str) :

Conserve dans result le plus grand préfixe commun entre result et str.
*/

void IMPLEMENT(mkCommon)(char *result, const char *str)
{
	size_t tailleresult = stringLength(result);
	size_t taillestr = stringLength(str);
	size_t mintaille = 0;
	size_t i = 0;
	if (tailleresult > taillestr)
	{
		mintaille = taillestr;
	}
	else
	{
		mintaille = tailleresult;
	}
	// char* machaine=malloc(sizeof(char)*mintaille+1);
	char machaine[mintaille];
	while (result[i] == str[i] && i < mintaille)
	{
		machaine[i] = result[i];
		i++;
	}
	machaine[i] = '\0';
	copyStringWithLength(result, machaine, i + 1); // On copie machaine dans result
												   // provided_mkCommon(result, str);
}

/*
int isNotEmpty(const char *str) :

Retourne faux si la chaîne de caractères str est vide (ne contient que des espaces) et vrai sinon.
*/

int IMPLEMENT(isNotEmpty)(const char *str)
{
	int i = 0;			   // Initialisation de la variable de boucle i
	while (str[i] != '\0') // Tant que le caractère courant est différent du caractère de fin de chaine '\0'
	{
		if (str[i] != ' ') // Si le caractère courant n'est pas un espace
		{
			return 1;
		}
		i++; // Incrément de la variable de boucle i
	}
	return 0;
	// return provided_isNotEmpty(str);
}

/*
char* getProtString(const char *str, char c) :

Retourne une nouvelle chaîne allouée sur le tas. Cette chaîne est obtenue en échappant (doublant) toutes les occurrences de c dans str (“s#s” devient “s##s” après échappement de “#”).
*/

char *IMPLEMENT(getProtString)(const char *str, char c)
{
	int j = 0;
	size_t taille = stringLength(str) + 1;
	while (str[j] != '\0')
	{
		if (str[j] == c)
		{
			taille++;
		}
		j++;
	}
	char *machaine = malloc((sizeof(char) * (taille))); // MALLOC : NE PAS OUBLIER DE FREE
	if (!machaine)// Si malloc a échoué
	{
		free(machaine);
	}
	j = 0;
	int b = 0;
	while (str[j] != '\0')
	{
		machaine[b] = str[j];
		if (str[j] == c)
		{
			b++;
			machaine[b] = str[j];
		}
		b++;
		j++;
	}
	machaine[b] = '\0';
	return machaine;
	// return provided_getProtString(str, c);
}

/*
char* getRealString(const char *str, char c, char **firstNotEscaped) :

Réalise l'opération inverse. La position du premier caractère égal à c non-échappé doit être stockée dans firstNotEscaped. firstNotEscaped est égal à NULL si un tel caractère n'existe pas.
*/

/* "cd /pa##l/ # commentaire" -> "cd /pa#l/ # commentaire" (on laisse le # seul mais on aura placé le pointeur firstNotEscaped,
si on fait par la suite *firstNotEscaped = '\0' on aura "cd /pa#l/ " */

char *IMPLEMENT(getRealString)(const char *str, char c, char **firstNotEscaped)
{
	// TODO getRealString
	//  *firstNotEscaped = ...
	return provided_getRealString(str, c, firstNotEscaped);
}

/*
Les fonctions Tokenizer permettent de découper une liste de chaînes de caractères selon un ou plusieurs séparateurs, comme dans l'exemple ci-dessous.
*/

// TOKENIZER EN BONUS

MAKE_NEW_2(Tokenizer, const char *, const char *)
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

char *IMPLEMENT(Tokenizer_get)(const Tokenizer *tokenizer)
{
	return provided_Tokenizer_get(tokenizer);
}

void IMPLEMENT(Tokenizer_next)(Tokenizer *tokenizer)
{
	provided_Tokenizer_next(tokenizer);
}
