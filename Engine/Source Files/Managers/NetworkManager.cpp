#include "NetworkManager.h"

NetworkManager::NetworkManager()
{
	InitWinSock();
}

void NetworkManager::InitWinSock()
{
	//Create version identifier
	WORD wVersionRequested = MAKEWORD( 2, 0 );

	// Startup windows sockets
	WSADATA wsaData;
	if ( WSAStartup( wVersionRequested, &wsaData )) 
	{
		std::cerr << "Socket initialisation failed" << std::endl;
	}
}

NetworkManager::~NetworkManager()
{
	// Cleanup windows sockets
	WSACleanup();
}

std::shared_ptr<NetworkManager> NetworkManager::Instance()
{
	static std::shared_ptr<NetworkManager> instance{ new NetworkManager };
	return instance;
}
