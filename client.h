#include "usrv.h"
#include <regex>
#include <unordered_map>


class client:userver
{
private:
    std::regex header, remeta;
    typedef std::pair<std::string,std::string> pair_s;
protected:
    void data_handle(usocket_t) override;
    void erase(usocket_t conn) override;
    std::stringstream post;
    std::unordered_map<usocket_t,std::unordered_map<std::string,std::string>> meta;

    void handle_request(usocket_t conn);
public:
    client(uint16_t port);
    ~client();

    template <typename T>
    void send_GET(T addr, uint16_t port, T req);
};


template <> void client::send_GET(std::string&, uint16_t,std::string&);
template <> void client::send_GET(const char* const, uint16_t,const char* const);
