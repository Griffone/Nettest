#include "Utility.h"

StringParser::StringParser(const unsigned short & size) {
	dictionary.reserve(size);
}

StringParser::~StringParser() {
	dictionary.clear();
}

void StringParser::addWord(const std::string & _word, void(*_pFunc)(const std::string & args)) {
	DictionaryWord dw;
	dw.string = std::string(_word);
	dw.pFunc = _pFunc;
	dictionary.push_back(dw);
}

bool StringParser::removeWord(const std::string & word) {
	for (auto i = dictionary.begin(); i != dictionary.end(); ++i) {
		if ((*i).string == word) {
			dictionary.erase(i);
			return true;
		}
	}
	return false;
}

std::string String::firstWord(const std::string & string, size_t * _t) {
	size_t t = string.find_first_of(' ');
	if (_t)
		*_t = t;
	return (t != std::string::npos) ? string.substr(0, t) : string;
}

std::string String::lower(const std::string & string) {
	std::string s = string;
	for (auto i = s.begin(); i != s.end(); ++i)
		(*i) = tolower(*i);
	return s;
}

bool StringParser::parse(const std::string & string) {
	size_t t = string.find_first_of(' ');
	std::string word, args;
	if (t != std::string::npos) {
		word = string.substr(0, t);
		args = string.substr(t + 1);
	} else {
		word = string;
		args = "";
	}

	for (auto i = word.begin(); i != word.end(); ++i) {
		(*i) = tolower(*i);
	}

	for (auto i = dictionary.begin(); i != dictionary.end(); ++i) {
		if ((*i).string == word) {
			(*i).pFunc(args);
#if TEST_BUILD
			(*i).uses++;
#endif // TEST_BUILD
			return true;
		}
	}
}

#if TEST_BUILD
std::string StringParser::usage() {
	std::string s = "";
	for (auto i = dictionary.begin(); i != dictionary.end(); ++i) {
		s += (*i).string + ' ' + std::to_string((*i).uses) + '\n';
	}
	return s;
}
#endif // TEST_BUILD

Address stringToAddress(const std::string & str) {
	if (str == "")
		return Address();

	unsigned char addr[4];
	std::string work;
	size_t last_t = -1, t = 0;
	for (char i = 0; i < 4; i++) {
		t = str.find_first_of('.', t + 1);
		if (i != 3 && t == std::string::npos)
			return Address();
		work = str.substr(last_t + 1, t - last_t - 1);
		addr[i] = stoi(work);
		last_t = t;
	}
	t = str.find_last_of(':');
	if (t == std::string::npos)
		return Address(addr[0], addr[1], addr[2], addr[3], 0);

	work = str.substr(t + 1);
	PORT port = stoi(work);
	return Address(addr[0], addr[1], addr[2], addr[3], port);
}