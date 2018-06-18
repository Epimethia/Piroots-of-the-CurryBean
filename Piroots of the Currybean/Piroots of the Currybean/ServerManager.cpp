#include "ServerManager.h"

std::shared_ptr<ServerManager> ServerManager::ServerManagerPtr = nullptr;

void ServerManager::HostServer() {
	ServerPtr = static_cast<Server*>(NetworkPtr->GetInstance().GetNetworkEntity());
	_ClientReceiveThread = std::thread(&Server::ReceiveData, ServerPtr, std::ref(PacketData));
}

void ServerManager::ProcessServer() {
	if (NetworkPtr->IsOnline()) {
		if (ServerPtr != nullptr) {
			if (!ServerPtr->GetWorkQueue()->empty()) {
				NetworkPtr->GetInstance().GetNetworkEntity()->GetRemoteIPAddress(IPAddressArray);

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

std::vector<std::string> ServerManager::ConnectToServer() {
	return std::vector<std::string>();
}

std::shared_ptr<ServerManager> ServerManager::GetInstance() {
	if (ServerManagerPtr == nullptr) ServerManagerPtr = std::shared_ptr<ServerManager>();
	return ServerManagerPtr;
}

void ServerManager::DestroyInstance() {

}
