#include "uservice.h"

uservice::uservice(/* args */)
{
}

uservice::~uservice()
{
}

void 
uservice::update_schema()
{
    this->update_schema(this->schema_path.c_str());
}

void 
uservice::update_schema(const char* path)
{
    std::ifstream f;
    f.open(path);
    std::stringstream ss;
    ss<< f.rdbuf();

    schema =nlohmann::json::parse(ss.str());
    f.close();
    std::cout <<schema<<std::endl;
}


void 
uservice::add_function(const char* name,  callback_t func)
{
    functions.insert(std::pair<const std::string, callback_t>(name,func));
}

void 
uservice::call_function(const char* name, userver::usocket_t conn, const char* POST_data)
{
    if (1==functions.count(name))
    {
        functions[name](conn,POST_data);
    }
    
}

nlohmann::json& 
uservice::get_schema()
{
    return this->schema;
}

void 
uservice::set_schema(nlohmann::json Schema)
{
    this->schema=Schema;
}
