#include "configure/version.h"
#ifdef  _VERSION_0_0
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
    if(this->schema_path.length()>0)
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
uservice::add_function(const char* name,void* data,  callback_t func)
{
    this->fdata=data;
    functions.insert(std::pair<const std::string, callback_t>(name,func));
}

void 
uservice::add_function(const char* name,  callback_t func)
{
    // this->fdata=data;
    functions.insert(std::pair<const std::string, callback_t>(name,func));
}
void 
uservice::call_function(const char* name, usrvNS::usocket_t conn, std::stringstream& POST_data)
{
    if (1==functions.count(name))
    {
        functions[name](conn,POST_data,this->fdata);
    }else{
        std::cerr<<"no function!";
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

#endif //v.0.0