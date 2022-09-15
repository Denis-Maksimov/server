#include "uhtml.h"
#include <iostream>
#include "ctrlPage/serverCtrlPage.h"
#include "ucli.h"
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
    
    // uhtml srv(8081);
    ucli srv(8083);
    // srv.add_service("_sys_", simple_serviceFunction);
    // srv.send_GET("127.0.0.1",8081);
    nlohmann::json js=R"(
            {
                "shutdown": true
            }
        )"_json;

    srv.request("http://127.0.0.1:8081/_sys_",js.dump(),[](ucli::usocket_t s){
        std::cout <<"callback\n";
    });

    for (size_t i = 1; i < 10 && !srv.check() ; ++i);
    

    std::cout<<"exiting\n";
    
    #if defined(_WIN32)
    WSACleanup();
    #endif
    return EXIT_SUCCESS;
}
