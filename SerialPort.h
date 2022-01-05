#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include "CoordParams.h"

class MCUConn
{
private:
	HANDLE hSerial;
	DCB serialParams;
	COMMTIMEOUTS commTimeOuts;
	bool open = false;
	bool SetSerialParams();
public:
	bool OpenPort(char* portName);
	bool OpenPort(unsigned long portIndex);
	bool WriteData(unsigned char* data,int length);
	bool Read(unsigned char* buf, int nobtr, DWORD* bytesRead);
	bool SendCoordData(std::vector<cv::Point2f>& points);
	static std::vector<unsigned long> FindAvailableComPorts();
	void Close();
};

