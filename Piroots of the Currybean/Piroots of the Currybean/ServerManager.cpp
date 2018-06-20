#include "ServerManager.h"

std::shared_ptr<ServerManager> ServerManager::ServerManagerPtr = nullptr;

ServerManager::ServerManager() {
	ZeroMemory(&IPAddressArray, strlen(IPAddressArray));
	_rNetwork.StartUp();
}

ServerManager::~ServerManager() {
	StopNetworkEntity();
	_rNetwork.DestroyInstance();
	delete ClientPtr;
	ClientPtr = nullptr;
	delete ServerPtr;
	ServerPtr = nullptr;
}

std::shared_ptr<ServerManager> ServerManager::GetInstance() {
	if (ServerManagerPtr == nullptr) ServerManagerPtr = std::shared_ptr<ServerManager>(new ServerManager());
	return ServerManagerPtr;
}

void ServerManager::DestroyInstance() {
	ServerManagerPtr = nullptr;
}

void ServerManager::SelectServer(unsigned int _Opt) {
	ClientPtr->ChooseServer(_Opt);
}

void ServerManager::StartHost() {
	if (!_rNetwork.GetInstance().Initialise(SERVER)) {
		std::cout << "Unable to initialize the network.\n";
	}
	if (ServerPtr == nullptr) {
		ServerPtr = static_cast<Server*>(_rNetwork.GetInstance().GetNetworkEntity());
	}
	if (_ServerReceiveThread.joinable() == false) {
		std::cout << "Created Server Thread\n";
		_ServerReceiveThread = std::thread(&Server::ReceiveData, ServerPtr, std::ref(PacketData));
	}
}

void ServerManager::StartClient() {
	if (!_rNetwork.GetInstance().Initialise(CLIENT)) {
		std::cout << "Unable to initialize the network.\n";
	}
	if (ClientPtr == nullptr) {
		ClientPtr = static_cast<Client*>(_rNetwork.GetInstance().GetNetworkEntity());
	}
	if (_ClientReceiveThread.joinable() == false) {
		std::cout << "Created Client Thread\n";
		_ClientReceiveThread = std::thread(&Client::ReceiveData, ClientPtr, std::ref(PacketData));
	}
}

void ServerManager::ProcessNetworkEntity() {
	if (_rNetwork.IsOnline()) {
		if (ServerPtr != nullptr) {
			if (!ServerPtr->GetWorkQueue()->empty()) {
				_rNetwork.GetInstance().GetNetworkEntity()->GetRemoteIPAddress(IPAddressArray);

				//Retrieve off a message from the queue and process it
				ServerPtr->GetWorkQueue()->pop(PacketData);
				ServerPtr->ProcessData(PacketData);
			}
		}
		else if (ClientPtr != nullptr) {
			//If the message queue is empty 
			if (ClientPtr->GetWorkQueue()->empty()) {
				//Don't do anything
			}
			else {
				//Retrieve off a message from the queue and process it
				std::string temp;
				ClientPtr->GetWorkQueue()->pop(temp);
				ClientPtr->ProcessData(const_cast<char*>(temp.c_str()));
			}
		}
	}

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
