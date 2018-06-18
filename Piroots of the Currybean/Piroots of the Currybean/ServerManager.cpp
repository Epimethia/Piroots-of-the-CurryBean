#include "ServerManager.h"

std::shared_ptr<ServerManager> ServerManager::ServerManagerPtr = nullptr;

ServerManager::ServerManager() {
	_rNetwork.StartUp();
}

void ServerManager::HostServer() {
	ServerPtr = static_cast<Server*>(_rNetwork.GetInstance().GetNetworkEntity());
	_ServerReceiveThread = std::thread(&Server::ReceiveData, ServerPtr, std::ref(PacketData));
}

void ServerManager::ProcessServer() {
	if (_rNetwork.IsOnline()) {
		if (ServerPtr != nullptr) {
			if (!ServerPtr->GetWorkQueue()->empty()) {
				_rNetwork.GetInstance().GetNetworkEntity()->GetRemoteIPAddress(IPAddressArray);

				//Retrieve off a message from the queue and process it
				ServerPtr->GetWorkQueue()->pop(PacketData);
				ServerPtr->ProcessData(PacketData);
			}

			//Keep Alive Functionality
			//If 5 seconds has elapsed, send a handshake message to the user to see if the user is still there
			//Pushing a Keep Alive packet to the server

			//TO DO

			//Clock.Process();
			//Sleep(1);
			//TimeSinceLastCheck += Clock.GetDeltaTick();
			////If 2 seconds have gone by, perform a keep alive check
			//if (TimeSinceLastCheck >= 5000.0f) {

			//	ServerPtr->KeepAliveCheck();
			//	TimeSinceLastCheck = 0.0f;
			//}

		}
	}
}

void ServerManager::ConnectToServer() {
	if (!_rNetwork.GetInstance().Initialise(CLIENT)) {
		std::cout << "Unable to initialzie the network.\n";
	}
	ClientPtr = static_cast<Client*>(_rNetwork.GetInstance().GetNetworkEntity());
	_ClientReceiveThread = std::thread(&Client::ReceiveData, ClientPtr, std::ref(PacketData));

	Sleep(16);
	std::vector<std::string> ServerAddresses;

	for (auto it : ClientPtr->GetServers()) {
		ServerAddresses.push_back(ToString(it));
	}
}

std::shared_ptr<ServerManager> ServerManager::GetInstance() {
	if (ServerManagerPtr == nullptr) ServerManagerPtr = std::shared_ptr<ServerManager>(new ServerManager());
	return ServerManagerPtr;
}

void ServerManager::DestroyInstance() {

}
