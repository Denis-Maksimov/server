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
    for (auto &&i : this->Services)
    {
        i.second->update_schema();
    }
    
}

uhtml::~uhtml()
{
}

void 
send_file_from_json(uhtml* h,uhtml::usocket_t conn, std::string& host)
{
        // TODO: здесь должна быть проверка входных данных
        // //отправляем статус (например 200 OK\n)
        // h->send_code(conn, h->json_obj[host]["code"]);


        if (h->json_obj[host].find("path")!=h->json_obj[host].cend())
        {
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

    // if parsed_url in self.file_pages:
    auto url=this->meta[conn]["Host"];
    
    //пробегаем по сервисам и ищем в них страницу
    for (auto &i : this->Services)
    {
        if(1==i.second->get_schema().count(url))
        {
            this->send_code(conn, i.second->get_schema()[url]["code"]);
            //execute service function
            if(1==i.second->get_schema()[url].count("function"))
            {
                
                auto fname=i.second->get_schema()[url]["function"].get<std::string>();
                std::cout<<"hey"<<i.second->get_schema()[url]["function"].count(fname)<<"\n";
                // if (1==i.second->get_schema()[url].count(fname.c_str()))
                // {        
                // if(i.second->)
                    std::cout<<"yeah\n";            
                    i.second->call_function(fname.c_str(),conn,this->post);
                // }
            }
            //send file
            if(1==i.second->get_schema()[url].count("path"))
            {
                // send_file_from_json(this,conn, url);
                this->send_file(conn,i.second->get_schema()[url]["path"].get<std::string>().c_str());

            }
            return;
        };
    }
    //стандартный json набор
    if (json_obj.count(url) == 1) 
    {
        //отправляем статус (например 200 OK\n)
        this->send_code(conn, this->json_obj[url]["code"]);

        //execute service function
        // if(1==json_obj[url].count("function"))
        // {
        //     auto fname=json_obj[url]["function"].get<std::string>();
        //     if (1==services.count(fname))
        //     {
        //         services[fname](this,conn,this->post.str().c_str());
        //     }
        // }

        //send file
        if(1==json_obj[url].count("path"))
        {
            send_file_from_json(this,conn, url);
        }
        return;
    }

    this->send_code(conn, 404);
    send_file_from_json(this,conn, "404");

}

// void 
// uhtml::parse_post()
// {
//     if (this->post.str()=="terminate")
//     {
//         this->terminate_req=true;
//     }
//     else if (this->post.str()=="update_schema")
//     {
//         this->update_schema();
//     }
 
// }

// void 
// uhtml::add_service(std::string& name,  uhtml::_serviceFunction func)
// {
//     services.insert(std::pair<const std::string, uhtml::_serviceFunction>(name,func));
// }

// void 
// uhtml::add_service(const char* name,  uhtml::_serviceFunction func)
// {
//     std::string _name=name;
//     this->add_service(_name,  func);
// }

void 
uhtml::add_service(const char* name,  uservice* svc)
{
    std::string _name=name;
    Services.insert(std::pair<const std::string,uservice*>(_name,svc));
    // json_obj.merge_patch(svc->get_schema());
    // this->add_service(_name,  func);
}


 void 
 uhtml::send_file(usocket_t conn, const char* file)
 {
   
            //ищем в schema.json путь к файлу
            // auto file_path= h->json_obj[host]["path"].get<std::string>();

            //Открываем файл 
            std::ifstream f(file,std::ios::binary);

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
