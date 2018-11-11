#pragma once 

#include <unordered_map>
#include <string>
#include "Utils.h"

using namespace std;

class HttpReq {
public:
	HttpReq() {};
	HttpReq(string message);
	virtual ~HttpReq();
	string method;
	string url;
	string version;
	unordered_map<string, string> headers;
	string body;
	string rawMessage;
	void parse(string message);
};

class HttpRes {
public:
	HttpRes() { this->initStatusMap(this->statusMap); };
	virtual ~HttpRes();
	string version = "HTTP/1.1";
	int status = 200;
	string phrase = "ok";
	unordered_map<string, string> headers;
	string body;
	string toString();
	bool hasHeader(string name);
	void addHeader(string name, string value);
	string removeHeader(string name);
	unordered_map<int, string> statusMap;
private:
	void initStatusMap(unordered_map<int, string> statusMap);
};

class Http {
public:
	Http();
	Http(string message);
	virtual ~Http();
	HttpReq* req;
	HttpRes* res;
	static unordered_map<string, string> mimeType;
};
