#pragma once

// Library Includes
#include "Resource.h"

// Local Includes
#include "Networkentity.h"
#include "WorkQueue.h"

//Forward Declaration
class Socket;

//Structure to hold the details of all connected clients
struct TClientDetails {
	sockaddr_in m_ClientAddress;
	bool m_bIsActive;
	std::string m_strName;
	float m_fTimeSinceLastMessage = 0.0f;
};

class Server : public INetworkEntity {
public:
	// Default Constructors/Destructors
	Server();
	~Server();

	// Virtual Methods from the Network Entity Interface.
	virtual bool Initialise(); //Implicit in the intialization is the creation and binding of the socket
	virtual bool SendData(char* _pcDataToSend);
	virtual void ReceiveData(char* _pcBufferToReceiveData);
	virtual void ProcessData(char* _pcDataReceived);
	virtual void GetRemoteIPAddress(char* _pcSendersIP);
	virtual unsigned short GetRemotePort();
	void KeepAliveCheck();
	void IncrimentTimers(float TimeDelta);

	CWorkQueue<char*>* GetWorkQueue();

private:
	//Qs 2: Function to add clients to the map.
	bool AddClient(std::string _strClientName);

	//A Buffer to contain all packet data for the server
	char* m_pcPacketData;
	//A server has a socket object to create the UDP socket at its end.
	Socket* m_pServerSocket;
	// Make a member variable to extract the IP and port number of the sender from whom we are receiving
	//Since it is a UDP socket capable of receiving from multiple clients; these details will change depending on who has sent the packet we are currently processing.
	sockaddr_in m_ClientAddress;

	//Qs 2 : Make a map to hold the details of all the client who have connected. 
	//The structure maps client addresses to client details
	std::map<std::string, TClientDetails>* m_pConnectedClients;

	//A workQueue to distribute messages between the main thread and Receive thread.
	CWorkQueue<char*>* m_pWorkQueue;
};

