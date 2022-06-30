#include "App.h"
#include <iostream>
#include <fstream>
wxIMPLEMENT_APP(App);
bool App::OnInit()
{
	SelectCOMPort(MCUConn::FindAvailableComPorts());

	if (!SelectCameraDialog())
		return false;

	mainFrame= new MainFrame(wxString("Test"), wxSize(900, 800), &ocvProc, &serialPort);
	mainFrame->Show(true);
	ocvProc.StartCameraStream();
	return true;
}
bool App::SelectCOMPort(std::vector<unsigned long> ports)
{
	wxArrayString portNames;
	//if (portNames.size() == 0);
		//return false;
	for (auto i : ports)
	{
		wxString name("COM");
		name += std::to_string(i);
		portNames.Add(name);
	}
	int selectedIndex = SelectString(nullptr, portNames);
	if (selectedIndex!=-1)
	{
		serialPort.OpenPort(ports[selectedIndex],BR_9600);
	}
	return serialPort.open;
}

bool App::SelectCameraDialog()
{
	std::string deviceIP;
	int selectedDeviceID;
	std::vector<wchar_t*> deviceNames;

	if (EnterCameraIP(nullptr, deviceIP))
		return ocvProc.OpenCamera(deviceIP);

	HRESULT result = Helpers::getVideoDeviceNames(deviceNames);
	if (result != S_OK || deviceNames.size() == 0)
		return false;


	selectedDeviceID = SelectString(nullptr, Helpers::wxArrStrFromVector(deviceNames));
	if (selectedDeviceID>=0)
		return ocvProc.OpenCamera(selectedDeviceID);
	return false;
}

int App::SelectString(wxWindow* parent, wxArrayString strings)
{
	int index = -1;
	wxSingleChoiceDialog selectString(parent, wxString(""), wxString("Pick device"), strings);
	int dialogResult = selectString.ShowModal();
	if (dialogResult == wxID_OK)
		index = selectString.GetSelection();
	return index;
}

bool App::EnterCameraIP(wxWindow* parent, std::string& ip)
{
	std::string cameraIP;
	std::ifstream ifile;
	std::ofstream ofile;

	ifile.open("ip.txt");
	if (ifile.is_open())
	{
		std::getline(ifile, cameraIP);
		if (cameraIP.length() == 0)
			cameraIP = defaultIP;
	}
	else
	{
		std::ofstream{ "ip.txt" };
		cameraIP = defaultIP;
	}

	ifile.close();
	wxTextEntryDialog enterIP(parent, wxString(""), wxString("Enter camera IP"), wxString(cameraIP));
	int dialogResult = enterIP.ShowModal();
	ip = std::string(enterIP.GetValue());

	ofile.open("ip.txt");
	if (ofile.is_open())
	{
		ofile.clear();
		ofile << ip;
		ofile.close();
	}
	return dialogResult == wxID_OK;
}

App::~App()
{
	ocvProc.StopCameraStream();
}