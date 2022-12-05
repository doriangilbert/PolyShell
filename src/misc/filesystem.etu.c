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

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_2(FolderIterator, const char*, int)
MAKE_DEL_0(FolderIterator)

int IMPLEMENT(FolderIterator_init)(FolderIterator *fIterator, const char *path, int skipSpecials)
{
    return provided_FolderIterator_init(fIterator, path, skipSpecials);
}

void IMPLEMENT(FolderIterator_finalize)(FolderIterator *fIterator)
{
    provided_FolderIterator_finalize(fIterator);
}

int IMPLEMENT(FolderIterator_isOver)(const FolderIterator *fIterator)
{
    return provided_FolderIterator_isOver(fIterator);
}

const char* IMPLEMENT(FolderIterator_get)(const FolderIterator *fIterator)
{
    return provided_FolderIterator_get(fIterator);
}

int IMPLEMENT(FolderIterator_isDir)(const FolderIterator *fIterator)
{
    return provided_FolderIterator_isDir(fIterator);
}

void IMPLEMENT(FolderIterator_next)(FolderIterator *fIterator)
{
    provided_FolderIterator_next(fIterator);
}

MAKE_NEW_1(FileIterator, FILE*)
MAKE_DEL_0(FileIterator)

//FileIterator permet notamment de gérer les commentaires (ignorer les lignes qui débutent par #) avec getRealString (et firstNotEscaped)

int IMPLEMENT(FileIterator_init)(FileIterator *fIterator, FILE *file)
{
    return provided_FileIterator_init(fIterator, file);
}

void IMPLEMENT(FileIterator_finalize)(FileIterator *fIterator)
{
    provided_FileIterator_finalize(fIterator);
}

int IMPLEMENT(FileIterator_isOver)(const FileIterator *fIterator)
{
    return provided_FileIterator_isOver(fIterator);
}

const char* IMPLEMENT(FileIterator_get)(const FileIterator *fIterator)
{
    return provided_FileIterator_get(fIterator);
}

void IMPLEMENT(FileIterator_next)(FileIterator *fIterator)
{
    provided_FileIterator_next(fIterator);
}
