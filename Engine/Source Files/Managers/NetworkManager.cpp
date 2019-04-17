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
	bool readingMessage = false;
	std::string message;

	//Handle communication until the peer disconnects
	do
	{
		if (recv(peerSocket, &buffer, 1, 0) == SOCKET_ERROR)
		{
			OutputDebugString(L"Receive failed with " + WSAGetLastError());
			break;
		}
		else
		{
			//Check for identifier at beginning of message
			if (buffer == '$' && readingMessage == false)
			{
				char tempBuffer[4];
				tempBuffer[0] = buffer;
				recv(peerSocket, &tempBuffer[1], 3, 0);

				std::string tempBufferString = tempBuffer;

				//If identifier found start reading message
				if (tempBufferString == mIdentifier)
				{
					readingMessage = true;
					OutputDebugString(L"Reading message!");
				}
			}

			//Check for terminator at end of messsage
			if (buffer == '*' && readingMessage == true)
			{
				char tempBuffer[4];
				tempBuffer[0] = buffer;
				recv(peerSocket, &tempBuffer[1], 3, 0);

				std::string tempBufferString = tempBuffer;

				//If terminator found, add message to received messages queue and set reading to false
				if (tempBufferString == mTerminator)
				{
					readingMessage = false;
					std::lock_guard<std::mutex> lock(mx);
					mMessagesReceived.push(message);
					OutputDebugString(L"Message read!");
					message = "";
				}
				else
				{
					//Add buffer to message if not terminator
					message += tempBuffer;
				}
			}
			else if (readingMessage == true)
			{
				//Add buffer to message
				message += buffer;
			}
		}
	} while (true);

	OutputDebugString(L"Peer disconnected!");
}

/// <summary>
/// 
/// </summary>
void NetworkManager::SendMessages()
{

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
void NetworkManager::Listen()
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
			OutputDebugString(L"Accept failed with " + WSAGetLastError());
			break;
		}
		else
		{
			OutputDebugString(L"New peer connected!");

			//Add peer socket to thread
			mThreadManager->AddTask(std::bind(&NetworkManager::ProcessPeer, this, std::placeholders::_1), &peerSocket, nullptr, 1);
			mPeers.push_back(peerSocket);
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
	WORD wVersionRequested = MAKEWORD(2, 0);

	// Startup windows sockets
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData))
	{
		OutputDebugString(L"Socket initialisation failed");
	}

	//Set port
	mPort.sin_family = AF_INET;
	mPort.sin_port = htons(pPort);
	mPort.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//Create listening socket
	mListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mListenSocket == INVALID_SOCKET)
	{
		OutputDebugString(L"Create socket failed with " + WSAGetLastError());
	}
	else if (bind(mListenSocket, (sockaddr *)&mPort, sizeof(mPort)) == SOCKET_ERROR)
	{
		OutputDebugString(L"Bind failed with " + WSAGetLastError());
	}
	else if (listen(mListenSocket, 5) == SOCKET_ERROR)
	{
		OutputDebugString(L"Listen failed with " + WSAGetLastError());
	}
	else
	{
		//Add listener to thread
		mThreadManager->AddTask(std::bind(&NetworkManager::Listen, this), nullptr, nullptr, 1);
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="pMessage"></param>
void NetworkManager::AddMessage(const std::string & pMessage)
{
	std::lock_guard<std::mutex> lock(mx);
	mMessagesToSend.push(mIdentifier + pMessage + mTerminator);
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::queue<std::string>& NetworkManager::ReadMessages()
{
	return mMessagesReceived;
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
