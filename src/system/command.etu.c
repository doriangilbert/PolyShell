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

#include "system/command.h"
#include "misc/string.h"
#include "interactive/autocomplete.h"
#include <assert.h>

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_DEL_0(CmdMember)

CmdMember* IMPLEMENT(CmdMember_new)(const char *base)
{
    // MAKE_NEW is not sufficient here because new variables must
    // be registered using the CmdMember_addLivingCmdMember function.
    
	CmdMember *nmbr = malloc(sizeof(CmdMember));
	if (nmbr) {
		if (CmdMember_init(nmbr, base)) {
			free(nmbr);
			return NULL;
		}
		if (CmdMember_addLivingCmdMember(nmbr)) {
			CmdMember_delete(nmbr);
			return NULL;
		}
	}
	return nmbr;
	
	//return provided_CmdMember_new(base);
}

int IMPLEMENT(CmdMember_init)(CmdMember *mbr, const char *base)
{
    mbr->status = 1;
	mbr->base = duplicateString(base); //MALLOC
	if (mbr->base == NULL) {
		return 1;
	}
	mbr->capacityOption = 4;
	mbr->options = malloc(4*sizeof(char*)); //MALLOC
	if (mbr->options == NULL) {
		free(mbr->base); //FREE suite au duplicateString
		return 1;
	}
	mbr->nbOptions = 1;
	mbr->options[0] = duplicateString(base); //MALLOC
	if (mbr->options[0] == NULL) {
		free(mbr->base); //FREE suite au duplicateString
		free(mbr->options); //FREE suite au duplicateString
		return 1;
	}
	for (int fd=0 ; fd < 3 ; fd++) {
		mbr->redirections[fd] = NULL;
		mbr->redirectionTypes[fd] = UNDEFINED;
	}
	mbr->next = NULL;
	mbr->prev = NULL;
	return 0;
	//return provided_CmdMember_init(mbr, base);
}

//Ignorer dans un premier temps (à faire plus tard)
void IMPLEMENT(CmdMember_finalize)(CmdMember *mbr)
{
    provided_CmdMember_finalize(mbr);
}

CmdMember* IMPLEMENT(CmdMember_redirect)(CmdMember *mbr, int fd, const char *filename)
{
    assert(mbr && filename && fd>=0 && fd<=2);
	if (mbr->status){
		char *path=prependHomeDir(duplicateString(filename)); //MALLOC
		if (!path) mbr->status=0;
		else
		{
			if(mbr->redirections[fd]){
				free(mbr->redirections[fd]);
			}
			mbr->redirections[fd]=path;
			mbr->redirectionTypes[fd]=NORMAL;
		}
	}
	return mbr;
	//return provided_CmdMember_redirect(mbr, fd, filename);
}

CmdMember* IMPLEMENT(CmdMember_appendRedirect)(CmdMember *mbr, int fd, const char *filename)
{
    assert(mbr && filename && fd>=1 && fd<=2);
	if (mbr->status){
		char *path=prependHomeDir(duplicateString(filename)); //MALLOC
		if (!path) mbr->status=0;
		else
		{
			if(mbr->redirections[fd]){
				free(mbr->redirections[fd]);
			}
			mbr->redirections[fd]=path;
			mbr->redirectionTypes[fd]=APPEND;
		}
	}
	return mbr;
	//return provided_CmdMember_appendRedirect(mbr, fd, filename);
}

CmdMember* IMPLEMENT(CmdMember_mergeOutputs)(CmdMember *mbr)
{
    assert(mbr);
	if (mbr->status){
		mbr->redirections[2]=NULL;
		mbr->redirectionTypes[2]=FUSION;
	}
	return mbr;
	//return provided_CmdMember_mergeOutputs(mbr);
}

CmdMember* IMPLEMENT(CmdMember_pipe)(CmdMember *m1, CmdMember *m2)
{
    assert(m1 && m2);
	if (m1->status && m2->status && m1->next==NULL && m2->prev==NULL){
		m1->next=m2;
		m2->prev=m1;
	}
	else{
		m1->status=0;
		m2->status=0;
	}
	return m2;
	//return provided_CmdMember_pipe(m1, m2);
}

/* La fonction est notée en fonction de niveaux à valider via les tests unitaires, pas grave de laisser des tests unitaires qui ne marchent pas sur cette fonction
Niveaux 1 : Facile (gestion tableau dynamique)
Niveaux 2 et 3 sont assez facultatifs (à faire à la fin si le temps)
Niveau 2 : Difficile (adaptation autocomplete) "*.log" Chercher dans le repertoire courant tout ce qui finit par log et ajouter dans le tableau (toujours une seule étoile au début ou à la fin, possibilité de contourner avec startWith ... (ou regex "*.log" -> ".*\.log"))
Niveau 3 : TRES DIFFICILE (plus difficile du projet : Le Challenge) */

CmdMember* IMPLEMENT(CmdMember_addOption)(CmdMember *mbr, const char *option, int expend)
{
    return provided_CmdMember_addOption(mbr, option, expend);
}

size_t IMPLEMENT(Command_getNbMember)(const Command *cmd)
{
    return provided_Command_getNbMember(cmd);
}

int IMPLEMENT(Command_execute)(Command *cmd)
{
    return provided_Command_execute(cmd);
}
