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

#include "interactive/autocomplete.h"
#include "misc/filesystem.h"
#include "misc/string.h"
#include "system/info.h"

// #########################################################################
// #########################################################################
// #########################################################################

/*
char* prependHomeDir(char *str) :

Etend le chemin str si ce dernier commence par le raccourci “~” (répertoire de l'utilisateur).

1. Le cas échéant, la mémoire allouée à str est libérée et la fonction renvoie une nouvelle chaîne de caractères allouée sur le tas (e.g. “~/.historique” devient “/home/etudiant/.historique”).
2. Si le caractère spécial “~” est doublé (échappé), la fonction supprime l'une des deux occurrences.
3. Elle renvoie directement str dans tous les autre cas, y compris si str est NULL.
*/

char *IMPLEMENT(prependHomeDir)(char *str)
{
	if (str && str[0] == '~')
	{
		char *nstr = NULL;
		//~ or ~/xxx
		if (str[1] == '\0' || str[1] == '/')
		{
			const char *homedir;
			userInformation(NULL, &homedir, NULL);
			nstr = concatenateStrings(homedir, str + 1, 0);
		}
		// ~~ or ~~/xxx
		else if (str[1] == '~' && (str[2] == '/' || str[2] == '\0'))
		{
			nstr = duplicateString(str + 1);
		}
		// replace str by nstr if necessary and if concatenateStrings ran successfully
		if (nstr)
		{
			free(str);
			str = nstr;
		}
	}
	return str;
	// return provided_prependHomeDir(str);
}

/*
int autocomplete(const char *prefix, unsigned int limit,
				 // outputs
				 unsigned int *nbItems, char **extend, Fifo **results) :

La recherche est exhaustive. Le nombre de solutions trouvées est enregistré dans nbItems. Si la solution est unique, le résultat est stocké dans extend. S'il y en a plusieurs, au plus limit propositions sont rangées dans le conteneur results. La fonction retourne 0 en cas de succès ou 1 en cas d'erreur (attention, ne pas trouver de solution n'est pas un motif d'erreur !).
*/

/* startWith, prefix obtenu avec Input_getEditedWord, nbItems, extend et results sont le réseultat de la fonction
nbItems, nombre déléments qui matchent avec le prefix, dès qu'on a correspondance on incrémente (utiliser startWith)
La fonction retourne soit une chaine extend, soit un ensemble de chaine results contenant toutes les suggestions si on ne sait pas, si on sait on met le résultat dans extend
Dès que l'on peut écrire quelque chose pour l'utilisateur on met dans extend sinon dans results
Cas simple : On explore le dossier courant de l'utilisateur
FolderIterator sur le dossier courant (open sur .), on parcours et on regarde les éléments débutant par prefix et on construit extend et results dans la fonction et à la fin on choisira lequel retourner

La fonction est notée en fonction de niveaux à valider via les tests unitaires, pas grave de laisser des tests unitaires qui ne marchent pas sur cette fonction
Ne pas laisser vide : Haute pondération
Niveaux 1 à 4 : OK
Niveau 5 : Plus compliqué */

int IMPLEMENT(autocomplete)(const char *prefix, unsigned int limit, unsigned int *nbItems, char **extend, Fifo **results)
{
	// TODO autocomplete (levels 3 et 4)
	/* 
	//CODE FONCTIONNEL POUR LES LEVELS 1 ET 2 (COMMENTE POUR RELEASE, REMPLACE PAR PROVIDED_AUTOCOMPLETE)
	if (!prefix || !limit || !nbItems || !extend || !results) // On vérifie que les paramètres sont valides
	{
		return 1;
	}
	*nbItems = 0;
	*extend = NULL;
	*results = Fifo_new(limit, COMPOSE); // MALLOC + init
	FolderIterator it;
	if (FolderIterator_init(&it, ".", 1)) // On ouvre le dossier courant
	{
		printf("Erreur init!");
		return 1;
	}
	while (!FolderIterator_isOver(&it)) // Tant que l'on est pas arrivé à la fin du dossier
	{
		const char *elem = FolderIterator_get(&it);
		char *elem2 = duplicateString(elem); // Pour éviter const // MALLOC
		if (FolderIterator_isDir(&it))
		{
			size_t taille = stringLength(elem2) + 1;
			char *elem3 = concatenateStrings(elem2, "/", taille); // On ajoute "/", on sépare pour pouvoir free // MALLOC
			free(elem2);										  // On libère ce qu'on utilise dans concatenateStrings pour ne pas avoir d'erreur valgrind quand on remplace
			elem2 = duplicateString(elem3);						  // On récupère la chaine avec le "/" en plus // MALLOC
			free(elem3);										  // On libère l'élement séparé
		}
		const char *suffix = startWith(elem2, prefix, 1); // On regarde si elem2 commence par prefix
		if (suffix != NULL)
		{
			++(*nbItems);
			Fifo_push(*results, elem2);
			if (*extend == NULL)
			{
				*extend = duplicateString(suffix); // MALLOC
			}
			else
			{
				mkCommon(*extend, suffix);
			}
		}
		free(elem2); // Free du premier duplicateString
		FolderIterator_next(&it);
	}
	FolderIterator_finalize(&it);
	// Se baser sur nbItems aide mais ne suffit pas dans certains cas pour trancher
	if (*nbItems < 1)
	{
		// Fifo_finalize(*results);
		Fifo_delete(*results);
		*results = NULL;
	}
	else
	{
		if (stringLength(*extend) != 0) //
		{
			// Fifo_finalize(*results);
			Fifo_delete(*results);
			*results = NULL;
		}
		else
		{
			*extend = NULL;
			free(*extend);
		}
	}
	return 0;
	*/
	return provided_autocomplete(prefix, limit, nbItems, extend, results);
}
