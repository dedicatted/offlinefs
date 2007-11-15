/***************************************************************************
 *   Copyright (C) 2007 by                                                 *
 *                 Frank Gsellmann, Tobias Jaehnel, Carsten Kolassa        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef FILESYSTEMSTATUSMANAGER_H
#define FILESYSTEMSTATUSMANAGER_H
#include "mutexlocker.h"
#include "filesystem.h"
#include <string>
/**
	@author Carsten Kolassa <Carsten@Kolassa.de>
*/
class FilesystemStatusManager{
public:
    static FilesystemStatusManager& Instance();
    ~FilesystemStatusManager(); 
    register_filesystem(string MountPath, string TempMount);
    remove_filesystem(string MountPath, string TempMount);
    Filesystem give_me_Filesystem_to_File(string Path);
protected:
    FilesystemStatusManager();
  private:
    static std::auto_ptr<FilesystemStatusManager> theFilesystemStatusManagerInstance;
	vector<Filesystem> fslist;

    static Mutex m; 
};
#endif