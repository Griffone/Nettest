#pragma once

#include <WinSock2.h>

#pragma comment(lib, "wsock32.lib")

typedef unsigned short PORT;

namespace Net {

	extern bool initialized;

	bool initialize();

	void terminate();
}

class Address {
public:
	Address(const unsigned char & A = 127, const unsigned char & B = 0, const unsigned char & C = 0, const unsigned char & D  = 1, const PORT & port = 8118);
	Address(const unsigned int & adress, const PORT & port);

	unsigned char getA() const;
	unsigned char getB() const;
	unsigned char getC() const;
	unsigned char getD() const;

	unsigned int getAddress() const;
	PORT getPort() const;

	sockaddr_in toSockaddr() const;

private:
	unsigned int address;
	PORT port;
};

class NetSocket {
public:
	NetSocket();
	~NetSocket();

	bool open(const PORT &);
	bool send(const Address & addr, const char * data, const int & size) const;
	bool setBlocking(const bool &);
	bool isOpen() const;
	int receive(Address * pSender, char * data, const int & size) const;

private:
	int handle;
	bool bOpen = false;
};