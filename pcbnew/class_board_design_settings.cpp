/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 1992-2015 KiCad Developers, see AUTHORS.txt for contributors.
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
 * @file class_board_design_settings.cpp
 * BOARD_DESIGN_SETTINGS class functions.
 */

#include <fctsys.h>
#include <common.h>
#include <layers_id_colors_and_visibility.h>

#include <pcbnew.h>
#include <class_board_design_settings.h>

#include <class_track.h>
#include <convert_from_iu.h>


BOARD_DESIGN_SETTINGS::BOARD_DESIGN_SETTINGS() :
    m_Pad_Master( NULL )
{
    LSET    all_set = LSET().set();

    m_enabledLayers = all_set;              // All layers enabled at first.
                                            // SetCopperLayerCount() will adjust this.
    SetVisibleLayers( all_set );

    // set all but hidden text as visible.
    m_visibleElements = ~( 1 << MOD_TEXT_INVISIBLE );

    SetCopperLayerCount( 2 );               // Default design is a double sided board

    // via type (VIA_BLIND_BURIED, VIA_THROUGH VIA_MICROVIA).
    m_CurrentViaType = VIA_THROUGH;

    // if true, when creating a new track starting on an existing track, use this track width
    m_UseConnectedTrackWidth = false;

    m_BlindBuriedViaAllowed = false;            // true to allow blind/buried vias
    m_MicroViasAllowed = false;                 // true to allow micro vias

    m_DrawSegmentWidth = Millimeter2iu( DEFAULT_GRAPHIC_THICKNESS );     // current graphic line width (not EDGE layer)

    m_EdgeSegmentWidth = Millimeter2iu( DEFAULT_PCB_EDGE_THICKNESS );    // current graphic line width (EDGE layer only)
    m_PcbTextWidth     = Millimeter2iu( DEFAULT_TEXT_PCB_THICKNESS );    // current Pcb (not module) Text width

    m_PcbTextSize       = wxSize( Millimeter2iu( DEFAULT_TEXT_PCB_SIZE  ),
                                  Millimeter2iu( DEFAULT_TEXT_PCB_SIZE  ) );  // current Pcb (not module) Text size

    m_useCustomTrackVia = false;
    m_customTrackWidth  = Millimeter2iu( DEFAULT_CUSTOMTRACKWIDTH );
    m_customViaSize.m_Diameter = Millimeter2iu( DEFAULT_VIASMINSIZE );
    m_customViaSize.m_Drill = Millimeter2iu( DEFAULT_VIASMINDRILL );

    m_TrackMinWidth     = Millimeter2iu( DEFAULT_TRACKMINWIDTH );   // track min width
    m_ViasMinSize       = Millimeter2iu( DEFAULT_VIASMINSIZE );     // via (not uvia) min diam
    m_ViasMinDrill      = Millimeter2iu( DEFAULT_VIASMINDRILL );    // via (not uvia) min drill diam
    m_MicroViasMinSize  = Millimeter2iu( DEFAULT_MICROVIASMINSIZE );// uvia (not via) min diam
    m_MicroViasMinDrill = Millimeter2iu( DEFAULT_MICROVIASMINDRILL );// uvia (not via) min drill diam

    // Global mask margins:
    m_SolderMaskMargin  = Millimeter2iu( DEFAULT_SOLDERMASK_CLEARANCE ); // Solder mask margin
    m_SolderMaskMinWidth = Millimeter2iu( DEFAULT_SOLDERMASK_MIN_WIDTH );   // Solder mask min width
    m_SolderPasteMargin = 0;                    // Solder paste margin absolute value
    m_SolderPasteMarginRatio = 0.0;             // Solder pask margin ratio value of pad size
                                                // The final margin is the sum of these 2 values
                                                // Usually < 0 because the mask is smaller than pad

    // Layer thickness for 3D viewer
    m_boardThickness = Millimeter2iu( DEFAULT_BOARD_THICKNESS_MM );

    m_viaSizeIndex = 0;
    m_trackWidthIndex = 0;

    // Default values for the footprint editor and fp creation
    // (also covers footprints created on the fly by micor-waves tools)
    m_ModuleTextSize = wxSize( Millimeter2iu( DEFAULT_TEXT_MODULE_SIZE ),
                               Millimeter2iu( DEFAULT_TEXT_MODULE_SIZE ) );
    m_ModuleTextWidth = Millimeter2iu( DEFAULT_GR_MODULE_THICKNESS );
    m_ModuleSegmentWidth = Millimeter2iu( DEFAULT_GR_MODULE_THICKNESS );

    // These values will be overriden by config values after reading the config
    // Default ref text on fp creation. if empty, use footprint name as default
    m_RefDefaultText = wxT( "REF**" );
    m_RefDefaultVisibility = true;          // Default ref text visibility on fp creation
    m_RefDefaultlayer = int( F_SilkS );     // Default ref text layer on fp creation
    // Default value text on fp creation. if empty, use footprint name as default
    m_ValueDefaultText = wxEmptyString;
    m_ValueDefaultVisibility = true;
    m_ValueDefaultlayer = int( F_Fab );
}

// Add parameters to save in project config.
// values are saved in mm
void BOARD_DESIGN_SETTINGS::AppendConfigs( PARAM_CFG_ARRAY* aResult )
{
    m_Pad_Master.AppendConfigs( aResult );

    aResult->push_back( new PARAM_CFG_INT_WITH_SCALE( wxT( "PcbTextSizeV" ),
                    &m_PcbTextSize.y,
                    Millimeter2iu( DEFAULT_TEXT_PCB_SIZE  ), TEXTS_MIN_SIZE, TEXTS_MAX_SIZE,
                    NULL, MM_PER_IU ) );

    aResult->push_back( new PARAM_CFG_INT_WITH_SCALE( wxT( "PcbTextSizeH" ),
                    &m_PcbTextSize.x,
                    Millimeter2iu( DEFAULT_TEXT_PCB_SIZE  ), TEXTS_MIN_SIZE, TEXTS_MAX_SIZE,
                    NULL, MM_PER_IU ) );

    aResult->push_back( new PARAM_CFG_INT_WITH_SCALE( wxT( "PcbTextThickness" ),
                    &m_PcbTextWidth,
                    Millimeter2iu(DEFAULT_TEXT_PCB_THICKNESS ),
                    Millimeter2iu( 0.01 ), Millimeter2iu( 5.0 ),
                    NULL, MM_PER_IU ) );

    aResult->push_back( new PARAM_CFG_INT_WITH_SCALE( wxT( "ModuleTextSizeV" ),
                    &m_ModuleTextSize.y,
                    DEFAULT_TEXT_MODULE_SIZE, TEXTS_MIN_SIZE, TEXTS_MAX_SIZE,
                    NULL, MM_PER_IU ) );

    aResult->push_back( new PARAM_CFG_INT_WITH_SCALE( wxT( "ModuleTextSizeH" ),
                    &m_ModuleTextSize.x,
                    DEFAULT_TEXT_MODULE_SIZE, TEXTS_MIN_SIZE, TEXTS_MAX_SIZE,
                    NULL, MM_PER_IU ) );

    aResult->push_back( new PARAM_CFG_INT_WITH_SCALE( wxT( "ModuleTextSizeThickness" ),
                    &m_ModuleTextWidth,
                    Millimeter2iu( DEFAULT_GR_MODULE_THICKNESS ), 1, TEXTS_MAX_WIDTH,
                    NULL, MM_PER_IU ) );

    aResult->push_back( new PARAM_CFG_INT_WITH_SCALE( wxT( "SolderMaskClearance" ),
                    &m_SolderMaskMargin,
                    Millimeter2iu( DEFAULT_SOLDERMASK_CLEARANCE ), 0, Millimeter2iu( 1.0 ),
                    NULL, MM_PER_IU ) );

    aResult->push_back( new PARAM_CFG_INT_WITH_SCALE( wxT( "SolderMaskMinWidth" ),
                    &m_SolderMaskMinWidth,
                    Millimeter2iu( DEFAULT_SOLDERMASK_MIN_WIDTH ), 0, Millimeter2iu( 0.5 ),
                    NULL, MM_PER_IU ) );

    aResult->push_back( new PARAM_CFG_INT_WITH_SCALE( wxT( "DrawSegmentWidth" ),
                    &m_DrawSegmentWidth,
                    Millimeter2iu( DEFAULT_GRAPHIC_THICKNESS ),
                    Millimeter2iu( 0.01 ), Millimeter2iu( 5.0 ),
                    NULL, MM_PER_IU ) );

    aResult->push_back( new PARAM_CFG_INT_WITH_SCALE( wxT( "BoardOutlineThickness" ),
                    &m_EdgeSegmentWidth,
                    Millimeter2iu( DEFAULT_PCB_EDGE_THICKNESS ),
                    Millimeter2iu( 0.01 ), Millimeter2iu( 5.0 ),
                    NULL, MM_PER_IU ) );

    aResult->push_back( new PARAM_CFG_INT_WITH_SCALE( wxT( "ModuleOutlineThickness" ),
                    &m_ModuleSegmentWidth,
                    Millimeter2iu( DEFAULT_GR_MODULE_THICKNESS ),
                    Millimeter2iu( 0.01 ), Millimeter2iu( 5.0 ),
                    NULL, MM_PER_IU ) );
}


bool BOARD_DESIGN_SETTINGS::SetCurrentNetClass( const wxString& aNetClassName )
{
    NETCLASSPTR netClass = m_NetClasses.Find( aNetClassName );
    bool        lists_sizes_modified = false;

    // if not found (should not happen) use the default
    if( netClass == NULL )
        netClass = m_NetClasses.GetDefault();

    m_currentNetClassName = netClass->GetName();

    // Initialize others values:
    if( m_ViasDimensionsList.size() == 0 )
    {
        VIA_DIMENSION viadim;
        lists_sizes_modified = true;
        m_ViasDimensionsList.push_back( viadim );
    }

    if( m_TrackWidthList.size() == 0 )
    {
        lists_sizes_modified = true;
        m_TrackWidthList.push_back( 0 );
    }

    /* note the m_ViasDimensionsList[0] and m_TrackWidthList[0] values
     * are always the Netclass values
     */
    if( m_ViasDimensionsList[0].m_Diameter != netClass->GetViaDiameter() )
    {
        lists_sizes_modified = true;
        m_ViasDimensionsList[0].m_Diameter = netClass->GetViaDiameter();
    }

    if( m_ViasDimensionsList[0].m_Drill != netClass->GetViaDrill() )
    {
        lists_sizes_modified = true;
        m_ViasDimensionsList[0].m_Drill = netClass->GetViaDrill();
    }

    if( m_TrackWidthList[0] != netClass->GetTrackWidth() )
    {
        lists_sizes_modified = true;
        m_TrackWidthList[0] = netClass->GetTrackWidth();
    }

    if( GetViaSizeIndex() >= m_ViasDimensionsList.size() )
        SetViaSizeIndex( m_ViasDimensionsList.size() );

    if( GetTrackWidthIndex() >= m_TrackWidthList.size() )
        SetTrackWidthIndex( m_TrackWidthList.size() );

    return lists_sizes_modified;
}


int BOARD_DESIGN_SETTINGS::GetBiggestClearanceValue()
{
    int clearance = m_NetClasses.GetDefault()->GetClearance();

    //Read list of Net Classes
    for( NETCLASSES::const_iterator nc = m_NetClasses.begin(); nc != m_NetClasses.end(); ++nc )
    {
        NETCLASSPTR netclass = nc->second;
        clearance = std::max( clearance, netclass->GetClearance() );
    }

    return clearance;
}


int BOARD_DESIGN_SETTINGS::GetSmallestClearanceValue()
{
    int clearance = m_NetClasses.GetDefault()->GetClearance();

    //Read list of Net Classes
    for( NETCLASSES::const_iterator nc = m_NetClasses.begin(); nc != m_NetClasses.end(); ++nc )
    {
        NETCLASSPTR netclass = nc->second;
        clearance = std::min( clearance, netclass->GetClearance() );
    }

    return clearance;
}


int BOARD_DESIGN_SETTINGS::GetCurrentMicroViaSize()
{
    NETCLASSPTR netclass = m_NetClasses.Find( m_currentNetClassName );

    return netclass->GetuViaDiameter();
}


int BOARD_DESIGN_SETTINGS::GetCurrentMicroViaDrill()
{
    NETCLASSPTR netclass = m_NetClasses.Find( m_currentNetClassName );

    return netclass->GetuViaDrill();
}


void BOARD_DESIGN_SETTINGS::SetViaSizeIndex( unsigned aIndex )
{
    if( aIndex >= m_ViasDimensionsList.size() )
        m_viaSizeIndex = m_ViasDimensionsList.size();
    else
        m_viaSizeIndex = aIndex;

    m_useCustomTrackVia = false;
}


int BOARD_DESIGN_SETTINGS::GetCurrentViaDrill() const
{
    int drill;

    if( m_useCustomTrackVia )
        drill = m_customViaSize.m_Drill;
    else
        drill = m_ViasDimensionsList[m_viaSizeIndex].m_Drill;

    return drill > 0 ? drill : -1;
}


void BOARD_DESIGN_SETTINGS::SetTrackWidthIndex( unsigned aIndex )
{
    if( aIndex >= m_TrackWidthList.size() )
        m_trackWidthIndex = m_TrackWidthList.size();
    else
        m_trackWidthIndex = aIndex;

    m_useCustomTrackVia = false;
}


void BOARD_DESIGN_SETTINGS::SetVisibleAlls()
{
    SetVisibleLayers( LSET().set() );
    m_visibleElements = -1;
}


void BOARD_DESIGN_SETTINGS::SetLayerVisibility( LAYER_ID aLayer, bool aNewState )
{
    if( aNewState && IsLayerEnabled( aLayer ) )
        m_visibleLayers.set( aLayer, true );
    else
        m_visibleLayers.set( aLayer, false );
}


void BOARD_DESIGN_SETTINGS::SetElementVisibility( int aElementCategory, bool aNewState )
{
    if( aElementCategory < 0 || aElementCategory >= END_PCB_VISIBLE_LIST )
        return;

    if( aNewState )
        m_visibleElements |= 1 << aElementCategory;
    else
        m_visibleElements &= ~( 1 << aElementCategory );
}


void BOARD_DESIGN_SETTINGS::SetCopperLayerCount( int aNewLayerCount )
{
    // if( aNewLayerCount < 2 ) aNewLayerCount = 2;

    m_copperLayerCount = aNewLayerCount;

    // ensure consistency with the m_EnabledLayers member
#if 0
    // was:
    m_enabledLayers &= ~ALL_CU_LAYERS;
    m_enabledLayers |= LAYER_BACK;

    if( m_copperLayerCount > 1 )
        m_enabledLayers |= LAYER_FRONT;

    for( LAYER_NUM ii = LAYER_N_2; ii < aNewLayerCount - 1; ++ii )
        m_enabledLayers |= GetLayerSet( ii );
#else
    // Update only enabled copper layers mask
    m_enabledLayers &= ~LSET::AllCuMask();
    m_enabledLayers |= LSET::AllCuMask( aNewLayerCount );
#endif
}


void BOARD_DESIGN_SETTINGS::SetEnabledLayers( LSET aMask )
{
    // Back and front layers are always enabled.
    aMask.set( B_Cu ).set( F_Cu );

    m_enabledLayers = aMask;

    // A disabled layer cannot be visible
    m_visibleLayers &= aMask;

    // update m_CopperLayerCount to ensure its consistency with m_EnabledLayers
    m_copperLayerCount = ( aMask & LSET::AllCuMask() ).count();
}


#ifndef NDEBUG
struct list_size_check {
   list_size_check()
   {
       // Int (the type used for saving visibility settings) is only 32 bits guaranteed,
       // be sure that we do not cross the limit
       assert( END_PCB_VISIBLE_LIST <= 32 );
   };
};
static list_size_check check;
#endif
