#pragma once

//Library Includes
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <string>

//Local Includes
#include "Resource.h"
#include "Network.h"
//#include "networkentity.h"
#include "Socket.h"

//This includes
#include "Client.h"


Client::Client() {

	m_pcPacketData = 0;
	m_pClientSocket = 0;

	//Creating a server socket address instance and zeros it out
	ZeroMemory(&m_ServerSocketAddress, sizeof(m_ServerSocketAddress));

	//Create a Packet Array and fill it out with all zeros.
	m_pcPacketData = new char[MAX_MESSAGE_LENGTH];
	ZeroMemory(m_pcPacketData, MAX_MESSAGE_LENGTH);

}

Client::~Client() {
	delete[] m_pcPacketData;
	m_pcPacketData = 0;

	delete m_pClientSocket;
	m_pClientSocket = 0;

	delete m_pWorkQueue;
	m_pWorkQueue = 0;
}

/***********************
* Initialise: Initialises a client object by creating a client socket and filling out the socket address structure with details of server to send the data to.
* @author:
* @parameter: none
* @return: void
********************/
bool Client::Initialise() {

	char _cServerIPAddress[128];			//Server IP address char array
	ZeroMemory(&_cServerIPAddress, 128);

	char _cServerPort[10];					//Server port char array
	ZeroMemory(&_cServerPort, 10);
	unsigned short _usServerPort;

	char _cServerChosen[5];					//Chosen server index
	ZeroMemory(_cServerChosen, 5);
	unsigned int _uiServerIndex;

	char _cUserName[50];					//Username
	ZeroMemory(&m_cUserName, 50);

	//Create a work queue to distribute messages between the main thread and the receive thread.
	m_pWorkQueue = new CWorkQueue<std::string>();

	//Create a socket object
	m_pClientSocket = new Socket();

	//Getting and binding the client port number
	//unsigned short _usClientPort = QueryPortNumber(DEFAULT_CLIENT_PORT);

	//Setting default port to 0
	unsigned short _usClientPort = 0;
	if (!m_pClientSocket->Initialise(_usClientPort)) {
		return false;
	}

	//Set the client's online status to true
	m_bOnline = true;

	//Use a boolean flag to determine if a valid server has been chosen by the client or not
	//Question 7: Broadcast to detect server
	m_bDoBroadcast = true;
	m_pClientSocket->EnableBroadcast();
	BroadcastForServers();
	bool _bServerChosen = false;
	do {
		//Question 7: Broadcast to detect server
		m_bDoBroadcast = true;
		m_pClientSocket->EnableBroadcast();
		BroadcastForServers();
		if (m_vecServerAddr.size() == 0) {
			std::cout << "No Servers Found " << std::endl;
			continue;
		}
		else {

			////Give a list of servers for the user to choose from :
			for (unsigned int i = 0; i < m_vecServerAddr.size(); i++) {
				std::cout << std::endl << "[" << i << "]" << " SERVER : found at " << ToString(m_vecServerAddr[i]) << std::endl;
			}
			std::cout << "Servers Found\n";
			_bServerChosen = true;
		}
		m_pClientSocket->DisableBroadcast();
		
	} while (_bServerChosen == false);



	return true;
}

bool Client::BroadcastForServers() {
	//Make a broadcast packet
	TPacket _packet;
	_packet.Serialize(BROADCAST, "Hello");

	char _pcTempBuffer[MAX_MESSAGE_LENGTH];
	//Send out a broadcast message using the broadcast address
	m_pClientSocket->SetRemoteAddress(INADDR_BROADCAST);
	m_pClientSocket->SetRemotePort(DEFAULT_SERVER_PORT);
	m_ServerSocketAddress.sin_family = AF_INET;
	m_ServerSocketAddress.sin_addr.S_un.S_addr = INADDR_BROADCAST;
	for (int i = 0; i < 10; i++) //Just try  a series of 10 ports to detect a running server; this is needed since we are testing multiple servers on the same local machine
	{
		m_ServerSocketAddress.sin_port = htons(DEFAULT_SERVER_PORT + i);
		SendData(_packet.PacketData);
	}
	ReceiveBroadcastMessages(_pcTempBuffer);

	return true;
}

void Client::ReceiveBroadcastMessages(char* _pcBufferToReceiveData) {
	//set a timer on the socket for one second
	struct timeval timeValue;
	timeValue.tv_sec = 30;
	timeValue.tv_usec = 0;
	setsockopt(m_pClientSocket->GetSocketHandle(), SOL_SOCKET, SO_RCVTIMEO,
		(char*)&timeValue, sizeof(timeValue));

	//Receive data into a local buffer
	char _buffer[MAX_MESSAGE_LENGTH];
	sockaddr_in _FromAddress;
	int iSizeOfAdd = sizeof(sockaddr_in);
	//char _pcAddress[50];

	while (m_bDoBroadcast) {
		// pull off the packet(s) using recvfrom()
		int _iNumOfBytesReceived = recvfrom(				// pulls a packet from a single source...
			this->m_pClientSocket->GetSocketHandle(),	// client-end socket being used to read from
			_buffer,									// incoming packet to be filled
			MAX_MESSAGE_LENGTH,							// length of incoming packet to be filled
			0,											// flags
			reinterpret_cast<sockaddr*>(&_FromAddress),	// address to be filled with packet source
			&iSizeOfAdd								// size of the above address struct.
		);

		if (_iNumOfBytesReceived < 0) {
			//Error in receiving data 
			int _iError = WSAGetLastError();
			//std::cout << "recvfrom failed with error " << _iError;
			if (_iError == WSAETIMEDOUT) // Socket timed out on Receive
			{
				//ErrorRoutines::PrintWSAErrorInfo(_iError);
				m_bDoBroadcast = false; //Do not broadcast any more
				break;
			}
			_pcBufferToReceiveData = 0;
		}
		else if (_iNumOfBytesReceived == 0) {
			//The remote end has shutdown the connection
			_pcBufferToReceiveData = 0;
		}
		else {
			//There is valid data received.
			strcpy_s(_pcBufferToReceiveData, strlen(_buffer) + 1, _buffer);
			if (
				//Lambda to check if the server address is already in the server address vector
				[&]()->bool {
				for (auto it : m_vecServerAddr) {
					if (ToString(_FromAddress) == ToString(it)) {
						return true;
					}
					else return false;
				}
			}() == false) {
				m_ServerSocketAddress = _FromAddress;
				m_vecServerAddr.push_back(m_ServerSocketAddress);
			}
		}
	}//End of while loop
}

bool Client::SendData(char* _pcDataToSend) {
	int _iBytesToSend = (int)strlen(_pcDataToSend) + 1;

	char _RemoteIP[MAX_ADDRESS_LENGTH];
	inet_ntop(AF_INET, &m_ServerSocketAddress.sin_addr, _RemoteIP, sizeof(_RemoteIP));
	char _message[MAX_MESSAGE_LENGTH];
	strcpy_s(_message, strlen(_pcDataToSend) + 1, _pcDataToSend);

	int iNumBytes = sendto(
		m_pClientSocket->GetSocketHandle(),				// socket to send through.
		_pcDataToSend,									// data to send
		_iBytesToSend,									// number of bytes to send
		0,												// flags
		reinterpret_cast<sockaddr*>(&m_ServerSocketAddress),	// address to be filled with packet target
		sizeof(m_ServerSocketAddress)							// size of the above address struct.
	);
	//iNumBytes;
	if (_iBytesToSend != iNumBytes) {
		std::cout << "There was an error in sending data from client to server" << std::endl;

		return false;
	}
	return true;
}

void Client::ReceiveData(char* _pcBufferToReceiveData) {
	sockaddr_in _FromAddress; // Make a local variable to extract the IP and port number of the sender from whom we are receiving
							  //In this case; it should be the details of the server; since the client only ever receives from the server
	int iSizeOfAdd = sizeof(_FromAddress);
	int _iNumOfBytesReceived;

	//Receive data into a local buffer
	char _buffer[MAX_MESSAGE_LENGTH];
	//For debugging purpose only, convert the Address structure to a string.
	char _pcAddress[50];
	ZeroMemory(&_pcAddress, 50);
	while (m_bOnline) {
		// pull off the packet(s) using recvfrom()
		_iNumOfBytesReceived = recvfrom(			// pulls a packet from a single source...
			this->m_pClientSocket->GetSocketHandle(),						// client-end socket being used to read from
			_buffer,							// incoming packet to be filled
			MAX_MESSAGE_LENGTH,					   // length of incoming packet to be filled
			0,										// flags
			reinterpret_cast<sockaddr*>(&_FromAddress),	// address to be filled with packet source
			&iSizeOfAdd								// size of the above address struct.
		);
		inet_ntop(AF_INET, &_FromAddress, _pcAddress, sizeof(_pcAddress));

		if (_iNumOfBytesReceived < 0) {

			_pcBufferToReceiveData = 0;
		}
		else if (_iNumOfBytesReceived == 0) {
			//The remote end has shutdown the connection
			_pcBufferToReceiveData = 0;
		}
		else {
			//There is valid data received.
			strcpy_s(m_pcPacketData, strlen(_buffer) + 1, _buffer);
			//Put this packet data in the workQ
			m_ServerSocketAddress = _FromAddress;
			m_pWorkQueue->push(m_pcPacketData);
		}
		//std::this_thread::yield(); //Yield the processor; giving the main a chance to run.
	}
}

void Client::ProcessData(char* _pcDataReceived) {

	TPacket _packetRecvd;
	_packetRecvd = _packetRecvd.Deserialize(_pcDataReceived);
	switch (_packetRecvd.MessageType) {
	case HANDSHAKE:
	{
		//If the user gets an error, the message will say error
		std::string Message(_pcDataReceived);

		if (strstr(_pcDataReceived, "Invalid Username") != nullptr) {
			//There was an error
			std::cout << "\rUsername was invalid, please enter another username:";

			//Creating a temp username
			ZeroMemory(m_cUserName, 50);

			//Getting user input
			do {
				gets_s(m_cUserName);
			} while (m_cUserName[0] == 0);

			//Package it off and send it to the server as a handshake message to recheck
			TPacket _HandShakeMsg;
			_HandShakeMsg.Serialize(HANDSHAKE, m_cUserName);
			SendData(_HandShakeMsg.PacketData);
		}
		else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			std::cout << "User Connected\n" << std::endl;
		}
		break;
	}
	case DATA:
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		std::cout << _packetRecvd.MessageContent << std::endl;
		break;
	}
	case KEEPALIVE: {
		//Resend the same message back to the server to verify it is still alive
		TPacket KeepAlive;
		KeepAlive.Serialize(KEEPALIVE, "KeepAlive");
		SendData(KeepAlive.PacketData);
	}
	default:break;

	}
}

void Client::GetRemoteIPAddress(char *_pcSendersIP) {
	inet_ntop(AF_INET, &(m_ServerSocketAddress.sin_addr), _pcSendersIP, sizeof(_pcSendersIP));
	return;
}

unsigned short Client::GetRemotePort() {
	return ntohs(m_ServerSocketAddress.sin_port);
}

void Client::ChooseServer(unsigned int _ServerIndex) {
	char _cUserName[50];					//Username
	ZeroMemory(&m_cUserName, 50);

	m_ServerSocketAddress.sin_family = AF_INET;
	m_ServerSocketAddress.sin_port = m_vecServerAddr[_ServerIndex].sin_port;
	m_ServerSocketAddress.sin_addr.S_un.S_addr = m_vecServerAddr[_ServerIndex].sin_addr.S_un.S_addr;
	std::string _strServerAddress = ToString(m_vecServerAddr[_ServerIndex]);
	std::cout << "Attempting to connect to server at " << _strServerAddress << std::endl;

	strcpy_s(_cUserName, "Henlo dis a hanshek");
	TPacket _packet;
	_packet.Serialize(HANDSHAKE, _cUserName);
	SendData(_packet.PacketData);
}

void Client::GetPacketData(char* _pcLocalBuffer) {
	strcpy_s(_pcLocalBuffer, strlen(m_pcPacketData) + 1, m_pcPacketData);
}

CWorkQueue<std::string>* Client::GetWorkQueue() {
	return m_pWorkQueue;
}