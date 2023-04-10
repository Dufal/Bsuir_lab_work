#include "Device.h"

#include <string>
#include <iostream>

Device::Device() {
    pwsaResults = (LPWSAQUERYSET)buffer;
    pwsaResults->dwNameSpace = NS_BTH;
    pwsaResults->dwSize = sizeof(WSAQUERYSET);
}

INT Device::getDeviceInfo(HANDLE hLookup, DWORD dwControlFlags) {
    INT res = WSALookupServiceNext(hLookup, dwControlFlags, &swSize, pwsaResults);
    if (res == NO_ERROR) {
        CopyMemory(&pBtSockRemote,
            (PSOCKADDR_BTH)pwsaResults->lpcsaBuffer->RemoteAddr.lpSockaddr,
            sizeof(pBtSockRemote));
    }
    else {
        if (GetLastError() != WSA_E_NO_MORE)
            wprintf(L"WSALookupServiceNext failed with error: %d\n", WSAGetLastError());
    }

    return res;
}

void Device::printDeviceInfo() {
    wprintf(L"Device name: %s\n", pwsaResults->lpszServiceInstanceName);
    if (pwsaResults->dwOutputFlags & BTHNS_RESULT_DEVICE_CONNECTED) {
        wprintf(L"Device connected: yes\n");
    }
    else {
        wprintf(L"Device connected: no\n");
    }

    if ((pwsaResults->dwOutputFlags & BTHNS_RESULT_DEVICE_REMEMBERED) > 0) {
        wprintf(L"Device remembered: yes\n");
    }
    else {
        wprintf(L"Device remembered: no\n");
    }

    if ((pwsaResults->dwOutputFlags & BTHNS_RESULT_DEVICE_AUTHENTICATED) > 0) {
        wprintf(L"Device authenticated: yes\n");
    }
    else {
        wprintf(L"Device authenticated: no\n");
    }

    wprintf(L"Remote Bluetooth device address: 0x%04X%08X\n",
            GET_NAP(pBtSockRemote.btAddr), GET_SAP(pBtSockRemote.btAddr));
}
