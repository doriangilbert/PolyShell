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

// #########################################################################
// #########################################################################
// #########################################################################

//A ignorer dans un premier temps
char* IMPLEMENT(prependHomeDir)(char *str)
{
    return provided_prependHomeDir(str);
}

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
    if (!prefix || !limit || !nbItems || !extend || !results) {
		return 1;
	}
	*nbItems = 0;
	*extend = NULL;
	*results = Fifo_new(limit,COMPOSE); //MALLOC + init
	FolderIterator it;
	if (FolderIterator_init(&it, ".", 1)) {
		printf("Erreur init!");
		return 1;
	}
	while (!FolderIterator_isOver(&it)) {
		const char *elem = FolderIterator_get(&it);
		if (FolderIterator_isDir(&it)){
			size_t taille = stringLength(elem) +1;
			elem = concatenateStrings(elem,"/",taille);
		}
		const char *suffix=startWith(elem,prefix,1);
		if (suffix!=NULL){
			++(*nbItems);
			Fifo_push(*results,elem);
			if (*extend  == NULL){
				*extend=duplicateString(suffix);
			}
			else{
				mkCommon(*extend,suffix);
			}
		}
		FolderIterator_next(&it);
	}
	FolderIterator_finalize(&it);	
	
	//Se baser sur nbItems aide mais ne suffit pas dans certains cas pour trancher
	if (*nbItems<1){
		Fifo_finalize(*results);
		*results=NULL;
	}	
	else{
		if(stringLength(*extend)!=0){
			Fifo_finalize(*results);
			*results=NULL;
		}
		else{
			*extend=NULL;
		}
	}
	return 0;
	//return provided_autocomplete(prefix, limit, nbItems, extend, results);
}
