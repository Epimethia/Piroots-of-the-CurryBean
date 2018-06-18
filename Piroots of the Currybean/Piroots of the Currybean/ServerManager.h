#pragma once
#include "Resource.h"
#include "Network.h"
#include "Client.h"
#include "Server.h"

class ServerManager {
public:
	void HostServer();
	void ProcessServer();
	void ConnectToServer();

	//Singleton Methods
	static std::shared_ptr<ServerManager> GetInstance();
	void DestroyInstance();

private:
	ServerManager();
	static std::shared_ptr<ServerManager> ServerManagerPtr;

	//Network instance
	Network& _rNetwork = Network::GetInstance();

	//Client and server
	Client* ClientPtr;
	Server* ServerPtr;

	//Threads for receive
	std::thread _ClientReceiveThread, _ServerReceiveThread;
	char* PacketData = 0; //A local buffer to receive packet data info

	char IPAddressArray[MAX_ADDRESS_LENGTH]; // An array to hold the IP Address as a string
	//ZeroMemory(&_cIPAddress, strlen(_cIPAddress));
};