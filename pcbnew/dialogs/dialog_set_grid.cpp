/**
 * @file dialog_set_grid.cpp
 * @brief Manage user grid.
 */
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

#include <fctsys.h>
#include <macros.h>
#include <common.h>
#include <base_units.h>
#include <macros.h>

#include <pcbnew.h>
#include <pcbnew_id.h>
#include <dialog_set_grid_base.h>
#include <invoke_pcb_dialog.h>

#include <gal/graphics_abstraction_layer.h>
#include <class_draw_panel_gal.h>
#include <tool/tool_manager.h>
#include <tools/common_actions.h>

// Max values for grid size
#define MAX_GRID_SIZE ( 50.0 * IU_PER_MM )
#define MIN_GRID_SIZE ( 0.001 * IU_PER_MM )

// Min/Max value for grid offset
#define MAX_GRID_OFFSET ((double)INT_MAX/2)

class DIALOG_SET_GRID : public DIALOG_SET_GRID_BASE
{
public:

    /// This has no dependencies on calling wxFrame derivative, such as PCB_BASE_FRAME.
    DIALOG_SET_GRID( wxFrame* aCaller, EDA_UNITS_T* aGridUnits, EDA_UNITS_T aBoardUnits,
        wxRealPoint* aUserSize, wxPoint* aOrigin,
        int* aFastGrid1, int* aFastGrid2, const wxArrayString& aGridChoices );

private:
    void            OnResetGridOrgClick( wxCommandEvent& event );
    void            OnCancelClick( wxCommandEvent& event );
    void            OnOkClick( wxCommandEvent& event );
    void OnInitDlg( wxInitDialogEvent& event )
    {
        // Call the default wxDialog handler of a wxInitDialogEvent
        TransferDataToWindow();

        // Now all widgets have the size fixed, call FinishDialogSettings
        FinishDialogSettings();
    }

    EDA_UNITS_T&    m_callers_grid_units;
    EDA_UNITS_T     m_callers_board_units;
    wxRealPoint&    m_callers_user_size;
    wxPoint&        m_callers_origin;
    int&            m_callers_fast_grid1;
    int&            m_callers_fast_grid2;

    void            setGridUnits( EDA_UNITS_T units );
    EDA_UNITS_T     getGridUnits();

    void            setGridSize( const wxRealPoint& grid );
    bool            getGridSize( wxRealPoint& aGrisSize );

    void            setGridOrigin( const wxPoint& grid );
    bool            getGridOrigin( wxPoint& aGridOrigin );

    void            setGridForFastSwitching( const wxArrayString& aGrids, int aGrid1, int aGrid2 );
    void            getGridForFastSwitching( int& aGrid1, int& aGrid2 );
};


DIALOG_SET_GRID::DIALOG_SET_GRID( wxFrame* aCaller, EDA_UNITS_T* aGridUnits, EDA_UNITS_T aBoardUnits,
        wxRealPoint* aUserSize, wxPoint* aOrigin, int* aFastGrid1, int* aFastGrid2, const wxArrayString& aGridChoices ):
    DIALOG_SET_GRID_BASE( aCaller ),
    m_callers_grid_units( *aGridUnits ),
    m_callers_board_units( aBoardUnits ),
    m_callers_user_size( *aUserSize ),
    m_callers_origin( *aOrigin ),
    m_callers_fast_grid1( *aFastGrid1 ),
    m_callers_fast_grid2( *aFastGrid2 )
{
    m_TextPosXUnits->SetLabel( GetUnitsLabel( m_callers_board_units ) );
    m_TextPosYUnits->SetLabel( GetUnitsLabel( m_callers_board_units ) );

    m_sdbSizerOK->SetDefault();      // set OK button as default response to 'Enter' key

    setGridUnits( m_callers_grid_units );
    setGridSize( m_callers_user_size );
    setGridOrigin( m_callers_origin );
    setGridForFastSwitching( aGridChoices, m_callers_fast_grid1, m_callers_fast_grid2 );

    FixOSXCancelButtonIssue();
}


void DIALOG_SET_GRID::setGridUnits( EDA_UNITS_T aUnits )
{
    m_UnitGrid->SetSelection( aUnits != INCHES  );
}


EDA_UNITS_T DIALOG_SET_GRID::getGridUnits()
{
    return m_UnitGrid->GetSelection() == 0 ? INCHES : MILLIMETRES;
}


void DIALOG_SET_GRID::setGridSize( const wxRealPoint& grid )
{
    wxString msg;

    msg.Printf( wxT( "%.10g" ), grid.x );
    m_OptGridSizeX->SetValue( msg );

    msg.Printf( wxT( "%.10g" ), grid.y );
    m_OptGridSizeY->SetValue( msg );
}


bool DIALOG_SET_GRID::getGridSize( wxRealPoint& aGrisSize )
{
    wxRealPoint grid;
    m_callers_grid_units = getGridUnits();
    double grid_unit_to_iu = m_callers_grid_units == INCHES ? IU_PER_MILS*1000 : IU_PER_MM;

    wxString val = m_OptGridSizeX->GetValue();

    double tmp;

    if( !val.ToDouble( &tmp ) ||
        tmp*grid_unit_to_iu < MIN_GRID_SIZE || tmp*grid_unit_to_iu > MAX_GRID_SIZE )
    {
        return false;
    }
    else
        aGrisSize.x = tmp;

    val = m_OptGridSizeY->GetValue();

    if( !val.ToDouble( &tmp ) ||
        tmp*grid_unit_to_iu < MIN_GRID_SIZE || tmp*grid_unit_to_iu > MAX_GRID_SIZE )
    {
        return false;
    }
    else
        aGrisSize.y = tmp;

    return true;
}


bool DIALOG_SET_GRID::getGridOrigin( wxPoint& aGridOrigin )
{
    double tmp;

    tmp = DoubleValueFromString( g_UserUnit, m_GridOriginXCtrl->GetValue() );

    // Some error checking here is a good thing.
    if( tmp < -MAX_GRID_OFFSET || tmp > MAX_GRID_OFFSET )
        return false;

    aGridOrigin.x = KiROUND( tmp );

    tmp = DoubleValueFromString( g_UserUnit, m_GridOriginYCtrl->GetValue() );

    if( tmp < -MAX_GRID_OFFSET || tmp > MAX_GRID_OFFSET )
        return false;

    aGridOrigin.y = KiROUND( tmp );

    return true;
}


void DIALOG_SET_GRID::setGridOrigin( const wxPoint& grid )
{
    PutValueInLocalUnits( *m_GridOriginXCtrl, grid.x );
    PutValueInLocalUnits( *m_GridOriginYCtrl, grid.y );
}


void DIALOG_SET_GRID::setGridForFastSwitching( const wxArrayString& aGrids, int aGrid1, int aGrid2 )
{
    m_comboBoxGrid1->Append( aGrids );
    m_comboBoxGrid2->Append( aGrids );

    m_comboBoxGrid1->SetSelection( aGrid1 );
    m_comboBoxGrid2->SetSelection( aGrid2 );
}


void DIALOG_SET_GRID::getGridForFastSwitching( int& aGrid1, int& aGrid2 )
{
    aGrid1 = m_comboBoxGrid1->GetSelection();
    aGrid2 = m_comboBoxGrid2->GetSelection();
}


void DIALOG_SET_GRID::OnResetGridOrgClick( wxCommandEvent& event )
{
    setGridOrigin( wxPoint( 0, 0 ) );
}


void DIALOG_SET_GRID::OnCancelClick( wxCommandEvent& event )
{
    EndModal( wxID_CANCEL );
}


void DIALOG_SET_GRID::OnOkClick( wxCommandEvent& event )
{
    bool success  = getGridSize( m_callers_user_size );

    if( !success )
    {
        wxMessageBox( wxString::Format( _( "Incorrect grid size (size must be >= %.3f mm and <= %.3f mm)"),
            MIN_GRID_SIZE/IU_PER_MM, MAX_GRID_SIZE/IU_PER_MM ) );
        return;
    }

    success = getGridOrigin( m_callers_origin );

    if( !success )
    {
        wxMessageBox( wxString::Format( _( "Incorrect grid origin (coordinates must be >= %.3f mm and <= %.3f mm)" ),
            -MAX_GRID_OFFSET/IU_PER_MM, MAX_GRID_OFFSET/IU_PER_MM ) );
        return;
    }

    getGridForFastSwitching( m_callers_fast_grid1, m_callers_fast_grid2 );

    EndModal( wxID_OK );
}


#include <class_drawpanel.h>
#include <wxBasePcbFrame.h>

bool PCB_BASE_FRAME::InvokeDialogGrid()
{
    wxPoint grid_origin = GetGridOrigin();

    DIALOG_SET_GRID dlg( this, &m_UserGridUnit, g_UserUnit, &m_UserGridSize,
        &grid_origin, &m_FastGrid1, &m_FastGrid2,
        m_gridSelectBox->GetStrings() );

    int ret = dlg.ShowModal();

    if( ret == wxID_OK )
    {
        if( GetGridOrigin() != grid_origin && IsType( FRAME_PCB ) )
            OnModify();     // because grid origin is saved in board, show as modified

        SetGridOrigin( grid_origin );

        BASE_SCREEN* screen = GetScreen();

        screen->AddGrid( m_UserGridSize, m_UserGridUnit, ID_POPUP_GRID_USER );

        // If the user grid is the current option, recall SetGrid()
        // to force new values put in list as current grid value
        if( screen->GetGridCmdId() == ID_POPUP_GRID_USER )
            screen->SetGrid( ID_POPUP_GRID_USER );

        // Notify GAL
        TOOL_MANAGER* mgr = GetToolManager();

        if( mgr && IsGalCanvasActive() )
        {
            mgr->RunAction( "common.Control.gridPreset", true,
                    screen->GetGridCmdId() - ID_POPUP_GRID_LEVEL_1000 );

            TOOL_EVENT gridOriginUpdate = COMMON_ACTIONS::gridSetOrigin.MakeEvent();
            gridOriginUpdate.SetParameter( new VECTOR2D( grid_origin ) );
            mgr->ProcessEvent( gridOriginUpdate );
        }

        m_canvas->Refresh();

        return true;
    }

    return false;
}
