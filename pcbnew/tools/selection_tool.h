/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013-2016 CERN
 * @author Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
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

#ifndef __SELECTION_TOOL_H
#define __SELECTION_TOOL_H

#include <math/vector2d.h>
#include <tool/tool_interactive.h>
#include <tool/context_menu.h>
#include <class_undoredo_container.h>

#include "selection_conditions.h"
#include "conditional_menu.h"

class PCB_BASE_FRAME;
class SELECTION_AREA;
class BOARD_ITEM;
class GENERAL_COLLECTOR;
class SELECT_MENU;
class ZOOM_MENU;
class GRID_MENU;

namespace KIGFX
{
class VIEW_GROUP;
}

struct SELECTION
{
    /// Set of selected items
    PICKED_ITEMS_LIST items;

    /// VIEW_GROUP that holds currently selected items
    KIGFX::VIEW_GROUP* group;

    /// Checks if there is anything selected
    bool Empty() const
    {
        return ( items.GetCount() == 0 );
    }

    /// Returns the number of selected parts
    int Size() const
    {
        return items.GetCount();
    }

    /// Alias to make code shorter and clearer
    template <typename T>
    T* Item( unsigned int aIndex ) const
    {
        return static_cast<T*>( items.GetPickedItem( aIndex ) );
    }

    /// Returns the center point of the selection area bounding box.
    VECTOR2I GetCenter() const;

    /// Runs a function on all selected items.
    template <typename T>
    void ForAll( boost::function<void (T*)> aFunction ) const
    {
        for( unsigned int i = 0; i < items.GetCount(); ++i )
            aFunction( Item<T>( i ) );
    }

private:
    /// Clears both the VIEW_GROUP and set of selected items. Please note that it does not
    /// change properties of selected items (e.g. selection flag).
    void clear();

    friend class SELECTION_TOOL;
};

enum SELECTION_LOCK_FLAGS
{
    SELECTION_UNLOCKED = 0,
    SELECTION_LOCK_OVERRIDE = 1,
    SELECTION_LOCKED = 2
};

/**
 * Class SELECTION_TOOL
 *
 * Our sample selection tool: currently supports:
 * - pick single objects (click LMB)
 * - add objects to existing selection (Shift+LMB)
 * - draw selection box (drag LMB)
 * - handles MODULEs properly (i.e. selects either MODULE or its PADs, TEXTs, etc.)
 * - takes into account high-contrast & layer visibility settings
 * - invokes InteractiveEdit tool when user starts to drag selected items
 */
class SELECTION_TOOL : public TOOL_INTERACTIVE
{
public:
    SELECTION_TOOL();
    ~SELECTION_TOOL();

    /// @copydoc TOOL_BASE::Init()
    bool Init();

    /// @copydoc TOOL_BASE::Reset()
    void Reset( RESET_REASON aReason );

    /**
     * Function Main()
     *
     * The main loop.
     */
    int Main( const TOOL_EVENT& aEvent );

    /**
     * Function GetSelection()
     *
     * Returns the set of currently selected items.
     */
    const SELECTION& GetSelection();

    /**
     * Function EditModules()
     *
     * Toggles edit module mode. When enabled, one may select parts of modules individually
     * (graphics, pads, etc.), so they can be modified.
     * @param aEnabled decides if the mode should be enabled.
     */
    inline void EditModules( bool aEnabled )
    {
        m_editModules = aEnabled;
    }

    inline CONDITIONAL_MENU& GetMenu()
    {
        return m_menu;
    }

    ///> Checks if the user has agreed to modify locked items for the given selection.
    SELECTION_LOCK_FLAGS CheckLock();

    ///> Select a single item under cursor event handler.
    int CursorSelection( const TOOL_EVENT& aEvent );

    ///> Clear current selection event handler.
    int ClearSelection( const TOOL_EVENT& aEvent );

    ///> Makes sure a group selection does not contain items that would cause
    ///> conflicts when moving/rotating together (e.g. a footprint and one of the same footprint's pads)
    bool SanitizeSelection();

    ///> Item selection event handler.
    int SelectItem( const TOOL_EVENT& aEvent );

    ///> Item unselection event handler.
    int UnselectItem( const TOOL_EVENT& aEvent );

    ///> Event sent after an item is selected.
    static const TOOL_EVENT SelectedEvent;

    ///> Event sent after an item is unselected.
    static const TOOL_EVENT UnselectedEvent;

    ///> Event sent after selection is cleared.
    static const TOOL_EVENT ClearedEvent;

    ///> Sets up handlers for various events.
    void SetTransitions();

private:
    /**
     * Function selectPoint()
     * Selects an item pointed by the parameter aWhere. If there is more than one item at that
     * place, there is a menu displayed that allows to choose the item.
     *
     * @param aWhere is the place where the item should be selected.
     * @param aAllowDisambiguation decides what to do in case of disambiguation. If true, then
     * a menu is shown, otherise function finishes without selecting anything.
     * @return True if an item was selected, false otherwise.
     */
    bool selectPoint( const VECTOR2I& aWhere, bool aOnDrag = false );

    /**
     * Function selectCursor()
     * Selects an item under the cursor unless there is something already selected or aSelectAlways
     * is true.
     * @param aSelectAlways forces to select an item even if there is an item already selected.
     * @return true if eventually there is an item selected, false otherwise.
     */
    bool selectCursor( bool aSelectAlways = false );

    /**
     * Function selectMultiple()
     * Handles drawing a selection box that allows to select many items at the same time.
     *
     * @return true if the function was cancelled (i.e. CancelEvent was received).
     */
    bool selectMultiple();

    ///> Selects a trivial connection (between two junctions).
    int selectConnection( const TOOL_EVENT& aEvent );

    ///> Selects a continuous copper connection.
    int selectCopper( const TOOL_EVENT& aEvent );

    ///> Selects all copper connections belonging to a single net.
    int selectNet( const TOOL_EVENT& aEvent );

    ///> Find dialog callback.
    void findCallback( BOARD_ITEM* aItem );

    ///> Find an item.
    int find( const TOOL_EVENT& aEvent );

    ///> Find an item and start moving.
    int findMove( const TOOL_EVENT& aEvent );

    /**
     * Function clearSelection()
     * Clears the current selection.
     */
    void clearSelection();

    /**
     * Function disambiguationMenu()
     * Handles the menu that allows to select one of many items in case there is more than one
     * item at the selected point (@see selectCursor()).
     *
     * @param aItems contains list of items that are displayed to the user.
     */
    BOARD_ITEM* disambiguationMenu( GENERAL_COLLECTOR* aItems );

    /**
     * Function pickSmallestComponent()
     * Allows to find the smallest (in terms of bounding box area) item from the list.
     *
     * @param aCollector containes the list of items.
     */
    BOARD_ITEM* pickSmallestComponent( GENERAL_COLLECTOR* aCollector );

    /**
     * Function toggleSelection()
     * Changes selection status of a given item.
     *
     * @param aItem is the item to have selection status changed.
     */
    void toggleSelection( BOARD_ITEM* aItem );

    /**
     * Function selectable()
     * Checks conditions for an item to be selected.
     *
     * @return True if the item fulfills conditions to be selected.
     */
    bool selectable( const BOARD_ITEM* aItem ) const;

    /**
     * Function modifiable()
     * Checks if an item might be modified. This function is used to filter out items
     * from the selection when it is passed to other tools.
     *
     * @return True if the item fulfills conditions to be modified.
     */
    bool modifiable( const BOARD_ITEM* aItem ) const;

    /**
     * Function select()
     * Takes necessary action mark an item as selected.
     *
     * @param aItem is an item to be selected.
     */
    void select( BOARD_ITEM* aItem );

    /**
     * Function unselect()
     * Takes necessary action mark an item as unselected.
     *
     * @param aItem is an item to be unselected.
     */
    void unselect( BOARD_ITEM* aItem );

    /**
     * Function selectVisually()
     * Marks item as selected, but does not add it to the ITEMS_PICKED_LIST.
     * @param aItem is an item to be be marked.
     */
    void selectVisually( BOARD_ITEM* aItem ) const;

    /**
     * Function unselectVisually()
     * Marks item as selected, but does not add it to the ITEMS_PICKED_LIST.
     * @param aItem is an item to be be marked.
     */
    void unselectVisually( BOARD_ITEM* aItem ) const;

    /**
     * Function selectionContains()
     * Checks if the given point is placed within any of selected items' bounding box.
     *
     * @return True if the given point is contained in any of selected items' bouding box.
     */
    bool selectionContains( const VECTOR2I& aPoint ) const;

    /**
     * Function guessSelectionCandidates()
     * Tries to guess best selection candidates in case multiple items are clicked, by
     * doing some braindead heuristics.
     * @param aCollector is the collector that has a list of items to be queried.
     */
    void guessSelectionCandidates( GENERAL_COLLECTOR& aCollector ) const;

    /// Pointer to the parent frame.
    PCB_BASE_FRAME* m_frame;

    /// Current state of selection.
    SELECTION m_selection;

    /// Flag saying if items should be added to the current selection or rather replace it.
    bool m_additive;

    /// Flag saying if multiple selection mode is active.
    bool m_multiple;

    /// Edit module mode flag.
    bool m_editModules;

    /// Can other tools modify locked items.
    bool m_locked;

    /// Determines if the selection is preliminary or final.
    bool m_preliminary;

    /// Menu displayed by the tool.
    CONDITIONAL_MENU m_menu;

    /// Pointers to context menus
    CONTEXT_MENU* m_contextMenu;
    SELECT_MENU* m_selectMenu;
    ZOOM_MENU* m_zoomMenu;
    GRID_MENU* m_gridMenu;
};

#endif
