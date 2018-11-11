#pragma once

#include "Server.h"
#include "Http.h"

class HttpServer :public Server {
private:
	int httpPaser(string);
public:
	int createServer();
	int handleRequest();
	int handleResponse();
};