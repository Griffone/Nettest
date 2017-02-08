#pragma once

#include "GeneralHead.h"

#include <string>
#include <vector>
#define PARSER_DEFAULT_DICT_SIZE 16

#include "Network.h"

struct DictionaryWord {
	std::string string;
	void (*pFunc)(const std::string & args);
#if TEST_BUILD
	unsigned int uses = 0;
#endif // TEST_BUILD
};

namespace String {
	std::string firstWord(const std::string & string, size_t * pSecondWord = nullptr);
	std::string lower(const std::string &);
}

Address stringToAddress(const std::string &);

class StringParser {
public:
	StringParser(const unsigned short & size = PARSER_DEFAULT_DICT_SIZE);
	~StringParser();

	void addWord(const std::string & word, void (*pFunc)(const std::string & args));
	bool removeWord(const std::string &);

	bool parse(const std::string &);

#if TEST_BUILD
	std::string usage();
#endif // TEST_BUILD


private:
	std::vector<DictionaryWord> dictionary;
};