#include "NetworkListener.h"

void NetworkListener::CreateSocket(const int pPort)
{
	//Set port
	mPeer.sin_family = AF_INET;
	mPeer.sin_port = htons(pPort);
	mPeer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//Create listening socket
	mListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mListenSocket == INVALID_SOCKET)
	{
		std::cerr << "Create socket failed with " << WSAGetLastError() << std::endl;
	}
	else if (bind(mListenSocket, (sockaddr *)&mPeer, sizeof(mPeer)) == SOCKET_ERROR)
	{
		std::cerr << "Bind failed with " << WSAGetLastError() << std::endl;
	}
	else if (listen(mListenSocket, 5) == SOCKET_ERROR)
	{
		std::cerr << "Listen failed with " << WSAGetLastError() << std::endl;
	}
}

void NetworkListener::Listen()
{
	std::cout << "Waiting..." << std::endl;

	//Create transfer socket
	char buffer;
	mTransferSocket = accept(mListenSocket, NULL, NULL);
	do
	{
		if (mTransferSocket == INVALID_SOCKET)
		{
			std::cerr << "Accept failed with " << WSAGetLastError() << std::endl;
			break;
		}
		else if (recv(mTransferSocket, &buffer, 1, 0) == SOCKET_ERROR)
		{
			std::cerr << "Receive failed with " << WSAGetLastError() << std::endl;
			break;
		}
		else
		{
			std::cout << "Message = " << buffer << std::endl;

			if (send(mTransferSocket, "Hi", 2, 0) == SOCKET_ERROR)
			{
				std::cerr << "Send failed with " << WSAGetLastError() << std::endl;
			}
			std::cout << "Waiting..." << std::endl;
		} 
	} while (buffer != '*');

	std::cout << "Finished..." << std::endl; char ch; std::cin >> ch;
}

NetworkListener::NetworkListener(const int pPort)
{
	CreateSocket(pPort);
}

NetworkListener::~NetworkListener()
{
}
