// main.cpp : Defines the entry point for the console application.

#include "GeneralHead.h"

#include <iostream>
#include <fstream>

#include "Utility.h"
#include "Network.h"

#define STR_GREET "A simple networking test project by Grigory Glukhov"
#define STR_VERSION "Version "
#define STR_HELP "For help use ? or help"

using namespace std;

// Global variables
bool work = true;
NetSocket * pSocket;
#define MAX_PACKET_SIZE 256

// Command-related stuff
struct command {
	const char * name;
	void(*pFunction)(const string &);
	const char * description, * usage;
};
extern command commands[];
#define COMMANDS_COUNT 7

// Command functions
void quit(const string & args) {
	work = false;
}
void help(const string & args) {
	if (args != "") {
		string word = String::lower(String::firstWord(args));
		for (unsigned short i = 0; i < COMMANDS_COUNT; i++) {
			if (commands[i].name == word) {
				if (commands[i].description == "")
					cout << commands[i - 1].name << " - " << commands[i - 1].description << '\n' << commands[i - 1].name << commands[i - 1].usage << '\n';
				else
					cout << commands[i].name << " - " << commands[i].description << '\n' << commands[i].name << commands[i].usage << '\n';
				return;
			}
		}
	}

	cout << "The program uses command-line, with this format:\n"
		<< "command/alternative_name [arg1] [arg2] .. [argN] - command effect\n"
		<< "Supported commands:\n\n";
	
	for (unsigned short i = 0; i < COMMANDS_COUNT; i++) {
		if (commands[i].pFunction == commands[i + 1].pFunction) {
			cout << commands[i].name << '/' << commands[i + 1].name << " - " << commands[i].description << '\n';
			i++;
		} else
			cout << commands[i].name << " - " << commands[i].description << '\n';
	}
}
void open(const string & args) {
	delete pSocket;
	pSocket = new NetSocket();
	PORT port;
	if (args != "") {
		string word = String::lower(String::firstWord(args));
		port = stoi(word);
	} else
		port = 0;

	if (pSocket->open(port))
		cout << "Opened socket on port " << to_string(port) << '\n';
	else
		cout << "Failed to open a socket!\n";

	pSocket->setBlocking(false);
}
void send(const string & args) {
	if (args == "") {
		cout << "Please specify address and message!\n";
		return;
	}
	size_t t;
	Address def;
	Address addr = stringToAddress(String::lower(String::firstWord(args, &t)));
	if (t == string::npos) {
		cout << "Please specify message!\n";
		return;
	}
	string str = args.substr(t + 1) + '\0';

	pSocket->send(addr, str.c_str(), str.length()) ? cout << "Message sent\n" : cout << "Error sending message! Is socket open?\n";
}
void test(const string & args) {
	pSocket->send(Address(), "Test message", sizeof("Test Message"));
}

command commands[] = {
	{ "help", &help, "prints this help message", "/? [command] - shows how to use the given command" },
	{ "?", &help, "", "" },
	{ "open", &open, "opens a port for sending and receiveing packets", " [port] - sets up a socket on given port"},
	{ "send", &send,  "send a packet to given address", " [address(a.b.c.d:port)] {packet} - send the packet to the given address"},
	{ "test", &test, "some iteration of testing", "this heavily depends on the version"},
	{ "quit", &quit, "closes the application", "/q" },
	{ "q", &quit, "", "" }
};

int main() {
	// Greeting message
	cout << STR_GREET << '\n' << STR_VERSION << VERSION << '\n' << STR_HELP << '\n';

	// Initialization
	string str;
	StringParser parser(sizeof(commands));
	for (unsigned short i = 0; i < COMMANDS_COUNT; i++)
		parser.addWord(commands[i].name, commands[i].pFunction);

	Net::initialize();
	pSocket = new NetSocket();

	char packet[MAX_PACKET_SIZE];
	Address address;

	while (work) {
		std::getline(cin, str);

		if (!parser.parse(str))
			cout << "Uknown command!\n";

		if (pSocket->isOpen())
			while (pSocket->receive(&address, packet, MAX_PACKET_SIZE) > 0)
				cout << "Received a packet from " << to_string(address.getA()) << '.' << to_string(address.getB()) << '.' << to_string(address.getC()) << '.' << to_string(address.getD()) << ':'
					<< to_string(address.getPort()) << ":\n" << packet << '\n';
	}

	delete pSocket;
	Net::terminate();

#if TEST_BUILD
	ofstream file;
	file.open("usagelog.txt");
	file << parser.usage();
#endif // TEST_BUILD

	return 0;
}