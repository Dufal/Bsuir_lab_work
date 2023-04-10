#include <iostream>
#include <windows.h>
#include <powrprof.h>
#include<ioapiset.h>
#include <conio.h>
#pragma comment(lib, "PowrProf.lib")
using namespace std;
int main() 
{
	while (1) {
		system("cls");
		std::cout << "**INFORMATION ABOUT BATTARY CHARGE**" << endl;
		SYSTEM_POWER_STATUS a;
		GetSystemPowerStatus(&a);
		int b;
		int wait = 4;
		std::cout << "1.Computer power supply in real time: ";
		switch ((int)a.ACLineStatus) {
		case 0: cout << "offline" << endl;
			break;
		case 1: cout << "online" << endl;
			break;
		}

		cout << "2.Battery charge level(%): " << (int)a.BatteryLifePercent << endl;
		std::cout << "3.Current power saving mode: ";
		switch ((int)a.SystemStatusFlag) {
		case 1: cout << "Econom" << endl;
			break;
		case 0: cout << "Ne econom" << endl;
			break;
		}
		std::cout << "4.Battery life without charging connection:" << (int)a.BatteryLifeTime / 3600 << endl;
		

		std::cout << "5.Starting (1)sleep mode and (2)hibernation: (input number) ";
		if (_kbhit()) {
			cin >> b;
			switch (b) {
			case 1: SetSuspendState(false, true, false);
				break;
			case 2: SetSuspendState(true, false, false);
				break;
			}
		}
		Sleep(1000);
		--wait;
		if (wait == 0) {
			continue;
		}
		
		

	}
}