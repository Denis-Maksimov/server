#ifndef USRV_H
#define USRV_H
#include "configure/version.h"
#ifdef  _VERSION_0_0

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#define SHUT_RDWR SD_BOTH
#else
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <set>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>

#if defined(_WIN32)
#define WSASTART do{\
    WSADATA wsaData; \
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)\
    {printf("WSAStartup failed\n");\
    exit(1);}\
}while(0)
#else
#define WSASTART
#endif


class userver
{

public:
    #if defined(_WIN32)
    typedef SOCKET usocket_t;
    #else
    typedef int usocket_t;
    #define INVALID_SOCKET -1
    #endif
protected:
    typedef union
    {
        sockaddr_in sa_in;
        sockaddr sa;
        
    }usockaddr;
    
    usocket_t srv_sock =INVALID_SOCKET;

    std::set<usocket_t> inputs;
    std::set<usocket_t> outputs;
    std::unordered_map<usocket_t,std::stringstream> messages;

    struct timeval timeout={.tv_sec=1,.tv_usec=0};

    void accept_handle(usocket_t);
    void read_handle(usocket_t);
    void write_handle(usocket_t);
    virtual void data_handle(usocket_t);
    virtual void erase(usocket_t);
    bool terminate_req=false;
    bool is_cli;
public:
    void terminate();
    userver(uint16_t port=8088,bool is_cli=false);
    ~userver();
    bool check(void);
};

#endif //version 0.0
#endif
