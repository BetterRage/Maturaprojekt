#pragma once
#include <Windows.h>
#include <vector>
#include <opencv2/core.hpp>
enum BR
{
	BR_110 = CBR_110,
	BR_300 = CBR_300,
	BR_600 = CBR_600,
	BR_1200 = CBR_1200,
	BR_2400 = CBR_2400,
	BR_4800 = CBR_4800,
	BR_9600 = CBR_9600,
	BR_14400 = CBR_14400,
	BR_19200 = CBR_19200,
	BR_38400 = CBR_38400,
	BR_56000 = CBR_56000,
	BR_57600 = CBR_57600,
	BR_115200 = CBR_115200,
	BR_128000 = CBR_128000,
	BR_256000 = CBR_256000
};

class MCUConn
{
private:
	HANDLE hSerial;
	DCB serialParams;
	COMMTIMEOUTS commTimeOuts;
	bool SetSerialParams(BR baudRate);
public:
	
	bool open = false;
	bool OpenPort(const char* portName, BR baudRate);
	bool OpenPort(unsigned long portIndex, BR baudRate);
	bool WriteData(unsigned char* data,int length);
	bool Read(unsigned char* buf, int nobtr);
	bool SendCoordData(std::vector<cv::Point2f>& points);
	void SetManMode();
	void SetAutoMode();
	static std::vector<unsigned long> FindAvailableComPorts();
	void Close();
	
};


