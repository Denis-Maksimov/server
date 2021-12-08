#include "uhtml.h"
#include <iostream>
// #include <sysexits.h>
// #include <signal.h>

void simple_serviceFunction(uhtml* h, uhtml::usocket_t conn, const char* POST_JSON)
{
    nlohmann::json parsed=nlohmann::json::parse(POST_JSON);
    
    if (1==parsed.count("shutdown"))
    {
       
        if(parsed["shutdown"]==true)
        {
            h->terminate();
        };
    }
    
}




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
    srv.add_service("_sys_", simple_serviceFunction);

    while (!srv.check());

    std::cout<<"exiting\n";
    
    #if defined(_WIN32)
    WSACleanup();
    #endif
    return EXIT_SUCCESS;
}
