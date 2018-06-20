#pragma once
#include "Resource.h"
#include "Client.h"
#include "Server.h"

class ServerManager {
public:
	~ServerManager();

	//Singleton Methods
	static std::shared_ptr<ServerManager> GetInstance();
	static void DestroyInstance();

	//Server Methods
	void SelectServer(unsigned int _Opt);
	void StartHost();
	void StartClient();

	void ProcessNetworkEntity();
	void StopNetworkEntity();

	EEntityType NetworkEntityType;

	std::vector<sockaddr_in> GetServerList() { return ClientPtr->GetServers(); };

private:
	float TimeSinceLastCheck = 0.0f;
	ServerManager();
	static std::shared_ptr<ServerManager> ServerManagerPtr;

	//Network instance
	Network& _rNetwork = Network::GetInstance();

	//Client and server
	Client* ClientPtr;
	Server* ServerPtr;

	//Threads for receive
	std::thread _ClientReceiveThread, _ServerReceiveThread;

	//Packet Send/recieve information
	char* PacketData = 0;
	// An array to hold the IP Address as a string
	char IPAddressArray[MAX_ADDRESS_LENGTH];
};