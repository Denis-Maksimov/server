#include "configure/version.h"
#ifdef  _VERSION_0_0
#include "usrv.h"

#if defined(__linux__)
#include <csignal>
#endif

#if _FOR_VERSION(0,0)
void signal_handler(int signal){}
//=========================================================================
userver::userver(uint16_t port,bool is_cli):is_cli(is_cli)
{
        #if defined(__linux__)
            std::signal(SIGPIPE, signal_handler);
        #endif
        this->srv_sock=socket(AF_INET, SOCK_STREAM, IPPROTO_IP);//IPPROTO_TCP

        // usockaddr addr={0};
        struct sockaddr_in server;
        #if defined(__linux__)
        // server.sin_addr.s_addr=inet_addr("127.0.0.1");
        #else
        server.sin_addr.s_addr=inet_addr("10.26.0.250");
        #endif
        server.sin_family=AF_INET;
        server.sin_port=htons(port);

        // addr.sa_in.sin_addr.s_addr=INADDR_ANY;
        // addr.sa_in.sin_port=htons(port);
        std::cout<<this->srv_sock<<"serv!\n";

        while(auto rv=bind(this->srv_sock, (struct sockaddr *)&server, sizeof(server)))
        {
            std::cout<<rv;
            port+=1;
            server.sin_port=htons(port);
            // addr.sa_in.sin_port=htons(port);
        }
        std::cout<<"port="<<htons(server.sin_port)<<std::endl;

        this->inputs.clear();
        if(!is_cli){
            listen(this->srv_sock,3);
            this->inputs.insert(this->srv_sock);//сокеты, которые будем читать
        }
        this->outputs.clear();// сокеты, в которые надо писать
        this->messages.clear();
        // self.timeout=1.

}
//=========================================================================
userver::~userver()
{
    shutdown(this->srv_sock,SHUT_RDWR);
    for (auto &i : inputs)
    {
        shutdown(i,SHUT_RDWR);  
        std::cout<<"closing: " <<i<<std::endl;
    }
    for (auto &i : outputs)
    {
        shutdown(i,SHUT_RDWR);   
        std::cout<<"closing: " <<i<<std::endl;
    }

}

//=========================================================================
void
userver::accept_handle(usrvNS::usocket_t conn)
{
    usockaddr client_addr;
    socklen_t len;
    usrvNS::usocket_t new_conn = accept(conn, &client_addr.sa, &len);
    if (new_conn==-1)
    {
        #if defined(_WIN32)
        int err;
        CHAR msgbuf[256]; // for a message up to 255 bytes.
        msgbuf[0] = '\0'; // Microsoft doesn't guarantee this on man page.
        err = WSAGetLastError();
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // flags
                      NULL,                                                       // lpsource
                      err,                                                        // message id
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),                  // languageid
                      msgbuf,                                                     // output buffer
                      sizeof(msgbuf),                                             // size of msgbuf, bytes
                      NULL);                                                      // va_list of arguments

        std::cout<<"err"<<err<<"::"<< msgbuf<<std::endl;
        #endif
    }
    
        // # если это серверный сокет, то пришел новый
        // # клиент, принимаем подключение
        // new_conn, client_addr = conn.accept()
        // print('Успешное подключение!')
        // print(client_addr)

        // # поместим новый сокет в очередь 
        // # на прослушивание
        this->inputs.insert(new_conn);
        // self.inputs.append(new_conn)
        // pass
}
//=========================================================================

void
userver::read_handle(usrvNS::usocket_t conn)
{
        char buf[1024];
        ssize_t n=recv(conn,buf,1024,0);

//      # если сокет прочитался и есть сообщение 
//      # то кладем сообщение в словарь, где 
//      # ключом будет сокет клиента
        if(n>0)
        {

            buf[n]='\0';
            this->messages[conn]<<buf;

            std::cout <<"msg:"<<n<<"\n" <<this->messages[conn].str()<<std::endl;
        
        

//          # добавляем соединение клиента в очередь 
//          # на готовность к приему сообщений от сервера
            if (!this->is_cli)
            {               
                if(this->outputs.find(conn)==this->outputs.cend())
                {
                    this->outputs.insert(conn);
    //             if conn not in self.outputs:
    //                 self.outputs.append(conn)
                }
            }


        }else if(n==0)
        {
            this->data_handle(conn);

        }else
        {
//             # если сообщений нет, то клиент
//             # закрыл соединение или отвалился 
//             # удаляем его сокет из всех очередей
            if(this->outputs.find(conn)!=this->outputs.cend())
            {
                this->outputs.erase(conn);
            }
            if(this->inputs.find(conn)!=this->inputs.cend())
            {
                this->inputs.erase(conn);
            }

//             # закрываем сокет как положено, тем 
//             # самым очищаем используемые ресурсы
//             conn.close()
            shutdown(conn,SHUT_RDWR);


//             # удаляем сообщения для данного сокета
            if(this->messages.find(conn)!=this->messages.cend())
            {
                this->messages.erase(conn);
            }
            erase(conn);



        }
}

//=========================================================================

void
userver::write_handle(usrvNS::usocket_t conn)
{
  std::stringstream msg;
// def write_handle(self,conn):
if(this->messages.find(conn)!=this->messages.cend())
{
   this->messages[conn].copyfmt(msg);
//     msg = self.messages.get(conn, None)

    msg.seekg(0, std::ios::end);
    auto size = msg.tellg();
    msg.seekg(0, std::ios::beg);
    
//     if len(msg):
    if (!size)
    {
        std::cout<<"data\n";
//         # если есть сообщения - то переводим 
//         # его в верхний регистр и отсылаем
//         temp = msg.pop(0).decode('utf-8')
//         # print(temp)
//         self.handle_data(conn,temp)
            this->data_handle(conn);
    }else{
        std::cout<<"erase\n";
//         # если нет сообщений - удаляем из очереди
//         # сокетов, готовых принять сообщение 
//         self.outputs.remove(conn)
        this->outputs.erase(conn);
    }
}else {
    this->outputs.erase(conn);
    std::cout<<"hmmm\n";
    }
  
}   
//=========================================================================
void
userver::data_handle(usrvNS::usocket_t conn)
{

        send(conn,"HTTP/1.1 200 OK\r\n",17, 0);
        send(conn,"\r\nBeee",6,0);

       
        if(this->inputs.find(conn)!=this->inputs.cend())
        {
            this->inputs.erase(conn);
            //  std::cout<<"one\n";
        }
        
        if(this->outputs.find(conn)!=this->outputs.cend())
        {
            this->outputs.erase(conn);
            // std::cout<<"two\n";
        }
        erase(conn);

        // std::cout<<"four\n";

}



//=========================================================================



bool
userver::check()
{

    try{
        struct timeval tv;

        fd_set fd_in,fd_out,fd_err;
        std::set<usrvNS::usocket_t>::iterator it;
        usrvNS::usocket_t largest_sock,ls;
        while(1){  
            FD_ZERO( &fd_in );
            FD_ZERO( &fd_out );
            FD_ZERO( &fd_err );

            for (auto &i : inputs)
            {
                FD_SET(i, &fd_in );//добавляем в сет
                FD_SET(i, &fd_err );//добавляем в сет
            }
            for (auto &i : outputs)
            {
                FD_SET(i, &fd_out );//добавляем в сет
            }

            
            tv.tv_sec = timeout.tv_sec;
            tv.tv_usec = timeout.tv_usec;
    
            for (auto &s : inputs)
            {
                std::cout <<s<<"\n";
            }
            
            if(!inputs.empty())
            largest_sock=*inputs.rbegin();
            else largest_sock=0;

            if(!outputs.empty())
            ls=*outputs.rbegin();
            else ls=0;
            // std::cout<<largest_sock<<ls<<"LS!\n";
            largest_sock=std::max(largest_sock,ls);

            std::cout<<largest_sock<<"LS!\n";
            if(this->terminate_req)return this->terminate_req;

            int ret = select( largest_sock + 1, &fd_in, &fd_out, &fd_err, &tv );
            // проверяем успешность вызова
            if ( ret == -1 )
            {
                
                throw -1;  

            }else if( ret == 0 )
            {
                //!TODO
                // таймаут, событий не произошло
                std::cout<<"tick!\n";
                
                return this->terminate_req;

            }else{

                std::cout<<"Hyyy!\n";
                for (auto &conn : inputs)
                {
                    // обнаружили событие
                    if ( FD_ISSET( conn, &fd_in ) )
                    {
                        if((conn==this->srv_sock)&&(!this->is_cli))
                        {
                            std::cout<<"accept!\n";
                            //  если это серверный сокет, то пришел новый
                            //  клиент, принимаем подключение               
                            this->accept_handle(conn);
                            break;
                        }else{
                            std::cout<<"read!\n";
                            this->read_handle(conn);
                            //  если это НЕ серверный сокет, то 
                            //  клиент хочет что-то сказать
                            // self.read_handle(conn)
                            break;
                        }
              
                    }
                    
                }
                

                // # список SEND - сокеты, готовые принять сообщение
                for (auto &conn : outputs)
                {
                    // обнаружили событие
                    if ( FD_ISSET( conn, &fd_in ) )
                    {
                        std::cout<<"write!\n";
                        this->write_handle(conn);
                        break;
                        //  # выбираем из словаря сообщения
                        // # для данного сокета
                        // self.write_handle(conn) 
                    }
                    
                }
                
                // # список EXCEPTS - сокеты, в которых произошла ошибка
                for (auto &conn : inputs)
                {
                    // обнаружили событие
                    if ( FD_ISSET( conn, &fd_err ) )
                    {
                        //  print('Клиент отвалился...')
                        // # удаляем сокет с ошибкой из всех очередей
                        std::cout<<conn<<"this->inputs.erase(conn);\n";
                        this->inputs.erase(conn);
                        // self.inputs.remove(conn)
                        
                        // if conn in self.outputs:
                        //     self.outputs.remove(conn)

                        if(this->outputs.find(conn)!=this->outputs.cend())
                        {
                            this->outputs.erase(conn);
                        }
                        // # закрываем сокет как положено, тем 
                        // # самым очищаем используемые ресурсы
                        // conn.close()
                        shutdown(conn,SHUT_RDWR);
                        // # удаляем сообщения для данного сокета
                        this->messages.erase(conn);
                        erase(conn);
                        // del self.messages[conn]
                        break;
                    }
                    
                }


                //end events
            }

            //end cycle
            printf("end cycle\n");

        }
        //finalize
    }catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return true;
    }
    return this->terminate_req;

}



void
userver::erase(usrvNS::usocket_t s)
{
    
}

void 
userver::terminate()
{
    terminate_req=true;
}
#endif // _FOR_VERSION(0,0)

//========================================================================
//************************************************************************
//========================================================================
//************************************************************************
//========================================================================

#if _FOR_VERSION(0,1)

void 
userver::bind_address_(const char* a,uint16_t port)
{
    // usockaddr addr={0};
    struct sockaddr_in server;
    server.sin_addr.s_addr=inet_addr(a);
    server.sin_family=AF_INET;
    server.sin_port=htons(port);

    std::cout<<(*srv_end_point)<<"serv!\n";

    while(auto rv=bind(*srv_end_point, (struct sockaddr *)&server, sizeof(server)))
    {
        std::cout<<rv;
        port+=1;
        server.sin_port=htons(port);
        // addr.sa_in.sin_port=htons(port);
    }
    std::cout<<"port="<<htons(server.sin_port)<<std::endl;
}

userver::
userver(uint16_t port,bool is_cli)
{
        //--------------------------------------
        //-- загрузка библиотеки wsa2 для вэнди
        wsa2_lib::instance();
        //--------------------------------------
        //-- заглушка на SIGPIPE сигнал
        #if defined(__linux__)
            std::signal(SIGPIPE, [](int){});
        #endif

        //--------------------------------------
        //-- create socket
        // this->srv_sock=socketUniquePtr(new usrvNS::usocket_t);
        auto _sock=socket(AF_INET, SOCK_STREAM, IPPROTO_IP);//IPPROTO_TCP
        this->srv_end_point=_sock;

        if(INVALID_SOCKET==(*srv_end_point))
        {
            // this->srv_sock.reset();
            throw common::m_exception(__LINE__+"::INVALID_SOCKET");
        }
        
        //--------------------------------------
        //-- привязываем адрес
        #if defined(__linux__)
        bind_address_("127.0.0.1",port);
        #else
        bind_address_("10.26.0.250",port);
        #endif

        //--------------------------------------
        //-- чистим сеты
        this->inputs.clear();
        if(!is_cli)
        {
            listen(*srv_end_point,3);//TODO __n=3 
            this->inputs.insert(std::move(srv_end_point));//сокеты, которые будем читать
        }
        this->outputs.clear();// сокеты, в которые надо писать
        this->messages.clear();
        // self.timeout=1.
}

userver::~userver()
{
    // shutdown(this->srv_sock,SHUT_RDWR);

    // for (auto &i : inputs)
    // {
    //     shutdown(i,SHUT_RDWR);  
    //     std::cout<<"closing: " <<i<<std::endl;
    // }
    // for (auto &i : outputs)
    // {
    //     shutdown(i,SHUT_RDWR);   
    //     std::cout<<"closing: " <<i<<std::endl;
    // }

}
//=========================================================================



bool
userver::check()
{

    try{
        struct timeval tv;
        fd_set fd_in,fd_out,fd_err;
        std::set<usrvNS::usocket_t>::iterator it;
        
        usrvNS::usocket_t largest_sockR,largest_sockW,largest_sock;

        while(1){

            //avoid fd_sets
            FD_ZERO( &fd_in );
            FD_ZERO( &fd_out );
            FD_ZERO( &fd_err );

            for (auto& i : inputs)
            {
                FD_SET((*i), &fd_in );//добавляем в сет
                FD_SET((*i), &fd_err );//добавляем в сет
            }

            for (auto& i : outputs)
            {
                FD_SET(*i, &fd_out );//добавляем в сет
            }

            //ставим таймоут
            tv.tv_sec = timeout.tv_sec;
            tv.tv_usec = timeout.tv_usec;
    
            // for (auto &s : inputs)
            // {
            //     std::cout <<*s<<"\n";
            // }
            
            if(!inputs.empty())
                largest_sockR=(**inputs.rbegin());
            else largest_sockR=0;

            if(!outputs.empty())
                largest_sockW=**outputs.rbegin();
            else largest_sockW=0;

            // std::cout<<largest_sock<<ls<<"LS!\n";
            largest_sock=std::max(largest_sockR,largest_sockW);

            std::cout<<largest_sock<<"LS!\n";
            if(this->terminate_req)return this->terminate_req;

            int ret = select( largest_sock + 1, &fd_in, &fd_out, &fd_err, &tv );
            // проверяем успешность вызова
            if ( ret == -1 )
            {   
                throw common::m_exception("select error");  
            }
            else if( ret == 0 )
            {
                //!TODO
                // таймаут, событий не произошло
                std::cout<<"tick!\n";
                return this->terminate_req;
            }else{

                // std::cout<<"Hyyy!\n";
                for (const connection& conn : inputs)
                {
                    // обнаружили событие
                    if ( FD_ISSET( *conn, &fd_in ) )
                    {
                        if((*conn==(*srv_end_point))&&(!this->is_cli))
                        {
                            std::cout<<"accept!\n";
                            //  если это серверный сокет, то пришел новый
                            //  клиент, принимаем подключение               
                            this->accept_handle(conn);
                            break;
                        }else{
                            std::cout<<"read!\n";
                            this->read_handle( conn);
                            //  если это НЕ серверный сокет, то 
                            //  клиент хочет что-то сказать
                            break;
                        }
              
                    }
                    
                }
                
                // # список SEND - сокеты, готовые принять сообщение
                for (auto &conn : outputs)
                {
                    // обнаружили событие
                    if ( FD_ISSET( *conn, &fd_in ) )
                    {
                        //  # выбираем из словаря сообщения
                        // # для данного сокета
                        std::cout<<"write!\n";
                        this->write_handle(conn);
                        break;
                        
                    }
                    
                }
                
                // # список EXCEPTS - сокеты, в которых произошла ошибка
                for (auto &conn : inputs)
                {
                    // обнаружили событие
                    if ( FD_ISSET( *conn, &fd_err ) )
                    {

                        // # удаляем сообщения для данного сокета
                        this->messages.erase(*conn);


                        //  print('Клиент отвалился...')
                        // # удаляем сокет с ошибкой из всех очередей
                        // std::cout<<conn<<"this->inputs.erase(conn);\n";
                        this->inputs.erase(conn);
                        // self.inputs.remove(conn)
                        
                        // if conn in self.outputs:
                        //     self.outputs.remove(conn)

                        if(this->outputs.find(conn)!=this->outputs.cend())
                        {
                            this->outputs.erase(conn);
                        }
                        // # закрываем сокет как положено, тем 
                        // # самым очищаем используемые ресурсы
                        // conn.close()
                        // shutdown(conn,SHUT_RDWR);
                        
                        // erase(conn);
                        // del self.messages[conn]
                        break;
                    }
                    
                }
                //end events
            }
            //end cycle
            printf("end cycle\n");
        }
        //finalize
    }catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return true;
    }
    return this->terminate_req;
}

void
userver::accept_handle(const connection& conn)
{
    usockaddr client_addr;
    socklen_t len;
    // auto new_conn=socketUniquePtr(new usrvNS::usocket_t);
    connection new_conn = accept(*conn, &client_addr.sa, &len);
    if (*new_conn==INVALID_SOCKET)
    {
        wsa2_lib::get_last_err();
    }
    
        // # если это серверный сокет, то пришел новый
        // # клиент, принимаем подключение
        // new_conn, client_addr = conn.accept()
        // print('Успешное подключение!')
        // print(client_addr)

        // # поместим новый сокет в очередь 
        // # на прослушивание
        this->inputs.insert(std::move(new_conn));
        // self.inputs.append(new_conn)
        // pass
}
//=========================================================================

void
userver::read_handle(const connection& conn)
{
        char buf[1024];
        ssize_t n=recv(*conn,buf,1024,0);

//      # если сокет прочитался и есть сообщение 
//      # то кладем сообщение в словарь, где 
//      # ключом будет сокет клиента
        if(n>0)
        {

            buf[n]='\0';
            this->messages[*conn]<<buf;

            std::cout <<"msg:"<<n<<"\n" <<this->messages[*conn].str()<<std::endl;
        
        

//          # добавляем соединение клиента в очередь 
//          # на готовность к приему сообщений от сервера
            if (!this->is_cli)
            {               
                // if(this->outputs.find(conn)==this->outputs.cend())
                // {
                    auto conn_b = conn;
                    this->outputs.insert(std::move(conn_b));
    //             if conn not in self.outputs:
    //                 self.outputs.append(conn)
                // }
            }

        // makeSocketSharedPtr(&conn);

//      # если сокет прочитался и сообщения нет
//      # значит сообщение можно обрабатывать
        }else if(n==0)
        {
            this->data_handle(conn);

        }else
        {
//             # если сообщений нет, то клиент
//             # закрыл соединение или отвалился 
//             # удаляем его сокет из всех очередей
            if(this->outputs.find(conn)!=this->outputs.cend())
            {
                this->outputs.erase(conn);
            }
            if(this->inputs.find(conn)!=this->inputs.cend())
            {
                this->inputs.erase(conn);
            }

//             # закрываем сокет как положено, тем 
//             # самым очищаем используемые ресурсы
//             conn.close()
            // shutdown(conn,SHUT_RDWR);


//             # удаляем сообщения для данного сокета
            if(this->messages.find(*conn)!=this->messages.cend())
            {
                this->messages.erase(*conn);
            }
            // erase(conn);



        }
}

//=========================================================================

void
userver::write_handle(const connection& conn)
{
  std::stringstream msg;
// def write_handle(self,conn):
if(this->messages.find(*conn)!=this->messages.cend())
{
   this->messages[*conn].copyfmt(msg);
//     msg = self.messages.get(conn, None)

    msg.seekg(0, std::ios::end);
    auto size = msg.tellg();
    msg.seekg(0, std::ios::beg);
    
//     if len(msg):
    if (!size)
    {
        std::cout<<"data\n";
//         # если есть сообщения - то переводим 
//         # его в верхний регистр и отсылаем
//         temp = msg.pop(0).decode('utf-8')
//         # print(temp)
//         self.handle_data(conn,temp)
            this->data_handle(conn);
    }else{
        std::cout<<"erase\n";
//         # если нет сообщений - удаляем из очереди
//         # сокетов, готовых принять сообщение 
//         self.outputs.remove(conn)
        this->outputs.erase(conn);
    }
}else {
    this->outputs.erase(conn);
    std::cout<<"hmmm\n";
    }
  
}   
//=========================================================================
void
userver::data_handle(const connection& conn)
{

        send(*conn,"HTTP/1.1 200 OK\r\n",17, 0);
        send(*conn,"\r\nBeee",6,0);

       
        if(this->inputs.find(conn)!=this->inputs.cend())
        {
            this->inputs.erase(conn);
            //  std::cout<<"one\n";
        }
        
        if(this->outputs.find(conn)!=this->outputs.cend())
        {
            this->outputs.erase(conn);
            // std::cout<<"two\n";
        }
        // erase(conn);

        // std::cout<<"four\n";

}

#endif //_FOR_VERSION(0,1)

#ifdef _VERSION_0_1

connection::connection(rwe_type t)
:type(t)
{
    socket_ptr=
    std::make_unique <usrvNS::usocket_t>(new usrvNS::usocket_t(0));
}
connection::connection(usrvNS::usocket_t s, rwe_type t)
:type(t)
{
    socket_ptr=
        std::make_unique <usrvNS::usocket_t>(new usrvNS::usocket_t(s));

}
// connection::connection(std::initializer_list<int>) = delete;
connection::connection(connection && other)
{
    shutdown(*socket_ptr,type);
    socket_ptr.reset();
    type=other.type;
    socket_ptr=std::move(other.socket_ptr);
    other.socket_ptr.reset();
}

connection::connection(const connection & other)
{
    // socket_ptr.reset();
    *socket_ptr=*other.socket_ptr;
}

connection&
connection::operator=(connection&& other)
{
    if(this==&other)
    {
        return *this;
    }
    if(*socket_ptr!=(*other.socket_ptr))
    {
        shutdown(*socket_ptr,type);
        socket_ptr.reset();

        socket_ptr=std::move(other.socket_ptr);
        type=other.type;
        // other.socket_ptr.reset();
        other.type=both;
        data=std::move(other.data);
    }
    return *this;
}

usrvNS::usocket_t 
connection::operator*()
{
    return *(this->socket_ptr);
}

usrvNS::usocket_t 
connection::operator*() const
{
    return *(this->socket_ptr);
}
// connection& 
// connection::operator=(const connection& other)
// {
//     return *this;
// }

connection::~connection()
{
     shutdown(*socket_ptr,type);
}


connection& 
connection::operator=(usrvNS::usocket_t s)
{
    this->type=both;
    *socket_ptr=0;
}

void 
connection::set_type(rwe_type t)
{
    this->type=t;
}
#endif // _VERSION_0_1
#endif // _VERSION_0_0
