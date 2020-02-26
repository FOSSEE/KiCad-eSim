/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013-2016 CERN
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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

#ifndef __COMMON_ACTIONS_H
#define __COMMON_ACTIONS_H

#include <tool/tool_action.h>
#include <boost/optional.hpp>

class TOOL_EVENT;
class TOOL_MANAGER;

/**
 * Class COMMON_ACTIONS
 *
 * Gathers all the actions that are shared by tools. The instance of COMMON_ACTION is created
 * inside of ACTION_MANAGER object that registers the actions.
 */
class COMMON_ACTIONS
{
public:
    // Selection Tool
    /// Activation of the selection tool
    static TOOL_ACTION selectionActivate;

    /// Select a single item under the cursor position
    static TOOL_ACTION selectionCursor;

    /// Clears the current selection
    static TOOL_ACTION selectionClear;

    /// Selects an item (specified as the event parameter).
    static TOOL_ACTION selectItem;

    /// Unselects an item (specified as the event parameter).
    static TOOL_ACTION unselectItem;

    /// Selects a connection between junctions.
    static TOOL_ACTION selectConnection;

    /// Selects whole copper connection.
    static TOOL_ACTION selectCopper;

    /// Selects all connections belonging to a single net.
    static TOOL_ACTION selectNet;

    // Edit Tool
    /// Activation of the edit tool
    static TOOL_ACTION editActivate;

    /// Rotation of selected objects
    static TOOL_ACTION rotate;

    /// Flipping of selected objects
    static TOOL_ACTION flip;

    /// Activation of the edit tool
    static TOOL_ACTION properties;

    /// Activation of the exact move tool
    static TOOL_ACTION moveExact;

    /// Activation of the duplication tool
    static TOOL_ACTION duplicate;

    /// Activation of the duplication tool with incrementing (e.g. pad number)
    static TOOL_ACTION duplicateIncrement;

    /// Deleting a BOARD_ITEM
    static TOOL_ACTION remove;

    // Drawing Tool
    /// Activation of the drawing tool (line)
    static TOOL_ACTION drawLine;

    /// Activation of the drawing tool (circle)
    static TOOL_ACTION drawCircle;

    /// Activation of the drawing tool (arc)
    static TOOL_ACTION drawArc;

    /// Activation of the drawing tool (text)
    static TOOL_ACTION placeText;

    /// Activation of the drawing tool (dimension)
    static TOOL_ACTION drawDimension;

    /// Activation of the drawing tool (drawing a ZONE)
    static TOOL_ACTION drawZone;

    /// Activation of the drawing tool (drawing a keepout area)
    static TOOL_ACTION drawKeepout;

    /// Activation of the drawing tool (placing a TARGET)
    static TOOL_ACTION placeTarget;

    /// Activation of the drawing tool (placing a MODULE)
    static TOOL_ACTION placeModule;

    /// Activation of the drawing tool (placing a drawing from DXF file)
    static TOOL_ACTION placeDXF;

    /// Activation of the drawing tool (placing the footprint anchor)
    static TOOL_ACTION setAnchor;

    /// Increase width of currently drawn line
    static TOOL_ACTION incWidth;

    /// Decrease width of currently drawn line
    static TOOL_ACTION decWidth;

    /// Switch posture when drawing arc
    static TOOL_ACTION arcPosture;

    // Push and Shove Router Tool

    /// Activation of the Push and Shove router
    static TOOL_ACTION routerActivateSingle;

    /// Activation of the Push and Shove router (differential pair mode)
    static TOOL_ACTION routerActivateDiffPair;

    /// Activation of the Push and Shove router (tune single line mode)
    static TOOL_ACTION routerActivateTuneSingleTrace;

    /// Activation of the Push and Shove router (diff pair tuning mode)
    static TOOL_ACTION routerActivateTuneDiffPair;

    /// Activation of the Push and Shove router (skew tuning mode)
    static TOOL_ACTION routerActivateTuneDiffPairSkew;

    /// Activation of the Push and Shove settings dialogs
    static TOOL_ACTION routerActivateSettingsDialog;
    static TOOL_ACTION routerActivateDpDimensionsDialog;


    /// Activation of the Push and Shove router (inline dragging mode)
    static TOOL_ACTION routerInlineDrag;

    // Point Editor
    /// Update edit points
    static TOOL_ACTION pointEditorUpdate;

    /// Break outline (insert additional points to an edge)
    static TOOL_ACTION pointEditorAddCorner;

    /// Removes a corner
    static TOOL_ACTION pointEditorRemoveCorner;

    // Placement tool
    /// Align items to the top edge of selection bounding box
    static TOOL_ACTION alignTop;

    /// Align items to the bottom edge of selection bounding box
    static TOOL_ACTION alignBottom;

    /// Align items to the left edge of selection bounding box
    static TOOL_ACTION alignLeft;

    /// Align items to the right edge of selection bounding box
    static TOOL_ACTION alignRight;

    /// Distributes items evenly along the horizontal axis
    static TOOL_ACTION distributeHorizontally;

    /// Distributes items evenly along the vertical axis
    static TOOL_ACTION distributeVertically;

    // View controls
    static TOOL_ACTION zoomIn;
    static TOOL_ACTION zoomOut;
    static TOOL_ACTION zoomInCenter;
    static TOOL_ACTION zoomOutCenter;
    static TOOL_ACTION zoomCenter;
    static TOOL_ACTION zoomFitScreen;
    static TOOL_ACTION zoomPreset;

    // Display modes
    static TOOL_ACTION trackDisplayMode;
    static TOOL_ACTION padDisplayMode;
    static TOOL_ACTION viaDisplayMode;
    static TOOL_ACTION zoneDisplayEnable;
    static TOOL_ACTION zoneDisplayDisable;
    static TOOL_ACTION zoneDisplayOutlines;
    static TOOL_ACTION highContrastMode;
    static TOOL_ACTION highContrastInc;
    static TOOL_ACTION highContrastDec;

    // Layer control
    static TOOL_ACTION layerTop;
    static TOOL_ACTION layerInner1;
    static TOOL_ACTION layerInner2;
    static TOOL_ACTION layerInner3;
    static TOOL_ACTION layerInner4;
    static TOOL_ACTION layerInner5;
    static TOOL_ACTION layerInner6;
    static TOOL_ACTION layerBottom;
    static TOOL_ACTION layerNext;
    static TOOL_ACTION layerPrev;
    static TOOL_ACTION layerAlphaInc;
    static TOOL_ACTION layerAlphaDec;
    static TOOL_ACTION layerToggle;

    static TOOL_ACTION layerChanged;        // notification

    // Grid control
    static TOOL_ACTION gridFast1;
    static TOOL_ACTION gridFast2;
    static TOOL_ACTION gridNext;
    static TOOL_ACTION gridPrev;
    static TOOL_ACTION gridSetOrigin;
    static TOOL_ACTION gridResetOrigin;
    static TOOL_ACTION gridPreset;

    // Track & via size control
    static TOOL_ACTION trackWidthInc;
    static TOOL_ACTION trackWidthDec;
    static TOOL_ACTION viaSizeInc;
    static TOOL_ACTION viaSizeDec;

    static TOOL_ACTION trackViaSizeChanged;   // notification

    // Zone actions
    static TOOL_ACTION zoneFill;
    static TOOL_ACTION zoneFillAll;
    static TOOL_ACTION zoneUnfill;
    static TOOL_ACTION zoneUnfillAll;
    static TOOL_ACTION zoneMerge;

    // Module editor tools
    /// Activation of the drawing tool (placing a PAD)
    static TOOL_ACTION placePad;

    /// Tool for quick pad enumeration
    static TOOL_ACTION enumeratePads;

    /// Tool for creating an array of objects
    static TOOL_ACTION createArray;

    /// Copying module items to clipboard
    static TOOL_ACTION copyItems;

    /// Pasting module items from clipboard
    static TOOL_ACTION pasteItems;

    /// Display module edges as outlines
    static TOOL_ACTION moduleEdgeOutlines;

    /// Display module texts as outlines
    static TOOL_ACTION moduleTextOutlines;

    /// Cursor control with keyboard
    static TOOL_ACTION cursorUp;
    static TOOL_ACTION cursorDown;
    static TOOL_ACTION cursorLeft;
    static TOOL_ACTION cursorRight;

    static TOOL_ACTION cursorUpFast;
    static TOOL_ACTION cursorDownFast;
    static TOOL_ACTION cursorLeftFast;
    static TOOL_ACTION cursorRightFast;

    static TOOL_ACTION cursorClick;
    static TOOL_ACTION cursorDblClick;

    // Panning with keyboard
    static TOOL_ACTION panUp;
    static TOOL_ACTION panDown;
    static TOOL_ACTION panLeft;
    static TOOL_ACTION panRight;

    // Miscellaneous
    static TOOL_ACTION selectionTool;
    static TOOL_ACTION pickerTool;
    static TOOL_ACTION resetCoords;
    static TOOL_ACTION switchCursor;
    static TOOL_ACTION switchUnits;
    static TOOL_ACTION deleteItemCursor;
    static TOOL_ACTION highlightNet;
    static TOOL_ACTION highlightNetCursor;
    static TOOL_ACTION drillOrigin;
    static TOOL_ACTION crossProbeSchToPcb;
    static TOOL_ACTION toggleLockModule;
    static TOOL_ACTION appendBoard;
    static TOOL_ACTION showHelp;
    static TOOL_ACTION toBeDone;

    /// Find an item
    static TOOL_ACTION find;

    /// Find an item and start moving
    static TOOL_ACTION findMove;

    static TOOL_ACTION editFootprintInFpEditor;
    static TOOL_ACTION copyPadToSettings;
    static TOOL_ACTION copySettingsToPads;
    static TOOL_ACTION globalEditPads;


    /**
     * Function TranslateLegacyId()
     * Translates legacy tool ids to the corresponding TOOL_ACTION name.
     * @param aId is legacy tool id to be translated.
     * @return std::string is name of the corresponding TOOL_ACTION. It may be empty, if there is
     * no corresponding TOOL_ACTION.
     */
    static boost::optional<TOOL_EVENT> TranslateLegacyId( int aId );

    ///> Cursor control event types
    enum CURSOR_EVENT_TYPE { CURSOR_UP, CURSOR_DOWN, CURSOR_LEFT, CURSOR_RIGHT,
                             CURSOR_CLICK, CURSOR_DBL_CLICK, CURSOR_FAST_MOVE = 0x8000 };
};

void registerAllTools( TOOL_MANAGER* aToolManager );

#endif
