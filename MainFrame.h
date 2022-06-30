#pragma once
#include "Helpers.h"
#include "OCVProc.h"
#include "SerialPort.h"
#include "CoordParams.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class MainFrame : public wxFrame
{
	MCUConn* serialPort;
	OCVProc* ocvProc;
	wxSize defaultButtonSize = wxSize(60,30);
	wxPanel *streamContainer;
	wxPanel *basePanel;
	wxButton *btnCapture;
	wxButton *btnRotate;
	wxButton *btnMirror;
	wxChoice* choiceManAuto;
	wxCheckListBox* selectPoints;
	wxButton* sendPoints;
	void InitUI(wxSize size,wxSize cameraRes);
	void btnCaptureClicked(wxCommandEvent &event);
	void btnRotateClicked(wxCommandEvent &event);
	void btnMirrorClicked(wxCommandEvent &event);
	void autoManChanged(wxCommandEvent& event);
public:
	MainFrame(const wxString &title, const wxSize &size,OCVProc* ocvObj,MCUConn* serialObj);
	~MainFrame();
};
