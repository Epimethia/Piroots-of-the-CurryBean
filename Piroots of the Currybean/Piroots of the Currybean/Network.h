#pragma once

//Types
enum EEntityType {
	CLIENT = 1,
	SERVER
};

//constants
namespace {
	unsigned const DEFAULT_SERVER_PORT = 50023;
	unsigned const DEFAULT_CLIENT_PORT = 60013;
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
	static Network& GetInstance();
	static void DestroyInstance();

private:
	Network();
	Network(const Network& _kr);
	Network& operator= (const Network& _kr);

protected:

	INetworkEntity* m_pNetworkEntity;
	bool m_bOnline;

	static Network* s_pNetwork;
};
