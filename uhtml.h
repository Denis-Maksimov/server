// #include <nlohmann/json.hpp>
#include "json.hpp"
#include "uhttp.h"

class uhtml: public uhttp
{
private:
    nlohmann::json json_obj; //schema.json
    protected:
    void generate_html(usocket_t conn) override;
    void update_schema(const char* path);
    void update_schema();
    void parse_post();
    void send_file_from_json(usocket_t conn, std::string& host);
    void send_file_from_json(usocket_t conn, const char* host);
public:
    uhtml(uint16_t port);
    ~uhtml();
};