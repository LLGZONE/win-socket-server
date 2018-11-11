#include "Utils.h"

vector<string>& Utils::split(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
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

string Utils::trim(const string& str)
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

unordered_map<string, string> &Utils::parseMimeTypes(const string &path) {
	string content = readFile(path);
	unordered_map<string, string> mimeTypes;
	vector<string> lines;
	for (auto line : split(content, lines, "\n")) {
		auto str = trim(line);
		if (str[0] != '#') {
			vector<string> mimetypes;
			split(str, mimetypes, " ");
			mimeTypes[mimetypes[1]] = mimetypes[0];
		}
	}

	return mimeTypes;
}