/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2015 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 2008-2011 Wayne Stambaugh <stambaughw@verizon.net>
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

/**
 * @file class_library.h
 * @brief Definition for part library class.
 */

#ifndef CLASS_LIBRARY_H
#define CLASS_LIBRARY_H

#include <wx/filename.h>

#include <class_libentry.h>

#include <project.h>

class LINE_READER;
class OUTPUTFORMATTER;


/*
 * Part Library version and file header  macros.
 */
#define LIB_VERSION_MAJOR 2
#define LIB_VERSION_MINOR 3

/* Must be the first line of part library (.lib) files. */
#define LIBFILE_IDENT     "EESchema-LIBRARY Version"

#define LIB_VERSION( major, minor ) ( major * 100 + minor )

#define IS_LIB_CURRENT_VERSION( major, minor )              \
    (                                                       \
        LIB_VERSION( major1, minor1 ) ==                    \
        LIB_VERSION( LIB_VERSION_MAJOR, LIB_VERSION_MINOR)  \
    )

/*
 * Library versions 2.3 and lower use the old separate library (.lib) and
 * document (.dcm) files.  Part libraries after 2.3 merged the library
 * and document files into a single library file.  This macro checks if the
 * library version supports the old format
 */
#define USE_OLD_DOC_FILE_FORMAT( major, minor )                 \
    ( LIB_VERSION( major, minor ) <= LIB_VERSION( 2, 3 ) )

/* Must be the first line of part library document (.dcm) files. */
#define DOCFILE_IDENT     "EESchema-DOCLIB  Version 2.0"

#define DOC_EXT           wxT( "dcm" )

// Helper class to filter a list of libraries, and/or a list of PART_LIB
// in dialogs
class SCHLIB_FILTER
{
    wxArrayString m_allowedLibs;        ///< a list of lib names to list some libraries
                                        ///< if empty: no filter
    bool          m_filterPowerParts;   ///< true to filter (show only) power parts
    bool          m_forceLoad;          // When true, load a part lib from the lib
                                        // which is given in m_allowedLibs[0]

public:
    SCHLIB_FILTER()
    {
        m_filterPowerParts = false;
        m_forceLoad = false;
    }

    /**
     * add a lib name to the allowed libraries
     */
    void AddLib( const wxString& aLibName )
    {
        m_allowedLibs.Add( aLibName );
        m_forceLoad = false;
    }


    /**
     * add a lib name to the allowed libraries
     */
    void LoadFrom( const wxString& aLibName )
    {
        m_allowedLibs.Clear();
        m_allowedLibs.Add( aLibName );
        m_forceLoad = true;
    }

    /**
     * Clear the allowed libraries list (allows all libs)
     */
    void ClearLibList()
    {
        m_allowedLibs.Clear();
        m_forceLoad = false;
    }

    /**
     * set the filtering of power parts
     */
    void FilterPowerParts( bool aFilterEnable )
    {
        m_filterPowerParts = aFilterEnable;
    }

    // Accessors

    /**
     * Function GetFilterPowerParts
     * @return true if the filtering of power parts is on
     */
    bool GetFilterPowerParts() const { return m_filterPowerParts; }


    /**
     * Function GetAllowedLibList
     * @return am wxArrayString of the names of allowed libs
     */
    const wxArrayString& GetAllowedLibList() const { return m_allowedLibs; }

    /**
     * Function GetLibSource
     * @return the name of the lib to use to load a part, or an a emty string
     * Useful to load (in lib editor or lib viewer) a part from a given library
     */
    const wxString& GetLibSource() const
    {
        static wxString dummy;

        if( m_forceLoad && m_allowedLibs.GetCount() > 0 )
            return m_allowedLibs[0];
        else
            return dummy;
    }
};


/* Helpers for creating a list of part libraries. */
class PART_LIB;
class wxRegEx;

/**
 * LIB_ALIAS map sorting.
 */
struct AliasMapSort
{
    bool operator() ( const wxString& aItem1, const wxString& aItem2 ) const
    {
        return Cmp_KEEPCASE( aItem1, aItem2 ) < 0;
    }
};

/// Alias map used by part library object.

typedef std::map< wxString, LIB_ALIAS*, AliasMapSort >  LIB_ALIAS_MAP;
typedef std::vector< LIB_ALIAS* >                       LIB_ALIASES;
typedef boost::ptr_vector< PART_LIB >                   PART_LIBS_BASE;


/**
 * Class PART_LIBS
 * is a collection of PART_LIBs.  It extends from PROJECT::_ELEM so it can be
 * hung in the PROJECT.  It does not use any UI calls, but rather simply throws
 * an IO_ERROR when there is a problem.
 */
class PART_LIBS : public PART_LIBS_BASE, public PROJECT::_ELEM
{
public:

    static int s_modify_generation;     ///< helper for GetModifyHash()

    PART_LIBS()
    {
        ++s_modify_generation;
    }

    /// Return the modification hash for all libraries.  The value returned
    /// changes on every library modification.
    int GetModifyHash();

    /**
     * Function AddLibrary
     * allocates and adds a part library to the library list.
     *
     * @param aFileName - File name object of part library.
     * @throw IO_ERROR if there's any problem loading.
     */
    PART_LIB* AddLibrary( const wxString& aFileName ) throw( IO_ERROR, boost::bad_pointer );

    /**
     * Function AddLibrary
     * inserts a part library into the library list.
     *
     * @param aFileName - File name object of part library.
     * @param aIterator - Iterator to insert library in front of.
     * @return PART_LIB* - the new PART_LIB, which remains owned by this PART_LIBS container.
     * @throw IO_ERROR if there's any problem loading.
     */
    PART_LIB* AddLibrary( const wxString& aFileName, PART_LIBS::iterator& aIterator )
        throw( IO_ERROR, boost::bad_pointer );

    /**
     * Function RemoveLibrary
     * removes a part library from the library list.
     *
     * @param aName - Name of part library to remove.
     */
    void RemoveLibrary( const wxString& aName );

    void RemoveAllLibraries()       { clear(); }

    /**
     * Function LoadAllLibraries
     * loads all of the project's libraries into this container, which should
     * be cleared before calling it.
     */
    void LoadAllLibraries( PROJECT* aProject ) throw( IO_ERROR, boost::bad_pointer );

    /**
     * Function LibNamesAndPaths
     * either saves or loads the names of the currently configured part libraries
     * (without paths).
     */
    static void LibNamesAndPaths( PROJECT* aProject, bool doSave,
                                  wxString* aPaths, wxArrayString* aNames=NULL )
        throw( IO_ERROR, boost::bad_pointer );

    /**
     * Function cacheName
     * returns the name of the cache library after potentially fixing it from
     * an older naming scheme.  That is, the old file is renamed if needed.
     * @param aFullProjectFilename - the *.pro filename with absolute path.
     */
    static const wxString CacheName( const wxString& aFullProjectFilename );

    /**
     * Function FindLibrary
     * finds a part library by \a aName.
     *
     * @param aName - Library file name without path or extension to find.
     * @return Part library if found, otherwise NULL.
     */
    PART_LIB* FindLibrary( const wxString& aName );

    /**
     * Function GetLibraryNames
     * returns the list of part library file names without path and extension.
     *
     * @param aSorted - Sort the list of name if true.  Otherwise use the
     *                  library load order.
     * @return The list of library names.
     */
    wxArrayString GetLibraryNames( bool aSorted = true );

    /**
     * Function FindLibPart
     * searches all libraries in the list for a part.
     *
     * A part object will always be returned.  If the entry found
     * is an alias.  The root part will be found and returned.
     *
     * @param aPartName - Name of part to search for.
     * @param aLibraryName - Name of the library to search for part.
     * @return LIB_PART* - The part object if found, otherwise NULL.
     */
    LIB_PART* FindLibPart( const wxString& aPartName, const wxString& aLibraryName = wxEmptyString );

    /**
     * Function FindLibraryEntry
     * searches all libraries in the list for an entry.
     *
     * The object can be either a part or an alias.
     *
     * @param aEntryName - Name of entry to search for (case sensitive).
     * @param aLibraryName - Name of the library to search.
     * @return The entry object if found, otherwise NULL.
     */
    LIB_ALIAS* FindLibraryEntry( const wxString& aEntryName,
            const wxString& aLibraryName = wxEmptyString );

    /**
     * Function FindLibraryEntries
     * searches all libraries in the list for an entry, returns all matches.
     *
     * @param aEntryName - Name of entry to search for (case sensitive).
     * @param aEntries - a std::vector to store entries
     */
    void FindLibraryEntries( const wxString& aEntryName, std::vector<LIB_ALIAS*>& aEntries );

    /**
     * Function FindLibraryNearEntries
     * Searches all libraries in the list for an entry, using a case insensitive comparison.
     * Helper function used in dialog to find all candidates.
     * During a long time, eeschema was using a case insensitive search.
     * Therefore, for old schematics (<= 2013), or libs, for some components,
     * the chip name (name of alias in lib) can be broken.
     * This function can be used to display a list of candidates, in component properties dialog.
     *
     * @param aEntryName - Name of entries to search for (case insensitive).
     * @param aLibraryName - Name of the library to search.
     * @param aCandidates - a std::vector to store candidates
     */
    void FindLibraryNearEntries( std::vector<LIB_ALIAS*>& aCandidates, const wxString& aEntryName,
            const wxString& aLibraryName = wxEmptyString );

    /**
     * Function RemoveCacheLibrary
     * removes all cache libraries from library list.
     */
    //void RemoveCacheLibrary();

    int GetLibraryCount() { return size(); }

};


/**
 * Class PART_LIB
 * is used to load, save, search, and otherwise manipulate
 * part library files.
 */
class PART_LIB
{
    int             type;           ///< Library type indicator.
    wxFileName      fileName;       ///< Library file name.
    wxDateTime      timeStamp;      ///< Library save time and date.
    int             versionMajor;   ///< Library major version number.
    int             versionMinor;   ///< Library minor version number.
    bool            isCache;        /**< False for the "standard" libraries,
                                         True for the library cache */
    wxString        header;         ///< first line of loaded library.
    bool            isModified;     ///< Library modification status.
    LIB_ALIAS_MAP   m_amap;         ///< Map of alias objects associated with the library.
    int             m_mod_hash;     ///< incremented each time library is changed.

    friend class LIB_PART;
    friend class PART_LIBS;

public:
    PART_LIB( int aType, const wxString& aFileName );
    ~PART_LIB();

    /**
     * Function Save
     * writes library to \a aFormatter.
     *
     * @param aFormatter An #OUTPUTFORMATTER object to write the library to.
     * @return True if success writing to \a aFormatter.
     */
    bool Save( OUTPUTFORMATTER& aFormatter );

    /**
     * Function SaveDocs
     * write the library document information to \a aFormatter.
     *
     * @param aFormatter An #OUTPUTFORMATTER object to write the library documentation to.
     * @return True if success writing to \a aFormatter.
     */
    bool SaveDocs( OUTPUTFORMATTER& aFormatter );

    /**
     * Load library from file.
     *
     * @param aErrorMsg - Error message if load fails.
     * @return True if load was successful otherwise false.
     */
    bool Load( wxString& aErrorMsg );

    bool LoadDocs( wxString& aErrorMsg );

private:
    bool SaveHeader( OUTPUTFORMATTER& aFormatter );

    bool LoadHeader( LINE_READER& aLineReader );
    void LoadAliases( LIB_PART* aPart );

public:
    /**
     * Get library entry status.
     *
     * @return True if there are no entries in the library.
     */
    bool IsEmpty() const
    {
        return m_amap.empty();
    }

    /**
     * Function GetCount
     * returns the number of entries in the library.
     *
     * @return The number of part and alias entries.
     */
    int GetCount() const
    {
        return m_amap.size();
    }

    bool IsModified() const
    {
        return isModified;
    }

    bool IsCache() const { return isCache; }

    void SetCache( void ) { isCache = true; }

    /**
     * Function IsReadOnly
     * @return true if current user does not have write access to the library file.
     */
    bool IsReadOnly() const { return !fileName.IsFileWritable(); }

    /**
     * Load a string array with the names of all the entries in this library.
     *
     * @param aNames - String array to place entry names into.
     * @param aSort - Sort names if true.
     * @param aMakeUpperCase - Force entry names to upper case.
     */
    void GetEntryNames( wxArrayString& aNames, bool aSort = true,
                        bool aMakeUpperCase = false );

    /**
     * Load a string array with the names of  entries of type POWER in this library.
     *
     * @param aNames - String array to place entry names into.
     * @param aSort - Sort names if true.
     * @param aMakeUpperCase - Force entry names to upper case.
     */
    void GetEntryTypePowerNames( wxArrayString& aNames, bool aSort = true,
                        bool aMakeUpperCase = false );

    /**
     * Load string array with entry names matching name and/or key word.
     *
     * This currently mimics the old behavior of calling KeyWordOk() and
     * WildCompareString().  The names array will be populated with the
     * library entry names that meat the search criteria on exit.
     *
     * @param aNames - String array to place entry names into.
     * @param aNameSearch - Name wild card search criteria.
     * @param aKeySearch - Key word search criteria.
     * @param aSort - Sort names if true.
     */
    void SearchEntryNames( std::vector<wxArrayString>& aNames,
                           const wxString& aNameSearch = wxEmptyString,
                           const wxString& aKeySearch = wxEmptyString,
                           bool aSort = true );

    /**
     * Find parts in library by key word regular expression search.
     *
     * @param aNames - String array to place found part names into.
     * @param aRe - Regular expression used to search part key words.
     * @param aSort - Sort part name list.
     */
    void SearchEntryNames( wxArrayString& aNames, const wxRegEx& aRe, bool aSort = true );

    /**
     * Checks \a aPart for name conflict in the library.
     *
     * @param aPart - The part to check.
     * @return True if a conflict exists.  Otherwise false.
     */
    bool Conflicts( LIB_PART* aPart );

    /**
     * Find entry by name.
     *
     * @param aName - Name of entry, case sensitive.
     * @return Entry if found.  NULL if not found.
     */
    LIB_ALIAS* FindEntry( const wxString& aName );

    /**
     * Find part by \a aName.
     *
     * This is a helper for FindEntry so casting a LIB_ALIAS pointer to
     * a LIB_PART pointer is not required.
     *
     * @param aName - Name of part, case sensitive.
     * @return LIB_PART* - part if found, else NULL.
     */
    LIB_PART* FindPart( const wxString& aName );

    /**
     * Find alias by \a nName.
     *
     * @param aName - Name of alias, case sensitive.
     * @return Alias if found.  NULL if not found.
     */
    LIB_ALIAS* FindAlias( const wxString& aName )
    {
        return (LIB_ALIAS*) FindEntry( aName );
    }

    /**
     * Add a new \a aAlias entry to the library.
     *
     * First check if a part or alias with the same name already exists
     * in the library and add alias if no conflict occurs.  Once the alias
     * is added to the library it is owned by the library.  Deleting the
     * alias pointer will render the library unstable.  Use RemoveEntry to
     * remove the alias from the library.
     *
     * @param aAlias - Alias to add to library.
     * @return True if alias added to library.  False if a conflict exists.
     */
    bool AddAlias( LIB_ALIAS* aAlias );

    /**
     * Add \a aPart entry to library.
     * Note a part can have an alias list,
     * so these alias will be added in library.
     * Conflicts can happen if aliases are already existing.
     * User is asked to choose what alias is removed (existing, or new)
     *
     * @param aPart - Part to add, caller retains ownership, a clone is added.
     * @return bool - true iff successful.
     */
    bool AddPart( LIB_PART* aPart );

    /**
     * Safely remove \a aEntry from the library and return the next entry.
     *
     * The next entry returned depends on the entry being removed.  If the entry being
     * remove also removes the part, then the next entry from the list is returned.
     * If the entry being used only removes an alias from a part, then the next alias
     * of the part is returned.
     *
     * @param aEntry - Entry to remove from library.
     * @return The next entry in the library or NULL if the library is empty.
     */
    LIB_ALIAS* RemoveEntry( LIB_ALIAS* aEntry );

    /**
     * Replace an existing part entry in the library.
     * Note a part can have an alias list,
     * so these alias will be added in library (and previously existing alias removed)
     * @param aOldPart - The part to replace.
     * @param aNewPart - The new part.
     */
    LIB_PART* ReplacePart( LIB_PART* aOldPart, LIB_PART* aNewPart );

    /**
     * Return the first entry in the library.
     *
     * @return The first entry or NULL if the library has no entries.
     */
    LIB_ALIAS* GetFirstEntry();

    /**
     * Find next library entry by \a aName.
     *
     * If the name of the entry is the last entry in the library, the first
     * entry in the list is returned.
     *
     * @param aName - Name of current entry.
     * @return Next entry if entry name is found. Otherwise NULL.
     */
    LIB_ALIAS* GetNextEntry( const wxString& aName );

    /**
     * Find previous library entry by \a aName.
     *
     * If the name of the entry is the first entry in the library, the last
     * entry in the list is returned.
     *
     * @param aName - Name of current entry.
     * @return Previous entry if entry name is found, otherwise NULL.
     */
    LIB_ALIAS* GetPreviousEntry( const wxString& aName );

    /**
     * Return the file name without path or extension.
     *
     * @return Name of library file.
     */
    const wxString GetName() const            { return fileName.GetName(); }

    /**
     * Function GetFullFileName
     * returns the full file library name with path and extension.
     *
     * @return wxString - Full library file name with path and extension.
     */
    wxString GetFullFileName()          { return fileName.GetFullPath(); }

    /**
     * Function GetLogicalName
     * returns the logical name of the library.
     * @return wxString - The logical name of this library.
     */
    const wxString GetLogicalName()
    {
        /*  for now is the filename without path or extension.

            Technically the library should not know its logical name!
            This will eventually come out of a pair of lookup tables using a
            reverse lookup using the full name or library pointer as a key.
            Search will be by project lookup table and then user lookup table if
            not found.
        */
        return fileName.GetName();
    }


    /**
     * Function SetFileName
     * sets the part library file name.
     *
     * @param aFileName - New library file name.
     */
    void SetFileName( const wxString& aFileName )
    {
        if( aFileName != fileName.GetFullName() )
            fileName = aFileName;
    }

    /**
     * Function LoadLibrary
     * allocates and loads a part library file.
     *
     * @param aFileName - File name of the part library to load.
     * @return PART_LIB* - the allocated and loaded PART_LIB, which is owned by
     *   the caller.
     * @throw IO_ERROR if there's any problem loading the library.
     */
    static PART_LIB* LoadLibrary( const wxString& aFileName ) throw( IO_ERROR, boost::bad_pointer );

    /**
     * Function HasPowerParts
     * @return true if at least one power part is found in lib
     * Useful to select or list only libs containing power parts
     */
    bool HasPowerParts();
};


/**
 * Case insensitive library name comparison.
 */
bool operator==( const PART_LIB& aLibrary, const wxString& aName );
bool operator!=( const PART_LIB& aLibrary, const wxString& aName );

#endif  //  CLASS_LIBRARY_H
