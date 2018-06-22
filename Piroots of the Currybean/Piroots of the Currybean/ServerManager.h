#pragma once
#include "Resource.h"
#include "Client.h"
#include "Server.h"
#include "Network.h"

class ServerManager {
public:
	~ServerManager();

	//Singleton Methods
	static std::shared_ptr<ServerManager> GetInstance();
	static void DestroyInstance();

	//Server Methods
	bool SelectServer(unsigned int _Opt);
	std::vector<sockaddr_in> GetServerAddrs();

	void StartNetwork(EEntityType _Type);
	void ProcessNetworkEntity();
	void SendPacket(std::string _Data);
	void StopNetworkEntity();
	
	glm::vec3 GetPlayerPos();

	bool LobbyReady();


private:
	ServerManager();										//Singleton constructor
	static std::shared_ptr<ServerManager> ServerManagerPtr;	//Singleton pointer

	Client* ClientPtr;								//Pointer to the client instance (if it is running)
	Server* ServerPtr;								//Pointer to the server instance (if it is running)

	Network* NetworkPtr;

	EEntityType NetworkEntityType;

	std::thread _ClientReceiveThread;				//Separate thread for client receive so that it does not block main
	std::thread _ServerReceiveThread;				//Separate thread for server receive so that it does not block main

	char* _pcPacketData = 0;						//Packet Send/recieve information
	char IPAddressArray[MAX_ADDRESS_LENGTH];		//Ip Address array

	float TimeSinceLastCheck;						//Keep alive message timer

	
};