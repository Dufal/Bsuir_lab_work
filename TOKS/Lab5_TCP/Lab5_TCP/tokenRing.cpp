#include <iostream>
#include "tokenRing.h"


int main(int argc, char** argv) {
    if (argc < 6) {
        cerr << "Not enough arguments";
        return 0;
    }

    string protocol(argv[6]);

    if (protocol == "TCP") {
        runTCP(argc, argv);
    }

    else if (protocol == "UDP") {
        runUDP(argc, argv);
    }

    else
        cerr << "Protocol is not supported";
}