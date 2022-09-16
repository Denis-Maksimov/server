#include "configure/version.h"
#ifdef  _VERSION_0_0
#include "ctrlPage/serverCtrlPage.h"



void simple_serviceFunction(uhtml* h, uhtml::usocket_t conn, const char* POST_JSON)
{
    nlohmann::json parsed=nlohmann::json::parse(POST_JSON);
    
    if (1==parsed.count("shutdown"))
    {
       
        if(parsed["shutdown"]==true)
        {
            h->terminate();
        };
    }


    if (1==parsed.count("update_schema"))
    {
       
        if(parsed["update_schema"]==true)
        {
            h->update_schema();
        };
    }

    
    
}
uservice* example(userver* server){
    uservice* srv2=new uservice;
    srv2->update_schema("./ctrlPage/schema.json");
    srv2->add_function("_sys_",(void*)server,[](userver::usocket_t conn, std::stringstream& post,void*data)
    {
            nlohmann::json parsed=nlohmann::json::parse(post.str());
            std::cout<<"sda"<<parsed.dump()<<std::endl;
            if (1==parsed.count("shutdown"))
            {
            
                if(parsed["shutdown"]==true)
                {
                    // h->terminate();
                    ((uhtml*)data)->terminate();
                    // exit(EXIT_SUCCESS);
                };
            }


            if (1==parsed.count("update_schema"))
            {
            
                if(parsed["update_schema"]==true)
                {
                    ((uhtml*)data)->update_schema();
                    // h->update_schema();
                };
            }
    });

    return srv2;
}
#endif