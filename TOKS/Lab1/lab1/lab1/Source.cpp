#include <windows.h>
#include <windows.h>
#include <iostream>
#include <Tchar.h>
#include <conio.h>
using namespace std;
HANDLE hSerial;
HANDLE hSerial1;
void newspeed(HANDLE PORT)
{
	int n = 0;
	cout << "Set speed ";
	cin >> n;
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(PORT, &dcbSerialParams))
	{
		cout << "getting state error\n";
	}
	cout <<"Youre default speed is: " << dcbSerialParams.BaudRate << "\n";
	dcbSerialParams.BaudRate = n;
	if (!SetCommState(PORT, &dcbSerialParams))
	{
		cout << "error setting serial port state\n";
	}
	cout <<"Com-Port speed is: " << dcbSerialParams.BaudRate << "\n";
}
void setparm(HANDLE PORT)
{
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(PORT, &dcbSerialParams))
	{
		cout << "getting state error\n";
	}
	dcbSerialParams.BaudRate = 9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (!SetCommState(PORT, &dcbSerialParams))
	{
		cout << "error setting serial port state\n";
	}
}
void ReadCOM()
{
	DWORD iSize;
	char sReceivedChar;
	while (true)
	{
		ReadFile(hSerial1, &sReceivedChar, 1, &iSize, 0);
		if (iSize > 0)
			cout << sReceivedChar;
	}
}
int _tmain(int argc, _TCHAR* argv[])
{

	DCB dcbSerialParams = { 0 };
	LPCTSTR sPortName = L"COM1";
	LPCTSTR sPortName1 = L"COM2";
	hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);
	hSerial1 = ::CreateFile(sPortName1, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			cout << "serial port does not exist.\n";
		}
		cout << "some other error occurred.\n";
	}
	setparm(hSerial);
	setparm(hSerial1);
	char data[] = "WinnerChikenDinner";
	DWORD dwSize = strlen(data);
	DWORD dwBytesWritten;
	newspeed(hSerial);
	BOOL iRet = WriteFile(hSerial, data, dwSize, &dwBytesWritten, NULL);
	cout << dwSize << " Bytes in string. " << dwBytesWritten << " Bytes sended. " << endl;
	ReadCOM();
	CloseHandle(hSerial);
	CloseHandle(hSerial1);
}