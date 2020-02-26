/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2007-2014 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 1992-2012 KiCad Developers, see AUTHORS.txt for contributors.
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
 * @file pcbnew.h
 */

#ifndef PCBNEW_H
#define PCBNEW_H


#include <fctsys.h>         // wxWidgets include.
#include <base_struct.h>    // IS_DRAGGED and IN_EDIT definitions.
#include <dlist.h>
#include <convert_to_biu.h> // to define DMils2iu() conversion function
#include <layers_id_colors_and_visibility.h>

// Arcs are approximated by segments: define the number of segments per 360 deg (KiCad uses 0.1
// deg approximation).  Be aware 3600 / ARC_APPROX_SEGMENTS_COUNT_LOW_DEF is an integer.
#define ARC_APPROX_SEGMENTS_COUNT_LOW_DEF 16
#define ARC_APPROX_SEGMENTS_COUNT_HIGHT_DEF 32

/* Flag used in locate functions. The locate ref point is the on grid cursor or the off
 * grid mouse cursor. */
#define CURSEUR_ON_GRILLE  (0 << 0)
#define CURSEUR_OFF_GRILLE (1 << 0)

#define IGNORE_LOCKED (1 << 1)   ///< if module is locked, do not select for single module operation
#define MATCH_LAYER   (1 << 2)   ///< if module not on current layer, do not select
#define VISIBLE_ONLY  (1 << 3)   ///< if module not on a visible layer, do not select

/// Flag used in locate routines (from which endpoint work)
enum ENDPOINT_T {
    ENDPOINT_START = 0,
    ENDPOINT_END = 1
};

#define DIM_ANCRE_MODULE 3       // Anchor size (footprint center)


#define TEXTS_MIN_SIZE  DMils2iu( 50 )      ///< Minimum text size in Pcbnew units value (50 * 0.0001 mils)
#define TEXTS_MAX_SIZE  DMils2iu( 10000 )   ///< Maximum text size in Pcbnew units value (1 inch) )
#define TEXTS_MAX_WIDTH DMils2iu( 5000 )    ///< Maximum text width in Pcbnew units value (0.5 inches)
#define MIN_DRAW_WIDTH  1                   ///< Minimum trace drawing width in pixels.


// Flag to force the SKETCH mode to display items (.m_Flags member)
#define FORCE_SKETCH ( IS_DRAGGED | IN_EDIT )

/* Name of the document footprint list
 * usually located in share/modules/footprints_doc
 * this is of the responsibility to users to create this file
 * if they want to have a list of footprints
 * default is "footprints_doc/footprints.pdf"
 */
extern wxString g_DocModulesFileName;

// variables
extern bool     g_Drc_On;
extern bool     g_AutoDeleteOldTrack;
extern bool     g_Raccord_45_Auto;
extern bool     g_Track_45_Only_Allowed;
extern bool     g_Alternate_Track_Posture;
extern bool     g_Segments_45_Only;

// Layer pair for auto routing and switch layers by hotkey
extern LAYER_ID g_Route_Layer_TOP;
extern LAYER_ID g_Route_Layer_BOTTOM;

extern bool     g_TwoSegmentTrackBuild;

extern int      g_MagneticPadOption;
extern int      g_MagneticTrackOption;

extern bool     g_DumpZonesWhenFilling;

extern wxPoint  g_Offset_Module;         // Offset trace when moving footprint.

/// List of segments of the trace currently being drawn.
class TRACK;
extern DLIST<TRACK> g_CurrentTrackList;
#define g_CurrentTrackSegment g_CurrentTrackList.GetLast()    ///< most recently created segment
#define g_FirstTrackSegment   g_CurrentTrackList.GetFirst()   ///< first segment created


enum MagneticPadOptionValues {
    no_effect,
    capture_cursor_in_track_tool,
    capture_always
};


#endif // PCBNEW_H
