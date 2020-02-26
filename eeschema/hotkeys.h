/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2007 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 2014 KiCad Developers, see CHANGELOG.TXT for contributors.
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
 * eeschema/hotkeys.h
 */
#ifndef EESCHEMA_KOTKEYS_H_
#define EESCHEMA_KOTKEYS_H_

#include <hotkeys_basic.h>

// List of hot keys id.
// see also enum common_hotkey_id_commnand in hotkeys_basic.h
// for shared hotkeys id
enum hotkey_id_commnand {
    HK_FIND_NEXT_ITEM = HK_COMMON_END,
    HK_FIND_NEXT_DRC_MARKER,
    HK_FIND_ITEM,
    HK_FIND_REPLACE,
    HK_DELETE,
    HK_REPEAT_LAST,
    HK_LIBEDIT_MOVE_GRAPHIC_ITEM,
    HK_MOVEBLOCK_TO_DRAGBLOCK,
    HK_SAVE_BLOCK,
    HK_LIBEDIT_CREATE_PIN,
    HK_DELETE_PIN,
    HK_ROTATE,
    HK_EDIT,
    HK_EDIT_COMPONENT_VALUE,
    HK_EDIT_COMPONENT_REFERENCE,
    HK_EDIT_COMPONENT_FOOTPRINT,
    HK_EDIT_COMPONENT_WITH_LIBEDIT,
    HK_MIRROR_X,
    HK_MIRROR_Y,
    HK_ORIENT_NORMAL_COMPONENT,
    HK_MOVE_COMPONENT_OR_ITEM,
    HK_COPY_COMPONENT_OR_LABEL,
    HK_DRAG,
    HK_ADD_NEW_COMPONENT,
    HK_ADD_NEW_POWER,
    HK_BEGIN_WIRE,
    HK_BEGIN_BUS,
    HK_END_CURR_LINEWIREBUS,
    HK_ADD_WIRE_ENTRY,
    HK_ADD_BUS_ENTRY,
    HK_ADD_LABEL,
    HK_ADD_HLABEL,
    HK_ADD_GLABEL,
    HK_ADD_JUNCTION,
    HK_ADD_HIER_SHEET,
    HK_ADD_GRAPHIC_TEXT,
    HK_ADD_GRAPHIC_POLYLINE,
    HK_ADD_NOCONN_FLAG,
    HK_SAVE_LIB,
    HK_SAVE_SCH,
    HK_LOAD_SCH,
    HK_LEFT_CLICK,
    HK_LEFT_DCLICK,
    HK_LEAVE_SHEET,
    HK_DELETE_NODE
};

// List of hotkey descriptors for Eeschema
extern struct EDA_HOTKEY_CONFIG g_Eeschema_Hokeys_Descr[];

// List of hotkey descriptors for the schematic editor only
extern struct EDA_HOTKEY_CONFIG g_Schematic_Hokeys_Descr[];

// List of hotkey descriptors for the lib editor only
extern struct EDA_HOTKEY_CONFIG g_Libedit_Hokeys_Descr[];

// List of hotkey descriptors for the lib browser only
extern struct EDA_HOTKEY_CONFIG g_Viewlib_Hokeys_Descr[];

#endif      // EESCHEMA_KOTKEYS_H_
