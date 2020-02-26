/**
 * @file dialog_cleaning_options.cpp
 */

/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 1992-2012 KiCad Developers, see change_log.txt for contributors.
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
#include <wx/wx.h>

#include <dialog_cleaning_options.h>


DIALOG_CLEANING_OPTIONS::DIALOG_CLEANING_OPTIONS( wxWindow* parent ):
    DIALOG_CLEANING_OPTIONS_BASE( parent )
{
    m_cleanViasOpt->SetValue( m_cleanVias );
    m_mergeSegmOpt->SetValue( m_mergeSegments );
    m_deleteUnconnectedOpt->SetValue( m_deleteUnconnectedSegm );

    m_sdbSizerOK->SetDefault();
    GetSizer()->SetSizeHints(this);
    Centre();
}

// Static members of DIALOG_CLEANING_OPTIONS
bool DIALOG_CLEANING_OPTIONS::m_cleanVias = true;
bool DIALOG_CLEANING_OPTIONS::m_mergeSegments = true;
bool DIALOG_CLEANING_OPTIONS::m_deleteUnconnectedSegm = true;

