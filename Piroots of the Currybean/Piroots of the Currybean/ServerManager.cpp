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
				//std::cout << _cIPAddress
				//<< ":" << _rNetwork.GetInstance().GetNetworkEntity()->GetRemotePort() << "> " << _pcPacketData << std::endl;

				//Retrieve off a message from the queue and process it
				ServerPtr->GetWorkQueue()->pop(PacketData);
				ServerPtr->ProcessData(PacketData);
			}

			//Keep Alive Functionality
			//If 5 seconds has elapsed, send a handshake message to the user to see if the user is still there
			//Pushing a Keep Alive packet to the server
			Sleep(1);
			TimeSinceLastCheck++;
			//If 2 seconds have gone by, perform a keep alive check
			if (TimeSinceLastCheck >= 10000.0f) {
				std::cout << "Performing Keep Alive Check\n";
				ServerPtr->KeepAliveCheck();
				TimeSinceLastCheck = 0.0f;
			}
		}
		else {
			
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
