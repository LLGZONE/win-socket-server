#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <mutex>
using namespace std;

namespace Utils {
	inline vector<string>& split(const std::string& s, std::vector<std::string>& v, const std::string& c) {
		std::string::size_type pos1, pos2;
		size_t len = s.length();
		pos2 = s.find(c);
		pos1 = 0;
		while (std::string::npos != pos2)
		{
			v.emplace_back(s.substr(pos1, pos2 - pos1));

			pos1 = pos2 + c.size();
			pos2 = s.find(c, pos1);
		}
		if (pos1 != len)
			v.emplace_back(s.substr(pos1));
		return v;
	}

	inline string trim(const string& str)
	{
		string::size_type pos = str.find_first_not_of(' ');
		if (pos == string::npos)
		{
			return str;
		}
		string::size_type pos2 = str.find_last_not_of(' ');
		if (pos2 != string::npos)
		{
			return str.substr(pos, pos2 - pos + 1);
		}
		return str.substr(pos);
	}

	inline string readFile(const string &path) {
		ifstream f(path, ios::binary);
		if (!f) {
			throw runtime_error("open file error");
		}

		string content = string((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
		return content;
	}

	inline vector<char> readFile(const string &path, int v) {
		ifstream f(path, ifstream::binary);
		if (!f) {
			throw runtime_error("open file error");
		}
		f.seekg(0, f.end);
		int length = f.tellg();
		f.seekg(0, f.beg);

		char *buffer = new char[length];
		vector<char> vec;
		f.read(buffer, length);
		f.close();
		for (int i = 0; i < length; i++) {
			vec.push_back(buffer[i]);
		}
		return vec;
	}

	inline unordered_map<string, string> parseMimeTypes(const string &path) {
		unordered_map<string, string> mimeTypes;
		string content = readFile(path);

		vector<string> lines;
		for (auto line : split(content, lines, "\n")) {
			auto str = trim(line);
			if (str[0] != '#') {
				vector<string> mime;
				split(str, mime, " ");
				mimeTypes[mime[1]] = mime[0];
			}
		}
		return mimeTypes;

	}
}