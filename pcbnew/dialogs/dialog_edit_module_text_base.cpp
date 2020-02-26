///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  6 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "class_pcb_layer_box_selector.h"

#include "dialog_edit_module_text_base.h"

///////////////////////////////////////////////////////////////////////////

DialogEditModuleText_base::DialogEditModuleText_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : DIALOG_SHIM( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bMainSizer;
	bMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_ModuleInfoText = new wxStaticText( this, wxID_ANY, _("Footprint %s (%s) orientation %.1f"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ModuleInfoText->Wrap( -1 );
	m_ModuleInfoText->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );
	
	bMainSizer->Add( m_ModuleInfoText, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bMainSizer->Add( m_staticline1, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizerUpper;
	bSizerUpper = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 7, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_TextDataTitle = new wxStaticText( this, wxID_ANY, _("Reference:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TextDataTitle->Wrap( -1 );
	fgSizer1->Add( m_TextDataTitle, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_Name = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_Name, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_SizeXTitle = new wxStaticText( this, wxID_ANY, _("Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SizeXTitle->Wrap( -1 );
	fgSizer1->Add( m_SizeXTitle, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_TxtSizeCtrlX = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_TxtSizeCtrlX, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_SizeYTitle = new wxStaticText( this, wxID_ANY, _("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SizeYTitle->Wrap( -1 );
	fgSizer1->Add( m_SizeYTitle, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_TxtSizeCtrlY = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_TxtSizeCtrlY, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_WidthTitle = new wxStaticText( this, wxID_ANY, _("Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_WidthTitle->Wrap( -1 );
	fgSizer1->Add( m_WidthTitle, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_TxtWidthCtlr = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_TxtWidthCtlr, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_PosXTitle = new wxStaticText( this, wxID_ANY, _("Offset X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PosXTitle->Wrap( -1 );
	fgSizer1->Add( m_PosXTitle, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_TxtPosCtrlX = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_TxtPosCtrlX, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_PosYTitle = new wxStaticText( this, wxID_ANY, _("Offset Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PosYTitle->Wrap( -1 );
	fgSizer1->Add( m_PosYTitle, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_TxtPosCtrlY = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_TxtPosCtrlY, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_LayerLabel = new wxStaticText( this, wxID_ANY, _("Layer:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LayerLabel->Wrap( -1 );
	fgSizer1->Add( m_LayerLabel, 0, wxALL, 5 );
	
	m_LayerSelectionCtrl = new PCB_LAYER_BOX_SELECTOR( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer1->Add( m_LayerSelectionCtrl, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	
	bSizer10->Add( fgSizer1, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizerUpper->Add( bSizer10, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerOpts;
	bSizerOpts = new wxBoxSizer( wxVERTICAL );
	
	wxString m_StyleChoices[] = { _("Normal"), _("Italic") };
	int m_StyleNChoices = sizeof( m_StyleChoices ) / sizeof( wxString );
	m_Style = new wxRadioBox( this, wxID_ANY, _("Style"), wxDefaultPosition, wxDefaultSize, m_StyleNChoices, m_StyleChoices, 1, wxRA_SPECIFY_COLS );
	m_Style->SetSelection( 0 );
	bSizerOpts->Add( m_Style, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	wxString m_OrientChoices[] = { _("Horizontal"), _("Vertical") };
	int m_OrientNChoices = sizeof( m_OrientChoices ) / sizeof( wxString );
	m_Orient = new wxRadioBox( this, wxID_ANY, _("Orientation"), wxDefaultPosition, wxDefaultSize, m_OrientNChoices, m_OrientChoices, 1, wxRA_SPECIFY_COLS );
	m_Orient->SetSelection( 0 );
	bSizerOpts->Add( m_Orient, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	wxString m_ShowChoices[] = { _("Visible"), _("Invisible") };
	int m_ShowNChoices = sizeof( m_ShowChoices ) / sizeof( wxString );
	m_Show = new wxRadioBox( this, wxID_ANY, _("Display"), wxDefaultPosition, wxDefaultSize, m_ShowNChoices, m_ShowChoices, 1, wxRA_SPECIFY_COLS );
	m_Show->SetSelection( 0 );
	bSizerOpts->Add( m_Show, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	
	bSizerUpper->Add( bSizerOpts, 0, wxEXPAND, 5 );
	
	
	bMainSizer->Add( bSizerUpper, 0, wxALL|wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bMainSizer->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	
	bMainSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_sdbSizer = new wxStdDialogButtonSizer();
	m_sdbSizerOK = new wxButton( this, wxID_OK );
	m_sdbSizer->AddButton( m_sdbSizerOK );
	m_sdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer->AddButton( m_sdbSizerCancel );
	m_sdbSizer->Realize();
	
	bMainSizer->Add( m_sdbSizer, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	
	this->SetSizer( bMainSizer );
	this->Layout();
	
	// Connect Events
	m_sdbSizerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogEditModuleText_base::OnOkClick ), NULL, this );
}

DialogEditModuleText_base::~DialogEditModuleText_base()
{
	// Disconnect Events
	m_sdbSizerOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogEditModuleText_base::OnOkClick ), NULL, this );
	
}
