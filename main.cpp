#include "Server.h"

int main() {
  Server server;
  server.WinSockInit();
  server.createServer("127.0.0.1", 8080);
  server.listenSocket();
  while(true)
  server.handleReq();
}