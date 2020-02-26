/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2014-2015 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 2008-2015 Wayne Stambaugh <stambaughw@verizon.net>
 * Copyright (C) 1992-2015 KiCad Developers, see AUTHORS.txt for contributors.
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
 * @file common.cpp
 */

#include <fctsys.h>
#include <gr_basic.h>
#include <trigo.h>
#include <wxstruct.h>
#include <base_struct.h>
#include <common.h>
#include <macros.h>
#include <build_version.h>
#include <confirm.h>
#include <base_units.h>
#include <reporter.h>

#include <wx/process.h>
#include <wx/config.h>
#include <wx/utils.h>
#include <wx/stdpaths.h>

#include <pgm_base.h>


/**
 * Global variables definitions.
 *
 * TODO: All of these variables should be moved into the class were they
 *       are defined and used.  Most of them probably belong in the
 *       application class.
 */

bool           g_ShowPageLimits = true;
EDA_UNITS_T    g_UserUnit;
EDA_COLOR_T    g_GhostColor;


/* Class LOCALE_IO
 * is a class that can be instantiated within a scope in which you are expecting
 * exceptions to be thrown.  Its constructor sets a "C" locale, to read/print files
 * with fp numbers.
 * Its destructor insures that the default locale is restored if an exception
 * is thrown, or not.
 */

int LOCALE_IO::m_c_count = 0;

LOCALE_IO::LOCALE_IO()
{
    wxASSERT_MSG( m_c_count >= 0, wxT( "LOCALE_IO::m_c_count mismanaged." ) );

    // use thread safe, atomic operation
    if( __sync_fetch_and_add( &m_c_count, 1 ) == 0 )
    {
        // Store the user locale name, to restore this locale later, in dtor
        m_user_locale = setlocale( LC_ALL, 0 );
        // Switch the locale to C locale, to read/write files with fp numbers
        setlocale( LC_ALL, "C" );
    }
}

LOCALE_IO::~LOCALE_IO()
{
    // use thread safe, atomic operation
    if( __sync_sub_and_fetch( &m_c_count, 1 ) == 0 )
    {
        // revert to the user locale
        setlocale( LC_ALL, m_user_locale.c_str() );
    }

    wxASSERT_MSG( m_c_count >= 0, wxT( "LOCALE_IO::m_c_count mismanaged." ) );
}


wxSize GetTextSize( const wxString& aSingleLine, wxWindow* aWindow )
{
    wxCoord width;
    wxCoord height;

    {
        wxClientDC dc( aWindow );
        dc.SetFont( aWindow->GetFont() );
        dc.GetTextExtent( aSingleLine, &width, &height );
    }

    return wxSize( width, height );
}


bool EnsureTextCtrlWidth( wxTextCtrl* aCtrl, const wxString* aString )
{
    wxWindow* window = aCtrl->GetParent();

    if( !window )
        window = aCtrl;

    wxString ctrlText;

    if( !aString )
    {
        ctrlText = aCtrl->GetValue();
        aString  = &ctrlText;
    }

    wxSize  textz = GetTextSize( *aString, window );
    wxSize  ctrlz = aCtrl->GetSize();

    if( ctrlz.GetWidth() < textz.GetWidth() + 10 )
    {
        ctrlz.SetWidth( textz.GetWidth() + 10 );
        aCtrl->SetSizeHints( ctrlz );
        return true;
    }

    return false;
}


wxString ReturnUnitSymbol( EDA_UNITS_T aUnit, const wxString& formatString )
{
    wxString tmp;
    wxString label;

    switch( aUnit )
    {
    case INCHES:
        tmp = _( "\"" );
        break;

    case MILLIMETRES:
        tmp = _( "mm" );
        break;

    case UNSCALED_UNITS:
        break;

    case DEGREES:
        wxASSERT( false );
        break;
    }

    if( formatString.IsEmpty() )
        return tmp;

    label.Printf( formatString, GetChars( tmp ) );

    return label;
}


wxString GetUnitsLabel( EDA_UNITS_T aUnit )
{
    wxString label;

    switch( aUnit )
    {
    case INCHES:
        label = _( "inches" );
        break;

    case MILLIMETRES:
        label = _( "millimeters" );
        break;

    case UNSCALED_UNITS:
        label = _( "units" );
        break;

    case DEGREES:
        wxASSERT( false );
        break;
    }

    return label;
}


wxString GetAbbreviatedUnitsLabel( EDA_UNITS_T aUnit )
{
    wxString label;

    switch( aUnit )
    {
    case INCHES:
        label = _( "in" );
        break;

    case MILLIMETRES:
        label = _( "mm" );
        break;

    case UNSCALED_UNITS:
        break;

    case DEGREES:
        label = _( "deg" );
        break;

    default:
        label = wxT( "??" );
        break;
    }

    return label;
}


void AddUnitSymbol( wxStaticText& Stext, EDA_UNITS_T aUnit )
{
    wxString msg = Stext.GetLabel();

    msg += ReturnUnitSymbol( aUnit );

    Stext.SetLabel( msg );
}


void wxStringSplit( const wxString& aText, wxArrayString& aStrings, wxChar aSplitter )
{
    wxString tmp;

    for( unsigned ii = 0; ii < aText.Length(); ii++ )
    {
        if( aText[ii] == aSplitter )
        {
            aStrings.Add( tmp );
            tmp.Clear();
        }

        else
            tmp << aText[ii];
    }

    if( !tmp.IsEmpty() )
    {
        aStrings.Add( tmp );
    }
}


int ProcessExecute( const wxString& aCommandLine, int aFlags, wxProcess *callback )
{
    return wxExecute( aCommandLine, aFlags, callback );
}


time_t GetNewTimeStamp()
{
    static time_t oldTimeStamp;
    time_t newTimeStamp;

    newTimeStamp = time( NULL );

    if( newTimeStamp <= oldTimeStamp )
        newTimeStamp = oldTimeStamp + 1;

    oldTimeStamp = newTimeStamp;

    return newTimeStamp;
}


double RoundTo0( double x, double precision )
{
    assert( precision != 0 );

    long long ix = KiROUND( x * precision );

    if ( x < 0.0 )
        ix = -ix;

    int remainder = ix % 10;   // remainder is in precision mm

    if( remainder <= 2 )
        ix -= remainder;       // truncate to the near number
    else if( remainder >= 8 )
        ix += 10 - remainder;  // round to near number

    if ( x < 0 )
        ix = -ix;

    return (double) ix / precision;
}


wxConfigBase* GetNewConfig( const wxString& aProgName )
{
    wxConfigBase* cfg = 0;
    wxFileName configname;
    configname.AssignDir( GetKicadConfigPath() );
    configname.SetFullName( aProgName );

    cfg = new wxFileConfig( wxT( "" ), wxT( "" ), configname.GetFullPath() );
    return cfg;
}

wxString GetKicadLockFilePath()
{
    wxFileName lockpath;
    lockpath.AssignDir( wxGetHomeDir() ); // Default wx behavior

#if defined( __WXMAC__ )
    // In OSX use the standard per user cache directory
    lockpath.AppendDir( wxT( "Library" ) );
    lockpath.AppendDir( wxT( "Caches" ) );
    lockpath.AppendDir( wxT( "kicad" ) );
#elif defined( __UNIX__ )
    wxString envstr;
    // Try first the standard XDG_RUNTIME_DIR, falling back to XDG_CACHE_HOME
    if( wxGetEnv( wxT( "XDG_RUNTIME_DIR" ), &envstr ) && !envstr.IsEmpty() )
    {
        lockpath.AssignDir( envstr );
    }
    else if( wxGetEnv( wxT( "XDG_CACHE_HOME" ), &envstr ) && !envstr.IsEmpty() )
    {
        lockpath.AssignDir( envstr );
    }
    else
    {
        // If all fails, just use ~/.cache
        lockpath.AppendDir( wxT( ".cache" ) );
    }

    lockpath.AppendDir( wxT( "kicad" ) );
#endif

#if defined( __WXMAC__ ) || defined( __UNIX__ )
    if( !lockpath.DirExists() )
    {
        // Lockfiles should be only readable by the user
        lockpath.Mkdir( 0700, wxPATH_MKDIR_FULL );
    }
#endif
    return lockpath.GetPath();
}


wxString GetKicadConfigPath()
{
    wxFileName cfgpath;

    // From the wxWidgets wxStandardPaths::GetUserConfigDir() help:
    //      Unix: ~ (the home directory)
    //      Windows: "C:\Documents and Settings\username\Application Data"
    //      Mac: ~/Library/Preferences
    cfgpath.AssignDir( wxStandardPaths::Get().GetUserConfigDir() );

#if !defined( __WINDOWS__ ) && !defined( __WXMAC__ )
    wxString envstr;

    if( !wxGetEnv( wxT( "XDG_CONFIG_HOME" ), &envstr ) || envstr.IsEmpty() )
    {
        // XDG_CONFIG_HOME is not set, so use the fallback
        cfgpath.AppendDir( wxT( ".config" ) );
    }
    else
    {
        // Override the assignment above with XDG_CONFIG_HOME
        cfgpath.AssignDir( envstr );
    }
#endif

    cfgpath.AppendDir( wxT( "kicad" ) );

    if( !cfgpath.DirExists() )
    {
        cfgpath.Mkdir( wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL );
    }

    return cfgpath.GetPath();
}


#include <ki_mutex.h>
const wxString ExpandEnvVarSubstitutions( const wxString& aString )
{
    // wxGetenv( wchar_t* ) is not re-entrant on linux.
    // Put a lock on multithreaded use of wxGetenv( wchar_t* ), called from wxEpandEnvVars(),
    static MUTEX    getenv_mutex;

    MUTLOCK lock( getenv_mutex );

    // We reserve the right to do this another way, by providing our own member
    // function.
    return wxExpandEnvVars( aString );
}

bool EnsureFileDirectoryExists( wxFileName*     aTargetFullFileName,
                                const wxString& aBaseFilename,
                                REPORTER*       aReporter )
{
    wxString msg;
    wxString baseFilePath = wxFileName( aBaseFilename ).GetPath();

    // make aTargetFullFileName path, which is relative to aBaseFilename path (if it is not
    // already an absolute path) absolute:
    if( !aTargetFullFileName->MakeAbsolute( baseFilePath ) )
    {
        if( aReporter )
        {
            msg.Printf( _( "Cannot make path '%s' absolute with respect to '%s'." ),
                        GetChars( aTargetFullFileName->GetPath() ),
                        GetChars( baseFilePath ) );
            aReporter->Report( msg, REPORTER::RPT_ERROR );
        }

        return false;
    }

    // Ensure the path of aTargetFullFileName exists, and create it if needed:
    wxString outputPath( aTargetFullFileName->GetPath() );

    if( !wxFileName::DirExists( outputPath ) )
    {
        if( wxMkdir( outputPath ) )
        {
            if( aReporter )
            {
                msg.Printf( _( "Output directory '%s' created.\n" ), GetChars( outputPath ) );
                aReporter->Report( msg, REPORTER::RPT_INFO );
                return true;
            }
        }
        else
        {
            if( aReporter )
            {
                msg.Printf( _( "Cannot create output directory '%s'.\n" ),
                            GetChars( outputPath ) );
                aReporter->Report( msg, REPORTER::RPT_ERROR );
            }

            return false;
        }
    }

    return true;
}


#ifdef __WXMAC__
wxString GetOSXKicadUserDataDir()
{
    // According to wxWidgets documentation for GetUserDataDir:
    // Mac: ~/Library/Application Support/appname
    wxFileName udir( wxStandardPaths::Get().GetUserDataDir(), wxEmptyString );

    // Since appname is different if started via launcher or standalone binary
    // map all to "kicad" here
    udir.RemoveLastDir();
    udir.AppendDir( wxT( "kicad" ) );

    return udir.GetPath();
}


wxString GetOSXKicadMachineDataDir()
{
    return wxT( "/Library/Application Support/kicad" );
}


wxString GetOSXKicadDataDir()
{
    // According to wxWidgets documentation for GetDataDir:
    // Mac: appname.app/Contents/SharedSupport bundle subdirectory
    wxFileName ddir( wxStandardPaths::Get().GetDataDir(), wxEmptyString );

    // This must be mapped to main bundle for everything but kicad.app
    const wxArrayString dirs = ddir.GetDirs();
    if( dirs[dirs.GetCount() - 3] != wxT( "kicad.app" ) )
    {
        // Bundle structure resp. current path is
        //   kicad.app/Contents/Applications/<standalone>.app/Contents/SharedSupport
        // and will be mapped to
        //   kicad.app/Contents/SharedSupprt
        ddir.RemoveLastDir();
        ddir.RemoveLastDir();
        ddir.RemoveLastDir();
        ddir.RemoveLastDir();
        ddir.AppendDir( wxT( "SharedSupport" ) );
    }

    return ddir.GetPath();
}
#endif
