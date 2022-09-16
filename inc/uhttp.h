#ifndef U_HTTP_H
#define U_HTTP_H
#include "configure/version.h"
#ifdef  _VERSION_0_0
#include "usrv.h"
#include <regex>
#include <unordered_map>

#if _FOR_VERSION(0,0)
class uhttp: public userver
{
private:
    std::regex header, remeta;
    typedef std::pair<std::string,std::string> pair_s;
    
    
protected:
    std::stringstream post;
    std::unordered_map<usrvNS::usocket_t,std::unordered_map<std::string,std::string>> meta;
    // std::unordered_map<std::string,std::string> meta;
    std::map<size_t,std::stringstream> codes;

    //generate content
    virtual void generate_html(usrvNS::usocket_t conn);
    void send_code(usrvNS::usocket_t conn,size_t code);
    void data_handle(usrvNS::usocket_t) override;
    void erase(usrvNS::usocket_t conn) override;
public:
    uhttp(uint16_t port = (uint16_t)8088U);
    ~uhttp();
};
#endif//v 0.0

#if _FOR_VERSION(0,1)
class uhttp: public userver
{
private:
    std::regex header, remeta;
    typedef std::pair<std::string,std::string> pair_s;
    
    
protected:
    std::stringstream post;
    std::unordered_map<usrvNS::usocket_t,std::unordered_map<std::string,std::string>> meta;
    // std::unordered_map<std::string,std::string> meta;
    std::map<size_t,std::stringstream> codes;

    //generate content
    virtual void generate_html(const connection& conn);
    void send_code(const connection& conn,size_t code);
    void data_handle(const connection&) override;
    void erase(usrvNS::usocket_t conn) override;
public:
    uhttp(uint16_t port = (uint16_t)8088U);
    ~uhttp();
};





#endif//v 0.1

#endif //_VERSION_0_0
#endif // U_HTTP_H
