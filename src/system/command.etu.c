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

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_DEL_0(CmdMember)

CmdMember* IMPLEMENT(CmdMember_new)(const char *base)
{
    // MAKE_NEW is not sufficient here because new variables must
    // be registered using the CmdMember_addLivingCmdMember function.
    return provided_CmdMember_new(base);
}

int IMPLEMENT(CmdMember_init)(CmdMember *mbr, const char *base)
{
    return provided_CmdMember_init(mbr, base);
}

void IMPLEMENT(CmdMember_finalize)(CmdMember *mbr)
{
    provided_CmdMember_finalize(mbr);
}

CmdMember* IMPLEMENT(CmdMember_redirect)(CmdMember *mbr, int fd, const char *filename)
{
    return provided_CmdMember_redirect(mbr, fd, filename);
}

CmdMember* IMPLEMENT(CmdMember_appendRedirect)(CmdMember *mbr, int fd, const char *filename)
{
    return provided_CmdMember_appendRedirect(mbr, fd, filename);
}

CmdMember* IMPLEMENT(CmdMember_mergeOutputs)(CmdMember *mbr)
{
    return provided_CmdMember_mergeOutputs(mbr);
}

CmdMember* IMPLEMENT(CmdMember_pipe)(CmdMember *m1, CmdMember *m2)
{
    return provided_CmdMember_pipe(m1, m2);
}

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
