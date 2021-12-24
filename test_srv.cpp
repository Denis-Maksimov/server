#include "uhtml.h"
#include <iostream>
#include "ctrlPage/serverCtrlPage.h"
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
    
    srv.add_service("_sys_", simple_serviceFunction);

    //====================================
    uservice srv2;
    srv2.set_schema(R"({
        "ctrl2":
        {
            "path":"ctrlPage/serverCtrlPage.html",
            "code":200,
            "function":"_sys_",
            "headers":
            {
                "content-type":"text/html"
            }
        }
    })"_json);
    srv2.add_function("_sys_",[](userver::usocket_t conn, const char* post){
        std::cout<<"this is SPARTA!!!\n";
    });
    srv.add_service("service_name",&srv2);
    //====================================

    while (!srv.check());

    std::cout<<"exiting\n";
    
    #if defined(_WIN32)
    WSACleanup();
    #endif
    return EXIT_SUCCESS;
}
