#pragma once
#include "MediaFoundationLib.h"
#include "Helpers.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <wx/choicdlg.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class MainFrame :public wxFrame
{
	HRESULT PreviewInitialized=E_FAIL;
	int selectedDevice=-1;
	MediaFoundationLib* mfObject;
	wxPanel* mfStreamContainer;
	wxPanel* basePanel;
	void btnCaptureClicked(wxCommandEvent& event);
	DIBIMAGE CapturedImage{};
public:
	MainFrame(const wxString& title, const wxSize& size);
	~MainFrame();
};

