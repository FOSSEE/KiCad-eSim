#ifndef __DIALOG_SELECT_PRETTY_LIB_H__
#define __DIALOG_SELECT_PRETTY_LIB_H__

/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2014 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 1992-2014 KiCad Developers, see change_log.txt for contributors.
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file dialog_select_pretty_lib.h
 * @brief A dialog to select/create a .pretty folder which is a
 * footprint library.
 * footprints are .kicad_mod files inside this folder
 */

#include <dialog_select_pretty_lib_base.h>


class DIALOG_SELECT_PRETTY_LIB : public DIALOG_SELECT_PRETTY_LIB_BASE
{
public:
    DIALOG_SELECT_PRETTY_LIB( wxWindow* parent,
                              const wxString& aDefaultPath );
    ~DIALOG_SELECT_PRETTY_LIB() {};

    /**
     * @return the full .pretty lib name, which is an absolute path
     *, ending wityh ".pretty"
     */
    const wxString GetFullPrettyLibName();

private:
    virtual void OnSelectFolder( wxFileDirPickerEvent& event );
	virtual void OnOKButton( wxCommandEvent& event );
};


#endif //__DIALOG_SELECT_PRETTY_LIB_BASE_H__
