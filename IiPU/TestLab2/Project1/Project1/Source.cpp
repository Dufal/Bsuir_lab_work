#include <cstdio>
#include <stdio.h>
#include <windows.h>
#include <SetupAPI.h>
#include <devguid.h>
#include <regstr.h>
#include <locale>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int main()
{
	setlocale(LC_ALL, "RUS");
	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD i;
	hDevInfo = SetupDiGetClassDevs(NULL,
		REGSTR_KEY_PCIENUM,
		0,
		DIGCF_PRESENT | DIGCF_ALLCLASSES);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		return 1;
	}
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i,
		&DeviceInfoData); i++)
	{
		LPTSTR buffer = NULL;
		DWORD buffersize = 0;
		while (!SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DeviceInfoData,
			SPDRP_DEVICEDESC | SPDRP_HARDWAREID,
			0,
			(PBYTE)buffer,
			buffersize,
			&buffersize))
		{
			if (GetLastError() ==
				ERROR_INSUFFICIENT_BUFFER)
			{
				if (buffer) LocalFree(buffer);
				buffer = (LPTSTR)LocalAlloc(LPTR, buffersize * 2);
			}
			else
			{
				break;
			}
		}
		std::string deviceID;
		std::string vendorID;
		std::string Str = buffer;
		vendorID = Str.substr(8, 4);
		std::transform(vendorID.begin(), vendorID.end(), vendorID.begin(), tolower);
		deviceID = Str.substr(17, 4);
		std::transform(deviceID.begin(), deviceID.end(), deviceID.begin(), tolower);
		std::ifstream file("pci.ids");
		std::string line;
		for (;;)
		{
			std::getline(file, line);
			if (line.length() > 5)
			{
				if (line.substr(0, 4) == vendorID)
				{
					std::cout << "vendor: " + vendorID + " ";
					std::cout << line.substr(4, line.length() - 4) << std::endl;
					file.close();
					break;
				}
			}
		}
		file.open("pci.ids");
		for (;;)
		{
			if (line.length() > 5)
			{
				if (line.substr(1, 4) == deviceID) {
					std::cout << "\t device: " + deviceID + " ";
					std::cout << line.substr(5, line.length() - 5) << std::endl;
					file.close();
					break;
				}
			}
			std::getline(file, line);
		}
		if (buffer) LocalFree(buffer);
	}
	if (GetLastError() != NO_ERROR &&
		GetLastError() != ERROR_NO_MORE_ITEMS)
	{
		return 1;
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);
	system("pause");
	return 0;
}