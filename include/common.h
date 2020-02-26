/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2014-2016 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 2007-2015 SoftPLC Corporation, Dick Hollenbeck <dick@softplc.com>
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
 * The common library
 * @file common.h
 */

#ifndef INCLUDE__COMMON_H_
#define INCLUDE__COMMON_H_

#include <vector>

#include <wx/wx.h>
#include <wx/confbase.h>
#include <wx/fileconf.h>

#include <richio.h>
#include <colors.h>


class wxAboutDialogInfo;
class SEARCH_STACK;
class wxSingleInstanceChecker;
class REPORTER;


// Flag for special keys
#define GR_KB_RIGHTSHIFT 0x10000000                 /* Keybd states: right
                                                     * shift key depressed */
#define GR_KB_LEFTSHIFT  0x20000000                 /* left shift key depressed
                                                     */
#define GR_KB_CTRL       0x40000000                 // CTRL depressed
#define GR_KB_ALT        0x80000000                 // ALT depressed
#define GR_KB_SHIFT      (GR_KB_LEFTSHIFT | GR_KB_RIGHTSHIFT)
#define GR_KB_SHIFTCTRL  (GR_KB_SHIFT | GR_KB_CTRL)
#define MOUSE_MIDDLE     0x08000000                 /* Middle button mouse
                                                     * flag for block commands
                                                     */

/// default name for nameless projects
#define NAMELESS_PROJECT wxT( "noname" )


/// Pseudo key codes for command panning
enum pseudokeys {
    EDA_PANNING_UP_KEY = 1,
    EDA_PANNING_DOWN_KEY,
    EDA_PANNING_LEFT_KEY,
    EDA_PANNING_RIGHT_KEY,
    EDA_ZOOM_IN_FROM_MOUSE,
    EDA_ZOOM_OUT_FROM_MOUSE,
    EDA_ZOOM_CENTER_FROM_MOUSE
};

#define ESC 27

// TODO Executable names TODO
#ifdef __WINDOWS__
#define CVPCB_EXE           wxT( "cvpcb.exe" )
#define PCBNEW_EXE          wxT( "pcbnew.exe" )
#define EESCHEMA_EXE        wxT( "eeschema.exe" )
#define GERBVIEW_EXE        wxT( "gerbview.exe" )
#define BITMAPCONVERTER_EXE wxT( "bitmap2component.exe" )
#define PCB_CALCULATOR_EXE  wxT( "pcb_calculator.exe" )
#define PL_EDITOR_EXE       wxT( "pl_editor.exe" )
#else
#define CVPCB_EXE           wxT( "cvpcb" )
#define PCBNEW_EXE          wxT( "pcbnew" )
#define EESCHEMA_EXE        wxT( "eeschema" )
#define GERBVIEW_EXE        wxT( "gerbview" )
#define BITMAPCONVERTER_EXE wxT( "bitmap2component" )
#define PCB_CALCULATOR_EXE  wxT( "pcb_calculator" )
#define PL_EDITOR_EXE       wxT( "pl_editor" )
#endif


// Graphic Texts Orientation in 0.1 degree
#define TEXT_ORIENT_HORIZ 0
#define TEXT_ORIENT_VERT  900



//-----<KiROUND KIT>------------------------------------------------------------

/**
 * KiROUND
 * rounds a floating point number to an int using
 * "round halfway cases away from zero".
 * In Debug build an assert fires if will not fit into an int.
 */

#if !defined( DEBUG )

/// KiROUND: a function so v is not evaluated twice.  Unfortunately, compiler
/// is unable to pre-compute constants using this.
static inline int KiROUND( double v )
{
    return int( v < 0 ? v - 0.5 : v + 0.5 );
}

/// KIROUND: a macro so compiler can pre-compute constants.  Use this with compile
/// time constants rather than the inline function above.
#define KIROUND( v )    int( (v) < 0 ? (v) - 0.5 : (v) + 0.5 )

#else

// DEBUG: KiROUND() is a macro to capture line and file, then calls this inline

static inline int kiRound_( double v, int line, const char* filename )
{
    v = v < 0 ? v - 0.5 : v + 0.5;
    if( v > INT_MAX + 0.5 )
    {
        printf( "%s: in file %s on line %d, val: %.16g too ' > 0 ' for int\n", __FUNCTION__, filename, line, v );
    }
    else if( v < INT_MIN - 0.5 )
    {
        printf( "%s: in file %s on line %d, val: %.16g too ' < 0 ' for int\n", __FUNCTION__, filename, line, v );
    }
    return int( v );
}

#define KiROUND( v )    kiRound_( v, __LINE__, __FILE__ )

// in Debug build, use the overflow catcher since code size is immaterial
#define KIROUND( v )    KiROUND( v )

#endif

//-----</KiROUND KIT>-----------------------------------------------------------



/// Convert mm to mils.
inline int Mm2mils( double x ) { return KiROUND( x * 1000./25.4 ); }

/// Convert mils to mm.
inline int Mils2mm( double x ) { return KiROUND( x * 25.4 / 1000. ); }


enum EDA_UNITS_T {
    INCHES = 0,
    MILLIMETRES = 1,
    UNSCALED_UNITS = 2,
    DEGREES = 3,
};


extern EDA_UNITS_T  g_UserUnit;     ///< display units

/// Draw color for moving objects.
extern EDA_COLOR_T  g_GhostColor;


/**
 * Class LOCALE_IO
 * is a class that can be instantiated within a scope in which you are expecting
 * exceptions to be thrown.  Its constructor set a "C" laguage locale option,
 * to read/print files with fp numbers.
 * Its destructor insures that the default locale is restored if an exception
 * is thrown, or not.
 */
class LOCALE_IO
{
public:
    LOCALE_IO();
    ~LOCALE_IO();

private:
    void setUserLocale( const char* aUserLocale );

    // allow for nesting of LOCALE_IO instantiations
    static int  m_c_count;

    // The locale in use before switching to the "C" locale
    // (the locale can be set by user, and is not always the system locale)
    std::string m_user_locale;
};


/**
 * Function GetTextSize
 * returns the size of @a aSingleLine of text when it is rendered in @a aWindow
 * using whatever font is currently set in that window.
 */
wxSize GetTextSize( const wxString& aSingleLine, wxWindow* aWindow );

/**
 * Function EnsureTextCtrlWidth
 * sets the minimum pixel width on a text control in order to make a text
 * string be fully visible within it. The current font within the text
 * control is considered.
 * The text can come either from the control or be given as an argument.
 * If the text control is larger than needed, then nothing is done.
 * @param aCtrl the text control to potentially make wider.
 * @param aString the text that is used in sizing the control's pixel width.
 * If NULL, then
 *   the text already within the control is used.
 * @return bool - true if the \a aCtrl had its size changed, else false.
 */
bool EnsureTextCtrlWidth( wxTextCtrl* aCtrl, const wxString* aString = NULL );


/**
 * Function ProcessExecute
 * runs a child process.
 * @param aCommandLine The process and any arguments to it all in a single
 *                     string.
 * @param aFlags The same args as allowed for wxExecute()
 * @param callback wxProcess implementing OnTerminate to be run when the
                   child process finishes
 * @return int - pid of process, 0 in case of error (like return values of
 *               wxExecute())
 */
int ProcessExecute( const wxString& aCommandLine, int aFlags = wxEXEC_ASYNC,
                    wxProcess *callback = NULL );


/**************/
/* common.cpp */
/**************/

/**
 * @return an unique time stamp that changes after each call
 */
time_t GetNewTimeStamp();

EDA_COLOR_T DisplayColorFrame( wxWindow* parent, int OldColor );
int GetCommandOptions( const int argc, const char** argv,
                       const char* stringtst, const char** optarg,
                       int* optind );

/**
 * Returns the units symbol.
 *
 * @param aUnits - Units type, default is current units setting.
 * @param aFormatString - A formatting string to embed the units symbol into.  Note:
 *                        the format string must contain the %s format specifier.
 * @return The formatted units symbol.
 */
wxString ReturnUnitSymbol( EDA_UNITS_T aUnits = g_UserUnit,
                           const wxString& aFormatString = _( " (%s):" ) );

/**
 * Get a human readable units string.
 *
 * The strings returned are full text name and not abbreviations or symbolic
 * representations of the units.  Use ReturnUnitSymbol() for that.
 *
 * @param aUnits - The units text to return.
 * @return The human readable units string.
 */
wxString GetUnitsLabel( EDA_UNITS_T aUnits );
wxString GetAbbreviatedUnitsLabel( EDA_UNITS_T aUnit = g_UserUnit );

void AddUnitSymbol( wxStaticText& Stext, EDA_UNITS_T aUnit = g_UserUnit );

/**
 * Round to the nearest precision.
 *
 * Try to approximate a coordinate using a given precision to prevent
 * rounding errors when converting from inches to mm.
 *
 * ie round the unit value to 0 if unit is 1 or 2, or 8 or 9
 */
double RoundTo0( double x, double precision );

/**
 * Function wxStringSplit
 * splits \a aString to a string list separated at \a aSplitter.
 * @param aText is the text to split
 * @param aStrings will contain the splitted lines
 * @param aSplitter is the 'split' character
 */
void wxStringSplit( const wxString& aText, wxArrayString& aStrings, wxChar aSplitter );

/**
 * Function GetRunningMicroSecs
 * returns an ever increasing indication of elapsed microseconds.  Use this
 * by computing differences between two calls.
 * @author Dick Hollenbeck
 */
unsigned GetRunningMicroSecs();


/**
 * Function SystemDirsAppend
 * appends system places to aSearchStack in a platform specific way, and pertinent
 * to KiCad programs.  It seems to be a place to collect bad ideas and keep them
 * out of view.
 */
void SystemDirsAppend( SEARCH_STACK* aSearchStack );


/**
 * Function SearchHelpFileFullPath
 * returns the help file's full path.
 * <p>
 * Return the KiCad help file with path and extension.
 * Help files can be html (.html ext) or pdf (.pdf ext) files.
 * A \<BaseName\>.html file is searched and if not found,
 * \<BaseName\>.pdf file is searched in the same path.
 * If the help file for the current locale is not found, an attempt to find
 * the English version of the help file is made.
 * Help file is searched in directories in this order:
 *  help/\<canonical name\> like help/en_GB
 *  help/\<short name\> like help/en
 *  help/en
 * </p>
 * @param aSearchStack contains some possible base dirs that may be above the
 *  the one actually holding @a aBaseName.  These are starting points for nested searches.
 * @param aBaseName is the name of the help file to search for, <p>without extension</p>.
 * @return  wxEmptyString is returned if aBaseName is not found, else the full path & filename.
 */
wxString SearchHelpFileFullPath( const SEARCH_STACK& aSearchStack, const wxString& aBaseName );

/**
 * Helper function EnsureFileDirectoryExists
 * make \a aTargetFullFileName absolute and creates the path of this file if it doesn't yet exist.
 * @param aTargetFullFileName  the wxFileName containing the full path and file name to modify.  The path
 *                    may be absolute or relative to \a aBaseFilename .
 * @param aBaseFilename a full filename. Only its path is used to set the aTargetFullFileName path.
 * @param aReporter a point to a REPORTER object use to show messages (can be NULL)
 * @return true if \a aOutputDir already exists or was successfully created.
 */
bool EnsureFileDirectoryExists( wxFileName*     aTargetFullFileName,
                                  const wxString& aBaseFilename,
                                  REPORTER*       aReporter = NULL );

/**
 * Function LockFile
 * tests to see if aFileName can be locked (is not already locked) and only then
 * returns a wxSingleInstanceChecker protecting aFileName.  Caller owns the return value.
 */
wxSingleInstanceChecker* LockFile( const wxString& aFileName );


/// Put aPriorityPath in front of all paths in the value of aEnvVar.
const wxString PrePendPath( const wxString& aEnvVar, const wxString& aPriorityPath );

/**
 * Function GetNewConfig
 *
 * Use this function instead of creating a new wxConfig so we can put config files in
 * a more proper place for each platform. This is generally $HOME/.config/kicad/ in Linux
 * according to the FreeDesktop specification at
 * http://standards.freedesktop.org/basedir-spec/basedir-spec-0.6.html
 * The config object created here should be destroyed by the caller.
 *
 * @param aProgName is the name of the program calling this function - can be obtained by
 *  calling Pgm().App().GetAppName().  This will be the actual file name of the config file.
 * @return A pointer to a new wxConfigBase derived object is returned.  The caller is in charge
 *  of deleting it.
 */
wxConfigBase* GetNewConfig( const wxString& aProgName );

/**
 * Function GetKicadLockFilePath
 * @return A wxString containing the path for lockfiles in Kicad
 */
wxString GetKicadLockFilePath();

/**
 * Function GetKicadConfigPath
 * @return A wxString containing the config path for Kicad
 */
wxString GetKicadConfigPath();

/**
 * Function ExpandEnvVarSubstitutions
 * replaces any environment variable references with their values
 * @param aString = a string containing (perhaps) references to env var
 * @return a string where env var are replaced by their value
 */
const wxString ExpandEnvVarSubstitutions( const wxString& aString );


#ifdef __WXMAC__
/**
 * OSX specific function GetOSXKicadUserDataDir
 * @return A wxString pointing to the user data directory for Kicad
 */
wxString GetOSXKicadUserDataDir();

/**
 * OSX specific function GetOSXMachineDataDir
 * @return A wxString pointing to the machine data directory for Kicad
 */
wxString GetOSXKicadMachineDataDir();

/**
 * OSX specific function GetOSXKicadDataDir
 * @return A wxString pointing to the bundle data directory for Kicad
 */
wxString GetOSXKicadDataDir();
#endif

#endif  // INCLUDE__COMMON_H_
