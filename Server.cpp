#include "Server.h"
#include "WinSockEnv.h"

Server::Server() {
}

int Server::WinSockInit() {
	if (WinSockEnv::start() == -1) return -1;
	return 0;
}

int Server::createServer(string host, int port) {
	this->host = host;
	this->port = port;
	this->servSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->servSocket == INVALID_SOCKET) {
		cout << "Server socket create error!" << endl;
	}
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_port = htons(port);
	sockAddr.sin_addr.s_addr = inet_addr(host.c_str());
	int rtn = bind(this->servSocket, (LPSOCKADDR) &(sockAddr), sizeof(sockAddr));
	if (rtn == SOCKET_ERROR) {
		cout << "Server socket bind error" << endl;
		this->close();
		return -1;
	}

	return 0;
}

int Server::listenSocket(int maxConnection) {
	int rtn = listen(this->servSocket, maxConnection);
	if (rtn == SOCKET_ERROR) {
		cout << "Server listen error!" << endl;
		this->close();
		return -1;
	}
	return 0;
}

void Server::close() {
	printf("server close\n");
	closesocket(this->servSocket);
	WSACleanup();
}

void Server::handleReq() {
	SOCKADDR clntAddr;
	SOCKET prevSocket;
	while (true) {
		char recvbuf[255];
		int nSize = sizeof(SOCKADDR);

		cout << "waiting for client socket" << endl;
		while (true) {
			SOCKET clntSock = accept(this->servSocket, (SOCKADDR*)&clntAddr, &nSize);
			struct timeval tv_out;
			tv_out.tv_sec = 1000;
			tv_out.tv_usec = 0;
			setsockopt(clntSock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof(tv_out));
			cout << "client coming" << endl;
			while (true) {
				char *str = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 11\r\n\r\nhello world";
				int iResult;
				do {
					iResult = recv(clntSock, recvbuf, 255, 0);
					if (iResult > 0)
						printf("Bytes received: %d\n", iResult);
					cout << 1 << endl;
				} while (iResult == 255);
				if (iResult <= 0) {
					closesocket(clntSock);
					cout << "close1" << endl;
					break;
				}
				int sendLen = send(clntSock, str, strlen(str) + sizeof(char), NULL);
			}
		}
	}
}