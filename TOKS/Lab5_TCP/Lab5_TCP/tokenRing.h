#include <string>
#include <windows.h>

#ifndef UNTITLED_CONSTANTS_H
#define UNTITLED_CONSTANTS_H

using namespace std;

const string LOGGER_IP = "225.0.0.1";
const int LOGGER_PORT = 6666;
const string NEW_CLIENT = "NEW_CLIENT";
const string CHANGE_DESTINATION = "CHANGE_DESTINATION";
const string EMPTY = "EMPTY";
const string MESSAGE = "MESSAGE";
const int SECONDS_TO_WAIT = 1;
const size_t ADDRESS_SIZE = sizeof(sockaddr_in);
const int BUFFER_SIZE = 256;

int runTCP(int argc, char** argv);
int runUDP(int argc, char** argv);

#endif //UNTITLED_CONSTANTS_H