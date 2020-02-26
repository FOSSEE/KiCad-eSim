///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DIALOG_HOTKEYS_EDITOR_BASE_H__
#define __DIALOG_HOTKEYS_EDITOR_BASE_H__

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
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class HOTKEYS_EDITOR_DIALOG_BASE
///////////////////////////////////////////////////////////////////////////////
class HOTKEYS_EDITOR_DIALOG_BASE : public DIALOG_SHIM
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxNotebook* m_hotkeySections;
		wxStdDialogButtonSizer* m_sdbSizer;
		wxButton* m_sdbSizerOK;
		wxButton* m_sdbSizerCancel;
		wxButton* m_undoButton;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnOKClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void UndoClicked( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		HOTKEYS_EDITOR_DIALOG_BASE( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Hotkeys Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 450,500 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~HOTKEYS_EDITOR_DIALOG_BASE();
	
};

#endif //__DIALOG_HOTKEYS_EDITOR_BASE_H__
