/**
 * @file kicad.cpp
 * @brief Main KiCad Project manager file
 */

/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2004-2015 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 2004-2015 KiCad Developers, see change_log.txt for contributors.
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


#include <macros.h>
#include <fctsys.h>
#include <wx/stdpaths.h>
#include <kicad.h>
#include <kiway.h>
#include <pgm_kicad.h>
#include <tree_project_frame.h>
#include <online_help.h>
#include <wildcards_and_files_ext.h>
#include <boost/ptr_container/ptr_vector.hpp>
#include <hotkeys_basic.h>

#include <build_version.h>


// a dummy to quiet linking with EDA_BASE_FRAME::config();
#include <kiface_i.h>
KIFACE_I& Kiface()
{
    // This function should never be called.  It is only referenced from
    // EDA_BASE_FRAME::config() and this is only provided to satisfy the linker,
    // not to be actually called.
    wxLogFatalError( wxT( "Unexpected call to Kiface() in kicad/kicad.cpp" ) );

    return (KIFACE_I&) *(KIFACE_I*) 0;
}

static PGM_KICAD program;

PGM_KICAD& Pgm()
{
    return program;
}


bool PGM_KICAD::OnPgmInit( wxApp* aWxApp )
{
    m_wx_app = aWxApp;      // first thing.

    wxString absoluteArgv0 = wxStandardPaths::Get().GetExecutablePath();

    if( !wxIsAbsolutePath( absoluteArgv0 ) )
    {
        wxLogError( wxT( "No meaningful argv[0]" ) );
        return false;
    }

    if( !initPgm() )
        return false;

    m_bm.Init();

    // Add search paths to feed the PGM_KICAD::SysSearch() function,
    // currenly limited in support to only look for project templates
    {
        SEARCH_STACK bases;

        SystemDirsAppend( &bases );

        // DBG( bases.Show( (std::string(__func__) + " bases").c_str() );)

        for( unsigned i = 0; i < bases.GetCount(); ++i )
        {
            wxFileName fn( bases[i], wxEmptyString );

            // Add KiCad template file path to search path list.
            fn.AppendDir( wxT( "template" ) );
            m_bm.m_search.AddPaths( fn.GetPath() );
        }

        //DBG( m_bm.m_search.Show( (std::string( __func__ ) + " SysSearch()").c_str() );)
    }

    // Must be called before creating the main frame in order to
    // display the real hotkeys in menus or tool tips
    extern struct EDA_HOTKEY_CONFIG kicad_Manager_Hokeys_Descr[];
    ReadHotkeyConfig( KICAD_MANAGER_FRAME_NAME, kicad_Manager_Hokeys_Descr );

    KICAD_MANAGER_FRAME* frame = new KICAD_MANAGER_FRAME( NULL, wxT( "KiCad" ),
                                     wxDefaultPosition, wxDefaultSize );
    App().SetTopWindow( frame );

    Kiway.SetTop( frame );

    bool prjloaded = false;    // true when the project is loaded

    if( App().argc > 1 )
        frame->SetProjectFileName( App().argv[1] );

    else if( GetFileHistory().GetCount() )
    {
        wxString last_pro = GetFileHistory().GetHistoryFile( 0 );

        if( !wxFileExists( last_pro ) )
        {
            GetFileHistory().RemoveFileFromHistory( 0 );

            wxFileName namelessProject( wxStandardPaths::Get().GetDocumentsDir(), NAMELESS_PROJECT,
                                        ProjectFileExtension );

            frame->SetProjectFileName( namelessProject.GetFullPath() );
        }
        else
        {
            // Try to open the last opened project,
            // if a project name is not given when starting Kicad
            frame->SetProjectFileName( last_pro );

            wxCommandEvent cmd( 0, wxID_FILE1 );

            frame->OnFileHistory( cmd );
            prjloaded = true;    // OnFileHistory() loads the project
        }
    }
    else	// there is no history
    {
            wxFileName namelessProject( wxStandardPaths::Get().GetDocumentsDir(), NAMELESS_PROJECT,
                                        ProjectFileExtension );

            frame->SetProjectFileName( namelessProject.GetFullPath() );
    }

    if( !prjloaded )
    {
        wxCommandEvent cmd( 0, wxID_ANY );

        frame->OnLoadProject( cmd );
    }

    frame->Show( true );
    frame->Raise();

    return true;
}


void PGM_KICAD::OnPgmExit()
{
    Kiway.OnKiwayEnd();

    saveCommonSettings();

    // write common settings to disk, and destroy everything in PGM_KICAD,
    // especially wxSingleInstanceCheckerImpl earlier than wxApp and earlier
    // than static destruction would.
    destroy();
}


void PGM_KICAD::MacOpenFile( const wxString& aFileName )
{
#if defined(__WXMAC__)

    KICAD_MANAGER_FRAME* frame = (KICAD_MANAGER_FRAME*) App().GetTopWindow();

    frame->SetProjectFileName( aFileName );

    wxCommandEvent loadEvent( 0, wxID_ANY );

    frame->OnLoadProject( loadEvent );
#endif
}


void PGM_KICAD::destroy()
{
    // unlike a normal destructor, this is designed to be called more
    // than once safely:

    m_bm.End();

    PGM_BASE::destroy();
}


KIWAY  Kiway( &Pgm(), KFCTL_CPP_PROJECT_SUITE );


/**
 * Struct APP_KICAD
 * is not publicly visible because most of the action is in PGM_KICAD these days.
 */
struct APP_KICAD : public wxApp
{
#if defined (__LINUX__)
    APP_KICAD(): wxApp()
    {
        // Disable proxy menu in Unity window manager. Only usual menubar works with wxWidgets (at least <= 3.1)
        // When the proxy menu menubar is enable, some important things for us do not work: menuitems UI events and shortcuts.
        wxString wm;

        if( wxGetEnv( wxT( "XDG_CURRENT_DESKTOP" ), &wm ) && wm.CmpNoCase( wxT( "Unity" ) ) == 0 )
        {
            wxSetEnv ( wxT("UBUNTU_MENUPROXY" ), wxT( "0" ) );
        }
    }
#endif

    bool OnInit()           // overload wxApp virtual
    {
        // if( Kiways.OnStart( this ) )
        {
            return Pgm().OnPgmInit( this );
        }
        return false;
    }

    int  OnExit()           // overload wxApp virtual
    {
        // Kiways.OnEnd();

        Pgm().OnPgmExit();

        return wxApp::OnExit();
    }

    int OnRun()             // overload wxApp virtual
    {
        try
        {
            return wxApp::OnRun();
        }
        catch( const std::exception& e )
        {
            wxLogError( wxT( "Unhandled exception class: %s  what: %s" ),
                GetChars( FROM_UTF8( typeid(e).name() )),
                GetChars( FROM_UTF8( e.what() ) ) );
        }
        catch( const IO_ERROR& ioe )
        {
            wxLogError( GetChars( ioe.errorText ) );
        }
        catch(...)
        {
            wxLogError( wxT( "Unhandled exception of unknown type" ) );
        }

        return -1;
    }

    /**
     * Function MacOpenFile
     * is specific to MacOSX (not used under Linux or Windows).
     * MacOSX requires it for file association.
     * @see http://wiki.wxwidgets.org/WxMac-specific_topics
     */
    void MacOpenFile( const wxString& aFileName )   // overload wxApp virtual
    {
        Pgm().MacOpenFile( aFileName );
    }
};

IMPLEMENT_APP( APP_KICAD );


// The C++ project manager supports one open PROJECT, so Prj() calls within
// this link image need this function.
PROJECT& Prj()
{
    return Kiway.Prj();
}


#if 0   // there can be only one in C++ project manager.

bool KIWAY_MGR::OnStart( wxApp* aProcess )
{
    // The C++ project manager supports only one open PROJECT
    // We should need no copy constructor for KIWAY to push a pointer.
    m_kiways.push_back( new KIWAY() );

    return true;
}


void KIWAY_MGR::OnEnd()
{
}

#endif
