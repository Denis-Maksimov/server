#ifndef SERVER_CTRL_PAGE_H
#define SERVER_CTRL_PAGE_H
#include "../uhtml.h"

void simple_serviceFunction(uhtml* h, uhtml::usocket_t conn, const char* POST_JSON);


uservice* example(userver* server);

#endif // !SERVER_CTRL_PAGE_H
