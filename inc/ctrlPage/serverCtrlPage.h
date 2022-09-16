#ifndef SERVER_CTRL_PAGE_H
#define SERVER_CTRL_PAGE_H
#include "configure/version.h"
#ifdef  _VERSION_0_0
#include "uhtml.h"

void simple_serviceFunction(uhtml* h, usrvNS::usocket_t conn, const char* POST_JSON);


uservice* example(userver* server);

#endif
#endif // !SERVER_CTRL_PAGE_H
