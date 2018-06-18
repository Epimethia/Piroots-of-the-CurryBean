#pragma once
// Library Includes
#include <string>

// Local Includes
#include "Resource.h"
#include "Networkentity.h"
#include "WorkQueue.h"

// Types

// Constants


//Forward Declaration
class Socket;

class Client : public INetworkEntity {
public:
	// Default Constructors/Destructors
	Client();
	~Client();

	// Methods

	virtual bool Initialise(); //Implicit in the intialization is the creation and binding of the socket
	virtual bool SendData(char* _pcDataToSend);
	virtual void ReceiveData(char* _pcBufferToReceiveData);
	virtual void ProcessData(char* _pcDataReceived);
	virtual void GetRemoteIPAddress(char* _pcSendersIP);
	virtual unsigned short GetRemotePort();
	std::vector<sockaddr_in> GetServers() { return m_vecServerAddr; };

	void GetPacketData(char* _pcLocalBuffer);
	CWorkQueue<std::string>* GetWorkQueue();

	//Qs7 : Broadcast to Detect Servers
	bool BroadcastForServers();

private:
	// Question 7 : Broadcast to Detect Servers
	void ReceiveBroadcastMessages(char* _pcBufferToReceiveData);

private:
	//A buffer to contain all packet data for the client
	char* m_pcPacketData;
	//A client has a socket object to create the UDP socket at its end.
	Socket* m_pClientSocket;
	// A Sockaddress structure which will have the details of the server 
	sockaddr_in m_ServerSocketAddress;
	//A username to associate with a client
	char m_cUserName[50];
	//A workQueue to distribute messages between the main thread and Receive thread.
	CWorkQueue<std::string>* m_pWorkQueue;

	//Question 7
	//A vector to hold all the servers found after broadcasting
	std::vector<sockaddr_in> m_vecServerAddr;
	bool m_bDoBroadcast;
};