/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2009 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 2009-2016 Wayne Stambaugh <stambaughw@verizon.net>
 * Copyright (C) 1992-2016 KiCad Developers, see AUTHORS.txt for contributors.
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
 * @file id.h
 */


#ifndef ID_H_
#define ID_H_

/**
 * Common command IDs shared by more than one of the KiCad applications.
 *
 * Only place command IDs used in base window class event tables or shared
 * across multple applications such as the zoom, grid, and language IDs.
 * Application specific IDs should be defined in the appropriate header
 * file to prevent the entire project from being rebuilt.
 *
 * However, we must avoid duplicate IDs in menus and toolbar items, when wxUpdateUIEvent
 * are associated to menuitems and/or toolbar items
 * The reason is the fact wxWidgets try to send a wxUpdateUIEvent event to a given window and,
 * if a wxUpdateUIEvent event function is not defined for a menuitem, wxWidgets
 * propagates this event ID to parents of the given window.
 * Therefore duplicate IDs could create strange behavior in menus and subtle bugs, depending
 * on the code inside the wxUpdateUIEvent event functions called in parent frames.
 * I did not seen this propagation to child frames, only to parent frames
 *
 * Issues exist only if 2 menus have the same ID, and only one menu is associated to
 * a wxUpdateUIEvent event, and this one is defined in a parent Window.
 * The probability it happens is low, but not null.
 *
 * Therefore we reserve room in ID list for each sub application.
 * Please, change these values if needed
 */

// Define room for IDs, for each sub application
#define ROOM_FOR_KICADMANAGER 50
#define ROOM_FOR_3D_VIEWER 100

enum main_id
{
    ID_RUN_PCB                  = wxID_HIGHEST,
    ID_RUN_PCB_MODULE_EDITOR,
    ID_RUN_CVPCB,
    ID_RUN_LIBRARY,     // pcbnew & eeschema each use this internally to load their respective lib editors

    ID_LOAD_PROJECT,
    ID_APPEND_PROJECT,
    ID_NEW_PROJECT,
    ID_NEW_PROJECT_FROM_TEMPLATE,
    ID_SAVE_PROJECT,
    ID_SAVE_PROJECT_AS,
    ID_LOAD_FILE,
    ID_APPEND_FILE,
    ID_NEW_BOARD,
    ID_SAVE_BOARD,
    ID_SAVE_BOARD_AS,
    ID_AUTO_SAVE_TIMER,

    ID_CONFIG_REQ,
    ID_CONFIG_SAVE,
    ID_CONFIG_READ,

    ID_PREFERENCES_HOTKEY_START,
    ID_PREFERENCES_HOTKEY_EXPORT_CONFIG,
    ID_PREFERENCES_HOTKEY_IMPORT_CONFIG,
    ID_PREFERENCES_HOTKEY_SHOW_EDITOR,
    ID_PREFERENCES_HOTKEY_SHOW_CURRENT_LIST,
    ID_PREFERENCES_HOTKEY_END,
    ID_PREFERENCES_CONFIGURE_PATHS,

    ID_PREFRENCES_MACROS,
    ID_PREFRENCES_MACROS_SAVE,
    ID_PREFRENCES_MACROS_READ,

    ID_GEN_PLOT,
    ID_GEN_PLOT_PS,
    ID_GEN_PLOT_HPGL,
    ID_GEN_PLOT_GERBER,
    ID_GEN_PLOT_SVG,
    ID_GEN_PLOT_DXF,
    ID_GEN_PLOT_PDF,
    ID_GEN_COPY_SHEET_TO_CLIPBOARD,
    ID_GEN_COPY_BLOCK_TO_CLIPBOARD,

    ID_GEN_EXPORT_FILE,
    ID_GEN_IMPORT_FILE,

    ID_EXIT,
    ID_OPTIONS_SETUP,

    // id for toolbars
    ID_H_TOOLBAR,
    ID_V_TOOLBAR,
    ID_OPT_TOOLBAR,
    ID_AUX_TOOLBAR,

    ID_HELP_COPY_VERSION_STRING,

    ID_EDIT,
    ID_NO_TOOL_SELECTED,
    ID_SEL_BG_COLOR,

    ID_COLORS_SETUP,

    ID_REPEAT_BUTT,

    ID_LANGUAGE_CHOICE,
    ID_LANGUAGE_DEFAULT,
    ID_LANGUAGE_ENGLISH,
    ID_LANGUAGE_FRENCH,
    ID_LANGUAGE_FINNISH,
    ID_LANGUAGE_SPANISH,
    ID_LANGUAGE_GERMAN,
    ID_LANGUAGE_GREEK,
    ID_LANGUAGE_RUSSIAN,
    ID_LANGUAGE_PORTUGUESE,
    ID_LANGUAGE_ITALIAN,
    ID_LANGUAGE_SLOVENIAN,
    ID_LANGUAGE_HUNGARIAN,
    ID_LANGUAGE_POLISH,
    ID_LANGUAGE_CZECH,
    ID_LANGUAGE_KOREAN,
    ID_LANGUAGE_CATALAN,
    ID_LANGUAGE_CHINESE_SIMPLIFIED,
    ID_LANGUAGE_DUTCH,
    ID_LANGUAGE_JAPANESE,
    ID_LANGUAGE_BULGARIAN,
    ID_LANGUAGE_CHOICE_END,

    ID_SET_REPEAT_OPTION,

    // Popup Menu (mouse Right button) (id consecutifs)

    ID_POPUP_GENERAL_START_RANGE,   // first number
    ID_POPUP_CANCEL_CURRENT_COMMAND,
    ID_POPUP_CLOSE_CURRENT_TOOL,
    ID_POPUP_MOVE_BLOCK,
    ID_POPUP_MOVE_BLOCK_EXACT,
    ID_POPUP_DRAG_BLOCK,
    ID_POPUP_COPY_BLOCK,
    ID_POPUP_ROTATE_BLOCK,
    ID_POPUP_DELETE_BLOCK,
    ID_POPUP_FLIP_BLOCK,
    ID_POPUP_PLACE_BLOCK,
    ID_POPUP_ZOOM_BLOCK,
    ID_POPUP_SELECT_ITEMS_BLOCK,
    ID_POPUP_MIRROR_X_BLOCK,
    ID_POPUP_MIRROR_Y_BLOCK,
    ID_POPUP_OTHER_COMMANDS,
    ID_POPUP_GENERAL_END_RANGE, // last number

    ID_POPUP_ENTER_MENU,

    ID_ON_ZOOM_SELECT,
    ID_POPUP_ZOOM_START_RANGE,       // first zoom id
    ID_POPUP_CANCEL,
    ID_POPUP_ZOOM_IN,
    ID_POPUP_ZOOM_OUT,
    ID_POPUP_ZOOM_SELECT,
    ID_POPUP_ZOOM_CENTER,
    ID_POPUP_ZOOM_PAGE,
    ID_POPUP_ZOOM_REDRAW,

    /* Reserve IDs for popup menu zoom levels.  If you need more
     * levels of zoom, change ID_POPUP_ZOOM_LEVEL_END.  Note that more
     * than 15 entries in a context submenu may get too large to display
     * cleanly.  Add any additional popup zoom IDs above here or the
     * zoom event handler will not work properly.
     * currently room is provided for 32 levels (this is a very large value).
     * Pcbnew use 18 values. For schematic 15 is enought
     */
    ID_POPUP_ZOOM_LEVEL_START,
    ID_POPUP_ZOOM_LEVEL_END = ID_POPUP_ZOOM_LEVEL_START + 32,
    ID_POPUP_ZOOM_END_RANGE,         // last zoom id

    ID_ON_GRID_SELECT,
    ID_POPUP_GRID_PLUS,
    ID_POPUP_GRID_MOINS,
    ID_POPUP_GRID_SELECT,
    ID_POPUP_GRID_LEVEL_1000,       // id for first predefined grid in inches (1000 * 0.0001 inch)
    ID_POPUP_GRID_LEVEL_500,
    ID_POPUP_GRID_LEVEL_250,
    ID_POPUP_GRID_LEVEL_200,
    ID_POPUP_GRID_LEVEL_100,
    ID_POPUP_GRID_LEVEL_50,
    ID_POPUP_GRID_LEVEL_25,
    ID_POPUP_GRID_LEVEL_20,
    ID_POPUP_GRID_LEVEL_10,
    ID_POPUP_GRID_LEVEL_5,
    ID_POPUP_GRID_LEVEL_2,
    ID_POPUP_GRID_LEVEL_1,       // id for last predefined grid in inches ( 0.0001 inch)
    ID_POPUP_GRID_LEVEL_5MM,
    ID_POPUP_GRID_LEVEL_2_5MM,
    ID_POPUP_GRID_LEVEL_1MM,     // id for first predefined grid in mm (1mm)
    ID_POPUP_GRID_LEVEL_0_5MM,
    ID_POPUP_GRID_LEVEL_0_25MM,
    ID_POPUP_GRID_LEVEL_0_2MM,
    ID_POPUP_GRID_LEVEL_0_1MM,
    ID_POPUP_GRID_LEVEL_0_0_5MM,
    ID_POPUP_GRID_LEVEL_0_0_25MM,
    ID_POPUP_GRID_LEVEL_0_0_1MM,
    ID_POPUP_GRID_USER,

    ID_SHEET_SET,
    ID_COMPONENT_BUTT,

    ID_ZOOM_BEGIN,
    ID_ZOOM_IN = ID_ZOOM_BEGIN,
    ID_ZOOM_OUT,
    ID_ZOOM_PAGE,
    ID_ZOOM_REDRAW,
    ID_VIEWER_ZOOM_IN,
    ID_VIEWER_ZOOM_OUT,
    ID_VIEWER_ZOOM_PAGE,
    ID_VIEWER_ZOOM_REDRAW,
    // zoom commands for non center zooming
    ID_OFFCENTER_ZOOM_IN,
    ID_OFFCENTER_ZOOM_OUT,
    ID_ZOOM_END,

    // Panning command event IDs.
    ID_PAN_UP,
    ID_PAN_DOWN,
    ID_PAN_LEFT,
    ID_PAN_RIGHT,

    ID_GET_NETLIST,
    ID_GET_TOOLS,
    ID_FIND_ITEMS,

    ID_EDA_SOCKET_EVENT_SERV,
    ID_EDA_SOCKET_EVENT,

    // Common to all
    ID_TB_OPTIONS_SELECT_UNIT_MM,
    ID_TB_OPTIONS_SELECT_UNIT_INCH,
    ID_TB_OPTIONS_SELECT_CURSOR,
    ID_TB_OPTIONS_SHOW_POLAR_COORD,
    ID_TB_OPTIONS_SHOW_GRID,

    // Common to Pcbnew and CvPcb
    ID_TB_OPTIONS_SHOW_MODULE_TEXT_SKETCH,
    ID_TB_OPTIONS_SHOW_MODULE_EDGE_SKETCH,
    ID_TB_OPTIONS_SHOW_PADS_SKETCH,

    ID_DIALOG_ERC,      ///< eeschema ERC modeless dialog ID

    // IDs specifics to a sub-application (Eeschema, Kicad manager....) start here
    //
    // We reserve here Ids for each sub-application, to avoid duplicate IDs
    // between them.
    // mainly we experienced issues related to wxUpdateUIEvent calls when 2 (or more) wxFrames
    // share the same ID in menus, mainly in menubars/toolbars
    // The reason is the fact wxWidgets propagates the wxUpdateUIEvent to all parent windows
    // to find wxUpdateUIEvent event functions matching the menuitem IDs found when activate a menu in the first frame.

    // Reserve ROOM_FOR_KICADMANAGER IDs, for Kicad manager
    // Change it if this count is too small.
    ID_KICAD_MANAGER_START,
    ID_KICAD_MANAGER_END = ID_KICAD_MANAGER_START + ROOM_FOR_KICADMANAGER,

    // Reserve ROOM_FOR_KICADMANAGER IDs, for Kicad manager
    // Change it if this count is too small.
    ID_KICAD_3D_VIEWER_START,
    ID_KICAD_3D_VIEWER_END = ID_KICAD_3D_VIEWER_START + ROOM_FOR_3D_VIEWER,

    ID_END_LIST
};

#endif  // ID_H_
