/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2015 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 1992-2015 KiCad Developers, see change_log.txt for contributors.
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
 * @file dialog_select_pretty_lib.cpp
 * @brief A dialog to select/create a .pretty folder which is a
 * footprint library.
 * footprints are .kicad_mod files inside this folder
 */

#include <wildcards_and_files_ext.h>
#include <dialog_select_pretty_lib.h>
#include <project.h>


DIALOG_SELECT_PRETTY_LIB::DIALOG_SELECT_PRETTY_LIB( wxWindow* parent,
                                const wxString& aDefaultPath ) :
                                DIALOG_SELECT_PRETTY_LIB_BASE( parent )
{
    if( !aDefaultPath.IsEmpty() )
        m_dirCtrl->SetPath( aDefaultPath );

    m_sdbSizerOK->SetDefault();

    FixOSXCancelButtonIssue();

    // Now all widgets have the size fixed, call FinishDialogSettings
    FinishDialogSettings();
}


void DIALOG_SELECT_PRETTY_LIB::OnSelectFolder( wxFileDirPickerEvent& event )
{
    m_libName->SetValue( m_dirCtrl->GetPath() );

    event.Skip();
}


const wxString DIALOG_SELECT_PRETTY_LIB::GetFullPrettyLibName()
{
    wxFileName fn = m_libName->GetValue();

    if( !fn.IsAbsolute() )
        fn.MakeAbsolute( m_dirCtrl->GetPath() );

    // Enforce the extension:
    fn.SetExt( KiCadFootprintLibPathExtension );

    return fn.GetFullPath();
}

void DIALOG_SELECT_PRETTY_LIB::OnOKButton( wxCommandEvent& event )
{
    event.Skip();
}
