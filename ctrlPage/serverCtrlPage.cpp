
#include "serverCtrlPage.h"



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
