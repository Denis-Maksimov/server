#include "configure/version.h"
#ifdef  _VERSION_0_0
#include "ucli.h"
#include <fstream>


ucli::ucli(uint16_t port): userver(port,true)
{
   
    header.assign(R"(HTTP\/[\d.]+\s+([\d]+)\s+(.{0,}))");
    // HTTP/1.1 200 OK
    remeta.assign(R"((\S+):\s{0,}(.+))");
    // codes.insert(std::pair<size_t,std::stringstream>(200,"200 OK"));
    // codes.insert(std::pair<size_t,std::stringstream>(400,"400 Bad Request"));
    // codes.insert(std::pair<size_t,std::stringstream>(404,"404 Not Found"));
    // codes.insert(std::pair<size_t,std::stringstream>(418,"418 I'm a teapot"));

}

ucli::~ucli()
{
}


void 
ucli::handle_html(usocket_t conn)
{

    std::cout <<"since I'm a teapot so I cannot brew coffee.. So sorry((\n";
    this->cb(conn);
    this->terminate_req=true;

}


void
ucli::send_code(usocket_t conn,size_t code)
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
ucli::data_handle(usocket_t conn)
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

        this->meta[conn].insert(pair_s("Status",m[1].str()));
        this->meta[conn].insert(pair_s("Status-msg",  m[2].str()));
        

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
                    // std::cout << m[1]<<" <--> "<< m[2]<<"\n";
                }else{
                    break;
                    
                }
                a="";
    
        }

        for (auto &g : this->meta[conn])
        {
            std::cout << g.first<<"<=>"<<g.second<<std::endl;
        }

        // std::cout<< "hst=" << this->meta["Host"]<<std::endl;
        
        this->post.str("");
        this->post.clear();
        this->post << this->messages[conn].rdbuf();

        // std::cout << this->post.str();
  

    }while (false);



    this->handle_html(conn);
    

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
ucli::erase(usocket_t conn)
{
    std::cout <<"http\n";
    this->meta.erase(conn);
}

void 
ucli::send_GET(std::string url,uint16_t port)
{
    struct sockaddr_in server={0};
    server.sin_addr.s_addr=inet_addr(url.c_str());
    server.sin_family=AF_INET;
    server.sin_port=htons(port);

    connect(this->srv_sock,(struct sockaddr *)&server,sizeof(server));
    send(this->srv_sock,"GET / HTTP/1.1",14,0);
    this->inputs.insert(this->srv_sock);
}


// static void

#include <iostream>
void 
ucli::request(std::string url,std::string data, std::function<void(usocket_t)> f)
{
    std::regex uri;
    std::smatch groups;
    uri.assign(R"(([\w]+:\/\/)?([\w.]+)(:([\d]+))?(\/([\S]+))?)");
    std::regex_search(url, groups, uri);
    std::stringstream prt;
    for (auto &&i : groups)
    {
        std::cout <<i.str() <<std::endl;
    }
    
    prt<<groups[4];
    uint16_t port;
    prt>>port;

    std::cout <<port<<" - port\n";
    struct sockaddr_in server={0};
    server.sin_addr.s_addr=inet_addr(groups[2].str().c_str());
    server.sin_family=AF_INET;
    server.sin_port=htons(port);
    connect(this->srv_sock,(struct sockaddr *)&server,sizeof(server));

    std::stringstream msg;
//===========HTTP==================
    
    msg<<"POST /"<<groups[6]<<" HTTP/1.1"<<std::endl;
    msg<<"host: "<< groups[2]<<groups[3]<<std::endl;
    msg<<"Connection: close"<<std::endl;
    msg<<"User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0"<<std::endl;
    msg<<"Referer: https://www.google.com/"<<std::endl;
    msg<<"Content-Lenght:"<<data.length()<<std::endl;
    msg<<std::endl;
    msg<<data<<std::endl;
    // msg<<std::endl;
//===========HTTP==================
    send(this->srv_sock,msg.str().c_str(),msg.str().length(),0);

    this->cb=f;
    this->inputs.insert(this->srv_sock);
}



void 
ucli::request(std::string url,std::stringstream& data, std::function<void(usocket_t)> fu)
{

    this->request(url,data.str(),fu);
}

#endif //v0.0
