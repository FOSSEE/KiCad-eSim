/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2014 CERN
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

#include "selection_conditions.h"
#include "selection_tool.h"
#include <class_board_connected_item.h>

#include <boost/bind.hpp>


bool SELECTION_CONDITIONS::NotEmpty( const SELECTION& aSelection )
{
    return !aSelection.Empty();
}


bool SELECTION_CONDITIONS::OnlyConnectedItems( const SELECTION& aSelection )
{
    if( aSelection.Empty() )
        return false;

    for( int i = 0; i < aSelection.Size(); ++i )
    {
        KICAD_T type = aSelection.Item<EDA_ITEM>( i )->Type();

        if( type != PCB_PAD_T && type != PCB_VIA_T && type != PCB_TRACE_T && type != PCB_ZONE_T )
            return false;
    }

    return true;
}


SELECTION_CONDITION SELECTION_CONDITIONS::SameNet( bool aAllowUnconnected )
{
    return boost::bind( &SELECTION_CONDITIONS::sameNetFunc, _1, aAllowUnconnected );
}


SELECTION_CONDITION SELECTION_CONDITIONS::SameLayer()
{
    return boost::bind( &SELECTION_CONDITIONS::sameLayerFunc, _1 );
}


SELECTION_CONDITION SELECTION_CONDITIONS::HasType( KICAD_T aType )
{
    return boost::bind( &SELECTION_CONDITIONS::hasTypeFunc, _1, aType );
}


SELECTION_CONDITION SELECTION_CONDITIONS::OnlyType( KICAD_T aType )
{
    return boost::bind( &SELECTION_CONDITIONS::onlyTypeFunc, _1, aType );
}


SELECTION_CONDITION SELECTION_CONDITIONS::OnlyTypes( const std::vector<KICAD_T>& aTypes )
{
    return boost::bind( &SELECTION_CONDITIONS::onlyTypesFunc, _1, aTypes );
}


SELECTION_CONDITION SELECTION_CONDITIONS::Count( int aNumber )
{
    return boost::bind( &SELECTION_CONDITIONS::countFunc, _1, aNumber );
}


SELECTION_CONDITION SELECTION_CONDITIONS::MoreThan( int aNumber )
{
    return boost::bind( &SELECTION_CONDITIONS::moreThanFunc, _1, aNumber );
}


SELECTION_CONDITION SELECTION_CONDITIONS::LessThan( int aNumber )
{
    return boost::bind( &SELECTION_CONDITIONS::lessThanFunc, _1, aNumber );
}


bool SELECTION_CONDITIONS::sameNetFunc( const SELECTION& aSelection, bool aAllowUnconnected )
{
    if( aSelection.Empty() )
        return false;

    int netcode = -1;   // -1 stands for 'net code is not yet determined'

    for( int i = 0; i < aSelection.Size(); ++i )
    {
        int current_netcode = -1;

        const BOARD_CONNECTED_ITEM* item =
            dynamic_cast<const BOARD_CONNECTED_ITEM*>( aSelection.Item<EDA_ITEM>( i ) );

        if( item )
        {
            current_netcode = item->GetNetCode();
        }
        else
        {
            if( !aAllowUnconnected )
                return false;
            else
                // if it is not a BOARD_CONNECTED_ITEM, treat it as if there was no net assigned
                current_netcode = 0;
        }

        assert( current_netcode >= 0 );

        if( netcode < 0 )
        {
            netcode = current_netcode;

            if( netcode == NETINFO_LIST::UNCONNECTED && !aAllowUnconnected )
                return false;
        }
        else if( netcode != current_netcode )
        {
            return false;
        }
    }

    return true;
}


bool SELECTION_CONDITIONS::sameLayerFunc( const SELECTION& aSelection )
{
    if( aSelection.Empty() )
        return false;

    LSET layerSet;
    layerSet.set();

    for( int i = 0; i < aSelection.Size(); ++i )
    {
        const BOARD_ITEM* item = dynamic_cast<const BOARD_ITEM*>( aSelection.Item<EDA_ITEM>( i ) );

        if( !item )
            return false;

        layerSet &= item->GetLayerSet();

        if( !layerSet.any() )       // there are no common layers left
            return false;
    }

    return true;
}


bool SELECTION_CONDITIONS::hasTypeFunc( const SELECTION& aSelection, KICAD_T aType )
{
    for( int i = 0; i < aSelection.Size(); ++i )
    {
        if( aSelection.Item<EDA_ITEM>( i )->Type() == aType )
            return true;
    }

    return false;
}


bool SELECTION_CONDITIONS::onlyTypeFunc( const SELECTION& aSelection, KICAD_T aType )
{
    if( aSelection.Empty() )
        return false;

    for( int i = 0; i < aSelection.Size(); ++i )
    {
        if( aSelection.Item<EDA_ITEM>( i )->Type() != aType )
            return false;
    }

    return true;
}


bool SELECTION_CONDITIONS::onlyTypesFunc( const SELECTION& aSelection, const std::vector<KICAD_T>& aTypes )
{
    if( aSelection.Empty() )
        return false;

    for( int i = 0; i < aSelection.Size(); ++i )
    {
        bool valid = false;

        for( std::vector<KICAD_T>::const_iterator it = aTypes.begin(); it != aTypes.end(); ++it )
        {
            if( aSelection.Item<EDA_ITEM>( i )->Type() == *it )
            {
                valid = true;
                break;
            }
        }

        if( !valid )
            return false;
    }

    return true;
}


bool SELECTION_CONDITIONS::countFunc( const SELECTION& aSelection, int aNumber )
{
    return aSelection.Size() == aNumber;
}


bool SELECTION_CONDITIONS::moreThanFunc( const SELECTION& aSelection, int aNumber )
{
    return aSelection.Size() > aNumber;
}


bool SELECTION_CONDITIONS::lessThanFunc( const SELECTION& aSelection, int aNumber )
{
    return aSelection.Size() < aNumber;
}


SELECTION_CONDITION operator||( const SELECTION_CONDITION& aConditionA,
                                const SELECTION_CONDITION& aConditionB )
{
    return boost::bind( &SELECTION_CONDITIONS::orFunc, aConditionA, aConditionB, _1 );
}


SELECTION_CONDITION operator&&( const SELECTION_CONDITION& aConditionA,
                                const SELECTION_CONDITION& aConditionB )
{
    return boost::bind( &SELECTION_CONDITIONS::andFunc, aConditionA, aConditionB, _1 );
}
