#include "uhtml.h"
#include <fstream>

uhtml::uhtml(uint16_t port):uhttp(port)
{
    this->update_schema("schema.json");
}

void 
uhtml::update_schema(const char* path)
{
    std::ifstream f;
    f.open(path);
    std::stringstream ss;
    ss<< f.rdbuf();

    json_obj =nlohmann::json::parse(ss.str());
    f.close();
    std::cout <<json_obj<<std::endl;
}
void 
uhtml::update_schema()
{
    this->update_schema("schema.json");
}

uhtml::~uhtml()
{
}

void 
send_file_from_json(uhtml* h,uhtml::usocket_t conn, std::string& host)
{
        // TODO: здесь должна быть проверка входных данных
        //отправляем статус (например 200 OK\n)
        h->send_code(conn, h->json_obj[host]["code"]);

        //ищем в schema.json путь к файлу
        auto file_path= h->json_obj[host]["path"].get<std::string>();

        //Открываем файл 
        std::ifstream f(file_path,std::ios::binary);

        //Считаем размер
        f.seekg(0,std::ios_base::end);
        auto size= f.tellg();
        f.seekg(0,std::ios_base::beg);

        //Заполняем поля header-а
        std::stringstream _meta;
        _meta<<"Content-Length:"<<size<<std::endl;
        _meta<<"server:denisx"<<std::endl;
        _meta<<std::endl;
        send(conn,_meta.str().c_str(),_meta.str().length(),0);

        // TODO: это костыль
        // h->parse_post();
        
        //читаем файл и отправляем кусками размером MTU (1500)
        char buffer[1500]={0};
        while (f)
        {
            f.read(buffer,1500);
            size_t cnt=f.gcount();
            if(!cnt) break;
            send(conn,buffer,cnt,0);
        }
        f.close();
}

void 
send_file_from_json(uhtml* h,uhtml::usocket_t conn, const char* host)
{
    std::string _host=host;
    send_file_from_json(h, conn, _host);
}

void 
uhtml::generate_html(usocket_t conn)
{
    
    // std::cout <<"meta host:" <<this->meta[conn]["Host"]<<std::endl;
    // if parsed_url in self.file_pages:
    if (json_obj.find(this->meta[conn]["Host"]) != json_obj.end()) 
    {
        send_file_from_json(this,conn, this->meta[conn]["Host"]);
    }else{
        send_file_from_json(this,conn, "404");
    }

}

void 
uhtml::parse_post()
{
    if (this->post.str()=="terminate")
    {
        this->terminate_req=true;
    }
    else if (this->post.str()=="update_schema")
    {
        this->update_schema();
    }
 
}

void 
uhtml::add_service(std::string& name,  uhtml::_serviceFunction func)
{
    services.insert(std::pair<const std::string, uhtml::_serviceFunction>(name,func));
}

void 
uhtml::add_service(const char* name,  uhtml::_serviceFunction func)
{
    std::string _name=name;
    this->add_service(_name,  func);
}


void simple_serviceFunction(uhtml* h, int conn, nlohmann::json POST_data_parsed_to_JSON)
{
    // send_file_from_json(h, conn, "404");
    if(POST_data_parsed_to_JSON["shutdown"]==true)
    {
        h->terminate();
    };
}
