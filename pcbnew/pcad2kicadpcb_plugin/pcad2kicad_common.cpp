/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2007, 2008 Lubo Racko <developer@lura.sk>
 * Copyright (C) 2008, 2012 Alexander Lunev <al.lunev@yahoo.com>
 * Copyright (C) 2012 KiCad Developers, see CHANGELOG.TXT for contributors.
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
 * @file pcad2kicad_common.cpp
 */

#include <wx/wx.h>
#include <wx/config.h>

#include <common.h>
#include <convert_to_biu.h>

#include <pcad2kicad_common.h>

namespace PCAD2KICAD {

// PCAD stroke font average ratio of width to height
const double TEXT_WIDTH_TO_HEIGHT = 0.79;

wxString GetWord( wxString* aStr )
{
    wxString result = wxEmptyString;

    *aStr = aStr->Trim( false );

    if( aStr->Len() == 0 )
        return result;

    if( (*aStr)[0] == wxT( '"' ) )
    {
        result  += (*aStr)[0];
        *aStr   = aStr->Mid( 1 ); // remove Frot apostrofe

        while( aStr->Len() > 0 && (*aStr)[0] != wxT( '"' ) )
        {
            result  += (*aStr)[0];
            *aStr   = aStr->Mid( 1 );
        }

        if( aStr->Len() > 0 && (*aStr)[0] == wxT( '"' ) )
        {
            result  += (*aStr)[0];
            *aStr   = aStr->Mid( 1 ); // remove ending apostrophe
        }
    }
    else
    {
        while( aStr->Len() > 0
               && !( (*aStr)[0] == wxT( ' ' )
                     || (*aStr)[0] == wxT( '(' )
                     || (*aStr)[0] == wxT( ')' ) ) )
        {
            result  += (*aStr)[0];
            *aStr   = aStr->Mid( 1 );
        }
    }

    result.Trim( true );
    result.Trim( false );

    return result;
}


XNODE* FindPinMap( XNODE* aNode )
{
    XNODE* result, * lNode;

    result  = NULL;
    lNode   = FindNode( aNode, wxT( "attachedPattern" ) );

    if( lNode )
        result = FindNode( lNode, wxT( "padPinMap" ) );

    return result;
}


double StrToDoublePrecisionUnits( wxString aStr, char aAxe, wxString aActualConversion )
{
    wxString    ls;
    double      i;
    char        u;

    ls = aStr;
    ls.Trim( true );
    ls.Trim( false );

    if( ls.Len() > 0 )
    {
        u = ls[ls.Len() - 1];

        while( ls.Len() > 0
               && !( ls[ls.Len() - 1] == wxT( '.' )
                     || ls[ls.Len() - 1] == wxT( ',' )
                     || (ls[ls.Len() - 1] >= wxT( '0' ) && ls[ls.Len() - 1] <= wxT( '9' ) ) ) )
        {
            ls = ls.Left( ls.Len() - 1 );
        }

        while( ls.Len() > 0
               && !( ls[0] == wxT( '-' )
                     || ls[0] == wxT( '+' )
                     || ls[0] == wxT( '.' )
                     || ls[0] == wxT( ',' )
                     || (ls[0] >= wxT( '0' ) && ls[0] <= wxT( '9' ) ) ) )
        {
            ls = ls.Mid( 1 );
        }

        if( u == wxT( 'm' ) )
        {
            ls.ToDouble( &i );
#ifdef PCAD2KICAD_SCALE_SCH_TO_INCH_GRID
            if( aActualConversion == wxT( "SCH" )
                || aActualConversion == wxT( "SCHLIB" ) )
                i = i * (0.0254 / 0.025);
#endif
            i = Millimeter2iu( i );
        }
        else
        {
            ls.ToDouble( &i );
            i = Mils2iu( i );
        }
    }
    else
        i = 0.0;

    if( ( aActualConversion == wxT( "PCB" ) || aActualConversion == wxT( "SCH" ) )
        && aAxe == wxT( 'Y' ) )
        return -i;
    else
        return i; // Y axe is mirrored compared to P-Cad
}


int StrToIntUnits( wxString aStr, char aAxe, wxString aActualConversion )
{
    return KiROUND( StrToDoublePrecisionUnits( aStr, aAxe, aActualConversion ) );
}


wxString GetAndCutWordWithMeasureUnits( wxString* aStr, wxString aDefaultMeasurementUnit )
{
    wxString result;

    aStr->Trim( false );
    result = wxEmptyString;

    // value
    while( aStr->Len() > 0 && (*aStr)[0] != wxT( ' ' ) )
    {
        result  += (*aStr)[0];
        *aStr   = aStr->Mid( 1 );
    }

    aStr->Trim( false );

    // if there is also measurement unit
    while( aStr->Len() > 0
           && ( ( (*aStr)[0] >= wxT( 'a' ) && (*aStr)[0] <= wxT( 'z' ) )
                || ( (*aStr)[0] >= wxT( 'A' ) && (*aStr)[0] <= wxT( 'Z' ) ) ) )
    {
        result  += (*aStr)[0];
        *aStr   = aStr->Mid( 1 );
    }

    // and if not, add default....
    if( result.Len() > 0
        && ( result[result.Len() - 1] == wxT( '.' )
             || result[result.Len() - 1] == wxT( ',' )
             || (result[result.Len() - 1] >= wxT( '0' )
                 && result[result.Len() - 1] <= wxT( '9' ) ) ) )
    {
        result += aDefaultMeasurementUnit;
    }

    return result;
}


int StrToInt1Units( wxString aStr )
{
    double num, precision = 10;

    // TODO: Is the following commented string necessary?
    // if (pos(',',s)>0) then DecimalSeparator:=',' else DecimalSeparator:='.';
    aStr.ToDouble( &num );
    return KiROUND( num * precision );
}


wxString ValidateName( wxString aName )
{
    aName.Replace( wxT( " " ), wxT( "_" ) );

    return aName;
}


void SetWidth( wxString aStr,
               wxString aDefaultMeasurementUnit,
               int*     aWidth,
               wxString aActualConversion )
{
    *aWidth = StrToIntUnits( GetAndCutWordWithMeasureUnits( &aStr,
                                                            aDefaultMeasurementUnit ), wxT( ' ' ),
                             aActualConversion );
}


void SetHeight( wxString    aStr,
                wxString    aDefaultMeasurementUnit,
                int*        aHeight,
                wxString    aActualConversion )
{
    *aHeight = StrToIntUnits( GetAndCutWordWithMeasureUnits( &aStr,
                                                             aDefaultMeasurementUnit ), wxT( ' ' ),
                              aActualConversion );
}


void SetPosition( wxString  aStr,
                  wxString  aDefaultMeasurementUnit,
                  int*      aX,
                  int*      aY,
                  wxString  aActualConversion )
{
    *aX = StrToIntUnits( GetAndCutWordWithMeasureUnits( &aStr,
                                                        aDefaultMeasurementUnit ), wxT( 'X' ),
                         aActualConversion );
    *aY = StrToIntUnits( GetAndCutWordWithMeasureUnits( &aStr,
                                                        aDefaultMeasurementUnit ), wxT( 'Y' ),
                         aActualConversion );
}


void SetDoublePrecisionPosition( wxString   aStr,
                                 wxString   aDefaultMeasurementUnit,
                                 double*    aX,
                                 double*    aY,
                                 wxString   aActualConversion )
{
    *aX = StrToDoublePrecisionUnits( GetAndCutWordWithMeasureUnits( &aStr,
                                                                    aDefaultMeasurementUnit ), wxT( 'X' ),
                                     aActualConversion );
    *aY = StrToDoublePrecisionUnits( GetAndCutWordWithMeasureUnits( &aStr,
                                                                    aDefaultMeasurementUnit ), wxT( 'Y' ),
                                     aActualConversion );
}

TTEXT_JUSTIFY GetJustifyIdentificator( wxString aJustify )
{
    TTEXT_JUSTIFY id;

    if( aJustify == wxT( "LowerCenter" ) )
        id = LowerCenter;
    else if( aJustify == wxT( "LowerRight" ) )
        id = LowerRight;
    else if( aJustify == wxT( "UpperLeft" ) )
        id = UpperLeft;
    else if( aJustify == wxT( "UpperCenter" ) )
        id = UpperCenter;
    else if( aJustify == wxT( "UpperRight" ) )
        id = UpperRight;
    else if( aJustify == wxT( "Left" ) )
        id = Left;
    else if( aJustify == wxT( "Center" ) )
        id = Center;
    else if( aJustify == wxT( "Right" ) )
        id = Right;
    else
        id = LowerLeft;

    return id;
}

void SetTextParameters( XNODE*      aNode,
                        TTEXTVALUE* aTextValue,
                        wxString    aDefaultMeasurementUnit,
                        wxString    aActualConversion )
{
    XNODE*      tNode;
    wxString    str;

    tNode = FindNode( aNode, wxT( "pt" ) );

    if( tNode )
        SetPosition( tNode->GetNodeContent(),
                     aDefaultMeasurementUnit,
                     &aTextValue->textPositionX,
                     &aTextValue->textPositionY,
                     aActualConversion );

    tNode = FindNode( aNode, wxT( "rotation" ) );

    if( tNode )
    {
        str = tNode->GetNodeContent();
        str.Trim( false );
        aTextValue->textRotation = StrToInt1Units( str );
    }

    str = FindNodeGetContent( aNode, wxT( "isVisible" ) );

    if( str == wxT( "True" ) )
        aTextValue->textIsVisible = 1;
    else if( str == wxT( "False" ) )
        aTextValue->textIsVisible = 0;

    str = FindNodeGetContent( aNode, wxT( "justify" ) );
    aTextValue->justify = GetJustifyIdentificator( str );

    str = FindNodeGetContent( aNode, wxT( "isFlipped" ) );

    if( str == wxT( "True" ) )
        aTextValue->mirror = 1;

    tNode = FindNode( aNode, wxT( "textStyleRef" ) );

    if( tNode )
        SetFontProperty( tNode, aTextValue, aDefaultMeasurementUnit, aActualConversion );
}


void SetFontProperty( XNODE*        aNode,
                      TTEXTVALUE*   aTextValue,
                      wxString      aDefaultMeasurementUnit,
                      wxString      aActualConversion )
{
    wxString n, propValue;

    aNode->GetAttribute( wxT( "Name" ), &n );

    while( aNode->GetName() != wxT( "www.lura.sk" ) )
        aNode = aNode->GetParent();

    aNode = FindNode( aNode, wxT( "library" ) );

    if( aNode )
        aNode = FindNode( aNode, wxT( "textStyleDef" ) );

    if( aNode )
    {
        while( true )
        {
            aNode->GetAttribute( wxT( "Name" ), &propValue );
            propValue.Trim( false );
            propValue.Trim( true );

            if( propValue == n )
                break;

            aNode = aNode->GetNext();
        }

        if( aNode )
        {
            aNode = FindNode( aNode, wxT( "font" ) );

            if( aNode )
            {
                if( FindNode( aNode, wxT( "fontHeight" ) ) )
                    // // SetWidth(iNode.ChildNodes.FindNode('fontHeight').Text,
                    // //          DefaultMeasurementUnit,tv.TextHeight);
                    // Fixed By Lubo, 02/2008
                    SetHeight( FindNode( aNode, wxT(
                                             "fontHeight" ) )->GetNodeContent(),
                               aDefaultMeasurementUnit, &aTextValue->textHeight,
                               aActualConversion );

                if( FindNode( aNode, wxT( "strokeWidth" ) ) )
                    SetWidth( FindNode( aNode, wxT(
                                            "strokeWidth" ) )->GetNodeContent(),
                              aDefaultMeasurementUnit, &aTextValue->textstrokeWidth,
                              aActualConversion );
            }
        }
    }
}

int CalculateTextLengthSize( TTEXTVALUE* aText )
{
    return KiROUND( (double) aText->text.Len() *
                    (double) aText->textHeight * TEXT_WIDTH_TO_HEIGHT );
}

void CorrectTextPosition( TTEXTVALUE* aValue )
{
    int cm = aValue->mirror ? -1 : 1;
    // sizes of justify correction
    int cl = KiROUND( (double) CalculateTextLengthSize( aValue ) / 2.0 );
    int ch = KiROUND( (double) aValue->textHeight / 2.0 );

    aValue->correctedPositionX = aValue->textPositionX;
    aValue->correctedPositionY = aValue->textPositionY;

    switch( aValue->textRotation )
    {
    case 0:
        if( aValue->justify == LowerLeft ||
            aValue->justify == Left ||
            aValue->justify == UpperLeft )
            aValue->correctedPositionX += cl * cm;
        else if( aValue->justify == LowerRight ||
                 aValue->justify == Right ||
                 aValue->justify == UpperRight )
            aValue->correctedPositionX -= cl * cm;

        if( aValue->justify == LowerLeft ||
            aValue->justify == LowerCenter ||
            aValue->justify == LowerRight )
            aValue->correctedPositionY -= ch;
        else if( aValue->justify == UpperLeft ||
                 aValue->justify == UpperCenter ||
                 aValue->justify == UpperRight )
            aValue->correctedPositionY += ch;
        break;
    case 900:
        if( aValue->justify == LowerLeft ||
            aValue->justify == LowerCenter ||
            aValue->justify == LowerRight )
            aValue->correctedPositionX -= ch * cm;
        else if( aValue->justify == UpperLeft ||
                 aValue->justify == UpperCenter ||
                 aValue->justify == UpperRight )
            aValue->correctedPositionX += ch * cm;

        if( aValue->justify == LowerLeft ||
            aValue->justify == Left ||
            aValue->justify == UpperLeft )
            aValue->correctedPositionY -= cl;
        else if( aValue->justify == LowerRight ||
                 aValue->justify == Right ||
                 aValue->justify == UpperRight )
            aValue->correctedPositionY += cl;
        break;
    case 1800:
        if( aValue->justify == LowerLeft ||
            aValue->justify == Left ||
            aValue->justify == UpperLeft )
            aValue->correctedPositionX -= cl * cm;
        else if( aValue->justify == LowerRight ||
                 aValue->justify == Right ||
                 aValue->justify == UpperRight )
            aValue->correctedPositionX += cl * cm;

        if( aValue->justify == LowerLeft ||
            aValue->justify == LowerCenter ||
            aValue->justify == LowerRight )
            aValue->correctedPositionY += ch;
        else if( aValue->justify == UpperLeft ||
                 aValue->justify == UpperCenter ||
                 aValue->justify == UpperRight )
            aValue->correctedPositionY -= ch;
        break;
    case 2700:
        if( aValue->justify == LowerLeft ||
            aValue->justify == LowerCenter ||
            aValue->justify == LowerRight )
            aValue->correctedPositionX += ch * cm;
        else if( aValue->justify == UpperLeft ||
                 aValue->justify == UpperCenter ||
                 aValue->justify == UpperRight )
            aValue->correctedPositionX -= ch * cm;

        if( aValue->justify == LowerLeft ||
            aValue->justify == Left ||
            aValue->justify == UpperLeft )
            aValue->correctedPositionY += cl;
        else if( aValue->justify == LowerRight ||
                 aValue->justify == Right ||
                 aValue->justify == UpperRight )
            aValue->correctedPositionY -= cl;
        break;
    default:
        break;
    }
}


XNODE* FindNode( XNODE* aChild, wxString aTag )
{
    aChild = aChild->GetChildren();

    while( aChild )
    {
        if( aChild->GetName() == aTag )
            return aChild;

        aChild = aChild->GetNext();
    }

    return NULL;
}

wxString FindNodeGetContent( XNODE* aChild, wxString aTag )
{
    wxString str = wxEmptyString;

    aChild = FindNode( aChild, aTag );

    if( aChild )
    {
        str = aChild->GetNodeContent();
        str.Trim( false );
        str.Trim( true );
    }

    return str;
}

void InitTTextValue( TTEXTVALUE* aTextValue )
{
    aTextValue->text = wxEmptyString;
    aTextValue->textPositionX   = 0;
    aTextValue->textPositionY   = 0;
    aTextValue->textRotation    = 0;
    aTextValue->textHeight      = 0;
    aTextValue->textstrokeWidth = 0;
    aTextValue->textIsVisible   = 0;
    aTextValue->mirror      = 0;
    aTextValue->textUnit    = 0;
    aTextValue->correctedPositionX  = 0;
    aTextValue->correctedPositionY  = 0;
    aTextValue->justify = LowerLeft;
}

} // namespace PCAD2KICAD
