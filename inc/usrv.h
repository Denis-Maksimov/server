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

#if _FOR_VERSION(0,0)
namespace usrvNS
{
#if defined(_WIN32)
typedef SOCKET usocket_t;
#else
typedef int usocket_t;
#define INVALID_SOCKET -1
#endif
}
#endif

#ifdef  _VERSION_0_1
/****************************************************
* @name socketDeleteFunctor
* @author Maksimov Denis (dn.maksimow@gmail.com)
* @brief интерфейс для освобождения памяти unique_ptr
* @details description
****************************************************/
#include <memory>
namespace usrvNS
{
    const size_t TCP_maxlen=65535;
    #if defined(_WIN32)
    typedef SOCKET usocket_t;
    #else
    typedef int usocket_t;
    #define INVALID_SOCKET -1
    #endif

    struct socketDeleter
    {
        void
        operator()(usocket_t* instance)
        {
            // delete instance
            if(INVALID_SOCKET!=(*instance))
            {
                shutdown(*instance,SHUT_RDWR);
            }
            delete instance;
            
        }

    };
}

using socketUniquePtr=std::unique_ptr<usrvNS::usocket_t,usrvNS::socketDeleter>;
template <class ...Args>
std::shared_ptr<usrvNS::usocket_t> makeSocketSharedPtr(usrvNS::usocket_t* inst)
{
    return std::shared_ptr<usrvNS::usocket_t>(inst,usrvNS::socketDeleter{});
}
using socketSharedPtr=std::shared_ptr<usrvNS::usocket_t>;
//-------------------------------------------------
#endif//  _VERSION_0_1



#if _FOR_VERSION(0,0)
class userver
{
 
public:
    
protected:
    typedef union
    {
        sockaddr_in sa_in;
        sockaddr sa;
        
    }usockaddr;
    
    usrvNS::usocket_t srv_sock =INVALID_SOCKET;

    std::set<usrvNS::usocket_t> inputs;
    std::set<usrvNS::usocket_t> outputs;
    std::unordered_map<usrvNS::usocket_t,std::stringstream> messages;

    struct timeval timeout={.tv_sec=1,.tv_usec=0};

    void accept_handle(usrvNS::usocket_t);
    void read_handle(usrvNS::usocket_t);
    void write_handle(usrvNS::usocket_t);
    virtual void data_handle(usrvNS::usocket_t);
    virtual void erase(usrvNS::usocket_t);
    bool terminate_req=false;
    bool is_cli;
public:
    void terminate();
    userver(uint16_t port=8088,bool is_cli=false);
    ~userver();
    bool check(void);
};
#endif

#if _FOR_VERSION(0,1)
#include "common/except.hpp"
// #error не готово ещё

class wsa2_lib
{
private:
    wsa2_lib()
    {
    #if defined(_WIN32)
        printf("Startup  \n");
        WSADATA wsaData;
        int iResult;
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) 
        {
            printf("WSAStartup failed: %d\n", iResult);
            std::terminate();
        }
    #endif
    }
    wsa2_lib(std::initializer_list<int>) = delete;
    wsa2_lib(wsa2_lib &&) = delete;
    wsa2_lib(const wsa2_lib &) = delete;
    wsa2_lib &operator=(wsa2_lib &&) = delete;
    wsa2_lib &operator=(const wsa2_lib &) = delete;
    ~wsa2_lib()
    {
    #if defined(_WIN32)
        WSACleanup();
    #endif
    };
public:
    static wsa2_lib& 
    instance(void)
    {
        static wsa2_lib obj;
        return obj;
    }

    static void
    get_last_err()
    {
        #if defined(_WIN32)
        int err;
        CHAR msgbuf[256]; // for a message up to 255 bytes.
        msgbuf[0] = '\0'; // Microsoft doesn't guarantee this on man page.
        err = WSAGetLastError();
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // flags
                      NULL,                                                       // lpsource
                      err,                                                        // message id
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),                  // languageid
                      msgbuf,                                                     // output buffer
                      sizeof(msgbuf),                                             // size of msgbuf, bytes
                      NULL);                                                      // va_list of arguments

        std::cout<<"err"<<err<<"::"<< msgbuf<<std::endl;
        #endif
    }
};




class connection
{
public:

    #if defined(_WIN32)
    enum rwe_type
    {
        none,
        read=SD_RECEIVE,
        write=SD_SEND,
        both=SD_BOTH
    };
    #else
    enum rwe_type
    {
        none,
        read=SHUT_RD,
        write=SHUT_WR,
        both=SHUT_RDWR
    };
     #endif

    connection(rwe_type t=both);//TODO
    connection(usrvNS::usocket_t s,rwe_type t=both);
    // connection(std::initializer_list<int>) = delete;
    connection(connection &&);
    connection(const connection &);
    connection& operator=(connection &&);
    connection& operator=(usrvNS::usocket_t);
    connection& operator=(const connection &) = delete;

    bool operator<(const connection&);
    //write
    const connection& operator<<(const char* b);

    //read
    std::stringstream operator>>(connection&);

    // operator<<
    usrvNS::usocket_t& operator *();
    // usrvNS::usocket_t operator *();
    usrvNS::usocket_t operator *() const;
    // usrvNS::usocket_t operator *(const connection &);
    ~connection();
    void set_type(rwe_type t);
private:
    /*data*/
    rwe_type type;
    std::unique_ptr<usrvNS::usocket_t> socket_ptr;
    // usrvNS::usocket_t sock;
    std::stringstream data;//???unused???
};
#include <iterator>
std::ostream& operator<<(std::ostream& os, const connection& c);
std::istream& operator>>(std::istream& is, const connection& c);
std::istream& operator>>(const char* is, const connection& c);
// std::istream& operator>>(const char is[], const connection& c);
bool operator<(const connection&,const connection&);
class userver
{
 
public:
    
protected:
    typedef union
    {
        sockaddr_in sa_in;
        sockaddr sa;
        
    }usockaddr;
    
    // usrvNS::usocket_t srv_sock =INVALID_SOCKET;
    connection srv_end_point;
    // socketUniquePtr srv_sock;
    std::set<connection> inputs;
    std::set<connection> outputs;
    std::unordered_map<usrvNS::usocket_t,std::stringstream> messages;

    struct timeval timeout={.tv_sec=1,.tv_usec=0};

    void accept_handle(const connection&);//+TODO
    void read_handle(const connection&);//TODO
    void write_handle(const connection&);//TODO
    virtual void data_handle(const connection&);//TODO
    virtual void erase(usrvNS::usocket_t)=delete;//TODO
    bool terminate_req=false;
    bool is_cli;
public:
    void terminate();//TODO
    userver(uint16_t port=8088,bool is_cli=false);
    ~userver();//+TODO
    bool check(void);//+TODO
private:
    void bind_address_(const char* addr,uint16_t port);
};



#endif //version 0.1
#endif //version 0.0
#endif
