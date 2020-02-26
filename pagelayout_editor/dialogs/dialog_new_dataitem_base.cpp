///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "dialog_new_dataitem_base.h"

///////////////////////////////////////////////////////////////////////////

DIALOG_NEW_DATAITEM_BASE::DIALOG_NEW_DATAITEM_BASE( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : DIALOG_SHIM( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerUpper;
	bSizerUpper = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerPos;
	bSizerPos = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizerPosXY;
	bSizerPosXY = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextPosX = new wxStaticText( this, wxID_ANY, _("Pos X (mm)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPosX->Wrap( -1 );
	bSizerPosXY->Add( m_staticTextPosX, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_textCtrlPosX = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPosXY->Add( m_textCtrlPosX, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_staticTextPosY = new wxStaticText( this, wxID_ANY, _("Pos Y (mm)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPosY->Wrap( -1 );
	bSizerPosXY->Add( m_staticTextPosY, 0, wxRIGHT|wxLEFT, 5 );
	
	m_textCtrlPosY = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPosXY->Add( m_textCtrlPosY, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizerPos->Add( bSizerPosXY, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextOrgPos = new wxStaticText( this, wxID_ANY, _("Origin"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOrgPos->Wrap( -1 );
	bSizer6->Add( m_staticTextOrgPos, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxString m_choiceCornerPosChoices[] = { _("Upper Right"), _("Upper Left"), _("Lower Right"), _("Lower Left") };
	int m_choiceCornerPosNChoices = sizeof( m_choiceCornerPosChoices ) / sizeof( wxString );
	m_choiceCornerPos = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceCornerPosNChoices, m_choiceCornerPosChoices, 0 );
	m_choiceCornerPos->SetSelection( 2 );
	bSizer6->Add( m_choiceCornerPos, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizerPos->Add( bSizer6, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizerUpper->Add( bSizerPos, 1, wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizerUpper->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	m_SizerEndPosition = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizerEndXY;
	bSizerEndXY = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextEndX = new wxStaticText( this, wxID_ANY, _("End X (mm)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextEndX->Wrap( -1 );
	bSizerEndXY->Add( m_staticTextEndX, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_textCtrlEndX = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerEndXY->Add( m_textCtrlEndX, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_staticTextEndY = new wxStaticText( this, wxID_ANY, _("End Y (mm)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextEndY->Wrap( -1 );
	bSizerEndXY->Add( m_staticTextEndY, 0, wxRIGHT|wxLEFT, 5 );
	
	m_textCtrlEndY = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerEndXY->Add( m_textCtrlEndY, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	m_SizerEndPosition->Add( bSizerEndXY, 1, 0, 5 );
	
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextOrgPos1 = new wxStaticText( this, wxID_ANY, _("Origin"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOrgPos1->Wrap( -1 );
	bSizer61->Add( m_staticTextOrgPos1, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxString m_choiceCornerEndChoices[] = { _("Upper Right"), _("Upper Left"), _("Lower Right"), _("Lower Left") };
	int m_choiceCornerEndNChoices = sizeof( m_choiceCornerEndChoices ) / sizeof( wxString );
	m_choiceCornerEnd = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceCornerEndNChoices, m_choiceCornerEndChoices, 0 );
	m_choiceCornerEnd->SetSelection( 2 );
	bSizer61->Add( m_choiceCornerEnd, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	m_SizerEndPosition->Add( bSizer61, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizerUpper->Add( m_SizerEndPosition, 1, wxEXPAND, 5 );
	
	m_staticlineEndXY = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizerUpper->Add( m_staticlineEndXY, 0, wxEXPAND | wxALL, 5 );
	
	m_SizerText = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextTitle = new wxStaticText( this, wxID_ANY, _("Text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextTitle->Wrap( -1 );
	m_SizerText->Add( m_staticTextTitle, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_textCtrlText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_textCtrlText->SetMinSize( wxSize( 300,-1 ) );
	
	m_SizerText->Add( m_textCtrlText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_SizerText->Add( m_staticline3, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizerUpper->Add( m_SizerText, 1, wxEXPAND, 5 );
	
	
	bSizerMain->Add( bSizerUpper, 1, wxEXPAND, 5 );
	
	m_sdbSizerButtons = new wxStdDialogButtonSizer();
	m_sdbSizerButtonsOK = new wxButton( this, wxID_OK );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsOK );
	m_sdbSizerButtonsCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsCancel );
	m_sdbSizerButtons->Realize();
	
	bSizerMain->Add( m_sdbSizerButtons, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerMain );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_sdbSizerButtonsCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_NEW_DATAITEM_BASE::OnCancelClick ), NULL, this );
	m_sdbSizerButtonsOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_NEW_DATAITEM_BASE::OnOKClick ), NULL, this );
}

DIALOG_NEW_DATAITEM_BASE::~DIALOG_NEW_DATAITEM_BASE()
{
	// Disconnect Events
	m_sdbSizerButtonsCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_NEW_DATAITEM_BASE::OnCancelClick ), NULL, this );
	m_sdbSizerButtonsOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_NEW_DATAITEM_BASE::OnOKClick ), NULL, this );
	
}
