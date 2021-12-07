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
uhtml::generate_html(usocket_t conn)
{
    // if parsed_url in self.file_pages:
    std::cout <<"meta host" <<this->meta[conn]["Host"]<<std::endl;
    if (json_obj.find(this->meta[conn]["Host"]) != json_obj.end()) 
    {
        // std::cout<<"find"<<json_obj[this->meta[conn]["Host"]]["code"]<<std::endl;
        this->send_code(conn,json_obj[this->meta[conn]["Host"]]["code"]);
        auto file_path=json_obj[this->meta[conn]["Host"]]["path"].get<std::string>();

        std::ifstream f(file_path,std::ios::binary);
        f.seekg(0,std::ios_base::end);
        auto size= f.tellg();
        f.seekg(0,std::ios_base::beg);

        std::stringstream _meta;
        _meta<<"Content-Length:"<<size<<std::endl;
        _meta<<"server:denisx"<<std::endl;
        _meta<<std::endl;
        send(conn,_meta.str().c_str(),_meta.str().length(),0);

        this->parse_post();
        
        char buffer[1500]={0};
        while (f)
        {
            
            f.read(buffer,1500);
            size_t cnt=f.gcount();
            if(!cnt) break;
            // std::cout<<"sandy"<<std::endl;
            send(conn,buffer,cnt,0);
        }
        f.close();
        
    }else{
    this->send_code(conn,404);
    std::string content="since I'm a teapot so I cannot brew coffee.. So sorry((\n";
    std::stringstream meta;
    meta<<"Content-Length:"<<content.length()<<std::endl;
    meta<<std::endl;
    send(conn,meta.str().c_str(),meta.str().length(),0);
    send(conn,content.c_str(),content.length(),0);
    std::cout <<"since I'm a teapot so I cannot brew coffee.. So sorry((\n";
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

