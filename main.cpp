#include "uhtml.h"
// #include <nlohmann/json.hpp>
#include <iostream>
// #include <sysexits.h>
// #include <signal.h>
int main(int argc, char const *argv[])
{
    #if defined(_WIN32)
    std::cout <<"vas\n";
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

    // for (size_t i = 0; i < 60; i++)
    // {
    //     srv.check();
    // }
    while (!srv.check());
    std::cout<<"exiting\n";
    
    #if defined(_WIN32)
    WSACleanup();
    #endif
    return EXIT_SUCCESS;
}
