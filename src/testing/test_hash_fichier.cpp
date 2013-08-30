/*********************************************************************/
// dar - disk archive - a backup/restoration program
// Copyright (C) 2002-2052 Denis Corbin
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
// to contact the author : http://dar.linux.free.fr/email.html
/*********************************************************************/

#include "../my_config.h"

extern "C"
{
#if HAVE_FCNTL_H
#include <fcntl.h>
#endif
} // end extern "C"

#include "libdar.hpp"
#include "hash_fichier.hpp"
#include "shell_interaction.hpp"
#include "fichier_local.hpp"

using namespace libdar;

void f1(const string & filename, hash_algo algo);

static user_interaction *ui = NULL;

int main()
{
    U_I maj, med, min;

    get_version(maj, med, min);
    ui = shell_interaction_init(&cout, &cerr, false);
    if(ui == NULL)
	cout << "ERREUR !" << endl;
    try
    {
	f1("md5-toto", hash_md5);
	f1("sha1-toto", hash_sha1);
    }
    catch(...)
    {
	if(ui != NULL)
	    delete ui;
	throw;
    }
    if(ui != NULL)
	delete ui;
}

void f1(const string & filename, hash_algo algo)
{
    fichier_local fic = fichier_local(*ui, filename, gf_write_only, 0644, false, true, false);
    fichier_local hash_fic = fichier_local(*ui, filename + "." + hash_algo_to_string(algo), gf_write_only, 0644, false, true, false);
    const string message = "bonjour les amis, il fait chaud il fait beau, les mouches pettent et les cailloux fleurissent.";
    hash_fichier dst = hash_fichier(*ui, &fic, filename, &hash_fic, algo);

    dst.write(message.c_str(), message.size());
}
