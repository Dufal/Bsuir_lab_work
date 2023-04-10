#include <windows.h> 
#include <windows.h> 
#include <iostream> 
#include <Tchar.h> 
#include <conio.h> 
#include <string.h> 
using namespace std;
HANDLE hSerial;
HANDLE hSerial1;
typedef unsigned char UCHAR;

bool GetBit(UCHAR ucBuffer, int nBitShift);
void SetBit(UCHAR& ucBuffer, int nBitShift, bool bSetTo);
void ShowBits(const UCHAR ucBuffer);
UCHAR* BitStuff(const UCHAR* pucBuffer, const size_t nSize);
UCHAR* DeBitStuff(const UCHAR* pucBuffer, const size_t nSize);
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
	cout << "Youre default speed is: " << dcbSerialParams.BaudRate << "\n";
	dcbSerialParams.BaudRate = n;
	if (!SetCommState(PORT, &dcbSerialParams))
	{
		cout << "error setting serial port state\n";
	}
	cout << "Com-Port speed is: " << dcbSerialParams.BaudRate << "\n";
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

	ReadFile(hSerial1, &sReceivedChar, 1, &iSize, 0);
	if (iSize > 0 && sReceivedChar != '|')
		cout << sReceivedChar << endl;

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
	char a[2];
	a[1] = '|';
	DWORD dwSize = sizeof(a);
	DWORD dwBytesWritten;
	newspeed(hSerial);
	////////////
	UCHAR* pucBitStuf;
	UCHAR pucBuffer[15];
	memset(pucBuffer, '\0', sizeof(UCHAR) * 15);


	//##### BitStuffing
	cout << "*** Bitstaffing ***" << endl;

	pucBitStuf = BitStuff(pucBuffer, sizeof(pucBuffer));

	for (int i = 0; i < sizeof(data); i++)
	{
		cout << "Buffer     : "; ShowBits(data[i]);
		cout << "BitStuffed : "; ShowBits(data[i]);
	}

	

	free(pucBitStuf);
	

	_getch();
	
//////////////

	for (int i = 0; i < 2 * sizeof(data)-2; i++)
	{

		a[0] = data[i];
		//a[1] = '~';
		BOOL iRet = WriteFile(hSerial, a, dwSize, &dwBytesWritten, NULL);
		if (i%2==0) {
			cout << dwSize << " Bytes in string. " << dwBytesWritten << " Bytes sended. ";
		}
		ReadCOM();
		
	}
	//ReadCOM(); 
	CloseHandle(hSerial);
	CloseHandle(hSerial1);
}
UCHAR* BitStuff(const UCHAR* pucBuffer, const size_t nSize)
{
	size_t nLenght = nSize / sizeof(*pucBuffer); // Lenght of an array
	UCHAR* pucResult = (UCHAR*)malloc(nSize); //Allocate memory for new Bitstaffed array
	memset(pucResult, '\0', nSize);


	int nUnits = 0; //Counter of units  
	int nBitCounterBuf = 0; // Bit counter for buffer
	int nBitCounterRes = 0; // Bit counter for result
	int nElementBuf = 0; // Element counter for buffer array
	int nElementRes = 0; // Element counter for result array

	while (nElementBuf < nLenght)
	{


		// Set bit in result to bit from buffer
		SetBit(pucResult[nElementRes], (nBitCounterRes), GetBit(pucBuffer[nElementBuf], nBitCounterBuf));

		// If bit is '1', then increment counter of units
		if (GetBit(pucResult[nElementRes], nBitCounterRes))
		{
			nUnits++;
		}
		else
		{
			nUnits = 0;
		}

		// If counter of units counts 5 units, then let's add '0' after them
		if (nUnits == 5)
		{
			// If here's not last bit in this element of array, then add '0' to a next bit
			if (nBitCounterRes < 7)
			{
				nBitCounterRes++; SetBit(pucResult[nElementRes], nBitCounterRes, 0);
			}
			else if (nBitCounterRes == 7) // If here's last bit, then add '0' to a next element of array
			{
				nBitCounterRes = 0;
				nElementRes++;

				SetBit(pucResult[nElementRes], nBitCounterRes, 0);
			}

			// After adding '0' set units counter to 0
			nUnits = 0;
		}

		if (nBitCounterBuf == 7) // Counter for bits in buffer
		{
			nElementBuf++; nBitCounterBuf = 0;
		} // Element in buffer array
		else
			nBitCounterBuf++; // Increment our bit counter for buffer

		if (nBitCounterRes == 7) // Counter for bits in result array
		{
			nElementRes++; nBitCounterRes = 0;
		} // Element in result array
		else
			nBitCounterRes++; // Increment our bit counter for result
	}

	return pucResult;
}


bool GetBit(UCHAR cBuffer, int nBitShift)
{
	return cBuffer & (1U << nBitShift);
}

void SetBit(UCHAR& cBuffer, int nBitShift, bool bSetTo)
{
	if (bSetTo)
	{
		cBuffer = cBuffer | (1U << nBitShift);
	}
	else
	{
		cBuffer = cBuffer & ~(1U << nBitShift);
	}
}

void ShowBits(const UCHAR ucBuffer)
{
	for (int i = 0; i < 8; i++)
		cout << GetBit(ucBuffer, (7 - i));

	cout << endl;
}