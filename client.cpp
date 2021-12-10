
#include "client.h"



client::client(uint16_t port):userver(port)
{
    header.assign(R"((\d+)\s{0,}(.+))");
    remeta.assign(R"((\S+):\s{0,}(.+))");
}

client::~client()
{
}


void
client::data_handle(usocket_t conn)
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
        this->meta[conn].insert(pair_s("status",m[1]));
        this->meta[conn].insert(pair_s("status-code",  m[2]));
        

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

        

        // std::cout<< "hst=" << this->meta["Host"]<<std::endl;
        
        this->post.str("");
        this->post.clear();
        this->post << this->messages[conn].rdbuf();

        // std::cout << this->post.str();
  

    }while (false);



    // this->generate_html(conn);
    this->handle_request(conn);

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
client::handle_request(usocket_t conn)
{
    //--параметры
    for (auto &g : this->meta[conn])
    {
        std::cout << g.first<<"="<<g.second<<std::endl;
    }

    //--тело
    std::cout << this->post.str() << std::endl;
    

}


template <typename T> void
client::send_GET(T addr, uint16_t port, T req)
{
    struct sockaddr_in server;
    std::stringstream ss;
    ss << addr;
        server.sin_addr.s_addr=inet_addr(ss.str().c_str());
        server.sin_family=AF_INET;
        server.sin_port=htons(port);

    ss="";
    ss<<"GET /"<< req <<" HTTP/1.1"<<std::endl;
    ss<<"Host: "<<addr<<":"<<port<<std::endl;
    ss<<"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:95.0) Gecko/20100101 Firefox/95.0"<<std::endl;
    ss<<"Connection : close"<<std::endl;
    ss<<std::endl;

    sendto(this->srv_sock,ss.str().c_str(),ss.str().length(),0,&server,sizeof(server));
    while (this->check());
    
}

