#include "ServerManager.h"

std::shared_ptr<ServerManager> ServerManager::ServerManagerPtr = nullptr;

ServerManager::ServerManager() {
	_pcPacketData = new char[MAX_MESSAGE_LENGTH];
	strcpy_s(_pcPacketData, strlen("") + 1, "");

	NetworkPtr = Network::GetInstance();
	NetworkPtr->StartUp();

	ClientPtr = nullptr;
	ServerPtr = nullptr;

	ZeroMemory(&IPAddressArray, strlen(IPAddressArray));
}

ServerManager::~ServerManager() {
	StopNetworkEntity();
	Network::DestroyInstance();
	ClientPtr = nullptr;
	ServerPtr = nullptr;
	NetworkPtr = nullptr;

	delete[] _pcPacketData;
	_pcPacketData = nullptr;
}

std::shared_ptr<ServerManager> ServerManager::GetInstance() {
	if (ServerManagerPtr == nullptr) ServerManagerPtr = std::shared_ptr<ServerManager>(new ServerManager());
	return ServerManagerPtr;
}

void ServerManager::DestroyInstance() {
	ServerManagerPtr = nullptr;
}

void ServerManager::SelectServer(unsigned int _Opt) {

}

void ServerManager::StartNetwork(EEntityType _Type) {
	NetworkEntityType = _Type;
	if (!NetworkPtr->GetInstance()->Initialise(NetworkEntityType)) {
		std::cout << "Unable to initialise the Network\n";
		return;
	}
	//Run receive on a separate thread so that it does not block the main client thread.
	if (NetworkEntityType == CLIENT) { //if network entity is a client 
		ClientPtr = static_cast<Client*>(NetworkPtr->GetInstance()->GetNetworkEntity());
		_ClientReceiveThread = std::thread(&Client::ReceiveData, ClientPtr, std::ref(_pcPacketData));
	}
	//Run receive of server also on a separate thread 
	else if (NetworkEntityType == SERVER) { //if network entity is a server 
		ServerPtr = static_cast<Server*>(NetworkPtr->GetInstance()->GetNetworkEntity());
		_ServerReceiveThread = std::thread(&Server::ReceiveData, ServerPtr, std::ref(_pcPacketData));
	}
}

void ServerManager::ProcessNetworkEntity() {
	if (NetworkEntityType == CLIENT) {//if network entity is a client
		if (ClientPtr != nullptr) {
			//If the message queue is empty 
			if (ClientPtr->GetWorkQueue()->empty()) {
				//Don't do anything
			} else {
				//Retrieve off a message from the queue and process it
				std::string temp;
				ClientPtr->GetWorkQueue()->pop(temp);
				ClientPtr->ProcessData(const_cast<char*>(temp.c_str()));
			}
		}
	} else { //if you are running a server instance
		if (ServerPtr != nullptr) {
			if (!ServerPtr->GetWorkQueue()->empty()) {
				NetworkPtr->GetInstance()->GetNetworkEntity()->GetRemoteIPAddress(IPAddressArray);
				//std::cout << _cIPAddress
				//<< ":" << _rNetwork.GetInstance().GetNetworkEntity()->GetRemotePort() << "> " << _pcPacketData << std::endl;

				//Retrieve off a message from the queue and process it
				ServerPtr->GetWorkQueue()->pop(_pcPacketData);
				ServerPtr->ProcessData(_pcPacketData);
			}
			//Keep Alive Functionality
			//If 5 seconds has elapsed, send a handshake message to the user to see if the user is still there
			//Pushing a Keep Alive packet to the server
			TimeSinceLastCheck++;
			if (TimeSinceLastCheck >= 500.0f) {
				std::cout << "Performing Keep Alive Check\n";
				ServerPtr->KeepAliveCheck();
				TimeSinceLastCheck = 0.0f;
			}
		}
	}
}

void ServerManager::SendPacket(std::string _Data) {
	TPacket DataPacket;
	DataPacket.Serialize(PLAYERPOS, const_cast<char*>(_Data.c_str()));
	NetworkPtr->GetNetworkEntity()->SendData(DataPacket.PacketData);
}

void ServerManager::StopNetworkEntity() {
	if (ClientPtr != nullptr) {
		ClientPtr->GetOnlineState() = false;
		_ClientReceiveThread.join();
		ClientPtr = nullptr;
	}

	if (ServerPtr != nullptr) {
		ServerPtr->GetOnlineState() = false;
		_ServerReceiveThread.join();
		ServerPtr = nullptr;
	}
}

glm::vec3 ServerManager::GetPlayerPos() {
	return ServerPtr->Player1Pos;
}

bool ServerManager::LobbyReady() {
	return ServerPtr->LobbyReady;
}
