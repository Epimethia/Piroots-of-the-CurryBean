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
		}
	}
}

void ServerManager::StartClient() {
	if (!_rNetwork.GetInstance().Initialise(CLIENT)) {
		std::cout << "Unable to initialzie the network.\n";
	}
	ClientPtr = static_cast<Client*>(_rNetwork.GetInstance().GetNetworkEntity());
	_ClientReceiveThread = std::thread(&Client::ReceiveData, ClientPtr, std::ref(PacketData));

	Sleep(16);
}

void ServerManager::SelectServer(unsigned int _Opt) {
	ClientPtr->ChooseServer(_Opt);
}

std::shared_ptr<ServerManager> ServerManager::GetInstance() {
	if (ServerManagerPtr == nullptr) ServerManagerPtr = std::shared_ptr<ServerManager>(new ServerManager());
	return ServerManagerPtr;
}

void ServerManager::DestroyInstance() {

}
