#ifndef USERVICE
#define USERVICE
#include <functional>
#include "usrv.h"
#include "json.hpp"
#include <fstream>

class uservice
{
private:
    typedef std::function< void(userver::usocket_t conn, const char* POST_data_parsed_to_JSON)> callback_t;
    callback_t callback;
    std::string schema_path;
    nlohmann::json schema; //schema.json
    std::unordered_map< std::string,  callback_t > functions;
public:
    uservice(/* args */);
    ~uservice();
    void update_schema();
    void update_schema(const char* path);
    void set_schema(nlohmann::json Schema);
    void add_function(const char* name,  callback_t func);
    void call_function(const char* name, userver::usocket_t conn, const char* POST_data);
    nlohmann::json& get_schema();
};


#endif /* USERVICE */
