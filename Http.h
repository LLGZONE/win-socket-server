#include <unordered_map>
#include <string>

using namespace std;

class HttpReq {
  public:
    HttpReq();
    virtual ~HttpReq();
    string method;
    string url;
    string version;
    unordered_map<string, string> headers;
	string body;
	string toString();
};

class HttpRes {
	public:
		HttpRes();
		virtual ~HttpRes();
		string version = "HTTP/1.1";
		int status;
		string phrase;
		unordered_map<string, string> headers;
		string body;
		string toString();
};


class Http {
	public:
		Http();
		virtual ~Http();
		HttpReq req;
		HttpRes res;
};
