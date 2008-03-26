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
#ifndef PERSISTENCEMANAGER_H
#define PERSISTENCEMANAGER_H

#define PERSISTENCE_PATH "/var/ofs"
#include <confuse.h>
#include <string>
using namespace std;

/**
	@author Tobias Jaehnel <tjaehnel@gmail.com>
	
	Manages files, which hold persisent data.
	Each subclass gets its own file to store the data
*/
class PersistenceManager{
public:

    virtual ~PersistenceManager();
    void make_persistent();
    void reload();
    string get_modname();

protected:
    PersistenceManager(string modname);
    virtual cfg_opt_t *init_parser() = 0;
    virtual string get_persistence() = 0;
    virtual void read_values() = 0;
    string get_filename();
    void init();

protected:
    cfg_opt_t *opts;
    cfg_t *cfg;

private:
    string filename;
    string modname;
};

#endif