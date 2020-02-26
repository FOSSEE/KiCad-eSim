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
#include <limits>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <class_board.h>
#include <class_board_item.h>
#include <class_track.h>
#include <class_module.h>
#include <class_pcb_text.h>
#include <class_drawsegment.h>

#include <wxPcbStruct.h>
#include <collectors.h>
#include <confirm.h>
#include <dialog_find.h>

#include <class_draw_panel_gal.h>
#include <view/view_controls.h>
#include <view/view_group.h>
#include <painter.h>

#include <tool/tool_event.h>
#include <tool/tool_manager.h>
#include <ratsnest_data.h>

#include "selection_tool.h"
#include "selection_area.h"
#include "zoom_menu.h"
#include "grid_menu.h"
#include "bright_box.h"
#include "common_actions.h"

class SELECT_MENU: public CONTEXT_MENU
{
public:
    SELECT_MENU()
    {
        Add( COMMON_ACTIONS::selectConnection );
        Add( COMMON_ACTIONS::selectCopper );
        Add( COMMON_ACTIONS::selectNet );
    }
};


SELECTION_TOOL::SELECTION_TOOL() :
        TOOL_INTERACTIVE( "pcbnew.InteractiveSelection" ),
        m_frame( NULL ), m_additive( false ), m_multiple( false ), m_editModules( false ),
        m_locked( true ), m_menu( this ), m_contextMenu( NULL ), m_selectMenu( NULL ),
        m_zoomMenu( NULL ), m_gridMenu( NULL )
{
    // Do not leave uninitialized members:
    m_preliminary = false;
}


SELECTION_TOOL::~SELECTION_TOOL()
{
    delete m_selection.group;
    delete m_contextMenu;
    delete m_selectMenu;
    delete m_zoomMenu;
    delete m_gridMenu;
}


bool SELECTION_TOOL::Init()
{
    m_selection.group = new KIGFX::VIEW_GROUP;

    m_selectMenu = new SELECT_MENU;
    m_selectMenu->SetTool( this );

    m_menu.AddMenu( m_selectMenu, _( "Select..." ), false,
            ( SELECTION_CONDITIONS::OnlyType( PCB_VIA_T ) || SELECTION_CONDITIONS::OnlyType( PCB_TRACE_T ) ) &&
            SELECTION_CONDITIONS::Count( 1 ) );

    m_menu.AddSeparator( SELECTION_CONDITIONS::ShowAlways, 1000 );

    m_menu.AddItem( COMMON_ACTIONS::zoomCenter, SELECTION_CONDITIONS::ShowAlways, 1000 );
    m_menu.AddItem( COMMON_ACTIONS::zoomIn, SELECTION_CONDITIONS::ShowAlways, 1000  );
    m_menu.AddItem( COMMON_ACTIONS::zoomOut , SELECTION_CONDITIONS::ShowAlways, 1000 );
    m_menu.AddItem( COMMON_ACTIONS::zoomFitScreen , SELECTION_CONDITIONS::ShowAlways, 1000 );

    PCB_BASE_FRAME* frame = getEditFrame<PCB_BASE_FRAME>();

    m_zoomMenu = new ZOOM_MENU( frame );
    m_menu.AddMenu( m_zoomMenu, _( "Zoom" ), false, SELECTION_CONDITIONS::ShowAlways, 1000 );

    m_gridMenu = new GRID_MENU( frame );
    m_menu.AddMenu( m_gridMenu, _( "Grid" ), false, SELECTION_CONDITIONS::ShowAlways, 1000 );

    return true;
}


void SELECTION_TOOL::Reset( RESET_REASON aReason )
{
    m_frame = getEditFrame<PCB_BASE_FRAME>();
    m_locked = true;
    m_preliminary = true;

    if( aReason == TOOL_BASE::MODEL_RELOAD )
    {
        // Remove pointers to the selected items from containers
        // without changing their properties (as they are already deleted
        // while a new board is loaded)
        m_selection.clear();
        getView()->GetPainter()->GetSettings()->SetHighlight( false );
    }
    else
        // Restore previous properties of selected items and remove them from containers
        clearSelection();

    // Reinsert the VIEW_GROUP, in case it was removed from the VIEW
    getView()->Remove( m_selection.group );
    getView()->Add( m_selection.group );
}


int SELECTION_TOOL::Main( const TOOL_EVENT& aEvent )
{
    // Main loop: keep receiving events
    while( OPT_TOOL_EVENT evt = Wait() )
    {
        // Should selected items be added to the current selection or
        // become the new selection (discarding previously selected items)
        m_additive = evt->Modifier( MD_SHIFT );

        // single click? Select single object
        if( evt->IsClick( BUT_LEFT ) )
        {
            if( evt->Modifier( MD_CTRL ) && !m_editModules )
            {
                m_toolMgr->RunAction( COMMON_ACTIONS::highlightNet, true );
            }
            else
            {
                if( !m_additive )
                    clearSelection();

                selectPoint( evt->Position() );
            }
        }

        // right click? if there is any object - show the context menu
        else if( evt->IsClick( BUT_RIGHT ) )
        {
            bool emptySelection = m_selection.Empty();

            if( emptySelection )
                selectPoint( evt->Position() );

            delete m_contextMenu;
            m_contextMenu = m_menu.Generate( m_selection );

            if( m_contextMenu->GetMenuItemCount() > 0 )
                SetContextMenu( m_contextMenu, CMENU_NOW );

            m_preliminary = emptySelection;
        }

        // double click? Display the properties window
        else if( evt->IsDblClick( BUT_LEFT ) )
        {
            if( m_selection.Empty() )
                selectPoint( evt->Position() );

            m_toolMgr->RunAction( COMMON_ACTIONS::properties );
        }

        // drag with LMB? Select multiple objects (or at least draw a selection box) or drag them
        else if( evt->IsDrag( BUT_LEFT ) )
        {
            if( m_additive )
            {
                m_preliminary = false;

                selectMultiple();
            }
            else if( m_selection.Empty() )
            {
                m_preliminary = false;

                // There is nothing selected, so try to select something
                if( !selectCursor() )
                {
                    // If nothings has been selected or user wants to select more
                    // draw the selection box
                    selectMultiple();
                }
                else
                {
                    m_toolMgr->InvokeTool( "pcbnew.InteractiveEdit" );
                }
            }

            else
            {
                // Check if dragging has started within any of selected items bounding box
                if( selectionContains( evt->Position() ) )
                {
                    // Yes -> run the move tool and wait till it finishes
                    m_toolMgr->InvokeTool( "pcbnew.InteractiveEdit" );
                }
                else
                {
                    // No -> clear the selection list
                    clearSelection();
                }
            }
        }

        else if( evt->IsAction( &COMMON_ACTIONS::selectionCursor ) )
        {
            selectCursor( true );
        }

        else if( evt->IsAction( &COMMON_ACTIONS::find ) )
        {
            find( *evt );
        }

        else if( evt->IsAction( &COMMON_ACTIONS::findMove ) )
        {
            findMove( *evt );
        }

        else if( evt->IsAction( &COMMON_ACTIONS::selectItem ) )
        {
            SelectItem( *evt );
        }

        else if( evt->IsAction( &COMMON_ACTIONS::unselectItem ) )
        {
            UnselectItem( *evt );
        }

        else if( evt->IsCancel() || evt->Action() == TA_UNDO_REDO ||
                 evt->IsAction( &COMMON_ACTIONS::selectionClear ) )
        {
            clearSelection();
        }

        else if( evt->IsAction( &COMMON_ACTIONS::selectConnection ) )
        {
            selectConnection( *evt );
        }

        else if( evt->IsAction( &COMMON_ACTIONS::selectCopper ) )
        {
            selectCopper( *evt );
        }

        else if( evt->IsAction( &COMMON_ACTIONS::selectNet ) )
        {
            selectNet( *evt );
        }

        else if( evt->Action() == TA_CONTEXT_MENU_CLOSED )
        {
            if( m_preliminary )
                clearSelection();

            if( evt->Parameter<CONTEXT_MENU*>() == m_contextMenu )
            {
                delete m_contextMenu;
                m_contextMenu = NULL;
            }
        }
    }

    // This tool is supposed to be active forever
    assert( false );

    return 0;
}


const SELECTION& SELECTION_TOOL::GetSelection()
{
    // The selected items list has been requested, so it is no longer preliminary
    m_preliminary = false;

    // Filter out not modifiable items
    for( int i = 0; i < m_selection.Size(); )
    {
        BOARD_ITEM* item = m_selection.Item<BOARD_ITEM>( i );

        if( !modifiable( item ) )
        {
            m_selection.items.RemovePicker( i );
            m_selection.group->Remove( item );
        }
        else
        {
            ++i;
        }
    }

    return m_selection;
}


void SELECTION_TOOL::toggleSelection( BOARD_ITEM* aItem )
{
    if( aItem->IsSelected() )
    {
        unselect( aItem );

        // Inform other potentially interested tools
        m_toolMgr->ProcessEvent( UnselectedEvent );
    }
    else
    {
        if( !m_additive )
            clearSelection();

        // Prevent selection of invisible or inactive items
        if( selectable( aItem ) )
        {
            select( aItem );

            // Inform other potentially interested tools
            m_toolMgr->ProcessEvent( SelectedEvent );
        }
    }
}


bool SELECTION_TOOL::selectPoint( const VECTOR2I& aWhere, bool aOnDrag )
{
    BOARD_ITEM* item;
    GENERAL_COLLECTORS_GUIDE guide = m_frame->GetCollectorsGuide();
    GENERAL_COLLECTOR collector;

    if( m_editModules )
        collector.Collect( getModel<BOARD>(), GENERAL_COLLECTOR::ModuleItems,
                           wxPoint( aWhere.x, aWhere.y ), guide );
    else
        collector.Collect( getModel<BOARD>(), GENERAL_COLLECTOR::AllBoardItems,
                           wxPoint( aWhere.x, aWhere.y ), guide );

    bool anyCollected = collector.GetCount() != 0;

    // Remove unselectable items
    for( int i = collector.GetCount() - 1; i >= 0; --i )
    {
        if( !selectable( collector[i] ) || ( aOnDrag && collector[i]->IsLocked() ) )
            collector.Remove( i );
    }

    switch( collector.GetCount() )
    {
    case 0:
        if( !m_additive && anyCollected )
            clearSelection();

        return false;

    case 1:
        toggleSelection( collector[0] );

        return true;

    default:
        // Apply some ugly heuristics to avoid disambiguation menus whenever possible
        guessSelectionCandidates( collector );

        // Let's see if there is still disambiguation in selection..
        if( collector.GetCount() == 1 )
        {
            toggleSelection( collector[0] );

            return true;
        }
        else if( collector.GetCount() > 1 )
        {
            if( aOnDrag )
                Wait( TOOL_EVENT( TC_ANY, TA_MOUSE_UP, BUT_LEFT ) );

            item = disambiguationMenu( &collector );

            if( item )
            {
                toggleSelection( item );

                return true;
            }
        }
        break;
    }

    return false;
}


bool SELECTION_TOOL::selectCursor( bool aSelectAlways )
{
    if( aSelectAlways || m_selection.Empty() )
    {
        clearSelection();
        selectPoint( getViewControls()->GetCursorPosition() );
    }

    return !m_selection.Empty();
}


bool SELECTION_TOOL::selectMultiple()
{
    bool cancelled = false;     // Was the tool cancelled while it was running?
    m_multiple = true;          // Multiple selection mode is active
    KIGFX::VIEW* view = getView();
    getViewControls()->SetAutoPan( true );

    SELECTION_AREA area;
    view->Add( &area );

    while( OPT_TOOL_EVENT evt = Wait() )
    {
        if( evt->IsCancel() )
        {
            cancelled = true;
            break;
        }

        if( evt->IsDrag( BUT_LEFT ) )
        {
            if( !m_additive )
                clearSelection();

            // Start drawing a selection box
            area.SetOrigin( evt->DragOrigin() );
            area.SetEnd( evt->Position() );
            area.ViewSetVisible( true );
            area.ViewUpdate( KIGFX::VIEW_ITEM::GEOMETRY );
        }

        if( evt->IsMouseUp( BUT_LEFT ) )
        {
            // End drawing the selection box
            area.ViewSetVisible( false );

            // Mark items within the selection box as selected
            std::vector<KIGFX::VIEW::LAYER_ITEM_PAIR> selectedItems;
            BOX2I selectionBox = area.ViewBBox();
            view->Query( selectionBox, selectedItems );         // Get the list of selected items

            std::vector<KIGFX::VIEW::LAYER_ITEM_PAIR>::iterator it, it_end;

            for( it = selectedItems.begin(), it_end = selectedItems.end(); it != it_end; ++it )
            {
                BOARD_ITEM* item = static_cast<BOARD_ITEM*>( it->first );

                // Add only those items that are visible and fully within the selection box
                if( !item->IsSelected() && selectable( item ) &&
                        selectionBox.Contains( item->ViewBBox() ) )
                {
                    select( item );
                }
            }

            if( m_selection.Size() == 1 )
                m_frame->SetCurItem( m_selection.Item<BOARD_ITEM>( 0 ) );
            else
                m_frame->SetCurItem( NULL );

            // Inform other potentially interested tools
            if( !m_selection.Empty() )
                m_toolMgr->ProcessEvent( SelectedEvent );

            break;  // Stop waiting for events
        }
    }

    // Stop drawing the selection box
    area.ViewSetVisible( false );
    view->Remove( &area );
    m_multiple = false;         // Multiple selection mode is inactive
    getViewControls()->SetAutoPan( false );

    return cancelled;
}


void SELECTION_TOOL::SetTransitions()
{
    Go( &SELECTION_TOOL::Main, COMMON_ACTIONS::selectionActivate.MakeEvent() );
    Go( &SELECTION_TOOL::CursorSelection, COMMON_ACTIONS::selectionCursor.MakeEvent() );
    Go( &SELECTION_TOOL::ClearSelection, COMMON_ACTIONS::selectionClear.MakeEvent() );
    Go( &SELECTION_TOOL::SelectItem, COMMON_ACTIONS::selectItem.MakeEvent() );
    Go( &SELECTION_TOOL::UnselectItem, COMMON_ACTIONS::unselectItem.MakeEvent() );
    Go( &SELECTION_TOOL::find, COMMON_ACTIONS::find.MakeEvent() );
    Go( &SELECTION_TOOL::findMove, COMMON_ACTIONS::findMove.MakeEvent() );
    Go( &SELECTION_TOOL::selectConnection, COMMON_ACTIONS::selectConnection.MakeEvent() );
    Go( &SELECTION_TOOL::selectCopper, COMMON_ACTIONS::selectCopper.MakeEvent() );
    Go( &SELECTION_TOOL::selectNet, COMMON_ACTIONS::selectNet.MakeEvent() );
}


SELECTION_LOCK_FLAGS SELECTION_TOOL::CheckLock()
{
    if( !m_locked || m_editModules )
        return SELECTION_UNLOCKED;

    bool containsLocked = false;

    // Check if the selection contains locked items
    for( int i = 0; i < m_selection.Size(); ++i )
    {
        BOARD_ITEM* item = m_selection.Item<BOARD_ITEM>( i );

        switch( item->Type() )
        {
        case PCB_MODULE_T:
            if( static_cast<MODULE*>( item )->IsLocked() )
                containsLocked = true;
            break;

        case PCB_MODULE_EDGE_T:
        case PCB_MODULE_TEXT_T:
            if( static_cast<MODULE*>( item->GetParent() )->IsLocked() )
                containsLocked = true;
            break;

        default:    // suppress warnings
            break;
        }
    }

    if( containsLocked )
    {
        if( IsOK( m_frame, _( "Selection contains locked items. Do you want to continue?" ) ) )
        {
            m_locked = false;
            return SELECTION_LOCK_OVERRIDE;
        }
        else
            return SELECTION_LOCKED;
    }

    m_locked = false;

    return SELECTION_UNLOCKED;
}


int SELECTION_TOOL::CursorSelection( const TOOL_EVENT& aEvent )
{
    selectCursor( true );

    return 0;
}


int SELECTION_TOOL::ClearSelection( const TOOL_EVENT& aEvent )
{
    clearSelection();

    return 0;
}


int SELECTION_TOOL::SelectItem( const TOOL_EVENT& aEvent )
{
    // Check if there is an item to be selected
    BOARD_ITEM* item = aEvent.Parameter<BOARD_ITEM*>();

    if( item )
    {
        select( item );

        // Inform other potentially interested tools
        m_toolMgr->ProcessEvent( SelectedEvent );
    }

    return 0;
}


int SELECTION_TOOL::UnselectItem( const TOOL_EVENT& aEvent )
{
    // Check if there is an item to be selected
    BOARD_ITEM* item = aEvent.Parameter<BOARD_ITEM*>();

    if( item )
    {
        unselect( item );

        // Inform other potentially interested tools
        m_toolMgr->ProcessEvent( UnselectedEvent );
    }

    return 0;
}


int SELECTION_TOOL::selectConnection( const TOOL_EVENT& aEvent )
{
    if( !selectCursor( true ) )
        return 0;

    BOARD_CONNECTED_ITEM* item = m_selection.Item<BOARD_CONNECTED_ITEM>( 0 );
    clearSelection();

    if( item->Type() != PCB_TRACE_T && item->Type() != PCB_VIA_T )
        return 0;

    int segmentCount;
    TRACK* trackList = getModel<BOARD>()->MarkTrace( static_cast<TRACK*>( item ), &segmentCount,
                                                     NULL, NULL, true );

    if( segmentCount == 0 )
        return 0;

    for( int i = 0; i < segmentCount; ++i )
    {
        select( trackList );
        trackList = trackList->Next();
    }

    // Inform other potentially interested tools
    m_toolMgr->ProcessEvent( SelectedEvent );

    return 0;
}


int SELECTION_TOOL::selectCopper( const TOOL_EVENT& aEvent )
{
    if( !selectCursor( true ) )
        return 0;

    BOARD_CONNECTED_ITEM* item = m_selection.Item<BOARD_CONNECTED_ITEM>( 0 );
    clearSelection();

    if( item->Type() != PCB_TRACE_T && item->Type() != PCB_VIA_T )
        return 0;

    std::list<BOARD_CONNECTED_ITEM*> itemsList;
    RN_DATA* ratsnest = getModel<BOARD>()->GetRatsnest();

    ratsnest->GetConnectedItems( item, itemsList, (RN_ITEM_TYPE)( RN_TRACKS | RN_VIAS ) );

    BOOST_FOREACH( BOARD_CONNECTED_ITEM* i, itemsList )
        select( i );

    // Inform other potentially interested tools
    if( itemsList.size() > 0 )
        m_toolMgr->ProcessEvent( SelectedEvent );

    return 0;
}


int SELECTION_TOOL::selectNet( const TOOL_EVENT& aEvent )
{
    if( !selectCursor( true ) )
        return 0;

    BOARD_CONNECTED_ITEM* item = m_selection.Item<BOARD_CONNECTED_ITEM>( 0 );

    std::list<BOARD_CONNECTED_ITEM*> itemsList;
    RN_DATA* ratsnest = getModel<BOARD>()->GetRatsnest();
    int netCode = item->GetNetCode();

    clearSelection();
    ratsnest->GetNetItems( netCode, itemsList, (RN_ITEM_TYPE)( RN_TRACKS | RN_VIAS ) );

    BOOST_FOREACH( BOARD_CONNECTED_ITEM* i, itemsList )
        select( i );

    // Inform other potentially interested tools
    if( itemsList.size() > 0 )
        m_toolMgr->ProcessEvent( SelectedEvent );

    return 0;
}


void SELECTION_TOOL::findCallback( BOARD_ITEM* aItem )
{
    clearSelection();

    if( aItem )
    {
        select( aItem );
        EDA_RECT bbox = aItem->GetBoundingBox();
        BOX2D viewport( VECTOR2D( bbox.GetOrigin() ), VECTOR2D( bbox.GetSize() ) );
        getView()->SetViewport( viewport );

        // Inform other potentially interested tools
        m_toolMgr->ProcessEvent( SelectedEvent );
    }

    m_frame->GetGalCanvas()->ForceRefresh();
}


int SELECTION_TOOL::find( const TOOL_EVENT& aEvent )
{
    DIALOG_FIND dlg( m_frame );
    dlg.EnableWarp( false );
    dlg.SetCallback( boost::bind( &SELECTION_TOOL::findCallback, this, _1 ) );
    dlg.ShowModal();

    return 0;
}


int SELECTION_TOOL::findMove( const TOOL_EVENT& aEvent )
{
    MODULE* module = m_frame->GetModuleByName();

    if( module )
    {
        clearSelection();
        toggleSelection( module );
        m_toolMgr->InvokeTool( "pcbnew.InteractiveEdit" );
    }

    return 0;
}


void SELECTION_TOOL::clearSelection()
{
    if( m_selection.Empty() )
        return;

    KIGFX::VIEW_GROUP::const_iter it, it_end;

    // Restore the initial properties
    for( it = m_selection.group->Begin(), it_end = m_selection.group->End(); it != it_end; ++it )
    {
        BOARD_ITEM* item = static_cast<BOARD_ITEM*>( *it );

        item->ViewHide( false );
        item->ClearSelected();
        item->ViewUpdate( KIGFX::VIEW_ITEM::GEOMETRY ) ;
    }

    m_selection.clear();

    m_frame->SetCurItem( NULL );
    m_locked = true;

    // Inform other potentially interested tools
    m_toolMgr->ProcessEvent( ClearedEvent );
}


BOARD_ITEM* SELECTION_TOOL::disambiguationMenu( GENERAL_COLLECTOR* aCollector )
{
    BOARD_ITEM* current = NULL;
    boost::shared_ptr<BRIGHT_BOX> brightBox;
    CONTEXT_MENU menu;

    int limit = std::min( 10, aCollector->GetCount() );

    for( int i = 0; i < limit; ++i )
    {
        wxString text;
        BOARD_ITEM* item = ( *aCollector )[i];
        text = item->GetSelectMenuText();
        menu.Add( text, i + 1 );
    }

    menu.SetTitle( _( "Clarify selection" ) );
    SetContextMenu( &menu, CMENU_NOW );

    while( OPT_TOOL_EVENT evt = Wait() )
    {
        if( evt->Action() == TA_CONTEXT_MENU_UPDATE )
        {
            if( current )
                current->ClearBrightened();

            int id = *evt->GetCommandId();

            // User has pointed an item, so show it in a different way
            if( id > 0 && id <= limit )
            {
                current = ( *aCollector )[id - 1];
                current->SetBrightened();
            }
            else
            {
                current = NULL;
            }
        }
        else if( evt->Action() == TA_CONTEXT_MENU_CHOICE )
        {
            boost::optional<int> id = evt->GetCommandId();

            // User has selected an item, so this one will be returned
            if( id && ( *id > 0 ) )
                current = ( *aCollector )[*id - 1];
            else
                current = NULL;

            break;
        }

        // Draw a mark to show which item is available to be selected
        if( current && current->IsBrightened() )
        {
            brightBox.reset( new BRIGHT_BOX( current ) );
            getView()->Add( brightBox.get() );
            // BRIGHT_BOX is removed from view on destruction
        }
    }

    return current;
}


BOARD_ITEM* SELECTION_TOOL::pickSmallestComponent( GENERAL_COLLECTOR* aCollector )
{
    int count = aCollector->GetPrimaryCount();     // try to use preferred layer

    if( 0 == count )
        count = aCollector->GetCount();

    for( int i = 0; i < count; ++i )
    {
        if( ( *aCollector )[i]->Type() != PCB_MODULE_T )
            return NULL;
    }

    // All are modules, now find smallest MODULE
    int minDim = 0x7FFFFFFF;
    int minNdx = 0;

    for( int i = 0; i < count; ++i )
    {
        MODULE* module = (MODULE*) ( *aCollector )[i];

        int lx = module->GetBoundingBox().GetWidth();
        int ly = module->GetBoundingBox().GetHeight();

        int lmin = std::min( lx, ly );

        if( lmin < minDim )
        {
            minDim = lmin;
            minNdx = i;
        }
    }

    return (*aCollector)[minNdx];
}


bool SELECTION_TOOL::selectable( const BOARD_ITEM* aItem ) const
{
    // Is high contrast mode enabled?
    bool highContrast = getView()->GetPainter()->GetSettings()->GetHighContrast();

    if( highContrast )
    {
        bool onActive = false;          // Is the item on any of active layers?
        int layers[KIGFX::VIEW::VIEW_MAX_LAYERS], layers_count;

        // Filter out items that do not belong to active layers
        const std::set<unsigned int>& activeLayers = getView()->GetPainter()->
                                                     GetSettings()->GetActiveLayers();
        aItem->ViewGetLayers( layers, layers_count );

        for( int i = 0; i < layers_count; ++i )
        {
            if( activeLayers.count( layers[i] ) > 0 ) // Item is on at least one of the active layers
            {
                onActive = true;
                break;
            }
        }

        if( !onActive ) // We do not want to select items that are in the background
            return false;
    }

    BOARD* board = getModel<BOARD>();

    switch( aItem->Type() )
    {
    case PCB_VIA_T:
        {
            // For vias it is enough if only one of layers is visible
            LAYER_ID top, bottom;

            static_cast<const VIA*>( aItem )->LayerPair( &top, &bottom );

            return board->IsLayerVisible( top ) || board->IsLayerVisible( bottom );
        }
        break;

    case PCB_MODULE_T:
        if( aItem->IsOnLayer( F_Cu ) && board->IsElementVisible( MOD_FR_VISIBLE ) )
            return !m_editModules;

        if( aItem->IsOnLayer( B_Cu ) && board->IsElementVisible( MOD_BK_VISIBLE ) )
            return !m_editModules;

        return false;

        break;

    case PCB_MODULE_TEXT_T:
        if( m_multiple && !m_editModules )
            return false;

        return aItem->ViewIsVisible() && board->IsLayerVisible( aItem->GetLayer() );

    case PCB_MODULE_EDGE_T:
    case PCB_PAD_T:
    {
        if( m_multiple && !m_editModules )
            return false;

        MODULE* mod = static_cast<const D_PAD*>( aItem )->GetParent();
        if( mod && mod->IsLocked() )
            return false;

        break;
    }

    // These are not selectable
    case NOT_USED:
    case TYPE_NOT_INIT:
        return false;

    default:    // Suppress warnings
        break;
    }

    // All other items are selected only if the layer on which they exist is visible
    return board->IsLayerVisible( aItem->GetLayer() );
}


bool SELECTION_TOOL::modifiable( const BOARD_ITEM* aItem ) const
{
    if( aItem->Type() == PCB_MARKER_T )
        return false;

    return true;
}


void SELECTION_TOOL::select( BOARD_ITEM* aItem )
{
    if( aItem->IsSelected() )
        return;

    // Modules are treated in a special way - when they are selected, we have to mark
    // all the parts that make the module as selected
    if( aItem->Type() == PCB_MODULE_T )
    {
        MODULE* module = static_cast<MODULE*>( aItem );
        module->RunOnChildren( boost::bind( &SELECTION_TOOL::selectVisually, this, _1 ) );
    }

    if( aItem->Type() == PCB_PAD_T )
    {
        MODULE* module = static_cast<MODULE*>( aItem->GetParent() );

        if( m_selection.items.FindItem( module ) >= 0 )
            return;
    }

    selectVisually( aItem );
    ITEM_PICKER picker( aItem );
    m_selection.items.PushItem( picker );

    if( m_selection.Size() == 1 )
    {
        // Set as the current item, so the information about selection is displayed
        m_frame->SetCurItem( aItem, true );
    }
    else if( m_selection.Size() == 2 )  // Check only for 2, so it will not be
    {                                   // called for every next selected item
        // If multiple items are selected, do not show the information about the selected item
        m_frame->SetCurItem( NULL, true );
    }
}


void SELECTION_TOOL::unselect( BOARD_ITEM* aItem )
{
    if( !aItem->IsSelected() )
        return;

    // Modules are treated in a special way - when they are selected, we have to
    // unselect all the parts that make the module, not the module itself
    if( aItem->Type() == PCB_MODULE_T )
    {
        MODULE* module = static_cast<MODULE*>( aItem );
        module->RunOnChildren( boost::bind( &SELECTION_TOOL::unselectVisually, this, _1 ) );
    }

    unselectVisually( aItem );

    int itemIdx = m_selection.items.FindItem( aItem );
    if( itemIdx >= 0 )
        m_selection.items.RemovePicker( itemIdx );

    if( m_selection.Empty() )
    {
        m_frame->SetCurItem( NULL );
        m_locked = true;
    }
}


void SELECTION_TOOL::selectVisually( BOARD_ITEM* aItem ) const
{
    m_selection.group->Add( aItem );

    // Hide the original item, so it is shown only on overlay
    aItem->ViewHide( true );
    aItem->SetSelected();
}


void SELECTION_TOOL::unselectVisually( BOARD_ITEM* aItem ) const
{
    m_selection.group->Remove( aItem );

    // Restore original item visibility
    aItem->ViewHide( false );
    aItem->ClearSelected();
    aItem->ViewUpdate( KIGFX::VIEW_ITEM::GEOMETRY );
}


bool SELECTION_TOOL::selectionContains( const VECTOR2I& aPoint ) const
{
    const unsigned GRIP_MARGIN = 20;
    VECTOR2D margin = getView()->ToWorld( VECTOR2D( GRIP_MARGIN, GRIP_MARGIN ), false );

    // Check if the point is located within any of the currently selected items bounding boxes
    for( unsigned int i = 0; i < m_selection.items.GetCount(); ++i )
    {
        BOARD_ITEM* item = m_selection.Item<BOARD_ITEM>( i );
        BOX2I itemBox = item->ViewBBox();
        itemBox.Inflate( margin.x, margin.y );    // Give some margin for gripping an item

        if( itemBox.Contains( aPoint ) )
            return true;
    }

    return false;
}


static EDA_RECT getRect( const BOARD_ITEM* aItem )
{
    if( aItem->Type() == PCB_MODULE_T )
        return static_cast<const MODULE*>( aItem )->GetFootprintRect();

    return aItem->GetBoundingBox();
}


static double calcArea( const BOARD_ITEM* aItem )
{
    if( aItem->Type() == PCB_TRACE_T )
    {
        const TRACK* t = static_cast<const TRACK*>( aItem );
        return ( t->GetWidth() + t->GetLength() ) * t->GetWidth();
    }

    return getRect( aItem ).GetArea();
}


static double calcMinArea( GENERAL_COLLECTOR& aCollector, KICAD_T aType )
{
    double best = std::numeric_limits<double>::max();

    if( !aCollector.GetCount() )
        return 0.0;

    for( int i = 0; i < aCollector.GetCount(); i++ )
    {
        BOARD_ITEM* item = aCollector[i];
        if( item->Type() == aType )
            best = std::min( best, calcArea( item ) );
    }

    return best;
}


static double calcMaxArea( GENERAL_COLLECTOR& aCollector, KICAD_T aType )
{
    double best = 0.0;

    for( int i = 0; i < aCollector.GetCount(); i++ )
    {
        BOARD_ITEM* item = aCollector[i];
        if( item->Type() == aType )
            best = std::max( best, calcArea( item ) );
    }

    return best;
}


static inline double calcCommonArea( const BOARD_ITEM* aItem, const BOARD_ITEM* aOther )
{
    return getRect( aItem ).Common( getRect( aOther ) ).GetArea();
}


double calcRatio( double a, double b )
{
    if( a == 0.0 && b == 0.0 )
        return 1.0;
    if( b == 0.0 )
        return 10000000.0; // something arbitrarily big for the moment

    return a / b;
}


// todo: explain the selection heuristics
void SELECTION_TOOL::guessSelectionCandidates( GENERAL_COLLECTOR& aCollector ) const
{
    std::set<BOARD_ITEM*> rejected;

    const double footprintAreaRatio = 0.2;
    const double modulePadMinCoverRatio = 0.45;
    const double padViaAreaRatio = 0.5;
    const double trackViaLengthRatio = 2.0;
    const double trackTrackLengthRatio = 0.3;
    const double textToFeatureMinRatio = 0.2;
    const double textToFootprintMinRatio = 0.4;
    // If the common area of two compared items is above the following threshold, they cannot
    // be rejected (it means they overlap and it might be hard to pick one by selecting
    // its unique area).
    const double commonAreaRatio = 0.6;

    LAYER_ID actLayer = m_frame->GetActiveLayer();

    LSET silkLayers( 2, B_SilkS, F_SilkS );

    if( silkLayers[actLayer] )
    {
        std::set<BOARD_ITEM*> preferred;

        for( int i = 0; i < aCollector.GetCount(); ++i )
        {
            BOARD_ITEM* item = aCollector[i];
            KICAD_T type = item->Type();

            if( ( type == PCB_MODULE_TEXT_T || type == PCB_TEXT_T || type == PCB_LINE_T )
                    && silkLayers[item->GetLayer()] )
            {
                preferred.insert( item );
            }
        }

        if( preferred.size() != 0 )
        {
            aCollector.Empty();

            BOOST_FOREACH( BOARD_ITEM* item, preferred )
                aCollector.Append( item );
            return;
        }
    }

    if( aCollector.CountType( PCB_MODULE_TEXT_T ) > 0 )
    {
        for( int i = 0; i < aCollector.GetCount(); ++i )
        {
            if( TEXTE_MODULE* txt = dyn_cast<TEXTE_MODULE*>( aCollector[i] ) )
            {
                double textArea = calcArea( txt );

                for( int j = 0; j < aCollector.GetCount(); ++j )
                {
                    if( i == j )
                        continue;

                    BOARD_ITEM* item = aCollector[j];
                    double itemArea = calcArea( item );
                    double areaRatio = calcRatio( textArea, itemArea );
                    double commonArea = calcCommonArea( txt, item );
                    double itemCommonRatio = calcRatio( commonArea, itemArea );
                    double txtCommonRatio = calcRatio( commonArea, textArea );

                    if( item->Type() == PCB_MODULE_T && areaRatio < textToFootprintMinRatio &&
                            itemCommonRatio < commonAreaRatio )
                        rejected.insert( item );

                    switch( item->Type() )
                    {
                        case PCB_TRACE_T:
                        case PCB_PAD_T:
                        case PCB_LINE_T:
                        case PCB_VIA_T:
                        case PCB_MODULE_T:
                            if( areaRatio > textToFeatureMinRatio && txtCommonRatio < commonAreaRatio )
                                rejected.insert( txt );
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    if( aCollector.CountType( PCB_MODULE_T ) > 0 )
    {
        double minArea = calcMinArea( aCollector, PCB_MODULE_T );
        double maxArea = calcMaxArea( aCollector, PCB_MODULE_T );

        if( calcRatio( minArea, maxArea ) <= footprintAreaRatio )
        {
            for( int i = 0; i < aCollector.GetCount(); ++i )
            {
                if( MODULE* mod = dyn_cast<MODULE*>( aCollector[i] ) )
                {
                    double normalizedArea = calcRatio( calcArea( mod ), maxArea );

                    if( normalizedArea > footprintAreaRatio )
                        rejected.insert( mod );
                }
            }
        }
    }

    if( aCollector.CountType( PCB_PAD_T ) > 0 )
    {
        for( int i = 0; i < aCollector.GetCount(); ++i )
        {
            if( D_PAD* pad = dyn_cast<D_PAD*>( aCollector[i] ) )
            {
                double ratio = pad->GetParent()->PadCoverageRatio();

                if( ratio < modulePadMinCoverRatio )
                    rejected.insert( pad->GetParent() );
            }
        }
    }

    if( aCollector.CountType( PCB_VIA_T ) > 0 )
    {
        for( int i = 0; i < aCollector.GetCount(); ++i )
        {
            if( VIA* via = dyn_cast<VIA*>( aCollector[i] ) )
            {
                double viaArea = calcArea( via );

                for( int j = 0; j < aCollector.GetCount(); ++j )
                {
                    if( i == j )
                        continue;

                    BOARD_ITEM* item = aCollector[j];
                    double areaRatio = calcRatio( viaArea, calcArea( item ) );

                    if( item->Type() == PCB_MODULE_T && areaRatio < modulePadMinCoverRatio )
                        rejected.insert( item );

                    if( item->Type() == PCB_PAD_T && areaRatio < padViaAreaRatio )
                        rejected.insert( item );

                    if( TRACK* track = dyn_cast<TRACK*>( item ) )
                    {
                        if( track->GetNetCode() != via->GetNetCode() )
                            continue;

                        double lenRatio = (double) ( track->GetLength() + track->GetWidth() ) /
                                          (double) via->GetWidth();

                        if( lenRatio > trackViaLengthRatio )
                            rejected.insert( track );
                    }
                }
            }
        }
    }

    int nTracks = aCollector.CountType( PCB_TRACE_T );

    if( nTracks > 0 )
    {
        double maxLength = 0.0;
        double minLength = std::numeric_limits<double>::max();
        double maxArea = 0.0;

        for( int i = 0; i < aCollector.GetCount(); ++i )
        {
            if( TRACK* track = dyn_cast<TRACK*> ( aCollector[i] ) )
            {
                maxLength = std::max( track->GetLength(), maxLength );
                maxLength = std::max( (double) track->GetWidth(), maxLength );

                minLength = std::min( std::max( track->GetLength(), (double)track->GetWidth() ), minLength );

                double area =  ( track->GetLength() + track->GetWidth() * track->GetWidth() );
                maxArea = std::max(area, maxArea);
            }
        }

        if( maxLength > 0.0 && minLength / maxLength < trackTrackLengthRatio && nTracks > 1 )
        {
            for( int i = 0; i < aCollector.GetCount(); ++i )
             {
                if( TRACK* track = dyn_cast<TRACK*>( aCollector[i] ) )
                {
                    double ratio = std::max( (double) track->GetWidth(), track->GetLength() ) / maxLength;

                    if( ratio > trackTrackLengthRatio )
                        rejected.insert( track );
                }
            }
        }

        for( int j = 0; j < aCollector.GetCount(); ++j )
        {
            if( MODULE* mod = dyn_cast<MODULE*>( aCollector[j] ) )
            {
                double ratio = maxArea / mod->GetFootprintRect().GetArea();

                if( ratio < modulePadMinCoverRatio )
                    rejected.insert( mod );
            }
        }
    }

    if( (unsigned) aCollector.GetCount() > rejected.size() )  // do not remove everything
    {
        BOOST_FOREACH( BOARD_ITEM* item, rejected )
        {
            aCollector.Remove( item );
        }
    }
}


bool SELECTION_TOOL::SanitizeSelection()
{
    std::set<BOARD_ITEM*> rejected;
    std::set<BOARD_ITEM*> added;

    if( !m_editModules )
    {
        for( unsigned int i = 0; i < m_selection.items.GetCount(); ++i )
        {
            BOARD_ITEM* item = m_selection.Item<BOARD_ITEM>( i );

            if( item->Type() == PCB_PAD_T )
            {
                MODULE* mod = static_cast<MODULE*>( item->GetParent() );

                // case 1: module (or its pads) are locked
                if( mod && ( mod->PadsLocked() || mod->IsLocked() ) )
                {
                    rejected.insert( item );

                    if( !mod->IsLocked() && !mod->IsSelected() )
                        added.insert( mod );
                }

                // case 2: multi-item selection contains both the module and its pads - remove the pads
                if( mod && m_selection.items.FindItem( mod ) >= 0 )
                    rejected.insert( item );
            }
        }
    }

    if( !rejected.empty() )
    {
        BOOST_FOREACH( BOARD_ITEM* item, rejected )
            unselect( item );

        // Inform other potentially interested tools
        m_toolMgr->ProcessEvent( UnselectedEvent );
    }

    if( !added.empty() )
    {
        BOOST_FOREACH( BOARD_ITEM* item, added )
            select( item );

        // Inform other potentially interested tools
        m_toolMgr->ProcessEvent( UnselectedEvent );
    }

    return true;
}


void SELECTION::clear()
{
    items.ClearItemsList();
    group->Clear();
}


VECTOR2I SELECTION::GetCenter() const
{
    VECTOR2I centre;

    if( Size() == 1 )
    {
        centre = Item<BOARD_ITEM>( 0 )->GetCenter();
    }
    else
    {
        EDA_RECT bbox =  Item<BOARD_ITEM>( 0 )->GetBoundingBox();
        for( unsigned int i = 1; i < items.GetCount(); ++i )
        {
            BOARD_ITEM* item = Item<BOARD_ITEM>( i );
            bbox.Merge( item->GetBoundingBox() );
        }

        centre = bbox.Centre();
    }

    return centre;
}


const TOOL_EVENT SELECTION_TOOL::SelectedEvent( TC_MESSAGE, TA_ACTION, "pcbnew.InteractiveSelection.selected" );
const TOOL_EVENT SELECTION_TOOL::UnselectedEvent( TC_MESSAGE, TA_ACTION, "pcbnew.InteractiveSelection.unselected" );
const TOOL_EVENT SELECTION_TOOL::ClearedEvent( TC_MESSAGE, TA_ACTION, "pcbnew.InteractiveSelection.cleared" );
