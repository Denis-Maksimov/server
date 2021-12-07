#ifndef U_WEB_H
#define U_WEB_H
#include "uhttp.h"


#include <fstream>
//dependes: nlohmann-json3-dev
//https://github.com/nlohmann/json
#include <nlohmann/json.hpp>



class uweb: public uhttp
{
private:
    
    
    nlohmann::json file_pages;
    nlohmann::json gen_pages;
    typedef void(*gen_page_f)(uweb&,usocket_t);
    std::unordered_map<std::string, gen_page_f> namesf;
    typedef std::pair<const std::__cxx11::string, uweb::gen_page_f> func_names_pair;

    friend void parsm(uweb& w, usocket_t conn);
    friend void setparsm(uweb& w, usocket_t conn);
    void send_file(usocket_t conn,std::string& file);
    void parse_url();
public:
    nlohmann::json params;
    uweb(uint16_t);
    ~uweb();
};

void parsm(uweb& w, uweb::usocket_t conn)
{
    auto str = w.params.dump();
    send(conn,str.c_str(),str.length(),0);
    return;
}

void setparsm(uweb& w, uweb::usocket_t conn)
{
    auto s=w.post.str();
    auto json_obj = nlohmann::json::parse(s);
    w.params.update(json_obj);
    

        // line=json.loads(self.post)
        // for p in line:
        //     self.params.update({p:line[p]})
        // # conn.send(line.encode())
        // pass
}

uweb::uweb(uint16_t port=8081):uhttp(port)
{
    this->file_pages={
        {"index",       nlohmann::json::array({"test.html",200}) },
        {"style.css",   nlohmann::json::array({"style.css",200}) },
        {"script.js",   nlohmann::json::array({"script.js",200}) },
        {"table.js",    nlohmann::json::array({"table.js",200}) },
        {"404",         nlohmann::json::array({"test.html",404}) },
        {"418",         nlohmann::json::array({"test.html",418}) },
    };

    this->namesf.insert(func_names_pair("parsm",parsm));
    this->namesf.insert(func_names_pair("set_params",parsm));
    this->gen_pages={
        {"params.json",       nlohmann::json::array({"parsm",200}) },
    };
}







uweb::~uweb()
{
}



void 
uweb::send_file(usocket_t conn,std::string& filename)
{
    char buf[1024];
    std::ifstream file;
    file.open(filename);
    size_t n;
    while (!file.eof())
    {
        file.read(buf,1024);
        n=file.gcount();
        send(conn,buf,n,0);
    }
    file.close();

}

void 
uweb::parse_url()
{
    std::stringstream ss(this->meta["Host"]);
    //TODO
        // s=self.url
        // ur=s.split("?",1)
        // if ur.__len__()>=2:
        //     for math in ur[1].split("&"):
        //         a=self.regex.findall(math)
        //         try:
        //             self.params.update({a[0][0]:a[0][1].split("+")})
        //         except:
        //             pass
        // print(ur[0])
        // return ur[0]
}



#endif // U_WEB_H
