#include "HttpServer.h"
#include <functional>
#include <Windows.h>
#include <csignal>

int handle();

int main(int argc, char*argv) {
	HttpServer server;
	signal(SIGINT, (_crt_signal_t)handle);
	server.config("./conf.conf");
	server.createHttpServer(server.host, server.port);
}

int handle() {
	printf("bye!");
	Sleep(1000);
	exit(1);
	return 1;
}