#pragma once

#include "Resource.h"

class Socket {
public:
	Socket();
	~Socket();

	bool Initialise(unsigned short _usPortNumber);

	SOCKET GetSocketHandle();

	void SetRemotePort(unsigned short _usRemotePort);
	void SetRemoteAddress(unsigned long _ulRemoteAddress);

	int EnableBroadcast();
	int DisableBroadcast();

private:
	std::string GetLocalAddress();
	
	SOCKET m_hSocket;						//The local socket
	sockaddr_in m_SocketAddress;			//Socket address containing the local address and port of the socket

	unsigned long m_ulRemoteIPAddress;		//The server address and port
	unsigned short m_usRemotePort;
};
