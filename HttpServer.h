#pragma once

#include "Server.h"
#include "HttpParser.h"

class HttpServer : public Server {
public:
	string host;
	int port;
	HttpServer& createHttpServer(string host, int port);
	HttpServer& onRequest() {};
	HttpServer& onClose() {};
	vector<thread> threads = {};
	inline ~HttpServer() {
		for (thread &t : this->threads) {
			if (t.joinable())
				t.join();
		}
		this->close();
	};
	string baseDir = ".";
	void config(string path);
private:
	void handleSingleRequest(SOCKET fd);
};
