/**
 * @file eeschema_config.cpp
 */

/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2014-2016 KiCad Developers, see CHANGELOG.TXT for contributors.
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

#include <fctsys.h>
#include <pgm_base.h>
#include <kiface_i.h>
#include <class_drawpanel.h>
#include <confirm.h>
#include <gestfich.h>
#include <schframe.h>
#include <invoke_sch_dialog.h>
#include <common.h>

#include <eeschema_id.h>
#include <general.h>
#include <libeditframe.h>
#include <eeschema_config.h>
#include <hotkeys.h>
#include <sch_sheet.h>
#include <class_libentry.h>
#include <worksheet_shape_builder.h>
#include <class_library.h>

#include <dialog_hotkeys_editor.h>

#include <dialogs/dialog_color_config.h>
#include <dialogs/dialog_eeschema_options.h>
#include <dialogs/dialog_libedit_options.h>
#include <dialogs/dialog_schematic_find.h>

#include <wildcards_and_files_ext.h>

#define FR_HISTORY_LIST_CNT     10   ///< Maximum number of find and replace strings.


static int s_defaultBusThickness = DEFAULTBUSTHICKNESS;

int GetDefaultBusThickness()
{
    return s_defaultBusThickness;
}


void SetDefaultBusThickness( int aThickness)
{
    if( aThickness >= 1 )
        s_defaultBusThickness = aThickness;
    else
        s_defaultBusThickness = 1;
}


/// Default size for text (not only labels)
static int s_defaultTextSize = DEFAULT_SIZE_TEXT;

int GetDefaultTextSize()
{
    return s_defaultTextSize;
}


void SetDefaultTextSize( int aTextSize )
{
    s_defaultTextSize = aTextSize;
}


/*
 * Default line (in Eeschema units) thickness used to draw/plot items having a
 * default thickness line value (i.e. = 0 ).
 */
static int s_drawDefaultLineThickness  = DEFAULTDRAWLINETHICKNESS;


int GetDefaultLineThickness()
{
    return s_drawDefaultLineThickness;
}


void SetDefaultLineThickness( int aThickness )
{
    if( aThickness >=1 )
        s_drawDefaultLineThickness = aThickness;
    else
        s_drawDefaultLineThickness = 1;
}


// Color to draw selected items
EDA_COLOR_T GetItemSelectedColor()
{
    return BROWN;
}


// Color to draw items flagged invisible, in libedit (they are invisible
// in Eeschema
EDA_COLOR_T GetInvisibleItemColor()
{
    return DARKGRAY;
}


void LIB_EDIT_FRAME::InstallConfigFrame( wxCommandEvent& event )
{
    // Identical to SCH_EDIT_FRAME::InstallConfigFrame()

    PROJECT*        prj = &Prj();
    wxArrayString   lib_names;
    wxString        lib_paths;

    try
    {
        PART_LIBS::LibNamesAndPaths( prj, false, &lib_paths, &lib_names );
    }
    catch( const IO_ERROR& ioe )
    {
        DBG(printf( "%s: %s\n", __func__, TO_UTF8( ioe.errorText ) );)
        return;
    }

    if( InvokeEeschemaConfig( this, &lib_paths, &lib_names ) )
    {
        // save the [changed] settings.
        PART_LIBS::LibNamesAndPaths( prj, true, &lib_paths, &lib_names );

        // Force a reload of the PART_LIBS
        prj->SetElem( PROJECT::ELEM_SCH_PART_LIBS, NULL );
        prj->SetElem( PROJECT::ELEM_SCH_SEARCH_STACK, NULL );
    }
}


void LIB_EDIT_FRAME::OnColorConfig( wxCommandEvent& aEvent )
{
    DIALOG_COLOR_CONFIG dlg( this );

    dlg.ShowModal();
}


void LIB_EDIT_FRAME::Process_Config( wxCommandEvent& event )
{
    int        id = event.GetId();

    switch( id )
    {
    // Hotkey IDs
    case ID_PREFERENCES_HOTKEY_SHOW_EDITOR:
        InstallHotkeyFrame( this, g_Eeschema_Hokeys_Descr );
        break;

    case ID_PREFERENCES_HOTKEY_EXPORT_CONFIG:
        ExportHotkeyConfigToFile( g_Eeschema_Hokeys_Descr, wxT( "eeschema" ) );
        break;

    case ID_PREFERENCES_HOTKEY_IMPORT_CONFIG:
        ImportHotkeyConfigFromFile( g_Eeschema_Hokeys_Descr, wxT( "eeschema" ) );
        break;

    case ID_PREFERENCES_HOTKEY_SHOW_CURRENT_LIST:
        // Display current hotkey list for LibEdit.
        DisplayHotkeyList( this, g_Libedit_Hokeys_Descr );
        break;

    default:
        DisplayError( this, wxT( "LIB_EDIT_FRAME::Process_Config error" ) );
    }
}


void SCH_EDIT_FRAME::OnColorConfig( wxCommandEvent& aEvent )
{
    DIALOG_COLOR_CONFIG dlg( this );

    dlg.ShowModal();
}


void SCH_EDIT_FRAME::InstallConfigFrame( wxCommandEvent& event )
{
    // Identical to LIB_EDIT_FRAME::InstallConfigFrame()

    PROJECT*        prj = &Prj();
    wxArrayString   lib_names;
    wxString        lib_paths;

    try
    {
        PART_LIBS::LibNamesAndPaths( prj, false, &lib_paths, &lib_names );
    }
    catch( const IO_ERROR& ioe )
    {
        DBG(printf( "%s: %s\n", __func__, TO_UTF8( ioe.errorText ) );)
        return;
    }

    if( InvokeEeschemaConfig( this, &lib_paths, &lib_names ) )
    {
        // save the [changed] settings.
        PART_LIBS::LibNamesAndPaths( prj, true, &lib_paths, &lib_names );

#if defined(DEBUG)
        printf( "%s: lib_names:\n", __func__ );
        for( unsigned i=0; i<lib_names.size();  ++i )
        {
            printf( " %s\n", TO_UTF8( lib_names[i] ) );
        }

        printf( "%s: lib_paths:'%s'\n", __func__, TO_UTF8( lib_paths ) );
#endif

        // Force a reload of the PART_LIBS
        prj->SetElem( PROJECT::ELEM_SCH_PART_LIBS, NULL );
        prj->SetElem( PROJECT::ELEM_SCH_SEARCH_STACK, NULL );
    }
}


void SCH_EDIT_FRAME::Process_Config( wxCommandEvent& event )
{
    int        id = event.GetId();
    wxFileName fn;

    switch( id )
    {
    case ID_CONFIG_SAVE:
        SaveProjectSettings( true );
        break;

    case ID_CONFIG_READ:
        {
            fn = g_RootSheet->GetScreen()->GetFileName();
            fn.SetExt( ProjectFileExtension );

            wxFileDialog dlg( this, _( "Read Project File" ), fn.GetPath(),
                              fn.GetFullName(), ProjectFileWildcard,
                              wxFD_OPEN | wxFD_FILE_MUST_EXIST );

            if( dlg.ShowModal() == wxID_CANCEL )
                break;

            wxString chosen = dlg.GetPath();

            if( chosen == Prj().GetProjectFullName() )
                LoadProjectFile();
            else
            {
                // Read library list and library path list
                Prj().ConfigLoad( Kiface().KifaceSearch(), GROUP_SCH, GetProjectFileParametersList() );
                // Read schematic editor setup
                Prj().ConfigLoad( Kiface().KifaceSearch(), GROUP_SCH_EDITOR, GetProjectFileParametersList() );
            }
        }
        break;

    // Hotkey IDs
    case ID_PREFERENCES_HOTKEY_EXPORT_CONFIG:
        ExportHotkeyConfigToFile( g_Eeschema_Hokeys_Descr, wxT( "eeschema" ) );
        break;

    case ID_PREFERENCES_HOTKEY_IMPORT_CONFIG:
        ImportHotkeyConfigFromFile( g_Eeschema_Hokeys_Descr, wxT( "eeschema" ) );
        break;

    case ID_PREFERENCES_HOTKEY_SHOW_EDITOR:
        InstallHotkeyFrame( this, g_Eeschema_Hokeys_Descr );
        break;

    case ID_PREFERENCES_HOTKEY_SHOW_CURRENT_LIST:
        // Display current hotkey list for eeschema.
        DisplayHotkeyList( this, g_Schematic_Hokeys_Descr );
        break;

    default:
        DisplayError( this, wxT( "SCH_EDIT_FRAME::Process_Config error" ) );
    }
}


void SCH_EDIT_FRAME::OnPreferencesOptions( wxCommandEvent& event )
{
    wxArrayString units;
    GRIDS grid_list = GetScreen()->GetGrids();
    bool saveProjectConfig = false;

    DIALOG_EESCHEMA_OPTIONS dlg( this );

    units.Add( GetUnitsLabel( INCHES ) );
    units.Add( GetUnitsLabel( MILLIMETRES ) );

    dlg.SetUnits( units, g_UserUnit );
    dlg.SetGridSizes( grid_list, GetScreen()->GetGridCmdId() );
    dlg.SetBusWidth( GetDefaultBusThickness() );
    dlg.SetLineWidth( GetDefaultLineThickness() );
    dlg.SetTextSize( GetDefaultTextSize() );
    dlg.SetRepeatHorizontal( GetRepeatStep().x );
    dlg.SetRepeatVertical( GetRepeatStep().y );
    dlg.SetRepeatLabel( GetRepeatDeltaLabel() );
    dlg.SetAutoSaveInterval( GetAutoSaveInterval() / 60 );
    dlg.SetMaxUndoItems( GetScreen()->GetMaxUndoItems() );
    dlg.SetRefIdSeparator( LIB_PART::GetSubpartIdSeparator(),
                           LIB_PART::GetSubpartFirstId() );

    dlg.SetShowGrid( IsGridVisible() );
    dlg.SetShowHiddenPins( m_showAllPins );
    dlg.SetEnableMiddleButtonPan( m_canvas->GetEnableMiddleButtonPan() );
    dlg.SetEnableMousewheelPan( m_canvas->GetEnableMousewheelPan() );
    dlg.SetEnableZoomNoCenter( m_canvas->GetEnableZoomNoCenter() );
    dlg.SetMiddleButtonPanLimited( m_canvas->GetMiddleButtonPanLimited() );
    dlg.SetEnableAutoPan( m_canvas->GetEnableAutoPan() );
    dlg.SetEnableHVBusOrientation( GetForceHVLines() );
    dlg.SetShowPageLimits( m_showPageLimits );
    dlg.Layout();
    dlg.Fit();
    dlg.SetMinSize( dlg.GetSize() );
    dlg.SetTemplateFields( m_TemplateFieldNames.GetTemplateFieldNames() );

    if( dlg.ShowModal() == wxID_CANCEL )
        return;

    g_UserUnit = (EDA_UNITS_T)dlg.GetUnitsSelection();

    wxRealPoint  gridsize = grid_list[ (size_t) dlg.GetGridSelection() ].m_Size;
    m_LastGridSizeId = GetScreen()->SetGrid( gridsize );

    int sep, firstId;
    dlg.GetRefIdSeparator( sep, firstId);

    if( sep != (int)LIB_PART::GetSubpartIdSeparator() ||
        firstId != (int)LIB_PART::GetSubpartFirstId() )
    {
        LIB_PART::SetSubpartIdNotation( sep, firstId );
        saveProjectConfig = true;
    }

    SetDefaultBusThickness( dlg.GetBusWidth() );
    SetDefaultLineThickness( dlg.GetLineWidth() );

    if( dlg.GetTextSize() != GetDefaultTextSize() )
    {
        SetDefaultTextSize( dlg.GetTextSize() );
        saveProjectConfig = true;
    }

    wxPoint step;
    step.x = dlg.GetRepeatHorizontal();
    step.y = dlg.GetRepeatVertical();
    SetRepeatStep( step );
    SetRepeatDeltaLabel( dlg.GetRepeatLabel() );

    SetAutoSaveInterval( dlg.GetAutoSaveInterval() * 60 );
    GetScreen()->SetMaxUndoItems( dlg.GetMaxUndoItems() );
    SetGridVisibility( dlg.GetShowGrid() );
    m_showAllPins = dlg.GetShowHiddenPins();
    m_canvas->SetEnableMiddleButtonPan( dlg.GetEnableMiddleButtonPan() );
    m_canvas->SetEnableMousewheelPan( dlg.GetEnableMousewheelPan() );
    m_canvas->SetEnableZoomNoCenter( dlg.GetEnableZoomNoCenter() );
    m_canvas->SetMiddleButtonPanLimited( dlg.GetMiddleButtonPanLimited() );
    m_canvas->SetEnableAutoPan( dlg.GetEnableAutoPan() );
    SetForceHVLines( dlg.GetEnableHVBusOrientation() );
    m_showPageLimits = dlg.GetShowPageLimits();

    // Delete all template fieldnames and then restore them using the template field data from
    // the options dialog
    DeleteAllTemplateFieldNames();
    TEMPLATE_FIELDNAMES newFieldNames = dlg.GetTemplateFields();

    for( TEMPLATE_FIELDNAMES::iterator dlgfld = newFieldNames.begin();
         dlgfld != newFieldNames.end(); ++dlgfld )
    {
        TEMPLATE_FIELDNAME fld = *dlgfld;
        AddTemplateFieldName( fld );
    }

    SaveSettings( config() );  // save values shared by eeschema applications.

    if( saveProjectConfig )
        SaveProjectSettings( true );

    m_canvas->Refresh( true );
}


PARAM_CFG_ARRAY& SCH_EDIT_FRAME::GetProjectFileParametersList()
{
    if( !m_projectFileParams.empty() )
        return m_projectFileParams;

    m_projectFileParams.push_back( new PARAM_CFG_FILENAME( wxT( "PageLayoutDescrFile" ),
                                        &BASE_SCREEN::m_PageLayoutDescrFileName ) );

    m_projectFileParams.push_back( new PARAM_CFG_FILENAME( wxT( "PlotDirectoryName" ),
                                        &m_plotDirectoryName ) );

    m_projectFileParams.push_back( new PARAM_CFG_INT( wxT( "SubpartIdSeparator" ),
                                        LIB_PART::SubpartIdSeparatorPtr(),
                                        0, 0, 126 ) );
    m_projectFileParams.push_back( new PARAM_CFG_INT( wxT( "SubpartFirstId" ),
                                        LIB_PART::SubpartFirstIdPtr(),
                                        'A', '1', 'z' ) );

    /* moved to library load/save specific code, in a specific section in .pro file
    m_projectFileParams.push_back( new PARAM_CFG_FILENAME( wxT( "LibDir" ),
                                                           &m_userLibraryPath ) );
    m_projectFileParams.push_back( new PARAM_CFG_LIBNAME_LIST( wxT( "LibName" ),
                                                               &m_componentLibFiles,
                                                               GROUP_SCH_LIBS ) );
    */

    m_projectFileParams.push_back( new PARAM_CFG_WXSTRING( wxT( "NetFmtName" ),
                                                         &m_netListFormat) );
    m_projectFileParams.push_back( new PARAM_CFG_BOOL( wxT( "SpiceForceRefPrefix" ),
                                                    &m_spiceNetlistAddReferencePrefix, false ) );
    m_projectFileParams.push_back( new PARAM_CFG_BOOL( wxT( "SpiceUseNetNumbers" ),
                                                    &m_spiceNetlistUseNetcodeAsNetname, false ) );

    m_projectFileParams.push_back( new PARAM_CFG_INT( wxT( "LabSize" ),
                                                      &s_defaultTextSize,
                                                      DEFAULT_SIZE_TEXT, 5,
                                                      1000 ) );

    return m_projectFileParams;
}


bool SCH_EDIT_FRAME::LoadProjectFile()
{
    // Read library list and library path list
    bool isRead = Prj().ConfigLoad( Kiface().KifaceSearch(),
                    GROUP_SCH, GetProjectFileParametersList() );

    // Read schematic editor setup
    isRead = isRead && Prj().ConfigLoad( Kiface().KifaceSearch(),
                                         GROUP_SCH_EDITOR, GetProjectFileParametersList() );

    // Verify some values, because the config file can be edited by hand,
    // and have bad values:
    LIB_PART::SetSubpartIdNotation(
            LIB_PART::GetSubpartIdSeparator(),
            LIB_PART::GetSubpartFirstId() );

    // Load the page layout decr file, from the filename stored in
    // BASE_SCREEN::m_PageLayoutDescrFileName, read in config project file
    // If empty, or not existing, the default descr is loaded
    WORKSHEET_LAYOUT& pglayout = WORKSHEET_LAYOUT::GetTheInstance();
    wxString pg_fullfilename = WORKSHEET_LAYOUT::MakeFullFileName(
                                    BASE_SCREEN::m_PageLayoutDescrFileName,
                                    Prj().GetProjectPath() );

    pglayout.SetPageLayout( pg_fullfilename );

    return isRead;
}


void SCH_EDIT_FRAME::SaveProjectSettings( bool aAskForSave )
{
    PROJECT&        prj = Prj();
    wxFileName      fn = g_RootSheet->GetScreen()->GetFileName();  //ConfigFileName

    fn.SetExt( ProjectFileExtension );

    if( !IsWritable( fn ) )
        return;

    if( aAskForSave )
    {
        wxFileDialog dlg( this, _( "Save Project File" ),
                          fn.GetPath(), fn.GetFullName(),
                          ProjectFileWildcard, wxFD_SAVE );

        if( dlg.ShowModal() == wxID_CANCEL )
            return;

        fn = dlg.GetPath();
    }

    prj.ConfigSave( Kiface().KifaceSearch(), GROUP_SCH_EDITOR, GetProjectFileParametersList() );
}


static const wxChar DefaultBusWidthEntry[] =        wxT( "DefaultBusWidth" );
static const wxChar DefaultDrawLineWidthEntry[] =   wxT( "DefaultDrawLineWidth" );
static const wxChar ShowHiddenPinsEntry[] =         wxT( "ShowHiddenPins" );
static const wxChar HorzVertLinesOnlyEntry[] =      wxT( "HorizVertLinesOnly" );
static const wxChar PreviewFramePositionXEntry[] =  wxT( "PreviewFramePositionX" );
static const wxChar PreviewFramePositionYEntry[] =  wxT( "PreviewFramePositionY" );
static const wxChar PreviewFrameWidthEntry[] =      wxT( "PreviewFrameWidth" );
static const wxChar PreviewFrameHeightEntry[] =     wxT( "PreviewFrameHeight" );
static const wxChar PrintDialogPositionXEntry[] =   wxT( "PrintDialogPositionX" );
static const wxChar PrintDialogPositionYEntry[] =   wxT( "PrintDialogPositionY" );
static const wxChar PrintDialogWidthEntry[] =       wxT( "PrintDialogWidth" );
static const wxChar PrintDialogHeightEntry[] =      wxT( "PrintDialogHeight" );
static const wxChar FindDialogPositionXEntry[] =    wxT( "FindDialogPositionX" );
static const wxChar FindDialogPositionYEntry[] =    wxT( "FindDialogPositionY" );
static const wxChar FindDialogWidthEntry[] =        wxT( "FindDialogWidth" );
static const wxChar FindDialogHeightEntry[] =       wxT( "FindDialogHeight" );
static const wxChar FindReplaceFlagsEntry[] =       wxT( "LastFindReplaceFlags" );
static const wxChar FindStringEntry[] =             wxT( "LastFindString" );
static const wxChar ReplaceStringEntry[] =          wxT( "LastReplaceString" );
static const wxChar FindStringHistoryEntry[] =      wxT( "FindStringHistoryList%d" );
static const wxChar ReplaceStringHistoryEntry[] =   wxT( "ReplaceStringHistoryList%d" );
static const wxChar FieldNamesEntry[] =             wxT( "FieldNames" );
static const wxChar SimulatorCommandEntry[] =       wxT( "SimCmdLine" );

// Library editor wxConfig entry names.
static const wxChar lastLibExportPathEntry[] =      wxT( "LastLibraryExportPath" );
static const wxChar lastLibImportPathEntry[] =      wxT( "LastLibraryImportPath" );
static const wxChar defaultPinNumSizeEntry[] =      wxT( "LibeditPinNumSize" );
static const wxChar defaultPinNameSizeEntry[] =     wxT( "LibeditPinNameSize" );
static const wxChar DefaultPinLengthEntry[] =       wxT( "DefaultPinLength" );
static const wxChar repeatLibLabelIncEntry[] =      wxT( "LibeditRepeatLabelInc" );
static const wxChar pinRepeatStepEntry[] =          wxT( "LibeditPinRepeatStep" );
static const wxChar repeatLibStepXEntry[] =         wxT( "LibeditRepeatStepX" );
static const wxChar repeatLibStepYEntry[] =         wxT( "LibeditRepeatStepY" );


PARAM_CFG_ARRAY& SCH_EDIT_FRAME::GetConfigurationSettings()
{
    if( !m_configSettings.empty() )
        return m_configSettings;

    m_configSettings.push_back( new PARAM_CFG_BOOL( true, wxT( "ShowPageLimits" ),
                                                    &m_showPageLimits, true ) );
    m_configSettings.push_back( new PARAM_CFG_INT( true, wxT( "Units" ),
                                                   (int*)&g_UserUnit, MILLIMETRES ) );

    m_configSettings.push_back( new PARAM_CFG_BOOL( true, wxT( "PrintMonochrome" ),
                                                    &m_printMonochrome, true ) );
    m_configSettings.push_back( new PARAM_CFG_BOOL( true, wxT( "PrintSheetReferenceAndTitleBlock" ),
                                                    &m_printSheetReference, true ) );

    m_configSettings.push_back( new PARAM_CFG_INT( true, wxT( "RepeatStepX" ),
                                                      &m_repeatStep.x,
                                                      DEFAULT_REPEAT_OFFSET_X,
                                                      -REPEAT_OFFSET_MAX,
                                                      REPEAT_OFFSET_MAX ) );
    m_configSettings.push_back( new PARAM_CFG_INT( true, wxT( "RepeatStepY" ),
                                                      &m_repeatStep.y,
                                                      DEFAULT_REPEAT_OFFSET_Y,
                                                      -REPEAT_OFFSET_MAX,
                                                      REPEAT_OFFSET_MAX ) );
    m_configSettings.push_back( new PARAM_CFG_INT( true, wxT( "RepeatLabelIncrement" ),
                                                      &m_repeatDeltaLabel,
                                                      DEFAULT_REPEAT_LABEL_INC, -10, +10 ) );
    return m_configSettings;
}


void SCH_EDIT_FRAME::LoadSettings( wxConfigBase* aCfg )
{
    EDA_DRAW_FRAME::LoadSettings( aCfg );

    long tmp;

    wxConfigLoadSetups( aCfg, GetConfigurationSettings() );

    SetGridColor( GetLayerColor( LAYER_GRID ) );
    SetDrawBgColor( GetLayerColor( LAYER_BACKGROUND ) );

    SetDefaultBusThickness( aCfg->Read( DefaultBusWidthEntry, DEFAULTBUSTHICKNESS ) );
    SetDefaultLineThickness( aCfg->Read( DefaultDrawLineWidthEntry, DEFAULTDRAWLINETHICKNESS ) );
    aCfg->Read( ShowHiddenPinsEntry, &m_showAllPins, false );
    aCfg->Read( HorzVertLinesOnlyEntry, &m_forceHVLines, true );

    // Load print preview window session settings.
    aCfg->Read( PreviewFramePositionXEntry, &tmp, -1 );
    m_previewPosition.x = (int) tmp;
    aCfg->Read( PreviewFramePositionYEntry, &tmp, -1 );
    m_previewPosition.y = (int) tmp;
    aCfg->Read( PreviewFrameWidthEntry, &tmp, -1 );
    m_previewSize.SetWidth( (int) tmp );
    aCfg->Read( PreviewFrameHeightEntry, &tmp, -1 );
    m_previewSize.SetHeight( (int) tmp );

    // Load print dialog session settings.
    aCfg->Read( PrintDialogPositionXEntry, &tmp, -1 );
    m_printDialogPosition.x = (int) tmp;
    aCfg->Read( PrintDialogPositionYEntry, &tmp, -1 );
    m_printDialogPosition.y = (int) tmp;
    aCfg->Read( PrintDialogWidthEntry, &tmp, -1 );
    m_printDialogSize.SetWidth( (int) tmp );
    aCfg->Read( PrintDialogHeightEntry, &tmp, -1 );
    m_printDialogSize.SetHeight( (int) tmp );

    // Load netlists options:
    aCfg->Read( SimulatorCommandEntry, &m_simulatorCommand );

    // Load find dialog session setting.
    aCfg->Read( FindDialogPositionXEntry, &tmp, -1 );
    m_findDialogPosition.x = (int) tmp;
    aCfg->Read( FindDialogPositionYEntry, &tmp, -1 );
    m_findDialogPosition.y = (int) tmp;
    aCfg->Read( FindDialogWidthEntry, &tmp, -1 );
    m_findDialogSize.SetWidth( (int) tmp );
    aCfg->Read( FindDialogHeightEntry, &tmp, -1 );
    m_findDialogSize.SetHeight( (int) tmp );

    wxASSERT_MSG( m_findReplaceData,
                  wxT( "Find dialog data settings object not created. Bad programmer!" ) );

    aCfg->Read( FindReplaceFlagsEntry, &tmp, (long) wxFR_DOWN );
    m_findReplaceData->SetFlags( (wxUint32) tmp & ~FR_REPLACE_ITEM_FOUND );
    m_findReplaceData->SetFindString( aCfg->Read( FindStringEntry, wxEmptyString ) );
    m_findReplaceData->SetReplaceString( aCfg->Read( ReplaceStringEntry, wxEmptyString ) );

    // Load the find and replace string history list.
    for( int i = 0; i < FR_HISTORY_LIST_CNT; ++i )
    {
        wxString tmpHistory;
        wxString entry;
        entry.Printf( FindStringHistoryEntry, i );
        tmpHistory = aCfg->Read( entry, wxEmptyString );

        if( !tmpHistory.IsEmpty() )
            m_findStringHistoryList.Add( tmpHistory );

        entry.Printf( ReplaceStringHistoryEntry, i );
        tmpHistory = aCfg->Read( entry, wxEmptyString );

        if( !tmpHistory.IsEmpty() )
            m_replaceStringHistoryList.Add( tmpHistory );
    }

    wxString templateFieldNames = aCfg->Read( FieldNamesEntry, wxEmptyString );

    if( !templateFieldNames.IsEmpty() )
    {
        TEMPLATE_FIELDNAMES_LEXER  lexer( TO_UTF8( templateFieldNames ) );

        try
        {
            m_TemplateFieldNames.Parse( &lexer );
        }
        catch( const IO_ERROR& e )
        {
            // @todo show error msg
            DBG( printf( "templatefieldnames parsing error: '%s'\n",
                       TO_UTF8( e.errorText ) ); )
        }
    }
}


void SCH_EDIT_FRAME::SaveSettings( wxConfigBase* aCfg )
{
    EDA_DRAW_FRAME::SaveSettings( aCfg );

    wxConfigSaveSetups( aCfg, GetConfigurationSettings() );

    aCfg->Write( DefaultBusWidthEntry, (long) GetDefaultBusThickness() );
    aCfg->Write( DefaultDrawLineWidthEntry, (long) GetDefaultLineThickness() );
    aCfg->Write( ShowHiddenPinsEntry, m_showAllPins );
    aCfg->Write( HorzVertLinesOnlyEntry, GetForceHVLines() );

    // Save print preview window session settings.
    aCfg->Write( PreviewFramePositionXEntry, m_previewPosition.x );
    aCfg->Write( PreviewFramePositionYEntry, m_previewPosition.y );
    aCfg->Write( PreviewFrameWidthEntry, m_previewSize.GetWidth() );
    aCfg->Write( PreviewFrameHeightEntry, m_previewSize.GetHeight() );

    // Save print dialog session settings.
    aCfg->Write( PrintDialogPositionXEntry, m_printDialogPosition.x );
    aCfg->Write( PrintDialogPositionYEntry, m_printDialogPosition.y );
    aCfg->Write( PrintDialogWidthEntry, m_printDialogSize.GetWidth() );
    aCfg->Write( PrintDialogHeightEntry, m_printDialogSize.GetHeight() );

    // Save netlists options:
    aCfg->Write( SimulatorCommandEntry, m_simulatorCommand );

    // Save find dialog session setting.
    aCfg->Write( FindDialogPositionXEntry, m_findDialogPosition.x );
    aCfg->Write( FindDialogPositionYEntry, m_findDialogPosition.y );
    aCfg->Write( FindDialogWidthEntry, m_findDialogSize.GetWidth() );
    aCfg->Write( FindDialogHeightEntry, m_findDialogSize.GetHeight() );
    wxASSERT_MSG( m_findReplaceData,
                  wxT( "Find dialog data settings object not created. Bad programmer!" ) );
    aCfg->Write( FindReplaceFlagsEntry,
                (long) m_findReplaceData->GetFlags() & ~FR_REPLACE_ITEM_FOUND );
    aCfg->Write( FindStringEntry, m_findReplaceData->GetFindString() );
    aCfg->Write( ReplaceStringEntry, m_findReplaceData->GetReplaceString() );

    // Save the find and replace string history list.
    unsigned i;
    wxString tmpHistory;
    wxString entry;     // invoke constructor outside of any loops

    for( i = 0; i < m_findStringHistoryList.GetCount() && i < FR_HISTORY_LIST_CNT; i++ )
    {
        entry.Printf( FindStringHistoryEntry, i );
        aCfg->Write( entry, m_findStringHistoryList[ i ] );
    }

    for( i = 0; i < m_replaceStringHistoryList.GetCount() && i < FR_HISTORY_LIST_CNT; i++ )
    {
        entry.Printf( ReplaceStringHistoryEntry, i );
        aCfg->Write( entry, m_replaceStringHistoryList[ i ] );
    }

    // Save template fieldnames
    STRING_FORMATTER sf;
    m_TemplateFieldNames.Format( &sf, 0 );

    wxString record = FROM_UTF8( sf.GetString().c_str() );
    record.Replace( wxT("\n"), wxT(""), true );   // strip all newlines
    record.Replace( wxT("  "), wxT(" "), true );  // double space to single

    aCfg->Write( FieldNamesEntry, record );
}


void LIB_EDIT_FRAME::LoadSettings( wxConfigBase* aCfg )
{
    EDA_DRAW_FRAME::LoadSettings( aCfg );

    SetGridColor( GetLayerColor( LAYER_GRID ) );
    SetDrawBgColor( GetLayerColor( LAYER_BACKGROUND ) );

    SetDefaultLineThickness( aCfg->Read( DefaultDrawLineWidthEntry, DEFAULTDRAWLINETHICKNESS ) );
    SetDefaultPinLength( aCfg->Read( DefaultPinLengthEntry, DEFAULTPINLENGTH ) );
    m_textPinNumDefaultSize = aCfg->Read( defaultPinNumSizeEntry, DEFAULTPINNUMSIZE );
    m_textPinNameDefaultSize = aCfg->Read( defaultPinNameSizeEntry, DEFAULTPINNAMESIZE );
    SetRepeatDeltaLabel( aCfg->Read( repeatLibLabelIncEntry, DEFAULT_REPEAT_LABEL_INC ) );
    SetRepeatPinStep( aCfg->Read( pinRepeatStepEntry, DEFAULT_REPEAT_OFFSET_PIN ) );
    wxPoint step;
    step.x = aCfg->Read( repeatLibStepXEntry, (long)DEFAULT_REPEAT_OFFSET_X );
    step.y = aCfg->Read( repeatLibStepYEntry, (long)DEFAULT_REPEAT_OFFSET_Y );
    SetRepeatStep( step );
}


void LIB_EDIT_FRAME::SaveSettings( wxConfigBase* aCfg )
{
    EDA_DRAW_FRAME::SaveSettings( aCfg );

    aCfg->Write( DefaultPinLengthEntry, (long) GetDefaultPinLength() );
    aCfg->Write( defaultPinNumSizeEntry, (long) GetPinNumDefaultSize() );
    aCfg->Write( defaultPinNameSizeEntry, (long) GetPinNameDefaultSize() );
    aCfg->Write( repeatLibLabelIncEntry, (long) GetRepeatDeltaLabel() );
    aCfg->Write( pinRepeatStepEntry, (long) GetRepeatPinStep() );
    aCfg->Write( repeatLibStepXEntry, (long) GetRepeatStep().x );
    aCfg->Write( repeatLibStepYEntry, (long) GetRepeatStep().y );
}


void LIB_EDIT_FRAME::OnPreferencesOptions( wxCommandEvent& event )
{
    wxArrayString units;
    GRIDS grid_list = GetScreen()->GetGrids();

    DIALOG_LIBEDIT_OPTIONS dlg( this );

    dlg.SetGridSizes( grid_list, GetScreen()->GetGridCmdId() );
    dlg.SetLineWidth( GetDefaultLineThickness() );
    dlg.SetPinLength( GetDefaultPinLength() );
    dlg.SetPinNumSize( m_textPinNumDefaultSize );
    dlg.SetPinNameSize( m_textPinNameDefaultSize );
    dlg.SetMaxUndoItems( GetScreen()->GetMaxUndoItems() );

    dlg.SetShowGrid( IsGridVisible() );
    dlg.Layout();
    dlg.Fit();

    if( dlg.ShowModal() == wxID_CANCEL )
        return;

    wxRealPoint  gridsize = grid_list[ (size_t) dlg.GetGridSelection() ].m_Size;
    m_LastGridSizeId = GetScreen()->SetGrid( gridsize );

    SetDefaultLineThickness( dlg.GetLineWidth() );
    SetDefaultPinLength( dlg.GetPinLength() );
    m_textPinNumDefaultSize = dlg.GetPinNumSize();
    m_textPinNameDefaultSize = dlg.GetPinNameSize();
    SetGridVisibility( dlg.GetShowGrid() );
    SetRepeatPinStep( dlg.GetPinRepeatStep() );
    SetRepeatStep( dlg.GetItemRepeatStep() );
    SetRepeatDeltaLabel( dlg.GetRepeatLabelInc() );
    GetScreen()->SetMaxUndoItems( dlg.GetMaxUndoItems() );

    SaveSettings( config() );  // save values shared by eeschema applications.

    m_canvas->Refresh( true );
}

