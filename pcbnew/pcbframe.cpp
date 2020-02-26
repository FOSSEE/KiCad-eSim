/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 2013 SoftPLC Corporation, Dick Hollenbeck <dick@softplc.com>
 * Copyright (C) 2013 Wayne Stambaugh <stambaughw@verizon.net>
 * Copyright (C) 2013-2015 KiCad Developers, see change_log.txt for contributors.
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
 * @file pcbframe.cpp
 * @brief PCB editor main window implementation.
 */

#include <fctsys.h>
#include <kiface_i.h>
#include <pgm_base.h>
#include <class_drawpanel.h>
#include <confirm.h>
#include <wxPcbStruct.h>
#include <collectors.h>
#include <build_version.h>
#include <macros.h>
#include <3d_viewer.h>
#include <msgpanel.h>
#include <fp_lib_table.h>

#include <pcbnew.h>
#include <pcbnew_id.h>
#include <drc_stuff.h>
#include <layer_widget.h>
#include <dialog_design_rules.h>
#include <class_pcb_layer_widget.h>
#include <hotkeys.h>
#include <pcbnew_config.h>
#include <module_editor_frame.h>
#include <dialog_helpers.h>
#include <dialog_plot.h>
#include <convert_from_iu.h>
#include <view/view.h>
#include <view/view_controls.h>
#include <pcb_painter.h>
#include <invoke_pcb_dialog.h>

#include <class_track.h>
#include <class_board.h>
#include <class_module.h>
#include <worksheet_viewitem.h>
#include <ratsnest_data.h>
#include <ratsnest_viewitem.h>

#include <tool/tool_manager.h>
#include <tool/tool_dispatcher.h>
#include <tools/common_actions.h>

#include <scripting/python_console_frame.h>

#if defined(KICAD_SCRIPTING) || defined(KICAD_SCRIPTING_WXPYTHON)
#include <python_scripting.h>
#endif

#include <pcb_draw_panel_gal.h>
#include <gal/graphics_abstraction_layer.h>
#include <boost/bind.hpp>

// Keys used in read/write config
#define OPTKEY_DEFAULT_LINEWIDTH_VALUE  wxT( "PlotLineWidth_mm" )
#define PCB_MAGNETIC_PADS_OPT           wxT( "PcbMagPadOpt" )
#define PCB_MAGNETIC_TRACKS_OPT         wxT( "PcbMagTrackOpt" )
#define SHOW_MICROWAVE_TOOLS            wxT( "ShowMicrowaveTools" )
#define SHOW_LAYER_MANAGER_TOOLS        wxT( "ShowLayerManagerTools" )
#define SHOW_PAGE_LIMITS_KEY            wxT( "ShowPageLimits" )


BEGIN_EVENT_TABLE( PCB_EDIT_FRAME, PCB_BASE_FRAME )
    EVT_SOCKET( ID_EDA_SOCKET_EVENT_SERV, PCB_EDIT_FRAME::OnSockRequestServer )
    EVT_SOCKET( ID_EDA_SOCKET_EVENT, PCB_EDIT_FRAME::OnSockRequest )

    EVT_CHOICE( ID_ON_ZOOM_SELECT, PCB_EDIT_FRAME::OnSelectZoom )
    EVT_CHOICE( ID_ON_GRID_SELECT, PCB_EDIT_FRAME::OnSelectGrid )

    EVT_CLOSE( PCB_EDIT_FRAME::OnCloseWindow )
    EVT_SIZE( PCB_EDIT_FRAME::OnSize )

    EVT_TOOL( ID_LOAD_FILE, PCB_EDIT_FRAME::Files_io )
    EVT_TOOL( ID_MENU_READ_BOARD_BACKUP_FILE, PCB_EDIT_FRAME::Files_io )
    EVT_TOOL( ID_MENU_RECOVER_BOARD_AUTOSAVE, PCB_EDIT_FRAME::Files_io )
    EVT_TOOL( ID_NEW_BOARD, PCB_EDIT_FRAME::Files_io )
    EVT_TOOL( ID_SAVE_BOARD, PCB_EDIT_FRAME::Files_io )
    EVT_TOOL( ID_OPEN_MODULE_EDITOR, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_TOOL( ID_OPEN_MODULE_VIEWER, PCB_EDIT_FRAME::Process_Special_Functions )

    // Menu Files:
    EVT_MENU( ID_MAIN_MENUBAR, PCB_EDIT_FRAME::Process_Special_Functions )

    EVT_MENU( ID_APPEND_FILE, PCB_EDIT_FRAME::Files_io )
    EVT_MENU( ID_SAVE_BOARD_AS, PCB_EDIT_FRAME::Files_io )
    EVT_MENU( ID_COPY_BOARD_AS, PCB_EDIT_FRAME::Files_io )
    EVT_MENU_RANGE( wxID_FILE1, wxID_FILE9, PCB_EDIT_FRAME::OnFileHistory )

    EVT_MENU( ID_GEN_PLOT, PCB_EDIT_FRAME::ToPlotter )

    EVT_MENU( ID_GEN_EXPORT_SPECCTRA, PCB_EDIT_FRAME::ExportToSpecctra )
    EVT_MENU( ID_GEN_EXPORT_FILE_GENCADFORMAT, PCB_EDIT_FRAME::ExportToGenCAD )
    EVT_MENU( ID_GEN_EXPORT_FILE_MODULE_REPORT, PCB_EDIT_FRAME::GenFootprintsReport )
    EVT_MENU( ID_GEN_EXPORT_FILE_VRML, PCB_EDIT_FRAME::OnExportVRML )
    EVT_MENU( ID_GEN_EXPORT_FILE_IDF3, PCB_EDIT_FRAME::ExportToIDF3 )

    EVT_MENU( ID_GEN_IMPORT_SPECCTRA_SESSION,PCB_EDIT_FRAME::ImportSpecctraSession )
    EVT_MENU( ID_GEN_IMPORT_SPECCTRA_DESIGN, PCB_EDIT_FRAME::ImportSpecctraDesign )
    EVT_MENU( ID_GEN_IMPORT_DXF_FILE, PCB_EDIT_FRAME::Process_Special_Functions )

    EVT_MENU( ID_MENU_ARCHIVE_MODULES_IN_LIBRARY, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_MENU( ID_MENU_CREATE_LIBRARY_AND_ARCHIVE_MODULES, PCB_EDIT_FRAME::Process_Special_Functions )

    EVT_MENU( wxID_EXIT, PCB_EDIT_FRAME::OnQuit )

    // menu Config
    EVT_MENU( ID_PCB_DRAWINGS_WIDTHS_SETUP, PCB_EDIT_FRAME::OnConfigurePcbOptions )
    EVT_MENU( ID_PCB_LIB_TABLE_EDIT, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( ID_PCB_LIB_WIZARD, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( ID_PCB_3DSHAPELIB_WIZARD, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( ID_PREFERENCES_CONFIGURE_PATHS, PCB_EDIT_FRAME::OnConfigurePaths )
    EVT_MENU( ID_CONFIG_SAVE, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( ID_CONFIG_READ, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU_RANGE( ID_PREFERENCES_HOTKEY_START, ID_PREFERENCES_HOTKEY_END,
                    PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( ID_MENU_PCB_SHOW_HIDE_LAYERS_MANAGER_DIALOG, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( ID_MENU_PCB_SHOW_HIDE_MUWAVE_TOOLBAR, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( wxID_PREFERENCES, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( ID_PCB_LAYERS_SETUP, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( ID_PCB_MASK_CLEARANCE, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( ID_PCB_PAD_SETUP, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( ID_CONFIG_SAVE, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( ID_CONFIG_READ, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( ID_PREFRENCES_MACROS_SAVE, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( ID_PREFRENCES_MACROS_READ, PCB_EDIT_FRAME::Process_Config )
    EVT_MENU( ID_PCB_DISPLAY_OPTIONS_SETUP, PCB_EDIT_FRAME::InstallDisplayOptionsDialog )
    EVT_MENU( ID_PCB_USER_GRID_SETUP, PCB_EDIT_FRAME::Process_Special_Functions )

    // menu Postprocess
    EVT_MENU( ID_PCB_GEN_POS_MODULES_FILE, PCB_EDIT_FRAME::GenFootprintsPositionFile )
    EVT_MENU( ID_PCB_GEN_DRILL_FILE, PCB_EDIT_FRAME::InstallDrillFrame )
    EVT_MENU( ID_PCB_GEN_D356_FILE, PCB_EDIT_FRAME::GenD356File )
    EVT_MENU( ID_PCB_GEN_CMP_FILE, PCB_EDIT_FRAME::RecreateCmpFileFromBoard )
    EVT_MENU( ID_PCB_GEN_BOM_FILE_FROM_BOARD, PCB_EDIT_FRAME::RecreateBOMFileFromBoard )

    // menu Miscellaneous
    EVT_MENU( ID_MENU_LIST_NETS, PCB_EDIT_FRAME::ListNetsAndSelect )
    EVT_MENU( ID_PCB_GLOBAL_DELETE, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_MENU( ID_MENU_PCB_CLEAN, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_MENU( ID_MENU_PCB_SWAP_LAYERS, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_MENU( ID_MENU_PCB_RESET_TEXTMODULE_FIELDS_SIZES,
              PCB_EDIT_FRAME::OnResetModuleTextSizes )

    // Menu Help
    EVT_MENU( wxID_HELP, EDA_DRAW_FRAME::GetKicadHelp )
    EVT_MENU( wxID_INDEX, EDA_DRAW_FRAME::GetKicadHelp )
    EVT_MENU( wxID_ABOUT, EDA_BASE_FRAME::GetKicadAbout )

    // Menu 3D Frame
    EVT_MENU( ID_MENU_PCB_SHOW_3D_FRAME, PCB_EDIT_FRAME::Show3D_Frame )

    // Switching canvases
    EVT_MENU( ID_MENU_CANVAS_DEFAULT, PCB_BASE_FRAME::SwitchCanvas )
    EVT_MENU( ID_MENU_CANVAS_CAIRO, PCB_BASE_FRAME::SwitchCanvas )
    EVT_MENU( ID_MENU_CANVAS_OPENGL, PCB_BASE_FRAME::SwitchCanvas )

    // Menu Get Design Rules Editor
    EVT_MENU( ID_MENU_PCB_SHOW_DESIGN_RULES_DIALOG, PCB_EDIT_FRAME::ShowDesignRulesEditor )

    // Horizontal toolbar
    EVT_TOOL( ID_RUN_LIBRARY, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_TOOL( ID_SHEET_SET, EDA_DRAW_FRAME::Process_PageSettings )
    EVT_TOOL( wxID_CUT, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_TOOL( wxID_COPY, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_TOOL( wxID_PASTE, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_TOOL( wxID_UNDO, PCB_EDIT_FRAME::RestoreCopyFromUndoList )
    EVT_TOOL( wxID_REDO, PCB_EDIT_FRAME::RestoreCopyFromRedoList )
    EVT_TOOL( wxID_PRINT, PCB_EDIT_FRAME::ToPrinter )
    EVT_TOOL( ID_GEN_PLOT_SVG, PCB_EDIT_FRAME::SVG_Print )
    EVT_TOOL( ID_GEN_PLOT, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_TOOL( ID_FIND_ITEMS, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_TOOL( ID_GET_NETLIST, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_TOOL( ID_DRC_CONTROL, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_TOOL( ID_AUX_TOOLBAR_PCB_SELECT_LAYER_PAIR, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_TOOL( ID_AUX_TOOLBAR_PCB_SELECT_AUTO_WIDTH, PCB_EDIT_FRAME::Tracks_and_Vias_Size_Event )
    EVT_COMBOBOX( ID_TOOLBARH_PCB_SELECT_LAYER, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_CHOICE( ID_AUX_TOOLBAR_PCB_TRACK_WIDTH, PCB_EDIT_FRAME::Tracks_and_Vias_Size_Event )
    EVT_CHOICE( ID_AUX_TOOLBAR_PCB_VIA_SIZE, PCB_EDIT_FRAME::Tracks_and_Vias_Size_Event )
    EVT_TOOL( ID_TOOLBARH_PCB_MODE_MODULE, PCB_EDIT_FRAME::OnSelectAutoPlaceMode )
    EVT_TOOL( ID_TOOLBARH_PCB_MODE_TRACKS, PCB_EDIT_FRAME::OnSelectAutoPlaceMode )
    EVT_TOOL( ID_TOOLBARH_PCB_FREEROUTE_ACCESS, PCB_EDIT_FRAME::Access_to_External_Tool )

    // has meaning only with KICAD_SCRIPTING_WXPYTHON enabled
    EVT_TOOL( ID_TOOLBARH_PCB_SCRIPTING_CONSOLE, PCB_EDIT_FRAME::ScriptingConsoleEnableDisable )
    EVT_UPDATE_UI( ID_TOOLBARH_PCB_SCRIPTING_CONSOLE,
                   PCB_EDIT_FRAME::OnUpdateScriptingConsoleState )

    // Option toolbar
    EVT_TOOL( ID_TB_OPTIONS_DRC_OFF,
                    PCB_EDIT_FRAME::OnSelectOptionToolbar )
    EVT_TOOL( ID_TB_OPTIONS_SHOW_RATSNEST,
                    PCB_EDIT_FRAME::OnSelectOptionToolbar )
    EVT_TOOL( ID_TB_OPTIONS_SHOW_MODULE_RATSNEST,
                    PCB_EDIT_FRAME::OnSelectOptionToolbar )
    EVT_TOOL( ID_TB_OPTIONS_AUTO_DEL_TRACK,
                    PCB_EDIT_FRAME::OnSelectOptionToolbar )
    EVT_TOOL( ID_TB_OPTIONS_SHOW_VIAS_SKETCH,
                    PCB_EDIT_FRAME::OnSelectOptionToolbar )
    EVT_TOOL( ID_TB_OPTIONS_SHOW_TRACKS_SKETCH,
                    PCB_EDIT_FRAME::OnSelectOptionToolbar )
    EVT_TOOL( ID_TB_OPTIONS_SHOW_HIGH_CONTRAST_MODE,
                    PCB_EDIT_FRAME::OnSelectOptionToolbar )
    EVT_TOOL( ID_TB_OPTIONS_SHOW_EXTRA_VERTICAL_TOOLBAR_MICROWAVE,
                    PCB_EDIT_FRAME::OnSelectOptionToolbar )

    EVT_TOOL_RANGE( ID_TB_OPTIONS_SHOW_ZONES, ID_TB_OPTIONS_SHOW_ZONES_OUTLINES_ONLY,
                    PCB_EDIT_FRAME::OnSelectOptionToolbar )

    EVT_TOOL( ID_TB_OPTIONS_SHOW_MANAGE_LAYERS_VERTICAL_TOOLBAR,
              PCB_EDIT_FRAME::OnSelectOptionToolbar )

    // Vertical main toolbar:
    EVT_TOOL( ID_NO_TOOL_SELECTED, PCB_EDIT_FRAME::OnSelectTool )
    EVT_TOOL_RANGE( ID_PCB_HIGHLIGHT_BUTT, ID_PCB_PLACE_GRID_COORD_BUTT,
                    PCB_EDIT_FRAME::OnSelectTool )

    EVT_TOOL_RANGE( ID_PCB_MUWAVE_START_CMD, ID_PCB_MUWAVE_END_CMD,
                    PCB_EDIT_FRAME::ProcessMuWaveFunctions )

    EVT_MENU_RANGE( ID_POPUP_PCB_AUTOPLACE_START_RANGE, ID_POPUP_PCB_AUTOPLACE_END_RANGE,
                    PCB_EDIT_FRAME::OnPlaceOrRouteFootprints )

    EVT_MENU( ID_POPUP_PCB_REORIENT_ALL_MODULES, PCB_EDIT_FRAME::OnOrientFootprints )

    EVT_MENU_RANGE( ID_POPUP_PCB_START_RANGE, ID_POPUP_PCB_END_RANGE,
                    PCB_EDIT_FRAME::Process_Special_Functions )

    // Tracks and vias sizes general options
    EVT_MENU_RANGE( ID_POPUP_PCB_SELECT_WIDTH_START_RANGE,
                    ID_POPUP_PCB_SELECT_WIDTH_END_RANGE,
                    PCB_EDIT_FRAME::Tracks_and_Vias_Size_Event )

    // popup menus
    EVT_MENU( ID_POPUP_PCB_DELETE_TRACKSEG, PCB_EDIT_FRAME::Process_Special_Functions )
    EVT_MENU_RANGE( ID_POPUP_GENERAL_START_RANGE, ID_POPUP_GENERAL_END_RANGE,
                    PCB_EDIT_FRAME::Process_Special_Functions )

    // User interface update event handlers.
    EVT_UPDATE_UI( ID_SAVE_BOARD, PCB_EDIT_FRAME::OnUpdateSave )
    EVT_UPDATE_UI( ID_AUX_TOOLBAR_PCB_SELECT_LAYER_PAIR, PCB_EDIT_FRAME::OnUpdateLayerPair )
    EVT_UPDATE_UI( ID_TOOLBARH_PCB_SELECT_LAYER, PCB_EDIT_FRAME::OnUpdateLayerSelectBox )
    EVT_UPDATE_UI( ID_TB_OPTIONS_DRC_OFF, PCB_EDIT_FRAME::OnUpdateDrcEnable )
    EVT_UPDATE_UI( ID_TB_OPTIONS_SHOW_RATSNEST, PCB_EDIT_FRAME::OnUpdateShowBoardRatsnest )
    EVT_UPDATE_UI( ID_TB_OPTIONS_SHOW_MODULE_RATSNEST, PCB_EDIT_FRAME::OnUpdateShowModuleRatsnest )
    EVT_UPDATE_UI( ID_TB_OPTIONS_AUTO_DEL_TRACK, PCB_EDIT_FRAME::OnUpdateAutoDeleteTrack )
    EVT_UPDATE_UI( ID_TB_OPTIONS_SHOW_VIAS_SKETCH, PCB_EDIT_FRAME::OnUpdateViaDrawMode )
    EVT_UPDATE_UI( ID_TB_OPTIONS_SHOW_TRACKS_SKETCH, PCB_EDIT_FRAME::OnUpdateTraceDrawMode )
    EVT_UPDATE_UI( ID_TB_OPTIONS_SHOW_HIGH_CONTRAST_MODE,
                   PCB_EDIT_FRAME::OnUpdateHighContrastDisplayMode )
    EVT_UPDATE_UI( ID_TB_OPTIONS_SHOW_MANAGE_LAYERS_VERTICAL_TOOLBAR,
                   PCB_EDIT_FRAME::OnUpdateShowLayerManager )
    EVT_UPDATE_UI( ID_TB_OPTIONS_SHOW_EXTRA_VERTICAL_TOOLBAR_MICROWAVE,
                   PCB_EDIT_FRAME::OnUpdateShowMicrowaveToolbar )
    EVT_UPDATE_UI( ID_NO_TOOL_SELECTED, PCB_EDIT_FRAME::OnUpdateVerticalToolbar )
    EVT_UPDATE_UI( ID_AUX_TOOLBAR_PCB_TRACK_WIDTH, PCB_EDIT_FRAME::OnUpdateSelectTrackWidth )
    EVT_UPDATE_UI( ID_AUX_TOOLBAR_PCB_SELECT_AUTO_WIDTH,
                   PCB_EDIT_FRAME::OnUpdateSelectAutoTrackWidth )
    EVT_UPDATE_UI( ID_POPUP_PCB_SELECT_AUTO_WIDTH, PCB_EDIT_FRAME::OnUpdateSelectAutoTrackWidth )
    EVT_UPDATE_UI( ID_POPUP_PCB_SELECT_CUSTOM_WIDTH,
                   PCB_EDIT_FRAME::OnUpdateSelectCustomTrackWidth )
    EVT_UPDATE_UI( ID_AUX_TOOLBAR_PCB_VIA_SIZE, PCB_EDIT_FRAME::OnUpdateSelectViaSize )
    EVT_UPDATE_UI( ID_TOOLBARH_PCB_MODE_MODULE, PCB_EDIT_FRAME::OnUpdateAutoPlaceModulesMode )
    EVT_UPDATE_UI( ID_TOOLBARH_PCB_MODE_TRACKS, PCB_EDIT_FRAME::OnUpdateAutoPlaceTracksMode )
    EVT_UPDATE_UI_RANGE( ID_POPUP_PCB_SELECT_WIDTH1, ID_POPUP_PCB_SELECT_WIDTH8,
                         PCB_EDIT_FRAME::OnUpdateSelectTrackWidth )
    EVT_UPDATE_UI_RANGE( ID_POPUP_PCB_SELECT_VIASIZE1, ID_POPUP_PCB_SELECT_VIASIZE8,
                         PCB_EDIT_FRAME::OnUpdateSelectViaSize )
    EVT_UPDATE_UI_RANGE( ID_PCB_HIGHLIGHT_BUTT, ID_PCB_PLACE_GRID_COORD_BUTT,
                         PCB_EDIT_FRAME::OnUpdateVerticalToolbar )
    EVT_UPDATE_UI_RANGE( ID_TB_OPTIONS_SHOW_ZONES, ID_TB_OPTIONS_SHOW_ZONES_OUTLINES_ONLY,
                         PCB_EDIT_FRAME::OnUpdateZoneDisplayStyle )
    EVT_UPDATE_UI_RANGE( ID_PCB_MUWAVE_START_CMD, ID_PCB_MUWAVE_END_CMD,
                         PCB_EDIT_FRAME::OnUpdateMuWaveToolbar )

    EVT_COMMAND( wxID_ANY, LAYER_WIDGET::EVT_LAYER_COLOR_CHANGE, PCB_EDIT_FRAME::OnLayerColorChange )
END_EVENT_TABLE()


PCB_EDIT_FRAME::PCB_EDIT_FRAME( KIWAY* aKiway, wxWindow* aParent ) :
    PCB_BASE_EDIT_FRAME( aKiway, aParent, FRAME_PCB, wxT( "Pcbnew" ), wxDefaultPosition,
        wxDefaultSize, KICAD_DEFAULT_DRAWFRAME_STYLE, PCB_EDIT_FRAME_NAME )
{
    m_showBorderAndTitleBlock = true;   // true to display sheet references
    m_showAxis = false;                 // true to display X and Y axis
    m_showOriginAxis = true;
    m_showGridAxis = true;
    m_SelTrackWidthBox = NULL;
    m_SelViaSizeBox = NULL;
    m_SelLayerBox = NULL;
    m_show_microwave_tools = false;
    m_show_layer_manager_tools = true;
    m_hotkeysDescrList = g_Board_Editor_Hokeys_Descr;
    m_hasAutoSave = true;
    m_RecordingMacros = -1;
    m_microWaveToolBar = NULL;

    m_rotationAngle = 900;

    for ( int i = 0; i < 10; i++ )
        m_Macros[i].m_Record.clear();

    // Create GAL canvas
    EDA_DRAW_PANEL_GAL* galCanvas = new PCB_DRAW_PANEL_GAL( this, -1, wxPoint( 0, 0 ),
                                                m_FrameSize, EDA_DRAW_PANEL_GAL::GAL_TYPE_NONE );

    SetGalCanvas( galCanvas );

    SetBoard( new BOARD() );

    // Create the PCB_LAYER_WIDGET *after* SetBoard():

    wxFont font = wxSystemSettings::GetFont( wxSYS_DEFAULT_GUI_FONT );
    int pointSize = font.GetPointSize();
    int screenHeight = wxSystemSettings::GetMetric( wxSYS_SCREEN_Y );

    // printf( "pointSize:%d  80%%:%d\n", pointSize, (pointSize*8)/10 );

    if( screenHeight <= 900 )
        pointSize = (pointSize * 8) / 10;

    m_Layers = new PCB_LAYER_WIDGET( this, GetCanvas(), pointSize );

    m_drc = new DRC( this );        // these 2 objects point to each other

    wxIcon  icon;
    icon.CopyFromBitmap( KiBitmap( icon_pcbnew_xpm ) );
    SetIcon( icon );

    // LoadSettings() *after* creating m_LayersManager, because LoadSettings()
    // initialize parameters in m_LayersManager
    LoadSettings( config() );

    SetScreen( new PCB_SCREEN( GetPageSettings().GetSizeIU() ) );
    GetScreen()->SetMaxUndoItems( m_UndoRedoCountMax );

    // PCB drawings start in the upper left corner.
    GetScreen()->m_Center = false;

    SetSize( m_FramePos.x, m_FramePos.y, m_FrameSize.x, m_FrameSize.y );

    GetScreen()->AddGrid( m_UserGridSize, m_UserGridUnit, ID_POPUP_GRID_USER );
    GetScreen()->SetGrid( ID_POPUP_GRID_LEVEL_1000 + m_LastGridSizeId  );

    if( m_canvas )
        m_canvas->SetEnableBlockCommands( true );

    ReCreateMenuBar();
    ReCreateHToolbar();
    ReCreateAuxiliaryToolbar();
    ReCreateVToolbar();
    ReCreateOptToolbar();

    ReCreateMicrowaveVToolbar();

    m_auimgr.SetManagedWindow( this );

    EDA_PANEINFO horiz;
    horiz.HorizontalToolbarPane();

    EDA_PANEINFO vert;
    vert.VerticalToolbarPane();

    EDA_PANEINFO mesg;
    mesg.MessageToolbarPane();

    // Create a wxAuiPaneInfo for the Layers Manager, not derived from the template.
    // LAYER_WIDGET is floatable, but initially docked at far right
    EDA_PANEINFO   lyrs;
    lyrs.LayersToolbarPane();
    lyrs.MinSize( m_Layers->GetBestSize() );    // updated in ReFillLayerWidget
    lyrs.BestSize( m_Layers->GetBestSize() );
    lyrs.Caption( _( "Visibles" ) );
    lyrs.TopDockable( false ).BottomDockable( false );

    if( m_mainToolBar )    // The main horizontal toolbar
    {
        m_auimgr.AddPane( m_mainToolBar,
                          wxAuiPaneInfo( horiz ).Name( wxT( "m_mainToolBar" ) ).Top().Row( 0 ) );
    }

    if( m_auxiliaryToolBar )    // the auxiliary horizontal toolbar, that shows track and via sizes, zoom ...)
    {
        m_auimgr.AddPane( m_auxiliaryToolBar,
                          wxAuiPaneInfo( horiz ).Name( wxT( "m_auxiliaryToolBar" ) ).Top().Row( 1 ) );
    }

    if( m_microWaveToolBar )    // The auxiliary vertical right toolbar (currently microwave tools)
        m_auimgr.AddPane( m_microWaveToolBar,
                          wxAuiPaneInfo( vert ).Name( wxT( "m_microWaveToolBar" ) ).
                          Right().Layer( 1 ).Position(1).Hide() );

    if( m_drawToolBar )    // The main right vertical toolbar
        m_auimgr.AddPane( m_drawToolBar,
                          wxAuiPaneInfo( vert ).Name( wxT( "m_VToolBar" ) ).Right().Layer( 2 ) );

    // Add the layer manager ( most right side of pcbframe )
    m_auimgr.AddPane( m_Layers, lyrs.Name( wxT( "m_LayersManagerToolBar" ) ).Right().Layer( 3 ) );

    if( m_optionsToolBar )    // The left vertical toolbar (fast acces display options of Pcbnew)
    {
        m_auimgr.AddPane( m_optionsToolBar,
                          wxAuiPaneInfo( vert ).Name( wxT( "m_optionsToolBar" ) ).Left().Layer(1) );

        m_auimgr.GetPane( wxT( "m_LayersManagerToolBar" ) ).Show( m_show_layer_manager_tools );
        m_auimgr.GetPane( wxT( "m_microWaveToolBar" ) ).Show( m_show_microwave_tools );
    }

    if( m_canvas )
        m_auimgr.AddPane( m_canvas,
                          wxAuiPaneInfo().Name( wxT( "DrawFrame" ) ).CentrePane() );

    if( GetGalCanvas() )
        m_auimgr.AddPane( (wxWindow*) GetGalCanvas(),
                          wxAuiPaneInfo().Name( wxT( "DrawFrameGal" ) ).CentrePane().Hide() );

    if( m_messagePanel )
        m_auimgr.AddPane( m_messagePanel,
                          wxAuiPaneInfo( mesg ).Name( wxT( "MsgPanel" ) ).Bottom().Layer(10) );

    ReFillLayerWidget();        // this is near end because contents establish size
    m_Layers->ReFillRender();   // Update colors in Render after the config is read
    syncLayerWidgetLayer();

    m_auimgr.Update();

    setupTools();

    Zoom_Automatique( false );

    EDA_DRAW_PANEL_GAL::GAL_TYPE canvasType = LoadCanvasTypeSetting();

    if( canvasType != EDA_DRAW_PANEL_GAL::GAL_TYPE_NONE )
    {
        if( GetGalCanvas()->SwitchBackend( canvasType ) )
            UseGalCanvas( true );
    }

    enableGALSpecificMenus();
}


PCB_EDIT_FRAME::~PCB_EDIT_FRAME()
{
    m_RecordingMacros = -1;

    for( int i = 0; i < 10; i++ )
        m_Macros[i].m_Record.clear();

    delete m_drc;
}


void PCB_EDIT_FRAME::SetBoard( BOARD* aBoard )
{
    PCB_BASE_EDIT_FRAME::SetBoard( aBoard );

    if( IsGalCanvasActive() )
    {
        aBoard->GetRatsnest()->Recalculate();

        // reload the worksheet
        SetPageSettings( aBoard->GetPageSettings() );
    }
}


void PCB_EDIT_FRAME::SetPageSettings( const PAGE_INFO& aPageSettings )
{
    PCB_BASE_FRAME::SetPageSettings( aPageSettings );

    if( IsGalCanvasActive() )
    {
        PCB_DRAW_PANEL_GAL* drawPanel = static_cast<PCB_DRAW_PANEL_GAL*>( GetGalCanvas() );

        // Prepare worksheet template
        KIGFX::WORKSHEET_VIEWITEM* worksheet;
        worksheet = new KIGFX::WORKSHEET_VIEWITEM( &m_Pcb->GetPageSettings(),
                                                   &m_Pcb->GetTitleBlock() );
        worksheet->SetSheetName( std::string( GetScreenDesc().mb_str() ) );

        BASE_SCREEN* screen = GetScreen();

        if( screen != NULL )
        {
            worksheet->SetSheetNumber( screen->m_ScreenNumber );
            worksheet->SetSheetCount( screen->m_NumberOfScreens );
        }

        // PCB_DRAW_PANEL_GAL takes ownership of the worksheet
        drawPanel->SetWorksheet( worksheet );
    }
}


bool PCB_EDIT_FRAME::isAutoSaveRequired() const
{
    if( GetScreen() )
        return GetScreen()->IsSave();

    return false;
}


void PCB_EDIT_FRAME::setupTools()
{
    // Create the manager and dispatcher & route draw panel events to the dispatcher
    m_toolManager = new TOOL_MANAGER;
    m_toolManager->SetEnvironment( m_Pcb, GetGalCanvas()->GetView(),
                                   GetGalCanvas()->GetViewControls(), this );
    m_toolDispatcher = new TOOL_DISPATCHER( m_toolManager );

    // Register tools
    registerAllTools( m_toolManager );

    m_toolManager->ResetTools( TOOL_BASE::RUN );

    // Run the selection tool, it is supposed to be always active
    m_toolManager->InvokeTool( "pcbnew.InteractiveSelection" );
}


void PCB_EDIT_FRAME::ReFillLayerWidget()
{
    m_Layers->ReFill();

    wxAuiPaneInfo& lyrs = m_auimgr.GetPane( m_Layers );

    wxSize bestz = m_Layers->GetBestSize();

    lyrs.MinSize( bestz );
    lyrs.BestSize( bestz );
    lyrs.FloatingSize( bestz );

    if( lyrs.IsDocked() )
        m_auimgr.Update();
    else
        m_Layers->SetSize( bestz );
}


void PCB_EDIT_FRAME::OnQuit( wxCommandEvent& event )
{
    Close( false );
}


void PCB_EDIT_FRAME::OnCloseWindow( wxCloseEvent& Event )
{
    m_canvas->SetAbortRequest( true );

    if( GetScreen()->IsModify() )
    {
        wxString msg = wxString::Format( _(
                "Save the changes in\n"
                "'%s'\n"
                "before closing?" ),
                GetChars( GetBoard()->GetFileName() )
                );

        int ii = DisplayExitDialog( this, msg );
        switch( ii )
        {
        case wxID_CANCEL:
            Event.Veto();
            return;

        case wxID_NO:
            break;

        case wxID_YES:
            // save the board. if the board has no name,
            // the ID_SAVE_BOARD_AS will actually made
            Files_io_from_id( ID_SAVE_BOARD );
            break;
        }
    }

    GetGalCanvas()->StopDrawing();

    // Delete the auto save file if it exists.
    wxFileName fn = GetBoard()->GetFileName();

    // Auto save file name is the normal file name prefixed with '_autosave'.
    fn.SetName( GetAutoSaveFilePrefix() + fn.GetName() );

    // When the auto save feature does not have write access to the board file path, it falls
    // back to a platform specific user temporary file path.
    if( !fn.IsOk() || !fn.IsDirWritable() )
        fn.SetPath( wxFileName::GetTempDir() );

    wxLogTrace( traceAutoSave, "Deleting auto save file <" + fn.GetFullPath() + ">" );

    // Remove the auto save file on a normal close of Pcbnew.
    if( fn.FileExists() && !wxRemoveFile( fn.GetFullPath() ) )
    {
        wxString msg = wxString::Format( _(
                "The auto save file '%s' could not be removed!" ),
                GetChars( fn.GetFullPath() )
                );

        wxMessageBox( msg, Pgm().App().GetAppName(), wxOK | wxICON_ERROR, this );
    }

    // Delete board structs and undo/redo lists, to avoid crash on exit
    // when deleting some structs (mainly in undo/redo lists) too late
    Clear_Pcb( false );

    // do not show the window because ScreenPcb will be deleted and we do not
    // want any paint event
    Show( false );

    Destroy();
}


void PCB_EDIT_FRAME::Show3D_Frame( wxCommandEvent& event )
{
    if( m_Draw3DFrame )
    {
        // Raising the window does not show the window on Windows if iconized.
        // This should work on any platform.
        if( m_Draw3DFrame->IsIconized() )
             m_Draw3DFrame->Iconize( false );

        m_Draw3DFrame->Raise();

        // Raising the window does not set the focus on Linux.  This should work on any platform.
        if( wxWindow::FindFocus() != m_Draw3DFrame )
            m_Draw3DFrame->SetFocus();

        return;
    }

    m_Draw3DFrame = new EDA_3D_FRAME( &Kiway(), this, _( "3D Viewer" ) );
    m_Draw3DFrame->SetDefaultFileName( GetBoard()->GetFileName() );
    m_Draw3DFrame->Raise();     // Needed with some Window Managers
    m_Draw3DFrame->Show( true );
}


void PCB_EDIT_FRAME::UseGalCanvas( bool aEnable )
{
    if( aEnable )
    {
        BOARD* board = GetBoard();

        if( board )
            board->GetRatsnest()->ProcessBoard();
    }
    else
    {
        Compile_Ratsnest( NULL, true );
    }

    PCB_BASE_EDIT_FRAME::UseGalCanvas( aEnable );

    enableGALSpecificMenus();
}


void PCB_EDIT_FRAME::enableGALSpecificMenus()
{
    // some menus are active only in GAL mode and do nothing in legacy mode.
    // So enable or disable them, depending on the display mode

    if( GetMenuBar() )
    {
        // Enable / disable some menus which are usable only on GAL
        pcbnew_ids id_list[] =
        {
            ID_MENU_INTERACTIVE_ROUTER_SETTINGS,
            ID_DIFF_PAIR_BUTT,
            ID_TUNE_SINGLE_TRACK_LEN_BUTT,
            ID_TUNE_DIFF_PAIR_LEN_BUTT,
            ID_TUNE_DIFF_PAIR_SKEW_BUTT,
            ID_MENU_DIFF_PAIR_DIMENSIONS
        };

        bool enbl = IsGalCanvasActive();

        for( unsigned ii = 0; ii < DIM( id_list ); ii++ )
        {
            if( GetMenuBar()->FindItem( id_list[ii] ) )
                GetMenuBar()->FindItem( id_list[ii] )->Enable( enbl );
        }
    }
}


void PCB_EDIT_FRAME::ShowDesignRulesEditor( wxCommandEvent& event )
{
    DIALOG_DESIGN_RULES dR_editor( this );
    int returncode = dR_editor.ShowModal();

    if( returncode == wxID_OK )     // New rules, or others changes.
    {
        ReCreateLayerBox();
        ReCreateAuxiliaryToolbar();
        OnModify();
    }
}


void PCB_EDIT_FRAME::LoadSettings( wxConfigBase* aCfg )
{
    PCB_BASE_FRAME::LoadSettings( aCfg );

    wxConfigLoadSetups( aCfg, GetConfigurationSettings() );

    double dtmp;
    aCfg->Read( OPTKEY_DEFAULT_LINEWIDTH_VALUE, &dtmp, 0.1 ); // stored in mm

    if( dtmp < 0.01 )
        dtmp = 0.01;

    if( dtmp > 5.0 )
        dtmp = 5.0;

    g_DrawDefaultLineThickness = Millimeter2iu( dtmp );

    aCfg->Read( PCB_MAGNETIC_PADS_OPT, &g_MagneticPadOption );
    aCfg->Read( PCB_MAGNETIC_TRACKS_OPT, &g_MagneticTrackOption );
    aCfg->Read( SHOW_MICROWAVE_TOOLS, &m_show_microwave_tools );
    aCfg->Read( SHOW_LAYER_MANAGER_TOOLS, &m_show_layer_manager_tools );
    aCfg->Read( SHOW_PAGE_LIMITS_KEY, &m_showPageLimits );
}


void PCB_EDIT_FRAME::SaveSettings( wxConfigBase* aCfg )
{
    PCB_BASE_FRAME::SaveSettings( aCfg );

    wxConfigSaveSetups( aCfg, GetConfigurationSettings() );

    // This value is stored in mm )
    aCfg->Write( OPTKEY_DEFAULT_LINEWIDTH_VALUE, MM_PER_IU * g_DrawDefaultLineThickness );
    aCfg->Write( PCB_MAGNETIC_PADS_OPT, (long) g_MagneticPadOption );
    aCfg->Write( PCB_MAGNETIC_TRACKS_OPT, (long) g_MagneticTrackOption );
    aCfg->Write( SHOW_MICROWAVE_TOOLS, (long) m_show_microwave_tools );
    aCfg->Write( SHOW_LAYER_MANAGER_TOOLS, (long)m_show_layer_manager_tools );
    aCfg->Write( SHOW_PAGE_LIMITS_KEY, m_showPageLimits );
}


bool PCB_EDIT_FRAME::IsGridVisible() const
{
    return IsElementVisible( GRID_VISIBLE );
}


void PCB_EDIT_FRAME::SetGridVisibility(bool aVisible)
{
    SetElementVisibility( GRID_VISIBLE, aVisible );
}


EDA_COLOR_T PCB_EDIT_FRAME::GetGridColor() const
{
    return GetBoard()->GetVisibleElementColor( GRID_VISIBLE );
}


void PCB_EDIT_FRAME::SetGridColor( EDA_COLOR_T aColor )
{

    GetBoard()->SetVisibleElementColor( GRID_VISIBLE, aColor );

    if( IsGalCanvasActive() )
    {
        StructColors c = g_ColorRefs[ aColor ];
        KIGFX::COLOR4D color(  (double) c.m_Red / 255.0,
                        (double) c.m_Green / 255.0,
                        (double) c.m_Blue / 255.0,
                        0.7 );

         GetGalCanvas()->GetGAL()->SetGridColor( color );
    }
}


void PCB_EDIT_FRAME::SetCursorShape( int aCursorShape )
{
    const unsigned int BIG_CURSOR = 8000;
    const unsigned int SMALL_CURSOR = 80;

    EDA_DRAW_FRAME::SetCursorShape( aCursorShape );
    KIGFX::GAL* gal = GetGalCanvas()->GetGAL();

    if( gal )
        gal->SetCursorSize( aCursorShape ? BIG_CURSOR : SMALL_CURSOR );
}


bool PCB_EDIT_FRAME::IsMicroViaAcceptable()
{
    int copperlayercnt = GetBoard()->GetCopperLayerCount( );
    LAYER_ID currLayer = GetActiveLayer();

    if( !GetDesignSettings().m_MicroViasAllowed )
        return false;   // Obvious..

    if( copperlayercnt < 4 )
        return false;   // Only on multilayer boards..

    if( ( currLayer == B_Cu )
       || ( currLayer == F_Cu )
       || ( currLayer == copperlayercnt - 2 )
       || ( currLayer == In1_Cu ) )
        return true;

    return false;
}


void PCB_EDIT_FRAME::SetActiveLayer( LAYER_ID aLayer )
{
    PCB_BASE_FRAME::SetActiveLayer( aLayer );

    syncLayerWidgetLayer();

    if( IsGalCanvasActive() )
    {
        m_toolManager->RunAction( COMMON_ACTIONS::layerChanged );       // notify other tools
        GetGalCanvas()->SetFocus();                 // otherwise hotkeys are stuck somewhere
        GetGalCanvas()->SetHighContrastLayer( aLayer );
        GetGalCanvas()->Refresh();
    }
}


void PCB_EDIT_FRAME::syncLayerWidgetLayer()
{
    m_Layers->SelectLayer( GetActiveLayer() );
    m_Layers->OnLayerSelected();
}


void PCB_EDIT_FRAME::syncRenderStates()
{
    m_Layers->SyncRenderStates();
}


void PCB_EDIT_FRAME::syncLayerVisibilities()
{
    m_Layers->SyncLayerVisibilities();
    static_cast<PCB_DRAW_PANEL_GAL*>( GetGalCanvas() )->SyncLayersVisibility( m_Pcb );
}


void PCB_EDIT_FRAME::unitsChangeRefresh()
{
    PCB_BASE_FRAME::unitsChangeRefresh();    // Update the grid size select box.

    ReCreateAuxiliaryToolbar();
}


bool PCB_EDIT_FRAME::IsElementVisible( int aElement ) const
{
    return GetBoard()->IsElementVisible( aElement );
}


void PCB_EDIT_FRAME::SetElementVisibility( int aElement, bool aNewState )
{
    GetGalCanvas()->GetView()->SetLayerVisible( ITEM_GAL_LAYER( aElement ), aNewState );
    GetBoard()->SetElementVisibility( aElement, aNewState );
    m_Layers->SetRenderState( aElement, aNewState );
}


void PCB_EDIT_FRAME::SetVisibleAlls()
{
    GetBoard()->SetVisibleAlls();

    for( int ii = 0; ii < PCB_VISIBLE( END_PCB_VISIBLE_LIST ); ii++ )
        m_Layers->SetRenderState( ii, true );
}


void PCB_EDIT_FRAME::ShowChangedLanguage()
{
    // call my base class
    PCB_BASE_FRAME::ShowChangedLanguage();

    m_Layers->SetLayersManagerTabsText();

    wxAuiPaneInfo& pane_info = m_auimgr.GetPane( m_Layers );

    pane_info.Caption( _( "Visibles" ) );
    m_auimgr.Update();
    ReFillLayerWidget();
}


wxString PCB_EDIT_FRAME::GetLastNetListRead()
{
    wxFileName absoluteFileName = m_lastNetListRead;
    wxFileName pcbFileName = GetBoard()->GetFileName();

    if( !absoluteFileName.MakeAbsolute( pcbFileName.GetPath() ) || !absoluteFileName.FileExists() )
    {
        absoluteFileName.Clear();
        m_lastNetListRead = wxEmptyString;
    }

    return absoluteFileName.GetFullPath();
}


void PCB_EDIT_FRAME::SetLastNetListRead( const wxString& aLastNetListRead )
{
    wxFileName relativeFileName = aLastNetListRead;
    wxFileName pcbFileName = GetBoard()->GetFileName();

    if( relativeFileName.MakeRelativeTo( pcbFileName.GetPath() )
        && relativeFileName.GetFullPath() != aLastNetListRead )
    {
        m_lastNetListRead = relativeFileName.GetFullPath();
    }
}


void PCB_EDIT_FRAME::OnModify( )
{
    PCB_BASE_FRAME::OnModify();

    if( m_Draw3DFrame )
        m_Draw3DFrame->ReloadRequest();
}


void PCB_EDIT_FRAME::SVG_Print( wxCommandEvent& event )
{
    PCB_PLOT_PARAMS  plot_prms = GetPlotSettings();

    // we don't want dialogs knowing about complex wxFrame functions so
    // pass everything the dialog needs without reference to *this frame's class.
    if( InvokeSVGPrint( this, GetBoard(), &plot_prms ) )
    {
        if( !plot_prms.IsSameAs( GetPlotSettings(), false ) )
        {
            // First, mark board as modified only for parameters saved in file
            if( !plot_prms.IsSameAs( GetPlotSettings(), true ) )
                OnModify();

            // Now, save any change, for the session
            SetPlotSettings( plot_prms );
        }
    }
}


void PCB_EDIT_FRAME::UpdateTitle()
{
    wxFileName  fileName = GetBoard()->GetFileName();
    wxString    title = wxString::Format( wxT( "Pcbnew %s " ), GetChars( GetBuildVersion() ) );

    if( fileName.IsOk() && fileName.FileExists() )
    {
        title << fileName.GetFullPath();

        if( !fileName.IsFileWritable() )
            title << _( " [Read Only]" );
    }
    else
    {
        title << _( " [new file]" ) << wxT(" ") << fileName.GetFullPath();
    }

    SetTitle( title );
}


wxSize PYTHON_CONSOLE_FRAME::m_frameSize;   ///< The size of the PYTHON_CONSOLE_FRAME frame, stored during a session
wxPoint PYTHON_CONSOLE_FRAME::m_framePos;   ///< The position ofPYTHON_CONSOLE_FRAME  the frame, stored during a session

void PCB_EDIT_FRAME::ScriptingConsoleEnableDisable( wxCommandEvent& aEvent )
{

    wxWindow * pythonPanelFrame = findPythonConsole();
    bool pythonPanelShown = true;

    if( pythonPanelFrame == NULL )
        pythonPanelFrame = new PYTHON_CONSOLE_FRAME( this, pythonConsoleNameId() );
    else
        pythonPanelShown = ! pythonPanelFrame->IsShown();

    if( pythonPanelFrame )
        pythonPanelFrame->Show( pythonPanelShown );
    else
        wxMessageBox( wxT( "Error: unable to create the Python Console" ) );
}


void PCB_EDIT_FRAME::OnSelectAutoPlaceMode( wxCommandEvent& aEvent )
{
    // Automatic placement of modules and tracks is a mutually exclusive operation so
    // clear the other tool if one of the two is selected.
    // Be careful: this event function is called both by the
    // ID_TOOLBARH_PCB_MODE_MODULE and the ID_TOOLBARH_PCB_MODE_TRACKS tool
    // Therefore we should avoid a race condition when deselecting one of these tools
    // inside this function (seems happen on some Linux/wxWidgets versions)
    // when the other tool is selected

    switch( aEvent.GetId() )
    {
    case ID_TOOLBARH_PCB_MODE_MODULE:
        if( aEvent.IsChecked() &&
            m_mainToolBar->GetToolToggled( ID_TOOLBARH_PCB_MODE_TRACKS ) )
        {
            m_mainToolBar->ToggleTool( ID_TOOLBARH_PCB_MODE_TRACKS, false );
        }
        break;

    case ID_TOOLBARH_PCB_MODE_TRACKS:
        if( aEvent.IsChecked() &&
            m_mainToolBar->GetToolToggled( ID_TOOLBARH_PCB_MODE_MODULE ) )
        {
            m_mainToolBar->ToggleTool( ID_TOOLBARH_PCB_MODE_MODULE, false );
        }
        break;
    }
}


void PCB_EDIT_FRAME::OnLayerColorChange( wxCommandEvent& aEvent )
{
    ReCreateLayerBox();
}


void PCB_EDIT_FRAME::ToPlotter( wxCommandEvent& event )
{
    DIALOG_PLOT dlg( this );

    dlg.ShowModal();
}


bool PCB_EDIT_FRAME::SetCurrentNetClass( const wxString& aNetClassName )
{
    bool change = GetDesignSettings().SetCurrentNetClass( aNetClassName );

    if( change )
    {
        ReCreateAuxiliaryToolbar();
    }

    return change;
}


void PCB_EDIT_FRAME::OnConfigurePaths( wxCommandEvent& aEvent )
{
    Pgm().ConfigurePaths( this );
}
