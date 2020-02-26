/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2015 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 2012 SoftPLC Corporation, Dick Hollenbeck <dick@softplc.com>
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
 * @file class_module.cpp
 * @brief TEXT_MODULE class implementation.
 */

#include <fctsys.h>
#include <gr_basic.h>
#include <wxstruct.h>
#include <trigo.h>
#include <class_drawpanel.h>
#include <drawtxt.h>
#include <kicad_string.h>
#include <colors_selection.h>
#include <richio.h>
#include <macros.h>
#include <wxBasePcbFrame.h>
#include <msgpanel.h>
#include <base_units.h>

#include <class_board.h>
#include <class_module.h>

#include <pcbnew.h>


TEXTE_MODULE::TEXTE_MODULE( MODULE* parent, TEXT_TYPE text_type ) :
    BOARD_ITEM( parent, PCB_MODULE_TEXT_T ),
    EDA_TEXT()
{
    MODULE* module = static_cast<MODULE*>( m_Parent );

    m_Type = text_type;
    m_NoShow = false;
    // Set text tickness to a default value
    m_Thickness = Millimeter2iu( 0.15 );
    SetLayer( F_SilkS );

    // Set position and give a default layer if a valid parent footprint exists
    if( module && ( module->Type() == PCB_MODULE_T ) )
    {
        m_Pos = module->GetPosition();

        if( IsBackLayer( module->GetLayer() ) )
        {
            SetLayer( B_SilkS );
            m_Mirror = true;
        }
    }

    SetDrawCoord();
}


TEXTE_MODULE::~TEXTE_MODULE()
{
}


void TEXTE_MODULE::Rotate( const wxPoint& aRotCentre, double aAngle )
{
    // Used in footprint edition
    // Note also in module editor, m_Pos0 = m_Pos
    RotatePoint( &m_Pos, aRotCentre, aAngle );
    SetOrientation( GetOrientation() + aAngle );
    SetLocalCoord();
}


void TEXTE_MODULE::Flip( const wxPoint& aCentre )
{
    // flipping the footprint is relative to the X axis
    MIRROR( m_Pos.y, aCentre.y );
    NEGATE_AND_NORMALIZE_ANGLE_POS( m_Orient );
    SetLayer( FlipLayer( GetLayer() ) );
    m_Mirror = IsBackLayer( GetLayer() );
    SetLocalCoord();
}


void TEXTE_MODULE::Mirror( const wxPoint& aCentre, bool aMirrorAroundXAxis )
{
    // Used in modedit, to transform the footprint
    // the mirror is around the Y axis or X axis if aMirrorAroundXAxis = true
    // the position is mirrored, but the text itself is not mirrored
    if( aMirrorAroundXAxis )
        MIRROR( m_Pos.y, aCentre.y );
    else
        MIRROR( m_Pos.x, aCentre.x );

    NEGATE_AND_NORMALIZE_ANGLE_POS( m_Orient );
    SetLocalCoord();
}


void TEXTE_MODULE::Move( const wxPoint& aMoveVector )
{
    m_Pos += aMoveVector;
    SetLocalCoord();
}

void TEXTE_MODULE::Copy( TEXTE_MODULE* source )
{
    if( source == NULL )
        return;

    m_Pos = source->m_Pos;
    SetLayer( source->GetLayer() );

    m_Mirror = source->m_Mirror;
    m_NoShow = source->m_NoShow;
    m_Type   = source->m_Type;
    m_Orient = source->m_Orient;
    m_Pos0   = source->m_Pos0;
    m_Size   = source->m_Size;
    m_Thickness  = source->m_Thickness;
    m_Italic = source->m_Italic;
    m_Bold   = source->m_Bold;
    m_Text   = source->m_Text;
}


int TEXTE_MODULE::GetLength() const
{
    return m_Text.Len();
}


void TEXTE_MODULE::SetDrawCoord()
{
    const MODULE* module = static_cast<const MODULE*>( m_Parent );

    m_Pos = m_Pos0;

    if( module  )
    {
        double angle = module->GetOrientation();

        RotatePoint( &m_Pos.x, &m_Pos.y, angle );
        m_Pos += module->GetPosition();
    }
}


void TEXTE_MODULE::SetLocalCoord()
{
    const MODULE* module = static_cast<const MODULE*>( m_Parent );

    if( module )
    {
        m_Pos0 = m_Pos - module->GetPosition();
        double angle = module->GetOrientation();
        RotatePoint( &m_Pos0.x, &m_Pos0.y, -angle );
    }
    else
    {
        m_Pos0 = m_Pos;
    }
}


bool TEXTE_MODULE::HitTest( const wxPoint& aPosition ) const
{
    wxPoint  rel_pos;
    EDA_RECT area = GetTextBox( -1, -1 );

    /* Rotate refPos to - angle to test if refPos is within area (which
     * is relative to an horizontal text)
     */
    rel_pos = aPosition;
    RotatePoint( &rel_pos, m_Pos, -GetDrawRotation() );

    if( area.Contains( rel_pos ) )
        return true;

    return false;
}


/*
 * Function GetBoundingBox (virtual)
 * returns the bounding box of this Text (according to text and footprint
 * orientation)
 */
const EDA_RECT TEXTE_MODULE::GetBoundingBox() const
{
    double   angle = GetDrawRotation();
    EDA_RECT text_area = GetTextBox( -1, -1 );

    if( angle )
        text_area = text_area.GetBoundingBoxRotated( m_Pos, angle );

    return text_area;
}


/**
 * Function Draw
 * Draw the text according to the footprint pos and orient
 * @param panel = draw panel, Used to know the clip box
 * @param DC = Current Device Context
 * @param offset = draw offset (usually wxPoint(0,0)
 * @param draw_mode = GR_OR, GR_XOR..
 */
void TEXTE_MODULE::Draw( EDA_DRAW_PANEL* panel, wxDC* DC, GR_DRAWMODE draw_mode,
                         const wxPoint& offset )
{
    if( panel == NULL )
        return;

    /* parent must *not* be NULL (a footprint text without a footprint
       parent has no sense) */
    wxASSERT( m_Parent );

    BOARD* brd = GetBoard( );
    EDA_COLOR_T color = brd->GetLayerColor( GetLayer() );

    /* For reference and value suppress the element if the layer it is
     * on is on a disabled side, user text also has standard layer
     * hiding.
     * If the whole module side is disabled this isn't even called */
    LAYER_ID text_layer = GetLayer();

    if( (IsFrontLayer( text_layer ) && !brd->IsElementVisible( MOD_TEXT_FR_VISIBLE )) ||
        (IsBackLayer( text_layer ) && !brd->IsElementVisible( MOD_TEXT_BK_VISIBLE )) )
        return;

    // text which are not ref or value are shown only if the layer is visible
    // ref or value have a specific display option
    if( GetType() == TEXT_is_DIVERS && ! brd->IsLayerVisible( m_Layer ) )
        return;

    // Invisible texts are still drawn (not plotted) in MOD_TEXT_INVISIBLE
    // Just because we must have to edit them (at least to make them visible)
    if( m_NoShow )
    {
        if( !brd->IsElementVisible( MOD_TEXT_INVISIBLE ) )
            return;
        color = brd->GetVisibleElementColor( MOD_TEXT_INVISIBLE );
    }

    DISPLAY_OPTIONS* displ_opts = (DISPLAY_OPTIONS*)panel->GetDisplayOptions();

    // shade text if high contrast mode is active
    if( ( draw_mode & GR_ALLOW_HIGHCONTRAST ) && displ_opts &&
        displ_opts->m_ContrastModeDisplay )
    {
        LAYER_ID curr_layer = ( (PCB_SCREEN*) panel->GetScreen() )->m_Active_Layer;

        if( !IsOnLayer( curr_layer ) )
            ColorTurnToDarkDarkGray( &color );
    }

    // Draw mode compensation for the width
    int width = m_Thickness;

    if( displ_opts && displ_opts->m_DisplayModTextFill == SKETCH )
        width = -width;

    GRSetDrawMode( DC, draw_mode );
    wxPoint pos = m_Pos - offset;

    // Draw the text anchor point
    if( brd->IsElementVisible( ANCHOR_VISIBLE ) )
    {
        EDA_COLOR_T anchor_color = brd->GetVisibleElementColor(ANCHOR_VISIBLE);
        GRDrawAnchor( panel->GetClipBox(), DC, pos.x, pos.y,
                      DIM_ANCRE_TEXTE, anchor_color );
    }

    // Draw the text proper, with the right attributes
    wxSize size   = m_Size;
    double orient = GetDrawRotation();

    // If the text is mirrored : negate size.x (mirror / Y axis)
    if( m_Mirror )
        size.x = -size.x;

    DrawGraphicText( panel->GetClipBox(), DC, pos, color, GetShownText(), orient,
                     size, m_HJustify, m_VJustify, width, m_Italic, m_Bold );

    // Enable these line to draw the bounding box (debug test purpose only)
#if 0
    {
        EDA_RECT BoundaryBox = GetBoundingBox();
        GRRect( panel->GetClipBox(), DC, BoundaryBox, 0, BROWN );
    }
#endif
}

/* Draws a line from the TEXTE_MODULE origin to parent MODULE origin.
*/
void TEXTE_MODULE::DrawUmbilical( EDA_DRAW_PANEL* aPanel,
                                  wxDC*           aDC,
                                  GR_DRAWMODE     aDrawMode,
                                  const wxPoint&  aOffset )
{
    MODULE* parent = static_cast<MODULE*>( GetParent() );

    if( !parent )
        return;

    GRSetDrawMode( aDC, GR_XOR );
    GRLine( aPanel->GetClipBox(), aDC,
            parent->GetPosition(), GetTextPosition() + aOffset,
            0, UMBILICAL_COLOR);
}

/* Return text rotation for drawings and plotting
 */
double TEXTE_MODULE::GetDrawRotation() const
{
    MODULE* module = (MODULE*) m_Parent;
    double  rotation = m_Orient;

    if( module )
        rotation += module->GetOrientation();

    NORMALIZE_ANGLE_POS( rotation );

    // For angle = 0 .. 180 deg
    while( rotation > 900 )
        rotation -= 1800;

    return rotation;
}


// see class_text_mod.h
void TEXTE_MODULE::GetMsgPanelInfo( std::vector< MSG_PANEL_ITEM >& aList )
{
    MODULE* module = (MODULE*) m_Parent;

    if( module == NULL )        // Happens in modedit, and for new texts
        return;

    wxString msg, Line;

    static const wxString text_type_msg[3] =
    {
        _( "Ref." ), _( "Value" ), _( "Text" )
    };

    Line = module->GetReference();
    aList.push_back( MSG_PANEL_ITEM( _( "Footprint" ), Line, DARKCYAN ) );

    Line = GetShownText();
    aList.push_back( MSG_PANEL_ITEM( _( "Text" ), Line, BROWN ) );

    wxASSERT( m_Type >= TEXT_is_REFERENCE && m_Type <= TEXT_is_DIVERS );
    aList.push_back( MSG_PANEL_ITEM( _( "Type" ), text_type_msg[m_Type], DARKGREEN ) );

    if( m_NoShow )
        msg = _( "No" );
    else
        msg = _( "Yes" );

    aList.push_back( MSG_PANEL_ITEM( _( "Display" ), msg, DARKGREEN ) );

    // Display text layer
    aList.push_back( MSG_PANEL_ITEM( _( "Layer" ), GetLayerName(), DARKGREEN ) );

    if( m_Mirror )
        msg = _( " Yes" );
    else
        msg = _( " No" );

    aList.push_back( MSG_PANEL_ITEM( _( "Mirror" ), msg, DARKGREEN ) );

    msg.Printf( wxT( "%.1f" ), m_Orient / 10.0 );
    aList.push_back( MSG_PANEL_ITEM( _( "Angle" ), msg, DARKGREEN ) );

    msg = ::CoordinateToString( m_Thickness );
    aList.push_back( MSG_PANEL_ITEM( _( "Thickness" ), msg, DARKGREEN ) );

    msg = ::CoordinateToString( m_Size.x );
    aList.push_back( MSG_PANEL_ITEM( _( "Width" ), msg, RED ) );

    msg = ::CoordinateToString( m_Size.y );
    aList.push_back( MSG_PANEL_ITEM( _( "Height" ), msg, RED ) );
}


wxString TEXTE_MODULE::GetSelectMenuText() const
{
    wxString text;
    const wxChar *reference = GetChars( static_cast<MODULE*>( GetParent() )->GetReference() );

    switch( m_Type )
    {
    case TEXT_is_REFERENCE:
        text.Printf( _( "Reference %s" ), reference );
        break;

    case TEXT_is_VALUE:
        text.Printf( _( "Value %s of %s" ), GetChars( GetShownText() ), reference );
        break;

    default:    // wrap this one in quotes:
        text.Printf( _( "Text \"%s\" on %s of %s" ), GetChars( ShortenedShownText() ),
                     GetChars( GetLayerName() ), reference );
        break;
    }

    return text;
}


EDA_ITEM* TEXTE_MODULE::Clone() const
{
    return new TEXTE_MODULE( *this );
}


const BOX2I TEXTE_MODULE::ViewBBox() const
{
    double   angle = GetDrawRotation();
    EDA_RECT text_area = GetTextBox( -1, -1 );

    if( angle )
        text_area = text_area.GetBoundingBoxRotated( m_Pos, angle );

    return BOX2I( text_area.GetPosition(), text_area.GetSize() );
}


void TEXTE_MODULE::ViewGetLayers( int aLayers[], int& aCount ) const
{
    if( m_NoShow )      // Hidden text
        aLayers[0] = ITEM_GAL_LAYER( MOD_TEXT_INVISIBLE );
    //else if( IsFrontLayer( m_Layer ) )
        //aLayers[0] = ITEM_GAL_LAYER( MOD_TEXT_FR_VISIBLE );
    //else if( IsBackLayer( m_Layer ) )
        //aLayers[0] = ITEM_GAL_LAYER( MOD_TEXT_BK_VISIBLE );
    else
        aLayers[0] = GetLayer();

    aCount = 1;
}


unsigned int TEXTE_MODULE::ViewGetLOD( int aLayer ) const
{
    const int MAX = std::numeric_limits<unsigned int>::max();

    if( !m_view )
        return 0;

    if( m_Type == TEXT_is_VALUE && !m_view->IsLayerVisible( ITEM_GAL_LAYER( MOD_VALUES_VISIBLE ) ) )
        return MAX;

    if( m_Type == TEXT_is_REFERENCE && !m_view->IsLayerVisible( ITEM_GAL_LAYER( MOD_REFERENCES_VISIBLE ) ) )
        return MAX;

    if( IsFrontLayer( m_Layer ) && ( !m_view->IsLayerVisible( ITEM_GAL_LAYER( MOD_TEXT_FR_VISIBLE ) ) ||
                                     !m_view->IsLayerVisible( ITEM_GAL_LAYER( MOD_FR_VISIBLE ) ) ) )
        return MAX;

    if( IsBackLayer( m_Layer ) && ( !m_view->IsLayerVisible( ITEM_GAL_LAYER( MOD_TEXT_BK_VISIBLE ) ) ||
                                    !m_view->IsLayerVisible( ITEM_GAL_LAYER( MOD_BK_VISIBLE ) ) ) )
        return MAX;

    return 0;
}


/**
 * Macro-expansion for text in library modules
 */
wxString TEXTE_MODULE::GetShownText() const
{
    /* First order optimization: no % means that no processing is
     * needed; just hope that RVO and copy constructor implementation
     * avoid to copy the whole block; anyway it should be better than
     * rebuild the string one character at a time...
     * Also it seems wise to only expand macros in user text (but there
     * is no technical reason, probably) */

    if( (m_Type != TEXT_is_DIVERS) || (wxString::npos == m_Text.find('%')) )
        return m_Text;

    wxString newbuf;
    const MODULE *module = static_cast<MODULE*>( GetParent() );

    for( wxString::const_iterator it = m_Text.begin();
            it != m_Text.end(); ++it )
    {
        // Process '%' and copy everything else
        if( *it != '%' )
            newbuf.append(*it);
        else
        {
            /* Look at the next character (if is it there) and append
             * its expansion */
            ++it;
            if( it != m_Text.end() )
            {
                switch( char(*it) )
                {
                case '%':
                    newbuf.append( '%' );
                    break;

                case 'R':
                    if( module )
                        newbuf.append( module->GetReference() );
                    break;

                case 'V':
                    if( module )
                        newbuf.append( module->GetValue() );
                    break;

                default:
                    newbuf.append( '?' );
                    break;
                }
            }
            else
                break; // The string is over and we can't ++ anymore
        }
    }
    return newbuf;
}


