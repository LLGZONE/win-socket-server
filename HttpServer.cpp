#include "HttpServer.h"
#include "pages.h"
#include <thread>

HttpServer& HttpServer::createHttpServer(string host, int port) {
	this->WinSockInit();
	this->createServer(host, port);
	this->listenSocket();
	cout << "Server is listening on " << host << ':' << port << endl;
	SOCKADDR clntAddr;
	cout << "waiting for client socket" << endl;
	int nSize = sizeof(SOCKADDR);
	while (true) {
		SOCKET clntSock = accept(this->servSocket, (SOCKADDR*)&clntAddr, &nSize);
		if (clntSock != INVALID_SOCKET) {
			this->threads.push_back(thread(&HttpServer::handleSingleRequest, this, std::ref(clntSock)));
		}
		else {
			cout << "accept error" << endl;
		}
	}
	return *this;
}

void HttpServer::handleSingleRequest(SOCKET fd) {
	SOCKADDR clntAddr;
	SOCKET prevSocket;
	string reqStr = "";
	int nSize = sizeof(SOCKADDR);

	sockaddr_in addr;
	int ipsize = sizeof(addr);
	getpeername(fd, (sockaddr*)&addr, &ipsize);
	string clntIp = inet_ntoa(addr.sin_addr);
	int clntPort = addr.sin_port;
	printf("\nnew client: ip: %s, port: %d\n", clntIp.c_str(), addr.sin_port);
	struct timeval tv_out;
	//tv_out.tv_sec = 1000;
	//tv_out.tv_usec = 0;
	//setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof(tv_out));
	while (true) {
		int nrecv = 0;
		do {
			int iResult;
			char recvbuf[2] = { '\0', '\0' };
			iResult = recv(fd, recvbuf, 1, 0);
			if (iResult <= 0) {
				closesocket(fd);
				cout << "close client: " << clntIp + ":" << clntPort << "\n" << endl;
				return;
			}
			nrecv += iResult;
			reqStr += string(recvbuf);
		} while (nrecv < 4 || !(reqStr.substr(reqStr.size() - 4) == "\r\n\r\n"));
		printf("reqStr:%s", reqStr.c_str());
		Http *http = new Http(reqStr);
		printf("HTTP message: %s\n", http->req->rawMessage.c_str());
		string url = http->req->url;
		string ContentType = "Content-Type";
		string html = "text/html";
		if (http->req->version == "HTTP/2") {
			http->res->status = 505;
			http->res->phrase = http->res->statusMap[http->res->status];
			http->res->addHeader(ContentType, html);
			string page = pages::p_505;
			http->res->body.assign(page.begin(), page.end());
		}
		else if (http->req->method != "GET") {
			http->res->status = 405;
			http->res->phrase = http->res->statusMap[http->res->status];
			http->res->addHeader(ContentType, html);
			string page = pages::p_405;
			http->res->body.assign(page.begin(), page.end());
		}
		else {
			string src = http->req->url;
			printf("src: %s\n", http->req->url.c_str());
			if (http->req->url == "/") {
				src = "/index.html";
			}
			string fullpath = this->baseDir + src;
			string ext = fullpath.substr(fullpath.find_last_of(".") + 1);
			printf("extension: %s\n", ext.c_str());
			http->res->addHeader(ContentType, Http::mimeType[ext]);
			printf("mime type: %s, path: %s\n", Http::mimeType[ext].c_str(), fullpath.c_str());
			try {
				http->res->body = Utils::readFile(fullpath, 1);
			}
			catch (std::runtime_error) {
				string html = "text/html";
				printf("open file %s error\n", fullpath.c_str());
				http->res->status = 404;
				http->res->phrase = http->res->statusMap[404];
				http->res->addHeader(ContentType, html);
				string page = pages::p_404;
				http->res->body.assign(page.begin(), page.end());
			}
		}
		char* res = http->res->c_str();
		int resLen = http->res->size();
		res[resLen] = '\0';
		printf("HTTP res: %s\n", res);
		int sendLen = send(fd, res, resLen, NULL);
		reqStr = "";
	}
}

void HttpServer::config(string path) {
	unordered_map<string, string> mimeTypes;
	string content = Utils::readFile(path);

	vector<string> lines;
	for (auto line : Utils::split(content, lines, "\n")) {
		auto str = Utils::trim(line);
		vector<string> mime;
		Utils::split(str, mime, "=");
		mimeTypes[mime[0]] = mime[1];
	}
	this->host = mimeTypes["ip"];
	this->port = stoi(mimeTypes["port"]);
	this->baseDir = mimeTypes["index"];
}