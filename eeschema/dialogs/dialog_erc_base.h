///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DIALOG_ERC_BASE_H__
#define __DIALOG_ERC_BASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class DIALOG_SHIM;
class ERC_HTML_LISTFRAME;

#include "dialog_shim.h"
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/html/htmlwin.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_ERASE_DRC_MARKERS 1000
#define ID_ERC_CMP 1001
#define ID_RESET_MATRIX 1002

///////////////////////////////////////////////////////////////////////////////
/// Class DIALOG_ERC_BASE
///////////////////////////////////////////////////////////////////////////////
class DIALOG_ERC_BASE : public DIALOG_SHIM
{
	private:
	
	protected:
		wxNotebook* m_NoteBook;
		wxPanel* m_PanelERC;
		wxStaticText* m_ErcTotalErrorsText;
		wxTextCtrl* m_TotalErrCount;
		wxStaticText* m_WarnErcErrorsText;
		wxTextCtrl* m_LastWarningCount;
		wxStaticText* m_LastErrCountText;
		wxTextCtrl* m_LastErrCount;
		wxCheckBox* m_WriteResultOpt;
		wxStaticText* m_titleMessages;
		wxTextCtrl* m_MessagesList;
		wxStaticText* m_textMarkers;
		ERC_HTML_LISTFRAME* m_MarkersList;
		wxButton* m_buttondelmarkers;
		wxButton* m_buttonERC;
		wxButton* m_buttonClose;
		wxPanel* m_PanelERCOptions;
		wxButton* m_ResetOptButton;
		wxPanel* m_matrixPanel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseErcDialog( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnLeftClickMarkersList( wxHtmlLinkEvent& event ) { event.Skip(); }
		virtual void OnLeftDblClickMarkersList( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnEraseDrcMarkersClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnErcCmpClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonCloseClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnResetMatrixClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DIALOG_ERC_BASE( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Electrical Rules Checker"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 519,457 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~DIALOG_ERC_BASE();
	
};

#endif //__DIALOG_ERC_BASE_H__
