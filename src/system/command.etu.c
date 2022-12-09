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
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_DEL_0(CmdMember)

/*
CmdMember* CmdMember_new(const char *base) :

Retourne un membre de commande simple alloué sur le tas. Pour des raisons techniques liées à bison, CmdMember_new appele CmdMember_addLivingCmdMember et lui transmet l'adresse de cette variable.
*/

CmdMember *IMPLEMENT(CmdMember_new)(const char *base)
{
	// MAKE_NEW is not sufficient here because new variables must
	// be registered using the CmdMember_addLivingCmdMember function.

	CmdMember *nmbr = malloc(sizeof(CmdMember));
	if (nmbr)
	{
		if (CmdMember_init(nmbr, base))
		{
			free(nmbr);
			return NULL;
		}
		if (CmdMember_addLivingCmdMember(nmbr))
		{
			CmdMember_delete(nmbr);
			return NULL;
		}
	}
	return nmbr;

	// return provided_CmdMember_new(base);
}

/*
int CmdMember_init(CmdMember *mbr, const char *base) :

Initialise un membre de commande simple (sans redirection ni option supplémentaire).
*/

int IMPLEMENT(CmdMember_init)(CmdMember *mbr, const char *base)
{
	mbr->status = 1;
	mbr->base = duplicateString(base); // MALLOC
	if (mbr->base == NULL)
	{
		return 1;
	}
	mbr->capacityOption = 4;
	mbr->options = malloc(4 * sizeof(char *)); // MALLOC
	if (mbr->options == NULL)
	{
		free(mbr->base); // FREE suite au duplicateString
		return 1;
	}
	mbr->nbOptions = 1;
	mbr->options[0] = duplicateString(base); // MALLOC
	if (mbr->options[0] == NULL)
	{
		free(mbr->base);	// FREE suite au duplicateString
		free(mbr->options); // FREE suite au duplicateString
		return 1;
	}
	for (int fd = 0; fd < 3; fd++)
	{
		mbr->redirections[fd] = NULL;
		mbr->redirectionTypes[fd] = UNDEFINED;
	}
	mbr->next = NULL;
	mbr->prev = NULL;
	return 0;
	// return provided_CmdMember_init(mbr, base);
}

/*
void CmdMember_finalize(CmdMember *mbr) :

Libère les ressources allouées par un membre de commande donné.
*/

// Ignorer dans un premier temps (à faire plus tard)
void IMPLEMENT(CmdMember_finalize)(CmdMember *mbr)
{
	// TODO CmdMember_finalize
	provided_CmdMember_finalize(mbr);
}

/*
CmdMember* CmdMember_redirect(CmdMember *mbr, int fd, const char *filename) :

Met en place une redirection du flot fd vers le fichier filename (<, > ou 2>).
*/

CmdMember *IMPLEMENT(CmdMember_redirect)(CmdMember *mbr, int fd, const char *filename)
{
	assert(mbr && filename && fd >= 0 && fd <= 2);
	if (mbr->status)
	{
		char *path = prependHomeDir(duplicateString(filename)); // MALLOC
		if (!path)
			mbr->status = 0;
		else
		{
			if (mbr->redirections[fd])
			{
				free(mbr->redirections[fd]);
			}
			mbr->redirections[fd] = path;
			mbr->redirectionTypes[fd] = NORMAL;
		}
	}
	return mbr;
	// return provided_CmdMember_redirect(mbr, fd, filename);
}

/*
CmdMember* CmdMember_appendRedirect(CmdMember *mbr, int fd, const char *filename) :

Redirige le flot fd vers la fin du fichier filename (>> ou 2>>).
*/

CmdMember *IMPLEMENT(CmdMember_appendRedirect)(CmdMember *mbr, int fd, const char *filename)
{
	assert(mbr && filename && fd >= 1 && fd <= 2);
	if (mbr->status)
	{
		char *path = prependHomeDir(duplicateString(filename)); // MALLOC
		if (!path)
			mbr->status = 0;
		else
		{
			if (mbr->redirections[fd])
			{
				free(mbr->redirections[fd]);
			}
			mbr->redirections[fd] = path;
			mbr->redirectionTypes[fd] = APPEND;
		}
	}
	return mbr;
	// return provided_CmdMember_appendRedirect(mbr, fd, filename);
}

/*
CmdMember* CmdMember_mergeOutputs(CmdMember *mbr) :

Redirige la sortie d'erreur (fusionne) vers la sortie standard (2>&1).
*/

CmdMember *IMPLEMENT(CmdMember_mergeOutputs)(CmdMember *mbr)
{
	assert(mbr);
	if (mbr->status)
	{
		mbr->redirections[2] = NULL;
		mbr->redirectionTypes[2] = FUSION;
	}
	return mbr;
	// return provided_CmdMember_mergeOutputs(mbr);
}

/*
CmdMember* CmdMember_pipe(CmdMember *m1, CmdMember *m2) :

Met en place un pipe entre m1 et m2, puis renvoie m2. Si m1 ou m2 est dans un état d'erreur, alors m1 et m2 sont tous les deux mis dans un état d'erreur. Idem s'ils ne peuvent être chaînés sans défaire un lien existant.
*/

CmdMember *IMPLEMENT(CmdMember_pipe)(CmdMember *m1, CmdMember *m2)
{
	assert(m1 && m2);
	if (m1->status && m2->status && m1->next == NULL && m2->prev == NULL)
	{
		m1->next = m2;
		m2->prev = m1;
	}
	else
	{
		m1->status = 0;
		m2->status = 0;
	}
	return m2;
	// return provided_CmdMember_pipe(m1, m2);
}

/*
CmdMember* CmdMember_addOption(CmdMember *mbr, const char *option, int expend) :

Ajoute l'option "option" à mbr. expend indique si les astérisques de option doivent être interprétés ou non. Traiter les cas simples (ls *.backup) avant les cas complexes (cas avec de nombreuses étoiles réparties dans la commande). Il doit être possible d'ajouter la chaîne NULL à la liste des options de mbr.
*/

/* La fonction est notée en fonction de niveaux à valider via les tests unitaires, pas grave de laisser des tests unitaires qui ne marchent pas sur cette fonction
Niveaux 1 : Facile (gestion tableau dynamique)
Niveaux 2 et 3 sont assez facultatifs (à faire à la fin si le temps)
Niveau 2 : Difficile (adaptation autocomplete) "*.log" Chercher dans le repertoire courant tout ce qui finit par log et ajouter dans le tableau (toujours une seule étoile au début ou à la fin, possibilité de contourner avec startWith ... (ou regex "*.log" -> ".*\.log"))
Niveau 3 : TRES DIFFICILE (plus difficile du projet : Le Challenge) */

CmdMember *IMPLEMENT(CmdMember_addOption)(CmdMember *mbr, const char *option, int expend)
{
	// TODO CmdMember_addOption (minimum level 1)
	return provided_CmdMember_addOption(mbr, option, expend);
}

/*
size_t Command_getNbMember(const Command *cmd) :

Retourne le nombre total de membres d'une commande donnée.
*/

size_t IMPLEMENT(Command_getNbMember)(const Command *cmd)
{
	// TODO Command_getNbMember
	return provided_Command_getNbMember(cmd);
}

/* L'* est deja gérée par le parser (on a deja remplacé)
Options > pipes > redirections */

// Ne compte pas dans la partie Projet Tutoré 1
int IMPLEMENT(Command_execute)(Command *cmd)
{
	size_t nbM = Command_getNbMember(cmd);
	int pipeG[2] = {-1, -1};
	int pipeD[2] = {-1, -1};
	while (cmd)
	{
		if (cmd->next != NULL)
		{
			pipe(pipeD);
		}
		pid_t pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return 1;
		}
		else if (pid == 0)
		{
			if (cmd->prev != NULL)
			{
				close(0);
				dup(pipeG[0]);
				close(pipeG[0]);
				close(pipeG[1]);
			}
			// Redirection de l'entrée standard
			else if (cmd->redirectionTypes[0] != UNDEFINED)
			{
				int r;
				// Redirection < (entrée std NORMAL)
				if (cmd->redirectionTypes[0] == NORMAL)
				{
					r = open(cmd->redirections[0], O_RDONLY, 0644);
				}
				if (r == -1)
				{
					perror(" open");
					exit(1);
				}
				close(0);
				dup(r);
				close(r);
			}
			if (cmd->next != NULL)
			{
				close(1);
				dup(pipeD[1]);
				close(pipeD[1]);
				close(pipeD[0]);
			}
			// Redirection de la sortie standard
			else if (cmd->redirectionTypes[1] != UNDEFINED)
			{
				int r;
				// Redirection > (sortie std NORMAL)
				if (cmd->redirectionTypes[1] == NORMAL)
				{
					r = open(cmd->redirections[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				}
				// Redirection >> (sortie std APPEND)
				if (cmd->redirectionTypes[1] == APPEND)
				{
					r = open(cmd->redirections[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
				}
				if (r == -1)
				{
					perror(" open");
					exit(1);
				}
				close(1);
				dup(r);
				close(r);
			}
			// Redirection de la sortie standard d'erreur
			if (cmd->redirectionTypes[2] != UNDEFINED)
			{
				int r;
				// Redirection > (sortie std d'erreur NORMAL)
				if (cmd->redirectionTypes[2] == NORMAL)
				{
					r = open(cmd->redirections[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				}
				// Redirection >> (sortie std d'erreur APPEND)
				if (cmd->redirectionTypes[2] == APPEND)
				{
					r = open(cmd->redirections[2], O_WRONLY | O_CREAT | O_APPEND, 0644);
				}
				if (r == -1)
				{
					perror(" open");
					exit(1);
				}
				close(2);
				dup(r);
				close(r);
			}
			cmd = CmdMember_addOption(cmd, NULL, 0);
			execvp(cmd->base, cmd->options);
			perror("execvp");
			exit(1);
		}
		close(pipeG[0]);
		close(pipeG[1]);
		pipeD[0] = pipeG[0];
		pipeD[1] = pipeG[1];
		cmd = cmd->next;
	}
	for (size_t i = 1; i <= nbM; i++)
	{
		wait(NULL);
	}
	return 0;

	/* if (cmd->status)
	{
		pid_t pid = fork();
		switch (pid)
		{
		case -1:
			perror("fork");
			return 1;
		case 0:
			// Redirection > (sortie std NORMAL)
			if (cmd->redirectionTypes[1] == NORMAL)
			{
				int r = open(cmd->redirections[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (r == -1)
				{
					perror(" open");
					exit(1);
				}
				close(1);
				dup(r);
				close(r);
			}
			cmd = CmdMember_addOption(cmd, NULL, 0);
			execvp(cmd->base, cmd->options);
			perror("execvp");
			exit(1);
		default:
			wait(NULL);
		}
		return 0;
	}
	else
	{
		return 1;
	} */
	// return provided_Command_execute(cmd);
}
