#include "uhttp.h"
#include <fstream>
uhttp::uhttp(uint16_t port): userver(port)
{
   
    header.assign(R"((GET|POST)\s+\/([\S]{0,})\s+HTTP\/1.1)");
    remeta.assign(R"((\S+):\s{0,}(.+))");
    codes.insert(std::pair<size_t,std::stringstream>(200,"200 OK"));
    codes.insert(std::pair<size_t,std::stringstream>(400,"400 Bad Request"));
    codes.insert(std::pair<size_t,std::stringstream>(404,"404 Not Found"));
    codes.insert(std::pair<size_t,std::stringstream>(418,"418 I'm a teapot"));

}

uhttp::~uhttp()
{
}


void 
uhttp::generate_html(usocket_t conn)
{
    this->send_code(conn,200);
    std::string content="since I'm a teapot so I cannot brew coffee.. So sorry((\n";
    std::stringstream meta;
    meta<<"Content-Length:"<<content.length()<<std::endl;
    meta<<std::endl;
    send(conn,meta.str().c_str(),meta.str().length(),0);
    send(conn,content.c_str(),content.length(),0);
    std::cout <<"since I'm a teapot so I cannot brew coffee.. So sorry((\n";

}


void
uhttp::send_code(usocket_t conn,size_t code)
{
    if(this->codes.find(code)==this->codes.cend())
    {
        code=418;
    }
    send(conn,"HTTP/1.1 ",9,0);
    std::cout<<this->codes[code].str();
    send(conn,this->codes[code].str().c_str(),this->codes[code].str().length(),0);
    send(conn,"\r\n",2,0);

    
}


void
uhttp::data_handle(usocket_t conn)
{
    char line[1024]={0};

    size_t err=200;
    std::smatch m;

    do
    {


        std::string a;
 

        this->messages[conn].getline(line,1024);

        a=line;

        if(std::strlen(line)<1){
            // this->send_code(conn,400);
            break;
        }
        
        std::regex_search(a, m, this->header);
        this->meta[conn].insert(pair_s("Method",m[1]));
        this->meta[conn].insert(pair_s("Host",  m[2]));
        

        a="";

        // int c=0;

        while (this->messages[conn].getline(line,1024))
        {
                a=line;
                // std::cout<<"чукча=" << a <<"\n";

                if(std::regex_search(a, m, this->remeta))
                {
                    this->meta[conn].insert(pair_s(m[1].str(),m[2].str()));
                    // meta[m[1].str()]=m[2].str();
                    std::cout << m[1]<<" <--> "<< m[2]<<"\n";
                }else{
                    break;
                    
                }
                a="";
    
        }

        for (auto &g : this->meta)
        {
            std::cout << g.first<<std::endl;
        }

        // std::cout<< "hst=" << this->meta["Host"]<<std::endl;
        
        this->post.str("");
        this->post.clear();
        this->post << this->messages[conn].rdbuf();

        std::cout << this->post.str();
  

    }while (false);



    this->generate_html(conn);

    if(this->inputs.find(conn)!=this->inputs.cend())
    {
        this->inputs.erase(conn);
    }
    
    if(this->outputs.find(conn)!=this->outputs.cend())
    {
        this->outputs.erase(conn);
    }
    if( this->meta[conn]["Connection"] != "keep-alive" ||
        this->meta[conn]["Connection"] != "Keep-Alive" )
    {
        shutdown(conn,SHUT_RDWR);
    }
    this->meta.erase(conn);

}


void
uhttp::erase(usocket_t conn)
{
    std::cout <<"http\n";
    this->meta.erase(conn);
}
