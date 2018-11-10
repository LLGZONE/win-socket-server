#include "WinSockEnv.h"
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int WinSockEnv::start() {
  WSADATA wsaData;
  WORD wv = MAKEWORD(2, 2);

  int rtn = WSAStartup(wv, &wsaData);
  if (rtn) {
    cout << "winsock init error!" << endl;
    return -1;
  }

  if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
    WSACleanup();
    cout << "Winsock version error!" << endl;
    return -1;
  }

  return 0;
}