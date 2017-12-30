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

    /// \file header.hpp
    /// \brief slice header structure is defined here
    /// \ingroup Private


#ifndef HEADER_HPP
#define HEADER_HPP

#include "../my_config.h"

#include "infinint.hpp"
#include "generic_file.hpp"
#include "user_interaction.hpp"
#include "tlv_list.hpp"
#include "label.hpp"

namespace libdar
{

	/// \addtogroup Private
	/// @{

    const U_32 SAUV_MAGIC_NUMBER = 123; // Why "SAUV_..." because SAUV was the name of DAR much before its first release :-)

    typedef U_32 magic_number;

    enum flag_type
    {
	flag_type_terminal = 'T',
	flag_type_non_terminal = 'N',
	flag_type_located_at_end_of_slice = 'E' // since archive format version 8
    };


	/// this class manages the header of each slice

	/// this class was a struct before release 2.4.0, now promoted
	/// to a class it hides the fields and uses TLV to store the
	/// fields in the archive. This makes the header easier to
	/// extend by adding new fields, while letting a chance for the
	/// old implementation to be able to use more recent archives
	/// the main use of TLV is to handle optional fields easily.

    class header
    {
    public:
	    // constructors & Co.

        header();
        header(const header & ref) { copy_from(ref); };
	header(header && ref) noexcept { nullifyptr(); move_from(std::move(ref)); };
        header & operator = (const header & ref) { free_pointers(); copy_from(ref); return *this; };
	header & operator = (header && ref) noexcept { move_from(std::move(ref)); return *this; };
	~header() { free_pointers(); };

	    // global methods

        void read(user_interaction & ui, generic_file & f, bool lax = false );
        void write(user_interaction &, generic_file & f) const;

	    /// minimal size of a header in an archive

	    /// \return min size of a header once stored in an archive
	    /// \note since release 2.4.0 the header used for each slice is exactly the same.
	    /// before this release the header of the first slice might be bigger, it was known that
	    /// the size of the other header was "min_size" this let dar be able to find the proper
	    /// slice for a given position. For compatibility with older DAR format, it is thus important
	    /// to not change the value returned by this class method. This call is only used
	    /// when reading archive generated by old versions of dar < 2.4.0. (aka archive format <= 7)
        static U_I min_size() { return sizeof(magic_number) + label::common_size() + 2*sizeof(char); };


	    // fields access methods

	magic_number & get_set_magic() { return magic; };
	label & get_set_internal_name() { return internal_name; };
	char & get_set_flag() { return flag; };
	label & get_set_data_name() { return data_name; };

	bool get_first_slice_size(infinint & size) const;
	void set_first_slice_size(const infinint & size);
	void unset_first_slice_size() { if(first_size != nullptr) { delete first_size; first_size = nullptr; } };

	bool get_slice_size(infinint & size) const;
	void set_slice_size(const infinint & size);
	void unset_slice_size() { if(slice_size != nullptr) { delete slice_size; slice_size = nullptr; } };

	bool is_old_header() const { return old_header; };
	void set_format_07_compatibility() { old_header = true; };

    private:
        magic_number magic;    //< constant string for all Dar archives
        label internal_name;   //< constant string for all slices of a given archive (computed based on date and pid)
	label data_name;       //< constant string for a set of data (constant with dar_xform, used to link isolated catalogue to its original data)
        char flag;             //< whether slice is the last of the archive or not
        infinint *first_size;  //< size of the first slice
	infinint *slice_size;  //< size of slices (except first slice if specified else and last if not fulfilled)
	bool old_header;       //< true if the header has been read from an old archive (before release 2.4.0, format 07 and below) and if true when writing, create an old slice header (compatible with format 07).

	void nullifyptr() noexcept { first_size = slice_size = nullptr; };
        void copy_from(const header & ref);
	void move_from(header && ref) noexcept;
	void free_pointers();
	void fill_from(user_interaction & ui, const tlv_list & list);
	tlv_list build_tlv_list(user_interaction & ui) const;
    };

	/// @}

} // end of namespace

#endif

