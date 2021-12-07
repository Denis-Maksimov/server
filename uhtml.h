// #include <nlohmann/json.hpp>
#include "json.hpp"
#include "uhttp.h"

class uhtml: public uhttp
{
private:
    nlohmann::json json_obj;
    protected:
    void generate_html(usocket_t conn) override;
    void update_schema(const char* path);
    void update_schema();
    void parse_post();
public:
    uhtml(uint16_t port);
    ~uhtml();
};