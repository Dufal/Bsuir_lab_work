#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <Windows.h>
#include <winsock.h>
#include <signal.h>
//#include <netdb.h>
#include <ctime>
#include <fcntl.h>
#include "tokenRing.h"

using namespace std;

namespace tcp {
    string userID;

    char buffer[BUFFER_SIZE];
    int receiveSocket;
    int sendSocket;
    int senderSocket;
    int controlSocket;
    int newSenderSocket;
    int loggerSocket;
    sockaddr_in loggerAddress;

    int createAndBindSocket(int port);
    void connectSocketToIP(int socket, string ip, int port);
    int acceptSocket(int socket, sockaddr_in* address);
    string createNewClientMessage(int port);
    string createChangeDestinationMessage(string senderIP, string senderPort, string destinationIP, string destinationPort);
    string createMessage(string contents, int port);
    string createEmptyMessage(int port);
    string readMessage(int socket);
    void prepareLoggerSocket();
    string getMyIP();
    string getTime();
    void sendMessageAfterSleep(string message);
    void atexitHandler();
    void SIGINT_handler(int sig);

    sockaddr_in newAddress(string ip, int port) {
        sockaddr_in address;
        memset((char*)&address,256, ADDRESS_SIZE);
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = (ip.empty()) ? INADDR_ANY : inet_addr(ip.c_str());
        address.sin_port = (port == 0) ? 0 : htons(port);
        return address;
    }

    int createAndBindSocket(int port) {
        int newSocket = socket(AF_INET, SOCK_STREAM, 0);
        int option = 1;
        setsockopt(newSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        if (port < 0)
            perror("ERROR opening");

        sockaddr_in address = newAddress("", port);
        if (bind(newSocket, (struct sockaddr*)&address, ADDRESS_SIZE) < 0)
            perror("ERROR on binding");

        return newSocket;
    }

    void connectSocketToIP(int socket, string ip, int port) {
        sockaddr_in address = newAddress(ip, port);
        if (connect(socket, (struct sockaddr*)&address, ADDRESS_SIZE) < 0)
            perror("ERROR connecting no token send");
    }

    void atexitHandler() {
        closesocket(receiveSocket);
        closesocket(sendSocket);
        closesocket(senderSocket);
        closesocket(controlSocket);
        closesocket(newSenderSocket);
    }

    void SIGINT_handler(int sig) {
        atexitHandler();
        exit(0);
    }

    int acceptSocket(int socket, sockaddr_in* address) {
        ;
        socklen_t senderLength = sizeof(*address);
        int senderSocket = accept(socket, (sockaddr*)address, &senderLength);
        if (senderSocket < 0);
        //perror("ERROR on accept");
        //printf("server: got connection from %s port %d\n", inet_ntoa(address->sin_addr), ntohs(address->sin_port));
        return senderSocket;
    }

    string getTime() {
        time_t timer;
        time(&timer);
        string t = ctime(&timer);
        return t.substr(0, t.length() - 1);
    }

    string createNewClientMessage(int port) {
        string message = NEW_CLIENT + " " + getMyIP() + " " + to_string(port);
        return message;
    }

    string createChangeDestinationMessage(string senderIP, string senderPort, string destinationIP, string destinationPort) {
        return CHANGE_DESTINATION + " " + senderIP + " " + senderPort + " " + destinationIP + " " + destinationPort;
    }

    string getMyIP() {
        sockaddr_in myAddress;
        socklen_t myAddressLength = ADDRESS_SIZE;
        getsockname(sendSocket, (sockaddr*)&myAddress, &myAddressLength);
        return string(inet_ntoa(myAddress.sin_addr));
    }

    string createMessage(string contents, int port) {
        return MESSAGE + " " + getMyIP() + " " + to_string(port) + " " + userID + ": " + contents;
    }

    string readMessage(int socket) {
        memset(buffer,256, BUFFER_SIZE);
        int n = read(socket, buffer, BUFFER_SIZE);
        if (n < 0)
            perror("ERROR reading from socket");
        if (n == 0) {
            cerr << "Cannot connect to the neighbour - exiting";
            exit(1);
        }
        return string(buffer);

    }

    string createEmptyMessage(int port) {
        return EMPTY + " " + getMyIP() + " " + to_string(port);
    }

    void sendMessageAfterSleep(string message) {
        Sleep(SECONDS_TO_WAIT);
        if (sendto(loggerSocket, message.c_str(), message.length(), 0, (sockaddr*)&loggerAddress, ADDRESS_SIZE) < 0) {
            perror("Error sending to logger socket");
        }
        send(sendSocket, message.c_str(), message.length(), 0);
    }

    void prepareLoggerSocket() {
        loggerSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (loggerSocket < 0)
            perror("Cannot create logger scoket");

        memset((char*)&loggerAddress,256, ADDRESS_SIZE);
        loggerAddress.sin_family = AF_INET;
        loggerAddress.sin_addr.s_addr = inet_addr(LOGGER_IP.c_str());
        loggerAddress.sin_port = htons(LOGGER_PORT);
    }
}

using namespace tcp;

int runTCP(int argc, char** argv) {
    atexit(atexitHandler);
    signal(SIGINT, SIGINT_handler);

    int receivePort;

    userID = argv[1];
    receivePort = stoi(argv[2]);
    string sendIP = argv[3];
    int sendPort = stoi(argv[4]);
    bool hasToken = (bool)stoi(argv[5]);
    string protocol = argv[6];

    receiveSocket = createAndBindSocket(receivePort);
    sendSocket = createAndBindSocket(0);

    listen(receiveSocket, 2);

    sockaddr_in senderAddress;

    prepareLoggerSocket();


    if (hasToken) {
        senderSocket = acceptSocket(receiveSocket, &senderAddress);
        connectSocketToIP(sendSocket, sendIP, sendPort);

        readMessage(senderSocket);

        char* type = strtok(buffer, " ");
        if (strcmp(type, NEW_CLIENT.c_str()) == 0); //first message for first client discarded
        else {
            cerr << "First message was not of NEW_CLIENT type";
            exit(1);
        }

        string message = createMessage("Let's go!", receivePort);
        sendMessageAfterSleep(message);
    }

    else {
        connectSocketToIP(sendSocket, sendIP, sendPort);

        string message = createNewClientMessage(receivePort);
        sendMessageAfterSleep(message);

        senderSocket = acceptSocket(receiveSocket, &senderAddress);
    }

    if (fcntl(receiveSocket, F_SETFL, O_NONBLOCK) < 0) {
        perror("Could not set NONBLOCK flag");
        exit(1);
    }

    bool sentLastTime = false;
    bool needToChangeDestination = false;
    string changeDestinationMessage;

    while (true) {
        controlSocket = acceptSocket(receiveSocket, &senderAddress); //checking if a new client wants to join
        if (controlSocket > 0) {
            string message = readMessage(controlSocket);
            char* type = strtok(buffer, " ");
            cerr << message << endl;
            if (strcmp(type, NEW_CLIENT.c_str()) == 0) {
                string ip = strtok(NULL, " ");
                string port = strtok(NULL, " ");
                changeDestinationMessage = createChangeDestinationMessage(getMyIP(), to_string(receivePort), ip, port);
                needToChangeDestination = true;
                newSenderSocket = controlSocket;
            }
            else {
                cerr << "First message from client is not of type NEW_CLIENT";
            }
        }

        string message = readMessage(senderSocket);
        cerr << message << endl;

        char* type = strtok(buffer, " ");
        string ip = strtok(NULL, " ");
        string port = strtok(NULL, " ");

        if (strcmp(type, EMPTY.c_str()) == 0) { //if I got an empty token I can send own message
            if (needToChangeDestination) { //if a new client wanted to join earlier
                senderSocket = newSenderSocket;
                sendMessageAfterSleep(changeDestinationMessage);
                needToChangeDestination = false;
            }
            else if (!sentLastTime) { //I didn't send last time - it's ok
                message = createMessage(getTime(), receivePort);
                sentLastTime = true;
                sendMessageAfterSleep(message);
            }
            else { //I sent last time so I can't now - avoiding starvation
                message = createEmptyMessage(receivePort);
                sentLastTime = false;
                sendMessageAfterSleep(message);
            }

        }

        else if (ip.compare(getMyIP()) == 0 && atoi(port.c_str()) == receivePort) { //received message I had sent earlier
            if (needToChangeDestination) { //ditto
                senderSocket = newSenderSocket;
                sendMessageAfterSleep(changeDestinationMessage);
                needToChangeDestination = false;
            }
            else if (!sentLastTime) { //ditto
                message = createMessage(getTime(), receivePort);
                sentLastTime = true;
                sendMessageAfterSleep(message);
            }
            else { //ditto
                message = createEmptyMessage(receivePort);
                sentLastTime = false;
                sendMessageAfterSleep(message);
            }
        }

        else if (strcmp(type, CHANGE_DESTINATION.c_str()) == 0) {
            string newIP = strtok(NULL, " ");
            string newPort = strtok(NULL, " ");
            if (ip == sendIP && port == (to_string(sendPort))) { //if the message is for me (sender socket matches) I need to change client I send to
                sendIP = newIP;
                sendPort = stoi(newPort);
                closesocket(sendSocket);
                sendSocket = createAndBindSocket(0);
                connectSocketToIP(sendSocket, newIP, atoi(newPort.c_str()));
                message = createMessage(getTime(), receivePort);
                sendMessageAfterSleep(message);
            }
            else {
                sendMessageAfterSleep(message);
            }
        }

        else { //Just passing the message further
            sendMessageAfterSleep(message);
        }
    }

}