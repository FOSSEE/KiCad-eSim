///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 10 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "dialog_global_edit_tracks_and_vias_base.h"

///////////////////////////////////////////////////////////////////////////

DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : DIALOG_SHIM( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bMainSizer;
	bMainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerCurrSettings;
	bSizerCurrSettings = new wxBoxSizer( wxVERTICAL );
	
	m_staticText12 = new wxStaticText( this, wxID_ANY, _("Current Settings:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	m_staticText12->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizerCurrSettings->Add( m_staticText12, 0, wxALL, 5 );
	
	wxBoxSizer* bSizerCurrSettingsLeft;
	bSizerCurrSettingsLeft = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizerCurrSettingsLeft->Add( 20, 0, 0, 0, 5 );
	
	wxBoxSizer* bSizerGrids;
	bSizerGrids = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizeNetInfo;
	fgSizeNetInfo = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizeNetInfo->SetFlexibleDirection( wxBOTH );
	fgSizeNetInfo->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_CurrentNetText = new wxStaticText( this, wxID_ANY, _("Current Net:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_CurrentNetText->Wrap( -1 );
	fgSizeNetInfo->Add( m_CurrentNetText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_CurrentNetName = new wxStaticText( this, wxID_ANY, _("NetName"), wxDefaultPosition, wxDefaultSize, 0 );
	m_CurrentNetName->Wrap( -1 );
	m_CurrentNetName->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	fgSizeNetInfo->Add( m_CurrentNetName, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_CurrentNetclassText = new wxStaticText( this, wxID_ANY, _("Current NetClass:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_CurrentNetclassText->Wrap( -1 );
	fgSizeNetInfo->Add( m_CurrentNetclassText, 0, wxALL, 5 );
	
	m_CurrentNetclassName = new wxStaticText( this, wxID_ANY, _("NetClassName"), wxDefaultPosition, wxDefaultSize, 0 );
	m_CurrentNetclassName->Wrap( -1 );
	m_CurrentNetclassName->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	fgSizeNetInfo->Add( m_CurrentNetclassName, 0, wxALL, 5 );
	
	
	bSizerGrids->Add( fgSizeNetInfo, 0, wxEXPAND, 5 );
	
	
	bSizerGrids->Add( 10, 10, 0, 0, 5 );
	
	m_gridDisplayCurrentSettings = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_gridDisplayCurrentSettings->CreateGrid( 2, 5 );
	m_gridDisplayCurrentSettings->EnableEditing( true );
	m_gridDisplayCurrentSettings->EnableGridLines( true );
	m_gridDisplayCurrentSettings->EnableDragGridSize( true );
	m_gridDisplayCurrentSettings->SetMargins( 10, 0 );
	
	// Columns
	m_gridDisplayCurrentSettings->AutoSizeColumns();
	m_gridDisplayCurrentSettings->EnableDragColMove( false );
	m_gridDisplayCurrentSettings->EnableDragColSize( true );
	m_gridDisplayCurrentSettings->SetColLabelSize( 30 );
	m_gridDisplayCurrentSettings->SetColLabelValue( 0, _("Track size") );
	m_gridDisplayCurrentSettings->SetColLabelValue( 1, _("Via diameter") );
	m_gridDisplayCurrentSettings->SetColLabelValue( 2, _("Via drill") );
	m_gridDisplayCurrentSettings->SetColLabelValue( 3, _("uVia size") );
	m_gridDisplayCurrentSettings->SetColLabelValue( 4, _("uVia Drill") );
	m_gridDisplayCurrentSettings->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gridDisplayCurrentSettings->AutoSizeRows();
	m_gridDisplayCurrentSettings->EnableDragRowSize( true );
	m_gridDisplayCurrentSettings->SetRowLabelSize( 100 );
	m_gridDisplayCurrentSettings->SetRowLabelValue( 0, _("Netclass value") );
	m_gridDisplayCurrentSettings->SetRowLabelValue( 1, _("Current value") );
	m_gridDisplayCurrentSettings->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gridDisplayCurrentSettings->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	bSizerGrids->Add( m_gridDisplayCurrentSettings, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizerCurrSettingsLeft->Add( bSizerGrids, 1, wxEXPAND, 5 );
	
	
	bSizerCurrSettings->Add( bSizerCurrSettingsLeft, 1, wxEXPAND, 5 );
	
	
	bMainSizer->Add( bSizerCurrSettings, 0, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bMainSizer->Add( m_staticline1, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bLowerSizer;
	bLowerSizer = new wxBoxSizer( wxVERTICAL );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, _("Global Edition Option:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	m_staticText11->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bLowerSizer->Add( m_staticText11, 0, wxALL, 5 );
	
	
	bMainSizer->Add( bLowerSizer, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizerOptions;
	bSizerOptions = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizerOptions->Add( 20, 0, 0, 0, 5 );
	
	wxBoxSizer* bSizerRadioButtons;
	bSizerRadioButtons = new wxBoxSizer( wxVERTICAL );
	
	m_Net2CurrValueButton = new wxRadioButton( this, ID_CURRENT_VALUES_TO_CURRENT_NET, _("Set tracks and vias of the current Net to the current value"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_Net2CurrValueButton->SetValue( true ); 
	bSizerRadioButtons->Add( m_Net2CurrValueButton, 0, wxALL, 5 );
	
	m_NetUseNetclassValueButton = new wxRadioButton( this, ID_NETCLASS_VALUES_TO_CURRENT_NET, _("Set tracks and vias of the current Net to the Netclass value"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerRadioButtons->Add( m_NetUseNetclassValueButton, 0, wxALL, 5 );
	
	m_radioBtn3 = new wxRadioButton( this, ID_ALL_TRACKS_VIAS, _("Set all tracks and vias to their Netclass value"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerRadioButtons->Add( m_radioBtn3, 0, wxALL, 5 );
	
	m_radioBtn4 = new wxRadioButton( this, ID_ALL_VIAS, _("Set all vias (no track) to their Netclass value"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerRadioButtons->Add( m_radioBtn4, 0, wxALL, 5 );
	
	m_radioBtn5 = new wxRadioButton( this, ID_ALL_TRACKS, _("Set all tracks (no via) to their Netclass value"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerRadioButtons->Add( m_radioBtn5, 0, wxALL, 5 );
	
	
	bSizerOptions->Add( bSizerRadioButtons, 1, wxEXPAND, 5 );
	
	
	bMainSizer->Add( bSizerOptions, 0, wxEXPAND, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	
	bMainSizer->Add( m_sdbSizer1, 0, wxEXPAND|wxALL, 5 );
	
	
	this->SetSizer( bMainSizer );
	this->Layout();
	
	// Connect Events
	m_Net2CurrValueButton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::OnSelectionClick ), NULL, this );
	m_NetUseNetclassValueButton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::OnSelectionClick ), NULL, this );
	m_radioBtn3->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::OnSelectionClick ), NULL, this );
	m_radioBtn4->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::OnSelectionClick ), NULL, this );
	m_radioBtn5->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::OnSelectionClick ), NULL, this );
	m_sdbSizer1Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::OnCancelClick ), NULL, this );
	m_sdbSizer1OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::OnOkClick ), NULL, this );
}

DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::~DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE()
{
	// Disconnect Events
	m_Net2CurrValueButton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::OnSelectionClick ), NULL, this );
	m_NetUseNetclassValueButton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::OnSelectionClick ), NULL, this );
	m_radioBtn3->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::OnSelectionClick ), NULL, this );
	m_radioBtn4->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::OnSelectionClick ), NULL, this );
	m_radioBtn5->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::OnSelectionClick ), NULL, this );
	m_sdbSizer1Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::OnCancelClick ), NULL, this );
	m_sdbSizer1OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS_BASE::OnOkClick ), NULL, this );
	
}
