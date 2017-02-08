#include "Network.h"

bool Net::initialized = false;

bool Net::initialize() {
	WSADATA data;

	Net::initialized = WSAStartup(MAKEWORD(2, 2), &data) == NO_ERROR;
	return Net::initialized;
}

void Net::terminate() {
	if (Net::initialized)
		WSACleanup();
}

Address::Address(const unsigned char & a, const unsigned char & b, const unsigned char & c, const unsigned char & d, const PORT & p) {
	address = (a << 24) | (b << 16) | (c << 8) | d;
	port = p;
}

Address::Address(const unsigned int & a, const PORT & p) {
	address = a;
	port = p;
}

unsigned char Address::getA() const {
	return address >> 24;
}

unsigned char Address::getB() const {
	return address >> 16;
}

unsigned char Address::getC() const {
	return address >> 8;
}

unsigned char Address::getD() const {
	return address;
}

unsigned int Address::getAddress() const {
	return address;
}

PORT Address::getPort() const {
	return port;
}

sockaddr_in Address::toSockaddr() const {
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(address);
	addr.sin_port = htons(port);
	return addr;
}


NetSocket::NetSocket() {
	handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

NetSocket::~NetSocket() {
	if (handle > 0)
		closesocket(handle);
}

bool NetSocket::open(const PORT & port) {
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(handle, (const sockaddr *)&address, sizeof(sockaddr_in)) != 0)
		return false;

	bOpen = true;
	return setBlocking(false);
}

bool NetSocket::setBlocking(const bool & b) {
	DWORD noBlock = !b;
	if (ioctlsocket(handle, FIONBIO, &noBlock) != 0)
		return false;
	return true;
}

bool NetSocket::isOpen() const {
	return bOpen;
}

bool NetSocket::send(const Address & addr, const char * data, const int & size) const {
	sockaddr_in a = addr.toSockaddr();
	int sentBytes = sendto(handle,
		data,
		size,
		0,
		(sockaddr*)&a,
		sizeof(sockaddr_in));
	return sentBytes == size;
}

int NetSocket::receive(Address * sender, char * data, const int & size) const {
	sockaddr_in from;
	int fromLength = sizeof(from);

	int bytes = recvfrom(handle, data, size, 0, (sockaddr*)&from, &fromLength);
	if (bytes > 0 && sender)
		*sender = Address(ntohl(from.sin_addr.s_addr), ntohs(from.sin_port));

	return bytes;
}