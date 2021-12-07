#include "uhtml.h"
#include <iostream>
// #include <sysexits.h>
// #include <signal.h>
int main(int argc, char const *argv[])
{
    #if defined(_WIN32)
        WSADATA wsaData;
        int iResult;
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) 
        {
            printf("WSAStartup failed: %d\n", iResult);
            return 1;
        }
    #endif
    
    uhtml srv(8081);

    while (!srv.check());

    std::cout<<"exiting\n";
    
    #if defined(_WIN32)
    WSACleanup();
    #endif
    return EXIT_SUCCESS;
}
