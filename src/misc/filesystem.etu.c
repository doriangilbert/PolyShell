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

#include "misc/filesystem.h"
#include "misc/string.h"

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_2(FolderIterator, const char *, int)
MAKE_DEL_0(FolderIterator)

/*
Les fonctions FolderIterator permettent d'explorer le contenu d'un dossier.
*/

/*Utilisation :
FolderIterator it;
if (FolderIterator_init(&it, "/home/ronan", 0)) {
	printf("Erreur init!");
	exit(1);
}
while (!FolderIterator_isOver(&it)) {
	printf("%s\n", FolderIterator_get(&it));
	FolderIterator_next(&it);
}
FolderIterator_finalize(&it);
exit(0);*/

/* L'argument path ne contient pas de . ou de .. , il s'agit deja d'un chemin absolu (pas de prependHomeDir)
Ouvrir avec opendir()
Lire la première entrée dans init avec FolderIterator_next
Si skipSpecials est vrai (il faut ignorer les sous dossiers . et ..) et qu'il y a . ou .. en sortie de readDir il faut refaire un ou plusieurs readdir pour passer au sous dossier suivant*/

int IMPLEMENT(FolderIterator_init)(FolderIterator *fIterator, const char *path, int skipSpecials)
{
	fIterator->skipSpecials = skipSpecials;
	if (path)
	{
		DIR *dossier = opendir(path);
		if (dossier)
		{
			fIterator->dir = dossier;
			fIterator->ent = NULL;
			FolderIterator_next(fIterator);
			return 0;
		}
	}
	return 1;

	// return provided_FolderIterator_init(fIterator, path, skipSpecials);
}

void IMPLEMENT(FolderIterator_finalize)(FolderIterator *fIterator)
{
	fIterator->skipSpecials = 0;
	closedir(fIterator->dir);
	fIterator->ent = NULL;
	// provided_FolderIterator_finalize(fIterator);
}

int IMPLEMENT(FolderIterator_isOver)(const FolderIterator *fIterator)
{
	return fIterator->ent == NULL;
	// return provided_FolderIterator_isOver(fIterator);
}

const char *IMPLEMENT(FolderIterator_get)(const FolderIterator *fIterator)
{
	if (!FolderIterator_isOver(fIterator))
	{
		return fIterator->ent->d_name;
	}
	return "";
	// return provided_FolderIterator_get(fIterator);
}

int IMPLEMENT(FolderIterator_isDir)(const FolderIterator *fIterator)
{
	if(!FolderIterator_isOver(fIterator)){
		if (fIterator->ent->d_type == DT_DIR) return 1;
	}
	return 0;
	//return provided_FolderIterator_isDir(fIterator);
}

void IMPLEMENT(FolderIterator_next)(FolderIterator *fIterator)
{
	// TODO FolderIterator_next
	/*struct dirent* m=readdir(fIterator->dir);
	if(m!=NULL){
		fIterator->ent=m;
		if(fIterator->skipSpecials){
			while(!FolderIterator_isOver(fIterator) && (stringCompare(fIterator->ent->d_name,".") || stringCompare(fIterator->ent->d_name,".."))){
				fIterator->ent=readdir(fIterator->dir);
			}
		}
	}*/
	provided_FolderIterator_next(fIterator);
}

MAKE_NEW_1(FileIterator, FILE *)
MAKE_DEL_0(FileIterator)

/*
Les fonctions FileIterator permettent de lire un fichier texte (une ligne à la fois). Cet itérateur prend en charge les commentaires (tout texte situé après une dièse non doublée est donc supprimé). Les lignes vides sont conservées.
*/

// FileIterator permet notamment de gérer les commentaires (ignorer les lignes qui débutent par #) avec getRealString (et firstNotEscaped)

/* 1) initialiser init et current
2) Lire une première ligne (Conseil : Appeler next) (on doit pouvoir appeler get si isOver=false) */

int IMPLEMENT(FileIterator_init)(FileIterator *fIterator, FILE *file)
{
	if (file)
	{
		fIterator->file = file;
		fIterator->current = NULL;
		FileIterator_next(fIterator);
		return 0;
	}
	return 1;
	// return provided_FileIterator_init(fIterator, file);
}

void IMPLEMENT(FileIterator_finalize)(FileIterator *fIterator)
{
	if (fIterator->current)
	{
		free(fIterator->current);
	}
	// provided_FileIterator_finalize(fIterator);
}

// Dernière ligne atteinte
int IMPLEMENT(FileIterator_isOver)(const FileIterator *fIterator)
{
	return feof(fIterator->file);
	// return provided_FileIterator_isOver(fIterator);
}

// Renvoie current
const char *IMPLEMENT(FileIterator_get)(const FileIterator *fIterator)
{
	return fIterator->current;
	// return provided_FileIterator_get(fIterator);
}

// Changer current pour qu'il contienne une nouvelle ligne

/* - Lire une ligne (Conseil : Utiliser fgets)
- Gérer les commentaires : - getRealString - *firstNotEscaped = '\0'; si non NULL
Simplifier tous les ## et on détecte avec firstNotEscape si il y a un # tout seul il s'agit d'un commentaire
char *firstNotEscaped;
char *res = getRealString(line, '#', &firstNotEscaped)
if (firstNotEscaped) *firstNotEscaped = '\0' (on supprime tout ce qu'il y a après le # seul) */

void IMPLEMENT(FileIterator_next)(FileIterator *fIterator)
{
	// TODO FileIterator_next
	provided_FileIterator_next(fIterator);
}
