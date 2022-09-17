#include "uhtml.h"
#include <iostream>
#include "ctrlPage/serverCtrlPage.h"
// #include <sysexits.h>
// #include <signal.h>

#if _FOR_VERSION(0,0)

int main(int argc, char const *argv[])
{
    
    fflush(stdout);
    #if defined(_WIN32)
    printf("Startup  \n");
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
    
    uhtml server(8081);
    
    // srv.add_service("_sys_", simple_serviceFunction);

    //====================================
    uservice* srv2=example(&server);
    // srv2.set_schema(R"({
    //     "ctrl":
    //     {
    //         "path":"ctrlPage/serverCtrlPage.html",
    //         "code":200,
    //         "function":"_sys_",
    //         "headers":
    //         {
    //             "content-type":"text/html"
    //         }
    //     }
    // })"_json);


    server.add_service("service_name",srv2);
    //====================================

    while (!server.check());
    delete srv2;
    std::cout<<"exiting\n";
    
    #if defined(_WIN32)
    WSACleanup();
    #endif
    return EXIT_SUCCESS;
}

#endif


int main(int argc, char const *argv[])
{
    
   
    
    uhtml server(8081);
    
    // srv.add_service("_sys_", simple_serviceFunction);

    //====================================
    uservice* srv2=example(&server);
    // srv2.set_schema(R"({
    //     "ctrl":
    //     {
    //         "path":"ctrlPage/serverCtrlPage.html",
    //         "code":200,
    //         "function":"_sys_",
    //         "headers":
    //         {
    //             "content-type":"text/html"
    //         }
    //     }
    // })"_json);


    server.add_service("service_name",srv2);
    //====================================

    while (!server.check());
    delete srv2;
    std::cout<<"exiting\n";
    

    return EXIT_SUCCESS;
}