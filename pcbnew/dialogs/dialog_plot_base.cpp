///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jul  2 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx_html_report_panel.h"

#include "dialog_plot_base.h"

///////////////////////////////////////////////////////////////////////////

DIALOG_PLOT_BASE::DIALOG_PLOT_BASE( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : DIALOG_SHIM( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	
	m_MainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bupperSizer;
	bupperSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizerPlotFmt;
	bSizerPlotFmt = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextPlotFmt = new wxStaticText( this, wxID_ANY, _("Plot format:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPlotFmt->Wrap( -1 );
	bSizerPlotFmt->Add( m_staticTextPlotFmt, 0, wxTOP, 5 );
	
	wxString m_plotFormatOptChoices[] = { _("Gerber"), _("Postscript"), _("SVG"), _("DXF"), _("HPGL"), _("PDF") };
	int m_plotFormatOptNChoices = sizeof( m_plotFormatOptChoices ) / sizeof( wxString );
	m_plotFormatOpt = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_plotFormatOptNChoices, m_plotFormatOptChoices, 0 );
	m_plotFormatOpt->SetSelection( 0 );
	bSizerPlotFmt->Add( m_plotFormatOpt, 0, 0, 5 );
	
	
	bupperSizer->Add( bSizerPlotFmt, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizerOutDir;
	bSizerOutDir = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextDir = new wxStaticText( this, wxID_ANY, _("Output directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDir->Wrap( -1 );
	bSizerOutDir->Add( m_staticTextDir, 0, wxEXPAND|wxTOP|wxLEFT, 5 );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxHORIZONTAL );
	
	m_outputDirectoryName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_outputDirectoryName->SetToolTip( _("Target directory for plot files. Can be absolute or relative to the board file location.") );
	
	bSizer29->Add( m_outputDirectoryName, 1, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	m_browseButton = new wxButton( this, wxID_ANY, _("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( m_browseButton, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5 );
	
	
	bSizerOutDir->Add( bSizer29, 1, wxEXPAND, 5 );
	
	
	bupperSizer->Add( bSizerOutDir, 1, 0, 5 );
	
	
	m_MainSizer->Add( bupperSizer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bmiddleSizer;
	bmiddleSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_LayersSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Layers") ), wxHORIZONTAL );
	
	wxArrayString m_layerCheckListBoxChoices;
	m_layerCheckListBox = new wxCheckListBox( m_LayersSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_layerCheckListBoxChoices, 0 );
	m_LayersSizer->Add( m_layerCheckListBox, 1, wxEXPAND, 5 );
	
	
	bmiddleSizer->Add( m_LayersSizer, 1, wxALL|wxEXPAND, 3 );
	
	m_PlotOptionsSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbOptionsSizer;
	sbOptionsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Options") ), wxVERTICAL );
	
	wxBoxSizer* bSizer192;
	bSizer192 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizerPlotItems;
	bSizerPlotItems = new wxBoxSizer( wxVERTICAL );
	
	m_plotSheetRef = new wxCheckBox( sbOptionsSizer->GetStaticBox(), wxID_ANY, _("Plot sheet reference on all layers"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPlotItems->Add( m_plotSheetRef, 0, wxTOP|wxRIGHT|wxLEFT, 2 );
	
	m_plotPads_on_Silkscreen = new wxCheckBox( sbOptionsSizer->GetStaticBox(), ID_ALLOW_PRINT_PAD_ON_SILKSCREEN, _("Plot pads on silkscreen"), wxDefaultPosition, wxDefaultSize, 0 );
	m_plotPads_on_Silkscreen->SetToolTip( _("Enable plotting of pads on silkscreen layers\nWhen disabled, pads are never plotted on silkscreen layers\nWhen enabled, pads are plotted only if they appear on silkscreen layers") );
	
	bSizerPlotItems->Add( m_plotPads_on_Silkscreen, 0, wxALL, 2 );
	
	m_plotModuleValueOpt = new wxCheckBox( sbOptionsSizer->GetStaticBox(), wxID_ANY, _("Plot footprint values"), wxDefaultPosition, wxDefaultSize, 0 );
	m_plotModuleValueOpt->SetValue(true); 
	bSizerPlotItems->Add( m_plotModuleValueOpt, 0, wxTOP|wxRIGHT|wxLEFT, 2 );
	
	m_plotModuleRefOpt = new wxCheckBox( sbOptionsSizer->GetStaticBox(), ID_PRINT_REF, _("Plot footprint references"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPlotItems->Add( m_plotModuleRefOpt, 0, wxTOP|wxRIGHT|wxLEFT, 2 );
	
	m_plotInvisibleText = new wxCheckBox( sbOptionsSizer->GetStaticBox(), wxID_ANY, _("Force plotting of invisible values/references"), wxDefaultPosition, wxDefaultSize, 0 );
	m_plotInvisibleText->SetToolTip( _("Force plot invisible values and/or references") );
	
	bSizerPlotItems->Add( m_plotInvisibleText, 0, wxALL, 2 );
	
	m_plotNoViaOnMaskOpt = new wxCheckBox( sbOptionsSizer->GetStaticBox(), wxID_ANY, _("Do not tent vias"), wxDefaultPosition, wxDefaultSize, 0 );
	m_plotNoViaOnMaskOpt->SetToolTip( _("Remove soldermask on vias") );
	
	bSizerPlotItems->Add( m_plotNoViaOnMaskOpt, 0, wxTOP|wxRIGHT|wxLEFT, 2 );
	
	m_excludeEdgeLayerOpt = new wxCheckBox( sbOptionsSizer->GetStaticBox(), wxID_ANY, _("Exclude PCB edge layer from other layers"), wxDefaultPosition, wxDefaultSize, 0 );
	m_excludeEdgeLayerOpt->SetToolTip( _("Exclude contents of the pcb edge layer from all other layers") );
	
	bSizerPlotItems->Add( m_excludeEdgeLayerOpt, 0, wxALL, 2 );
	
	m_plotMirrorOpt = new wxCheckBox( sbOptionsSizer->GetStaticBox(), ID_MIROR_OPT, _("Mirrored plot"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPlotItems->Add( m_plotMirrorOpt, 0, wxALL, 2 );
	
	m_plotPSNegativeOpt = new wxCheckBox( sbOptionsSizer->GetStaticBox(), wxID_ANY, _("Negative plot"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPlotItems->Add( m_plotPSNegativeOpt, 0, wxALL, 2 );
	
	m_useAuxOriginCheckBox = new wxCheckBox( sbOptionsSizer->GetStaticBox(), wxID_ANY, _("Use auxiliary axis as origin"), wxDefaultPosition, wxDefaultSize, 0 );
	m_useAuxOriginCheckBox->SetToolTip( _("Use auxiliary axis as coordinates origin in plot files") );
	
	bSizerPlotItems->Add( m_useAuxOriginCheckBox, 0, wxALL, 2 );
	
	
	bSizer192->Add( bSizerPlotItems, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText11 = new wxStaticText( sbOptionsSizer->GetStaticBox(), wxID_ANY, _("Drill marks:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer14->Add( m_staticText11, 0, wxRIGHT|wxLEFT, 5 );
	
	wxString m_drillShapeOptChoices[] = { _("None"), _("Small"), _("Actual size") };
	int m_drillShapeOptNChoices = sizeof( m_drillShapeOptChoices ) / sizeof( wxString );
	m_drillShapeOpt = new wxChoice( sbOptionsSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_drillShapeOptNChoices, m_drillShapeOptChoices, 0 );
	m_drillShapeOpt->SetSelection( 0 );
	bSizer14->Add( m_drillShapeOpt, 0, wxEXPAND|wxLEFT, 5 );
	
	m_staticText12 = new wxStaticText( sbOptionsSizer->GetStaticBox(), wxID_ANY, _("Scaling:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer14->Add( m_staticText12, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxString m_scaleOptChoices[] = { _("Auto"), _("1:1"), _("3:2"), _("2:1"), _("3:1") };
	int m_scaleOptNChoices = sizeof( m_scaleOptChoices ) / sizeof( wxString );
	m_scaleOpt = new wxChoice( sbOptionsSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_scaleOptNChoices, m_scaleOptChoices, 0 );
	m_scaleOpt->SetSelection( 1 );
	bSizer14->Add( m_scaleOpt, 0, wxEXPAND|wxLEFT, 5 );
	
	m_staticText13 = new wxStaticText( sbOptionsSizer->GetStaticBox(), wxID_ANY, _("Plot mode:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer14->Add( m_staticText13, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxString m_plotModeOptChoices[] = { _("Filled"), _("Sketch") };
	int m_plotModeOptNChoices = sizeof( m_plotModeOptChoices ) / sizeof( wxString );
	m_plotModeOpt = new wxChoice( sbOptionsSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_plotModeOptNChoices, m_plotModeOptChoices, 0 );
	m_plotModeOpt->SetSelection( 0 );
	bSizer14->Add( m_plotModeOpt, 0, wxEXPAND|wxLEFT, 5 );
	
	m_textDefaultPenSize = new wxStaticText( sbOptionsSizer->GetStaticBox(), wxID_ANY, _("Default line width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textDefaultPenSize->Wrap( -1 );
	m_textDefaultPenSize->SetToolTip( _("Pen size used to draw items that have no pen size specified.\nUsed mainly to draw items in sketch mode.") );
	
	bSizer14->Add( m_textDefaultPenSize, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_linesWidth = new wxTextCtrl( sbOptionsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_linesWidth->SetToolTip( _("Line width for, e.g., sheet references.") );
	
	bSizer14->Add( m_linesWidth, 0, wxBOTTOM|wxEXPAND|wxLEFT, 5 );
	
	
	bSizer192->Add( bSizer14, 1, wxRIGHT|wxLEFT, 3 );
	
	
	sbOptionsSizer->Add( bSizer192, 0, wxEXPAND, 5 );
	
	
	m_PlotOptionsSizer->Add( sbOptionsSizer, 0, wxALL|wxEXPAND, 3 );
	
	wxStaticBoxSizer* sbSizerSoldMaskLayerOpt;
	sbSizerSoldMaskLayerOpt = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Current solder mask settings:") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizerSoldMaskOpts;
	fgSizerSoldMaskOpts = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizerSoldMaskOpts->SetFlexibleDirection( wxBOTH );
	fgSizerSoldMaskOpts->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_SolderMaskMarginLabel = new wxStaticText( sbSizerSoldMaskLayerOpt->GetStaticBox(), wxID_ANY, _("Solder mask clearance:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SolderMaskMarginLabel->Wrap( -1 );
	m_SolderMaskMarginLabel->SetToolTip( _("Margin between pads and solder mask") );
	
	fgSizerSoldMaskOpts->Add( m_SolderMaskMarginLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_SolderMaskMarginCurrValue = new wxStaticText( sbSizerSoldMaskLayerOpt->GetStaticBox(), wxID_ANY, _("val"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SolderMaskMarginCurrValue->Wrap( -1 );
	fgSizerSoldMaskOpts->Add( m_SolderMaskMarginCurrValue, 0, wxALL, 5 );
	
	m_solderMaskMinWidthLabel = new wxStaticText( sbSizerSoldMaskLayerOpt->GetStaticBox(), wxID_ANY, _("Solder mask min width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_solderMaskMinWidthLabel->Wrap( -1 );
	m_solderMaskMinWidthLabel->SetToolTip( _("Minimum distance between 2 pad areas.\nTwo pad areas nearer than this value will be merged during plotting") );
	
	fgSizerSoldMaskOpts->Add( m_solderMaskMinWidthLabel, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_SolderMaskMinWidthCurrValue = new wxStaticText( sbSizerSoldMaskLayerOpt->GetStaticBox(), wxID_ANY, _("val"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SolderMaskMinWidthCurrValue->Wrap( -1 );
	fgSizerSoldMaskOpts->Add( m_SolderMaskMinWidthCurrValue, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	sbSizerSoldMaskLayerOpt->Add( fgSizerSoldMaskOpts, 1, wxEXPAND, 5 );
	
	
	m_PlotOptionsSizer->Add( sbSizerSoldMaskLayerOpt, 1, wxEXPAND, 5 );
	
	m_GerberOptionsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Gerber Options") ), wxHORIZONTAL );
	
	wxBoxSizer* bSizerGbrOpt;
	bSizerGbrOpt = new wxBoxSizer( wxVERTICAL );
	
	m_useGerberExtensions = new wxCheckBox( m_GerberOptionsSizer->GetStaticBox(), wxID_ANY, _("Use Protel filename extensions"), wxDefaultPosition, wxDefaultSize, 0 );
	m_useGerberExtensions->SetToolTip( _("Use conventional Protel Gerber extensions - .GBL, .GTL, etc...") );
	
	bSizerGbrOpt->Add( m_useGerberExtensions, 0, wxALL, 2 );
	
	m_useGerberAttributes = new wxCheckBox( m_GerberOptionsSizer->GetStaticBox(), wxID_ANY, _("Include extended attributes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_useGerberAttributes->SetToolTip( _("Include extended attributes (X2 Gerber files format) in the Gerber file") );
	
	bSizerGbrOpt->Add( m_useGerberAttributes, 0, wxALL, 2 );
	
	m_subtractMaskFromSilk = new wxCheckBox( m_GerberOptionsSizer->GetStaticBox(), wxID_ANY, _("Subtract soldermask from silkscreen"), wxDefaultPosition, wxDefaultSize, 0 );
	m_subtractMaskFromSilk->SetToolTip( _("Remove silkscreen from areas without soldermask") );
	
	bSizerGbrOpt->Add( m_subtractMaskFromSilk, 0, wxALL, 2 );
	
	
	m_GerberOptionsSizer->Add( bSizerGbrOpt, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_rbGerberFormatChoices[] = { _("4.5 (unit mm)"), _("4.6 (unit mm)") };
	int m_rbGerberFormatNChoices = sizeof( m_rbGerberFormatChoices ) / sizeof( wxString );
	m_rbGerberFormat = new wxRadioBox( m_GerberOptionsSizer->GetStaticBox(), wxID_ANY, _("Format"), wxDefaultPosition, wxDefaultSize, m_rbGerberFormatNChoices, m_rbGerberFormatChoices, 1, wxRA_SPECIFY_COLS );
	m_rbGerberFormat->SetSelection( 0 );
	m_rbGerberFormat->SetToolTip( _("Resolution of coordinates in Gerber files.\nUse the higher value if possible.") );
	
	m_GerberOptionsSizer->Add( m_rbGerberFormat, 1, wxEXPAND|wxALL, 5 );
	
	
	m_PlotOptionsSizer->Add( m_GerberOptionsSizer, 0, wxALL|wxEXPAND, 3 );
	
	m_HPGLOptionsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("HPGL Options") ), wxVERTICAL );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	m_textPenSize = new wxStaticText( m_HPGLOptionsSizer->GetStaticBox(), wxID_ANY, _("Pen size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textPenSize->Wrap( -1 );
	bSizer20->Add( m_textPenSize, 0, wxRIGHT|wxLEFT, 5 );
	
	m_HPGLPenSizeOpt = new wxTextCtrl( m_HPGLOptionsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_HPGLPenSizeOpt, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer22->Add( bSizer20, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	m_textPenOvr = new wxStaticText( m_HPGLOptionsSizer->GetStaticBox(), wxID_ANY, _("Pen overlay"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textPenOvr->Wrap( -1 );
	bSizer21->Add( m_textPenOvr, 0, wxRIGHT|wxLEFT, 5 );
	
	m_HPGLPenOverlayOpt = new wxTextCtrl( m_HPGLOptionsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_HPGLPenOverlayOpt->SetToolTip( _("Set plot overlay for filling") );
	
	bSizer21->Add( m_HPGLPenOverlayOpt, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer22->Add( bSizer21, 1, wxEXPAND, 5 );
	
	
	m_HPGLOptionsSizer->Add( bSizer22, 1, wxEXPAND, 5 );
	
	
	m_PlotOptionsSizer->Add( m_HPGLOptionsSizer, 0, wxALL|wxEXPAND, 3 );
	
	m_PSOptionsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Postscript Options") ), wxVERTICAL );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText7 = new wxStaticText( m_PSOptionsSizer->GetStaticBox(), wxID_ANY, _("X scale:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer18->Add( m_staticText7, 0, wxRIGHT|wxLEFT, 5 );
	
	m_fineAdjustXscaleOpt = new wxTextCtrl( m_PSOptionsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_fineAdjustXscaleOpt->SetToolTip( _("Set global X scale adjust for exact scale postscript output.") );
	
	bSizer18->Add( m_fineAdjustXscaleOpt, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer17->Add( bSizer18, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText8 = new wxStaticText( m_PSOptionsSizer->GetStaticBox(), wxID_ANY, _("Y scale:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer19->Add( m_staticText8, 0, wxRIGHT|wxLEFT, 5 );
	
	m_fineAdjustYscaleOpt = new wxTextCtrl( m_PSOptionsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_fineAdjustYscaleOpt->SetToolTip( _("Set global Y scale adjust for exact scale postscript output.") );
	
	bSizer19->Add( m_fineAdjustYscaleOpt, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer17->Add( bSizer19, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer191;
	bSizer191 = new wxBoxSizer( wxVERTICAL );
	
	m_textPSFineAdjustWidth = new wxStaticText( m_PSOptionsSizer->GetStaticBox(), wxID_ANY, _("Width correction"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textPSFineAdjustWidth->Wrap( -1 );
	bSizer191->Add( m_textPSFineAdjustWidth, 0, wxRIGHT|wxLEFT, 5 );
	
	m_PSFineAdjustWidthOpt = new wxTextCtrl( m_PSOptionsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_PSFineAdjustWidthOpt->SetToolTip( _("Set global width correction for exact width postscript output.\nThese width correction is intended to compensate tracks width and also pads and vias size errors.\nThe reasonable width correction value must be in a range of [-(MinTrackWidth-1), +(MinClearanceValue-1)] in decimils.") );
	
	bSizer191->Add( m_PSFineAdjustWidthOpt, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	
	bSizer17->Add( bSizer191, 1, wxEXPAND, 5 );
	
	
	m_PSOptionsSizer->Add( bSizer17, 1, wxEXPAND, 5 );
	
	m_forcePSA4OutputOpt = new wxCheckBox( m_PSOptionsSizer->GetStaticBox(), wxID_ANY, _("Force A4 output"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PSOptionsSizer->Add( m_forcePSA4OutputOpt, 0, wxALL, 2 );
	
	
	m_PlotOptionsSizer->Add( m_PSOptionsSizer, 0, wxALL|wxEXPAND, 3 );
	
	m_SizerDXF_options = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("DXF options") ), wxVERTICAL );
	
	m_DXF_plotModeOpt = new wxCheckBox( m_SizerDXF_options->GetStaticBox(), wxID_ANY, _("Plot  all layers in outline (polygon) mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_DXF_plotModeOpt->SetValue(true); 
	m_DXF_plotModeOpt->SetToolTip( _("DXF only:\nCheck to plot all layers in polygon mode.\nUncheck to plot in sketch mode layers that don't support polygons (*.SilkS, *_User, Edge.Cuts, Margin, *.CrtYd, *.Fab)\nand plot in polygon mode other layers (*.Cu, *.Adhes, *.Paste, *.Mask)") );
	
	m_SizerDXF_options->Add( m_DXF_plotModeOpt, 0, wxALL, 2 );
	
	m_DXF_plotTextStrokeFontOpt = new wxCheckBox( m_SizerDXF_options->GetStaticBox(), wxID_ANY, _("Use Pcbnew font to plot texts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_DXF_plotTextStrokeFontOpt->SetValue(true); 
	m_DXF_plotTextStrokeFontOpt->SetToolTip( _("Check to use Pcbnew stroke font\nUncheck to plot oneline ASCII texts as editable text (using DXF font)") );
	
	m_SizerDXF_options->Add( m_DXF_plotTextStrokeFontOpt, 0, wxALL, 2 );
	
	
	m_PlotOptionsSizer->Add( m_SizerDXF_options, 0, wxEXPAND, 5 );
	
	
	bmiddleSizer->Add( m_PlotOptionsSizer, 0, 0, 5 );
	
	
	m_MainSizer->Add( bmiddleSizer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* sbSizerMsg;
	sbSizerMsg = new wxBoxSizer( wxVERTICAL );
	
	m_messagesPanel = new WX_HTML_REPORT_PANEL( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_messagesPanel->SetMinSize( wxSize( -300,150 ) );
	
	sbSizerMsg->Add( m_messagesPanel, 1, wxEXPAND | wxALL, 5 );
	
	
	m_MainSizer->Add( sbSizerMsg, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerButtons;
	bSizerButtons = new wxBoxSizer( wxHORIZONTAL );
	
	m_plotButton = new wxButton( this, wxID_ANY, _("Plot"), wxDefaultPosition, wxDefaultSize, 0 );
	m_plotButton->SetDefault(); 
	bSizerButtons->Add( m_plotButton, 0, wxALL, 5 );
	
	m_buttonDrill = new wxButton( this, ID_CREATE_DRILL_FILE, _("Generate Drill File"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_buttonDrill, 0, wxALL, 5 );
	
	m_buttonQuit = new wxButton( this, wxID_CANCEL, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_buttonQuit, 0, wxALL, 5 );
	
	
	m_MainSizer->Add( bSizerButtons, 0, wxALIGN_RIGHT|wxRIGHT|wxLEFT, 5 );
	
	
	this->SetSizer( m_MainSizer );
	this->Layout();
	m_popMenu = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_popMenu, ID_LAYER_FAB, wxString( _("Select Fab Layers") ) , wxEmptyString, wxITEM_NORMAL );
	m_popMenu->Append( m_menuItem1 );
	
	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem( m_popMenu, ID_SELECT_COPPER_LAYERS, wxString( _("Select all Copper Layers") ) , wxEmptyString, wxITEM_NORMAL );
	m_popMenu->Append( m_menuItem2 );
	
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_popMenu, ID_DESELECT_COPPER_LAYERS, wxString( _("Deselect all Copper Layers") ) , wxEmptyString, wxITEM_NORMAL );
	m_popMenu->Append( m_menuItem3 );
	
	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_popMenu, ID_SELECT_ALL_LAYERS, wxString( _("Select all Layers") ) , wxEmptyString, wxITEM_NORMAL );
	m_popMenu->Append( m_menuItem4 );
	
	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_popMenu, ID_DESELECT_ALL_LAYERS, wxString( _("Deselect all Layers") ) , wxEmptyString, wxITEM_NORMAL );
	m_popMenu->Append( m_menuItem5 );
	
	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( DIALOG_PLOT_BASE::DIALOG_PLOT_BASEOnContextMenu ), NULL, this ); 
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DIALOG_PLOT_BASE::OnClose ) );
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DIALOG_PLOT_BASE::OnInitDialog ) );
	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( DIALOG_PLOT_BASE::OnRightClick ) );
	m_plotFormatOpt->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DIALOG_PLOT_BASE::SetPlotFormat ), NULL, this );
	m_browseButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnOutputDirectoryBrowseClicked ), NULL, this );
	m_layerCheckListBox->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( DIALOG_PLOT_BASE::OnRightClick ), NULL, this );
	m_scaleOpt->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnSetScaleOpt ), NULL, this );
	m_DXF_plotModeOpt->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnChangeDXFPlotMode ), NULL, this );
	m_plotButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_PLOT_BASE::Plot ), NULL, this );
	m_buttonDrill->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_PLOT_BASE::CreateDrillFile ), NULL, this );
	m_buttonQuit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnQuit ), NULL, this );
	this->Connect( m_menuItem1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnPopUpLayers ) );
	this->Connect( m_menuItem2->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnPopUpLayers ) );
	this->Connect( m_menuItem3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnPopUpLayers ) );
	this->Connect( m_menuItem4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnPopUpLayers ) );
	this->Connect( m_menuItem5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnPopUpLayers ) );
}

DIALOG_PLOT_BASE::~DIALOG_PLOT_BASE()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DIALOG_PLOT_BASE::OnClose ) );
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DIALOG_PLOT_BASE::OnInitDialog ) );
	this->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( DIALOG_PLOT_BASE::OnRightClick ) );
	m_plotFormatOpt->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DIALOG_PLOT_BASE::SetPlotFormat ), NULL, this );
	m_browseButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnOutputDirectoryBrowseClicked ), NULL, this );
	m_layerCheckListBox->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( DIALOG_PLOT_BASE::OnRightClick ), NULL, this );
	m_scaleOpt->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnSetScaleOpt ), NULL, this );
	m_DXF_plotModeOpt->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnChangeDXFPlotMode ), NULL, this );
	m_plotButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_PLOT_BASE::Plot ), NULL, this );
	m_buttonDrill->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_PLOT_BASE::CreateDrillFile ), NULL, this );
	m_buttonQuit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnQuit ), NULL, this );
	this->Disconnect( ID_LAYER_FAB, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnPopUpLayers ) );
	this->Disconnect( ID_SELECT_COPPER_LAYERS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnPopUpLayers ) );
	this->Disconnect( ID_DESELECT_COPPER_LAYERS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnPopUpLayers ) );
	this->Disconnect( ID_SELECT_ALL_LAYERS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnPopUpLayers ) );
	this->Disconnect( ID_DESELECT_ALL_LAYERS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DIALOG_PLOT_BASE::OnPopUpLayers ) );
	
	delete m_popMenu; 
}
