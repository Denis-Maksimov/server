#ifndef USERVICE
#define USERVICE
#include "configure/version.h"
#ifdef  _VERSION_0_0
#include <functional>
#include "usrv.h"
#include "json.hpp"
#include <fstream>

class uservice
{
private:
    typedef std::function< void(usrvNS::usocket_t conn, std::stringstream& POST_data_parsed_to_JSON,void*)> callback_t;
    callback_t callback;
    std::string schema_path;
    nlohmann::json schema; //schema.json
    std::unordered_map< std::string,  callback_t > functions;
    void* fdata;
public:
    uservice(/* args */);
    ~uservice();
    void update_schema();
    void update_schema(const char* path);
    void set_schema(nlohmann::json Schema);
    void add_function(const char* name,void* data,  callback_t func);
    void add_function(const char* name,  callback_t func);
    void call_function(const char* name, usrvNS::usocket_t conn, std::stringstream& POST_data);
    nlohmann::json& get_schema();
};

#endif //v.0.0
#endif /* USERVICE */
