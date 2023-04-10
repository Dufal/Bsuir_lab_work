#include <sys/types.h>
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include<iostream>
#include<vector>
#include<string>
#include <algorithm>
#pragma warning( disable : 4996)
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
using std::begin;
using std::end;
vector<int> parseInt(string a) {
    vector<int> result(a.length(), 0);
    for (int i = 0; i < a.length(); i++)
        result[i] = a[i] == '0' ? 0 : 1;
    return result;

}
 //char* toChar(vector<int>b)
//{
  //   vector<char*> vC;
    // transform(b.begin(), b.end(), std::back_inserter(vC), [](const int& i) { return '0' + i; });
//    return 0;
//}


// Divides and return the remainder
vector<int> divide(vector<int> data, vector<int> crc) {
    // Adding 0 to data
    vector<int> result = data;
    for (int i = 0; i < crc.size() - 1; i++)
        result.push_back(0);

    for (int i = 0; i <= result.size() - crc.size();) {
        // Using XOR for dividing
        for (int j = 0; j < crc.size(); j++)
            result[i + j] = result[i + j] ^ crc[j];

        // Move i to the next 0 for division again

        while (result[i] != 1) {
            i++;
            if (i == result.size())
                break;
        }
    }

    return vector<int>(result.end() - crc.size() + 1, result.end());
}
int main()
{


   
    while (1) {

        string temp;

        cout << "=======\nSender\n=======\n";
        cout << "Enter the CRC Code: ";
        cin >> temp;
        vector<int> crc = parseInt(temp);

        cout << "Enter the data: ";
        cin >> temp;
        vector<int> data = parseInt(temp);

        vector<int> rem = divide(data, crc);

        cout << "Remainder is: ";
        for (auto i : rem)
            cout << i;

        cout << "\nMessage to be transmitted: ";
        vector<int> trans = data;
        trans.insert(trans.end(), rem.begin(), rem.end());
        for (auto i : trans)
            cout << i;



        char sendMessage[1024], receiveMessage[1024];
        int sock, result;
        struct sockaddr_in serv_addr;
        struct hostent* host;

        //struct sockaddr_in serverAdd;

        host = gethostbyname("172.20.10.4");
        int iResult;

        // Initialize Winsock

        WSADATA wsaData;
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

        if (iResult != 0)
        {
            printf("WSAStartup failed: %d\n", iResult);
            return 1;
        }

        if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
        {
            perror("Socket creation failed");
            exit(1);
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(5000);

        //serverAdd.sin_addr = *((struct in_addr*)host->h_addr);

        memset(&(serv_addr.sin_zero), 8, sizeof(int));

        if (inet_pton(AF_INET, "172.20.10.4", &serv_addr.sin_addr) <= 0)
        {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }
        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) == -1)
        {
            perror("\nConnection failed");
            exit(1);
        }

        const char* g = nullptr;
        char* dat = new char[trans.size()];
        for (int i = 0; i < size(trans); i++)
        {
            dat[i] = trans[i] + '40';

        }

        g = dat;

        send(sock, g, trans.size(), 0);

        system("pause");
        cout << endl;
        continue;
    }


   /* while (1)
    {
        result = recv(sock, receiveMessage, 1024, 0);
        receiveMessage[result] = '\0';
        printf("\nRecieved Message: %s ", receiveMessage);
        printf("\nSEND The message: ");
        fgets(sendMessage, 512, stdin);
        send(sock, sendMessage, strlen(sendMessage), 0);
    }
    */

    return 0;
}


