#include <Winsock2.h>
#include <string>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class Server {
  private:
    SOCKET servSocket;
    char recvbuf[255];
    string host;
    int port;
  public:
    int createServer(string host, int port);
    int WinSockInit();
    virtual int listenSocket(int max = 1024);
    virtual void close();
    virtual void handleReq();
	virtual ~Server() {};
    Server();
};