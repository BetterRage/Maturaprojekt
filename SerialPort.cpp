#include "SerialPort.h"
#include <string>
bool SerialPort::OpenPort(const char* portName,BR baudRate = BR_9600)
{
	hSerial = CreateFileA(portName,
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hSerial == INVALID_HANDLE_VALUE)
		return false;
	if (!SetSerialParams(baudRate))
		return false;
	open = true;
	return true;
}

bool SerialPort::OpenPort(unsigned long portIndex, BR baudRate = BR_9600)
{
	std::string portName;
	portName.append("COM");
	portName.append(std::to_string(portIndex));
	return this->OpenPort(portName.data());
}

bool SerialPort::SetSerialParams(BR baudRate = BR_9600)
{
	serialParams = DCB{ 0 };
	if (!GetCommState(hSerial, &serialParams))
		return false;

	serialParams.BaudRate = baudRate;
	serialParams.ByteSize = 8;
	serialParams.StopBits = ONESTOPBIT;
	serialParams.Parity = NOPARITY;
	serialParams.fDtrControl = DTR_CONTROL_ENABLE;
	if (!SetCommState(hSerial, &serialParams))
		return false;

	commTimeOuts.ReadIntervalTimeout = 50;
	commTimeOuts.ReadTotalTimeoutConstant = 50;
	commTimeOuts.ReadTotalTimeoutMultiplier = 10;
	commTimeOuts.WriteTotalTimeoutConstant = 50;
	commTimeOuts.WriteTotalTimeoutMultiplier = 10;
	if (!SetCommTimeouts(hSerial, &commTimeOuts))
		return false;

	return PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

bool SerialPort::Write(unsigned char* data, int length)
{
	DWORD bytesWritten = 0;
	return WriteFile(hSerial, data, length, &bytesWritten, NULL);
}

bool SerialPort::Read(unsigned char* buf,int nobtr,DWORD* bytesRead)
{
	return ReadFile(hSerial, buf, nobtr, bytesRead, NULL);
}

std::vector<unsigned long> SerialPort::FindPorts()
{
	ULONG portIDs[10];
	ULONG count;
	if (!GetCommPorts(portIDs, sizeof(portIDs), &count))
		return std::vector<ULONG>(portIDs,portIDs+count);
	return std::vector<ULONG>();
}

void SerialPort::Close()
{
	CloseHandle(hSerial);
}
