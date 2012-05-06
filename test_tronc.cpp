/*********************************************************************/
// dar - disk archive - a backup/restoration program
// Copyright (C) 2002 Denis Corbin
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// to contact the author : dar.linux@free.fr
/*********************************************************************/
// $Id: test_tronc.cpp,v 1.10 2002/05/28 20:17:29 denis Rel $
//
/*********************************************************************/

#include <iostream.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tronc.hpp"
#include "deci.hpp"
#include "testtools.hpp"

int main()
{
    path p = "test/source.txt";
    fichier h = fichier(p, gf_read_only);
    display_read(h);
    
    try
    {
	fichier f = fichier("test/source.txt", gf_read_only);
	tronc *t;
	
	t = new tronc(&f, 0, 10);
	deci d = t->get_position();
	cout << d.human() << endl;
	d = f.get_position();
	cout << d.human() << endl;
	
	display_read(*t);
	d = t->get_position();
	cout << d.human() << endl;
	d = f.get_position();
	cout << d.human() << endl;
	
	display_read(*t);
	d = t->get_position();
	cout << d.human() << endl;
	d = f.get_position();
	cout << d.human() << endl;
	
	delete t;
	t = new tronc(&f, 50, 5);
	d = t->get_position();
	cout << d.human() << endl;
	d = f.get_position();
	cout << d.human() << endl;

	display_read(*t);
	d = t->get_position();
	cout << d.human() << endl;
	d = f.get_position();
	cout << d.human() << endl;

	delete t;
	int fd = open("test/destination.txt", O_RDWR|O_CREAT|O_TRUNC);
	fichier g = fd;
	f.skip(0);
	f.copy_to(g);
	t = new tronc(&g, 10, 10);

	try
	{
	    f.skip(0);
	    f.copy_to(*t);
	}
	catch(Egeneric & e)
	{
	    e.dump();
	}

	t->skip_to_eof();
	display_back_read(*t);
	display_back_read(*t);	
	g.skip(0);
	display_read(g);
	display_read(g);
	
	t->skip_relative(-5);
	display_read(*t);
	t->skip(3);
	display_read(*t);
	t->skip_relative(2);
	display_read(*t);

	delete t;
    }
    catch(Egeneric &f)
    {
	f.dump();
    }
}

    
    
