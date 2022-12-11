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
#include <errno.h>
#include <sys/wait.h>
#include "system/info.h"

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

void IMPLEMENT(CmdMember_finalize)(CmdMember *mbr)
{
	assert(mbr);
	free(mbr->base);
	for (unsigned i = 0; i < mbr->nbOptions; i++)
	{
		free(mbr->options[i]);
	}
	free(mbr->options);
	for (int fd = 0; fd < 3; fd++)
	{
		free(mbr->redirections[fd]);
	}
	// provided_CmdMember_finalize(mbr);
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
	assert(cmd);
	size_t cpt = 1;
	while (cmd->next != NULL)
	{
		cpt++;
		cmd = cmd->next;
	}
	return cpt;
	// return provided_Command_getNbMember(cmd);
}

/* L'* est deja gérée par le parser (on a deja remplacé)
Options > pipes > redirections */

// Ne compte pas dans la partie Projet Tutoré 1

static void __make_redirect__(const char *file, int flags, int fd)
{
	// on ouvre le fichier = on récupère un descriteur de fichier permettant de lire ou écrire file
	const int fdFichier = open(file, flags, 0644);
	if (fdFichier == -1)
	{
		fprintf(stderr, "Command_execute: cannot open %s\n", file);
		exit(1);
	}
	// on duplique ce descripteur de fichier et on place cette copie à la ligne fd de la table des descripteurs de fichier
	if (dup2(fdFichier, fd) == -1)
	{
		perror("Command_execute: dup2");
		exit(1);
	}
	// on ferme le descripteur de fichier original
	if (close(fdFichier))
	{
		perror("Command_execute: close");
		exit(1);
	}
}

static void __soft_fail__(int pipeGauche[2], int pipeDroite[2])
{
	if (pipeGauche[0] != -1)
	{
		close(pipeGauche[0]);
		pipeGauche[0] = -1;
	}
	if (pipeGauche[1] != -1)
	{
		close(pipeGauche[1]);
		pipeGauche[1] = -1;
	}
	if (pipeDroite[0] != -1)
	{
		close(pipeDroite[0]);
		pipeDroite[0] = -1;
	}
	if (pipeDroite[1] != -1)
	{
		close(pipeDroite[1]);
		pipeDroite[1] = -1;
	}
}

int IMPLEMENT(Command_execute)(Command *cmd)
{
	// A. COMMANDES INTÉGRÉES AU SHELL = qui ont un effet sur le shell lui-même
	// ========================================================================
	// A.1. exit
	if (!stringCompare(cmd->base, "exit"))
	{
		exit(0);
	}
	// A.2. cd
	else if (!stringCompare(cmd->base, "cd"))
	{
		if (cmd->nbOptions == 1)
		{
			const char *hd;
			userInformation(NULL, &hd, NULL);
			CmdMember_addOption(cmd, hd, 0);
			if (!cmd->status)
			{
				fprintf(stderr, "Command_execute: cd: CmdMember_addOption: failed.\n");
				return 1;
			}
		}
		else if (cmd->nbOptions != 2)
		{
			fprintf(stderr, "Command_execute: cd: invalid number of arguments.\n");
			return 1;
		}
		if (chdir(cmd->options[1]))
		{
			perror("Command_execute: cd");
			return 1;
		}
		return 0;
	}
	// B. PROGRAMMES EXTERNES = programmes situés dans les dossiers listés dans $PATH
	// ==============================================================================
	// B.1. on lance toutes les commandes
	int nbProcessCrees = 0;
	int codeErreur = 0;
	int pipeGauche[2] = {-1, -1}; // communication avec la commande qui précède
	int pipeDroite[2] = {-1, -1}; // communication avec la commande qui suit
	while (cmd)
	{
		// B.1.a. si cmd->next != NULL alors il faut créer un pipe pour communiquer avec la commande qui suit
		if (cmd->next)
		{
			if (pipe(pipeDroite))
			{
				perror("Command_execute: pipe");
				__soft_fail__(pipeGauche, pipeDroite);
				codeErreur = 1;
				break;
			}
		}
		// B.1.b. on duplique le processus courant pour faire exécuter cmd->base par un processus fils
		pid_t pid = fork();
		// erreur fork
		if (pid == -1)
		{
			perror("Command_execute: fork");
			__soft_fail__(pipeGauche, pipeDroite);
			codeErreur = 1;
			break;
		}
		// processus fils
		else if (pid == 0)
		{
			// redirection entrée
			if (cmd->prev)
			{
				// vers extremité en lecture du pipe de gauche
				if (dup2(pipeGauche[0], 0) == -1)
				{
					perror("Command_execute: dup2");
					exit(1);
				}
				if (close(pipeGauche[0]))
				{
					perror("Command_execute: close");
					exit(1);
				}
				if (close(pipeGauche[1]))
				{
					perror("Command_execute: close");
					exit(1);
				}
			}
			else if (cmd->redirectionTypes[0] == NORMAL)
			{
				__make_redirect__(cmd->redirections[0], O_RDONLY, 0);
			}
			// redirection sortie
			if (cmd->next)
			{
				// vers extremité en écriture du pipe de droite
				if (dup2(pipeDroite[1], 1) == -1)
				{
					perror("Command_execute: dup2");
					exit(1);
				}
				if (close(pipeDroite[0]))
				{
					perror("Command_execute: close");
					exit(1);
				}
				if (close(pipeDroite[1]))
				{
					perror("Command_execute: close");
					exit(1);
				}
			}
			else if (cmd->redirectionTypes[1] == NORMAL)
			{
				__make_redirect__(cmd->redirections[1], O_WRONLY | O_TRUNC | O_CREAT, 1);
			}
			else if (cmd->redirectionTypes[1] == APPEND)
			{
				__make_redirect__(cmd->redirections[1], O_WRONLY | O_APPEND | O_CREAT, 1);
			}
			// redirection erreur
			if (cmd->redirectionTypes[2] == NORMAL)
			{
				__make_redirect__(cmd->redirections[2], O_WRONLY | O_TRUNC | O_CREAT, 2);
			}
			else if (cmd->redirectionTypes[2] == APPEND)
			{
				__make_redirect__(cmd->redirections[2], O_WRONLY | O_APPEND | O_CREAT, 2);
			}
			else if (cmd->redirectionTypes[2] == FUSION)
			{
				if (dup2(1, 2) == -1)
				{
					perror("Command_execute: dup2");
					exit(1);
				}
			}
			// on remplace le programme exécuté par le processus fils (polyshell) par un nouveau (cmd->base)
			CmdMember_addOption(cmd, NULL, 0);
			if (!cmd->status)
			{
				fprintf(stderr, "Command_execute: cd: CmdMember_addOption: failed.\n");
				exit(1);
			}
			execvp(cmd->base, cmd->options);
			// on ne revient normalement pas de execvp ---> exécuter ces lignes == erreur
			perror("Command_execute: excevp");
			exit(1);
		}
		// processus parent
		else
		{
			++nbProcessCrees;
		}
		// B.1.c. itération suivante
		// pipe de gauche ne sera plus utile dans la suite
		if (cmd->prev)
		{
			if (close(pipeGauche[0]))
			{
				perror("Command_execute: close");
				__soft_fail__(pipeGauche, pipeDroite);
				codeErreur = 1;
				break;
			}
			if (close(pipeGauche[1]))
			{
				perror("Command_execute: close");
				__soft_fail__(pipeGauche, pipeDroite);
				codeErreur = 1;
				break;
			}
		}
		// le pipe de droite à itération i == le pipe de gauche à itération i + 1
		pipeGauche[0] = pipeDroite[0];
		pipeGauche[1] = pipeDroite[1];
		pipeDroite[0] = -1;
		pipeDroite[1] = -1;
		cmd = cmd->next;
	}
	// B.2. attente de la terminaison des commandes
	int nbProcessTermines = 0;
	while (nbProcessTermines < nbProcessCrees)
	{
		int status;
		if (wait(&status) != -1)
		{
			++nbProcessTermines;
			if (!WIFEXITED(status) || WEXITSTATUS(status))
			{
				codeErreur = 1;
			}
		}
	}
	// B.3. on retourne 0 si et seulement si tout est OK
	return codeErreur;

	/*size_t nbM = Command_getNbMember(cmd);
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
	return 0;*/

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
