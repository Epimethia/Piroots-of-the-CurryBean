#pragma once
#include "Networkentity.h"
#include "WorkQueue.h"
#include "Socket.h"

class Socket;

class Client : public INetworkEntity {
public:
	Client();
	~Client();

	virtual bool Initialise();

	virtual void GetRemoteIPAddress(char* _pcSendersIP);
	virtual unsigned short GetRemotePort();
	void GetPacketData(char* _pcLocalBuffer);

	virtual bool SendData(char* _pcDataToSend);
	virtual void ReceiveData(char* _pcBufferToReceiveData);
	virtual void ProcessData(char* _pcDataReceived);

	bool BroadcastForServers();
	void ChooseServer(unsigned int _ServerIndex);
	std::vector<sockaddr_in> GetServers() { return m_vecServerAddr; };

	std::shared_ptr<CWorkQueue<std::string>> GetWorkQueue();

private:
	void ReceiveBroadcastMessages(char* _pcBufferToReceiveData);

	char* m_pcPacketData;									//Packet to send
	std::shared_ptr<Socket> m_pClientSocket;								//Client Socket
	sockaddr_in m_ServerSocketAddress;						//Server details
	char m_cUserName[50];									//Client Username
	std::shared_ptr<CWorkQueue<std::string>> m_pWorkQueue;	//WorkQueue to process messages

	std::vector<sockaddr_in> m_vecServerAddr;				//Vector to hold server addresses
	bool m_bDoBroadcast;
};