#pragma once
#include "Resource.h"

//Types
enum EEntityType {
	CLIENT = 1,
	SERVER
};

//constants
namespace {
	unsigned const DEFAULT_SERVER_PORT = 17832;
	unsigned const DEFAULT_CLIENT_PORT = 17932;
	unsigned const MAX_MESSAGE_LENGTH = 256;
	unsigned const MAX_ADDRESS_LENGTH = 32;
}

namespace ErrorRoutines {
	void PrintWSAErrorInfo(int iError);
}

//Forward Decalarations
class INetworkEntity;

class Network {
public:
	~Network();

	bool Initialise(EEntityType _eType);
	void StartUp(); //A network has an ability to start up
	void ShutDown(); //& an ability to be shut down
	bool IsOnline();
	//Accessor methods
	INetworkEntity* GetNetworkEntity();

	// Singleton Methods
	static Network* GetInstance();
	static void DestroyInstance();

private:
	//Make the network class a singleton. There is only one instance of the network running
	Network();
	Network(const Network& _kr);
	Network& operator= (const Network& _kr);

protected:
	//A network has a network entity
	INetworkEntity * m_pNetworkEntity;
	bool m_bOnline;

	// Singleton Instance
	static Network* s_pNetwork;
};