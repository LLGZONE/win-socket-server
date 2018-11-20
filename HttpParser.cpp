#include "HttpParser.h"

HttpReq::HttpReq(string message) {
	this->rawMessage = message;
	this->parse(message);
}

HttpReq::~HttpReq() {
	this->headers.clear();
}

void HttpReq::parse(string message) {
	this->rawMessage = message;
	vector<string> v, header;
	Utils::split(message, v, "\r\n\r\n");
	if (v.size() > 1) {
		this->body = v.at(1);
		v.pop_back();
	}
	else {
		this->body = "";
	}
	Utils::split(v.front(), header, "\r\n");
	string statusLine = header.front();
	for (auto iter = header.begin() + 1; iter != header.end(); iter++) {
		string headerStr = *iter;
		vector<string> headerPair;
		Utils::split(headerStr, headerPair, ":");
		this->headers[headerPair[0]] = Utils::trim(headerPair[1]);
	}
	vector<string> statusVec;
	Utils::split(statusLine, statusVec, " ");
	this->method = statusVec[0];
	this->url = statusVec[1];
	this->version = statusVec[2];
}

HttpRes::~HttpRes() {
	this->headers.clear();
}

string HttpRes::toString() {
	string statusLine = this->version + " " + std::to_string(this->status) + " " + this->phrase + "\r\n";
	string headers = "";
	string headerEnd = "\r\n";
	string body = string(this->body.begin(), this->body.end());
	for (auto header : this->headers) {
		headers += header.first + ": " + header.second + "\r\n";
	}
	if (this->hasHeader("Content-Length")) {
		string length = std::to_string(body.size());
		headers += "Content-Length: " + length + "\r\n";
	}
	return statusLine + headers + headerEnd + body;
}

char* HttpRes::c_str() {
	string statusLine = this->version + " " + std::to_string(this->status) + " " + this->phrase + "\r\n";
	string headers = "";
	string headerEnd = "\r\n";
	for (auto header : this->headers) {
		headers += header.first + ": " + header.second + "\r\n";
	}
	if (this->hasHeader("Content-Length")) {
		string length = std::to_string(this->body.size());
		headers += "Content-Length: " + length + "\r\n";
	}
	string header = statusLine + headers + headerEnd;
	int len = header.size() + this->body.size();
	int bodySize = 0;
	const char* first = header.c_str();
	char* res = new char[len];
	for (int i = 0; i < strlen(first); i++) {
		res[i] = first[i];
	}
	for (int i = strlen(first), j = 0; i < len; i++) {
		res[i] = this->body[j++];
	}
	printf("len: %d,header: %d, body: %d", len, header.size(), this->body.size());
	return res;
}

bool HttpRes::hasHeader(string name) {
	auto iter = this->headers.find(name);
	if (this->headers.end() == iter) {
		return true;
	}
	return false;
}

void HttpRes::addHeader(string name, string value) {
	this->headers[name] = value;
}

string HttpRes::removeHeader(string name) {
	auto value = this->headers[name];
	this->headers.erase(name);
	return value;
}

int HttpRes::size() {
	string statusLine = this->version + " " + std::to_string(this->status) + " " + this->phrase + "\r\n";
	string headers = "";
	string headerEnd = "\r\n";
	for (auto header : this->headers) {
		headers += header.first + ": " + header.second + "\r\n";
	}
	if (this->hasHeader("Content-Length")) {
		string length = std::to_string(this->body.size());
		headers += "Content-Length: " + length + "\r\n";
	}
	string header = statusLine + headers + headerEnd;
	int len = header.size() + this->body.size();
	return len;
}

Http::Http() {
	this->req = new HttpReq();
	this->res = new HttpRes();
}

Http::Http(string message) {
	this->req = new HttpReq(message);
	this->res = new HttpRes();
}

Http::~Http() {
	delete this->req;
	delete this->res;
}


void HttpRes::initStatusMap(unordered_map<int, string> statusMap) {
	statusMap[100] = "Continue";
	statusMap[101] = "Switching Protocols";
	statusMap[200] = "OK";
	statusMap[201] = "Created";
	statusMap[202] = "Accepted";
	statusMap[203] = "Non-Authoritative Information";
	statusMap[204] = "No Content";
	statusMap[205] = "Reset Content";
	statusMap[206] = "Partial Content";
	statusMap[300] = "Multiple Choices";
	statusMap[301] = "Moved Permanently";
	statusMap[302] = "Found";
	statusMap[303] = "See Other";
	statusMap[304] = "Not Modified";
	statusMap[305] = "Use Proxy";
	statusMap[307] = "Temporary Redirect";
	statusMap[400] = "Bad Request";
	statusMap[401] = "Unauthorized";
	statusMap[402] = "Payment Required";
	statusMap[403] = "Forbidden";
	statusMap[404] = "Not Found";
	statusMap[405] = "Method Not Allowed";
	statusMap[406] = "Not Acceptable";
	statusMap[407] = "Proxy Authentication Required";
	statusMap[408] = "Request Time-out";
	statusMap[409] = "Conflict";
	statusMap[410] = "Gone";
	statusMap[411] = "Length Required";
	statusMap[412] = "Precondition Failed";
	statusMap[413] = "Request Entity Too Large";
	statusMap[414] = "Request-URI Too Large";
	statusMap[415] = "Unsupported Media Type";
	statusMap[416] = "Requested range not satisfiable";
	statusMap[417] = "Expectation Failed";
	statusMap[500] = "Internal Server Error";
	statusMap[501] = "Not Implemented";
	statusMap[502] = "Bad Gateway";
	statusMap[503] = "Service Unavailable";
	statusMap[504] = "Gateway Time-out";
	statusMap[505] = "HTTP Version not supported";
}

unordered_map<string, string> Http::mimeType(Utils::parseMimeTypes("./mime.types"));
