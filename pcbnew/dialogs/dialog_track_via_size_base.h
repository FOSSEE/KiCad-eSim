///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  6 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DIALOG_TRACK_VIA_SIZE_BASE_H__
#define __DIALOG_TRACK_VIA_SIZE_BASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class DIALOG_SHIM;

#include "dialog_shim.h"
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class DIALOG_TRACK_VIA_SIZE_BASE
///////////////////////////////////////////////////////////////////////////////
class DIALOG_TRACK_VIA_SIZE_BASE : public DIALOG_SHIM
{
	private:
	
	protected:
		wxStaticText* m_staticText3;
		wxTextCtrl* m_trackWidthText;
		wxStaticText* m_trackWidthLabel;
		wxStaticText* m_staticText5;
		wxTextCtrl* m_viaDiameterText;
		wxStaticText* m_viaDiameterLabel;
		wxStaticText* m_staticText7;
		wxTextCtrl* m_viaDrillText;
		wxStaticText* m_viaDrillLabel;
		wxStaticLine* m_staticline1;
		wxStdDialogButtonSizer* m_stdButtons;
		wxButton* m_stdButtonsOK;
		wxButton* m_stdButtonsCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void onCancelClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onOkClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DIALOG_TRACK_VIA_SIZE_BASE( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Track width and via size"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 303,187 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~DIALOG_TRACK_VIA_SIZE_BASE();
	
};

#endif //__DIALOG_TRACK_VIA_SIZE_BASE_H__
