#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
using namespace std;

namespace Utils {
	vector<string>& split(const std::string& s, std::vector<std::string>& v, const std::string& c);
	string trim(const string& str);
	inline string&readFile(const string &path) {
		ifstream f(path);
		if (!f) {
			throw runtime_error("open file error");
		}
		return string((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
	}
	unordered_map<string, string> &parseMimeTypes(const string &path);
}