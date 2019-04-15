#include "NetworkManager.h"

#pragma comment(lib, "Ws2_32.lib")

/// <summary>
/// 
/// </summary>
/// <param name="pPort"></param>
NetworkManager::NetworkManager(const int pPort)
	:mPeers(0)
{
	InitWinSock(pPort);
}


/// <summary>
/// 
/// </summary>
/// <param name="pPeerSocket"></param>
void NetworkManager::ProcessPeer(void* pPeerSocket)
{
	char buffer;
	SOCKET peerSocket = *(SOCKET*)pPeerSocket;
	//Handle communication until the peer disconnects
	do
	{
		if (recv(peerSocket, &buffer, 1, 0) == SOCKET_ERROR)
		{
			std::cerr << "Receive failed with " << WSAGetLastError() << std::endl;
			break;
		}
		else
		{
			std::cout << "Message = " << buffer << std::endl;

			if (send(peerSocket, "Hi", 2, 0) == SOCKET_ERROR)
			{
				std::cerr << "Send failed with " << WSAGetLastError() << std::endl;
			}
			std::cout << "Waiting..." << std::endl;
		}
	} while (buffer != '*');

	std::cout << "Finished..." << std::endl; char ch; std::cin >> ch;
}

/// <summary>
/// 
/// </summary>
NetworkManager::~NetworkManager()
{
	// Cleanup windows sockets
	WSACleanup();
}

/// <summary>
/// 
/// </summary>
void NetworkManager::Update()
{
	while (true)
	{
		SOCKET peerSocket = SOCKET_ERROR;

		//Wait for new peer and create socket for peer when peer connects
		while (peerSocket == SOCKET_ERROR)
		{
			OutputDebugString(L"Listening for peer!");
			peerSocket = accept(mListenSocket, NULL, NULL);
		}

		//Verify acception of socket
		if (peerSocket == INVALID_SOCKET)
		{
			std::cerr << "Accept failed with " << WSAGetLastError() << std::endl;
			break;
		}
		else
		{
			std::cout << "New peer connected!" << std::endl;

			//Add peer socket to thread
			mThreadManager->AddTask(std::bind(&NetworkManager::ProcessPeer, this, std::placeholders::_1), &peerSocket, nullptr, 1);
			mPeers++;
		}
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="pPort"></param>
void NetworkManager::InitWinSock(const int pPort)
{
	//Create version identifier
	WORD wVersionRequested = MAKEWORD( 2, 0 );

	// Startup windows sockets
	WSADATA wsaData;
	if ( WSAStartup( wVersionRequested, &wsaData )) 
	{
		std::cerr << "Socket initialisation failed" << std::endl;
	}

	//Set port
	mPort.sin_family = AF_INET;
	mPort.sin_port = htons(pPort);
	mPort.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//Create listening socket
	mListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mListenSocket == INVALID_SOCKET)
	{
		std::cerr << "Create socket failed with " << WSAGetLastError() << std::endl;
	}
	else if (bind(mListenSocket, (sockaddr *)&mPort, sizeof(mPort)) == SOCKET_ERROR)
	{
		std::cerr << "Bind failed with " << WSAGetLastError() << std::endl;
	}
	else if (listen(mListenSocket, 5) == SOCKET_ERROR)
	{
		std::cerr << "Listen failed with " << WSAGetLastError() << std::endl;
	}
	else
	{
		//Add listener to thread
		mThreadManager->AddTask(std::bind(&NetworkManager::Update, this), nullptr, nullptr, 1);
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="pPort"></param>
/// <returns></returns>
std::shared_ptr<NetworkManager> NetworkManager::Instance(const int pPort)
{
	static std::shared_ptr<NetworkManager> instance{ new NetworkManager(pPort) };
	return instance;
}
