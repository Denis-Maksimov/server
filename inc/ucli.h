#ifndef UCLI
#define UCLI
#include "configure/version.h"
#ifdef  _VERSION_0_0
#include "usrv.h"
#include <regex>
#include <unordered_map>
#include "json.hpp"
#include <functional>

class ucli: public userver
{
private:
    std::regex header, remeta;
    typedef std::pair<std::string,std::string> pair_s;
    std::function<void(int)> cb;
protected:
    std::stringstream post;
    std::unordered_map<usrvNS::usocket_t,std::unordered_map<std::string,std::string>> meta;
    // std::unordered_map<std::string,std::string> meta;
    std::map<size_t,std::stringstream> codes;

    //generate content
    virtual void handle_html(usrvNS::usocket_t conn);
    void send_code(usrvNS::usocket_t conn,size_t code);
    void data_handle(usrvNS::usocket_t) override;
    void erase(usrvNS::usocket_t conn) override;
public:
    ucli(uint16_t port = (uint16_t)8088U);
    ~ucli();
    void send_GET(std::string url,uint16_t port);
    void request(std::string url,std::stringstream& data,std::function<void(usrvNS::usocket_t)> f);
    void request(std::string url,std::string data,std::function<void(usrvNS::usocket_t)> f);
};


#endif
#endif /* UCLI */
