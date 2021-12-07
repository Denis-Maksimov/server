#ifndef USRV_H
#define USRV_H
#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#define SHUT_RDWR SD_BOTH
#else
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include <set>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>



class userver
{
protected:
    #if defined(_WIN32)
    typedef SOCKET usocket_t;
    #else
    typedef int usocket_t;
    #endif
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
public:
    
    userver(uint16_t port=8088);
    ~userver();
    bool check(void);
};

#endif
