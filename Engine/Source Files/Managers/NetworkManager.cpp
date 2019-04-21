#include "NetworkManager.h"

#pragma comment(lib, "Ws2_32.lib")

/// <summary>
/// Constructor
/// Reserves space for up to 5 peers
/// </summary>
NetworkManager::NetworkManager()
	:mMessagesToSend(), mMessagesToSend2(), mActiveQueue(&mMessagesToSend), mFlushingQueue(&mMessagesToSend2), mPeerCount(0)
{
	mPeers.reserve(5);
}


/// <summary>
/// Listens for messages from a single connected peer
/// Adds all received messages to the received messages queue to be processed later
/// </summary>
/// <param name="pPeerSocket">The socket of the peer to listen to</param>
void NetworkManager::ListenToPeer(void* pPeerSocket)
{
	char buffer;
	SOCKET peerSocket = *(int*)pPeerSocket;
	bool readingMessage = false;
	std::string message;

	//Handle communication until the peer disconnects
	do
	{
		if (recv(peerSocket, &buffer, 1, 0) == SOCKET_ERROR)
		{
			OutputDebugString(L"Receive failed with ");
			OutputDebugString(std::to_wstring(WSAGetLastError()).c_str());
			break;
		}
		else
		{
			//Check for identifier at beginning of message
			if (buffer == '$' && readingMessage == false)
			{
				char tempBuffer[5];
				tempBuffer[4] = '\0';
				tempBuffer[0] = buffer;
				recv(peerSocket, &tempBuffer[1], 3, 0);

				std::string tempBufferString = tempBuffer;

				//If identifier found start reading message
				if (tempBufferString == mIdentifier)
				{
					readingMessage = true;
					OutputDebugString(L"Reading message!");
					buffer = '\0';
				}
			}

			//Check for terminator at end of messsage
			if (buffer == '*' && readingMessage == true)
			{
				char tempBuffer[5];
				tempBuffer[4] = '\0';
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
			else if (readingMessage == true && buffer != '\0')
			{
				//Add buffer to message
				message += buffer;
			}
		}
	} while (true);

	//When peer disconnects
	mPeerCount--;
	mPeers.erase(remove(mPeers.begin(), mPeers.end(), peerSocket), mPeers.end());
	OutputDebugString(L"Peer disconnected!");
}

/// <summary>
/// Sends all the messages in the messages queue
/// </summary>
void NetworkManager::SendMessages()
{
	while (true)
	{
		{
			//Switches the active queue and the flushing queue so while messages are being flushed new messages can be put on the queue
			std::lock_guard<std::mutex> lock(mx);
			std::queue<std::string>* temp = mActiveQueue;
			mActiveQueue = mFlushingQueue;
			mFlushingQueue = temp;
		}

		//Loops through all the messages in the queue
		int messagesToSend = static_cast<int>(mFlushingQueue->size());
		for (int j = 0; j < messagesToSend; j++)
		{
			//Loops through all the peers and sends messages to them
			int peerCount = static_cast<int>(mPeers.size());
			for (int i = 0; i < peerCount; i++)
			{
				OutputDebugString(L"Sending message!");
				if (send(mPeers[i], mFlushingQueue->front().c_str(), static_cast<int>(mFlushingQueue->front().length()), 0) == SOCKET_ERROR)
				{
					OutputDebugString(L"Send failed with ");
					OutputDebugString(std::to_wstring(WSAGetLastError()).c_str());
				}
				else
				{
					OutputDebugString(L"Send succeeded!");
				}
			}

			//Pop message off queue
			mFlushingQueue->pop();
		}
	}
}

/// <summary>
/// Reads all the peers IPs and ports from a file
/// Sends a message out to all peers requesting a connection
/// If connection is accepted creates a socket for communication with that peer
/// </summary>
void NetworkManager::FindPeers()
{
	std::vector<std::string> ipAddresses;
	std::vector<int> ports;
	std::string line;
	std::ifstream fin("Config.txt");
	bool lineTwo = false;

	//Reads the peers addresses and ports from file
	while (std::getline(fin, line))
	{
		if (!lineTwo)
		{
			ipAddresses.push_back(line);
			lineTwo = true;
		}
		else
		{
			ports.push_back(std::stoi(line));
			lineTwo = false;
		}
	}

	sockaddr_in address;
	address.sin_family = AF_INET;

	//Loops through all the addresses and attempts to make a connection with the peer at this address
	for (int i = 0; i < ipAddresses.size(); i++)
	{
		address.sin_port = htons(ports[i]);
		inet_pton(address.sin_family, ipAddresses[i].c_str(), &address.sin_addr);

		//Create peer socket
		SOCKET peerSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (peerSocket == INVALID_SOCKET)
		{
			OutputDebugString(L"Create socket failed with ");
			OutputDebugString(std::to_wstring(WSAGetLastError()).c_str());
		}
		//Connect to peer
		else if (connect(peerSocket, (sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
		{
			OutputDebugString(L"Connecting to peer failed with ");
			OutputDebugString(std::to_wstring(WSAGetLastError()).c_str());
		}
		else
		{
			//Upon successful connection send the connect command to the peer to register the connection
			std::string message = mIdentifier + "CONNECT:" + mTerminator;
			if (send(peerSocket, message.c_str(), 17, 0) == SOCKET_ERROR)
			{
				OutputDebugString(L"Send failed with ");
				OutputDebugString(std::to_wstring(WSAGetLastError()).c_str());
			}
			else
			{
				//If connect command was sent successfully, add the peer listener to a thread to handle communication with this peer
				mPeers.push_back(peerSocket);
				mThreadManager->AddTask(std::bind(&NetworkManager::ListenToPeer, this, std::placeholders::_1), &mPeers[mPeerCount], nullptr, std::vector<int>{2, 3, 4, 5, 6, 7});
				mPeerCount++;
			}
		}
	}
}

/// <summary>
/// Destructor
/// Cleans up Windows Sockets
/// </summary>
NetworkManager::~NetworkManager()
{
	// Cleanup windows sockets
	WSACleanup();
}

/// <summary>
/// Listens for incoming connection requests on the given port from any address using the TCP protocol
/// Creates a new peer when a a new connection request is received and adds the peer to a new thread
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
			OutputDebugString(L"Accept failed with ");
			OutputDebugString(std::to_wstring(WSAGetLastError()).c_str());
			break;
		}
		else
		{
			OutputDebugString(L"New peer connected!");

			//Add peer socket to thread
			mPeers.push_back(peerSocket);
			mThreadManager->AddTask(std::bind(&NetworkManager::ListenToPeer, this, std::placeholders::_1), &mPeers[mPeerCount], nullptr, std::vector<int>{2, 3, 4, 5, 6, 7});
			mPeerCount++;
		}
	}
}

/// <summary>
/// Initialises a win sock environment
/// Creates a tcp listener on the given port
/// Adds the listener and message sender to new threads
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

	FindPeers();

	//Set port
	mListenAddress.sin_family = AF_INET;
	mListenAddress.sin_port = htons(pPort);
	mListenAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//Create listening socket
	mListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mListenSocket == INVALID_SOCKET)
	{
		OutputDebugString(L"Create socket failed with ");
		OutputDebugString(std::to_wstring(WSAGetLastError()).c_str());
	}
	else if (bind(mListenSocket, (sockaddr *)&mListenAddress, sizeof(mListenAddress)) == SOCKET_ERROR)
	{
		OutputDebugString(L"Bind failed with ");
		OutputDebugString(std::to_wstring(WSAGetLastError()).c_str());

		//Try binding socket on alternative ports (THIS IS FOR DEBUGGING PURPOSES ON A SINGLE MACHINE)
		for (int i = 1; i < 4; i++)
		{
			mListenAddress.sin_family = AF_INET;
			mListenAddress.sin_port = htons(pPort + i);
			mListenAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

			//Create listening socket
			mListenSocket = socket(AF_INET, SOCK_STREAM, 0);

			if (bind(mListenSocket, (sockaddr *)&mListenAddress, sizeof(mListenAddress)) == SOCKET_ERROR)
			{
				OutputDebugString(L"Secondary bind failed with ");
				OutputDebugString(std::to_wstring(WSAGetLastError()).c_str());
			}
			else
			{
				//Listen on socket
				listen(mListenSocket, 5);

				//Add listener and sender to threads
				mThreadManager->AddTask(std::bind(&NetworkManager::Listen, this), nullptr, nullptr, std::vector<int>{2, 3, 4, 5, 6, 7});
				mThreadManager->AddTask(std::bind(&NetworkManager::SendMessages, this), nullptr, nullptr, std::vector<int>{2, 3, 4, 5, 6, 7});
				break;
			}
		}
	}
	else if (listen(mListenSocket, 5) == SOCKET_ERROR)
	{
		OutputDebugString(L"Listen failed with ");
		OutputDebugString(std::to_wstring(WSAGetLastError()).c_str());
	}
	else
	{
		//Add listener and sender to threads
		mThreadManager->AddTask(std::bind(&NetworkManager::Listen, this), nullptr, nullptr, std::vector<int>{2, 3, 4, 5, 6, 7});
		mThreadManager->AddTask(std::bind(&NetworkManager::SendMessages, this), nullptr, nullptr, std::vector<int>{2, 3, 4, 5, 6, 7});
	}
}

/// <summary>
/// Adds a given message to the message queue
/// </summary>
/// <param name="pMessage">Given message to add</param>
void NetworkManager::AddMessage(const std::string & pMessage)
{
	std::lock_guard<std::mutex> lock(mx);
	mActiveQueue->push(mIdentifier + pMessage + mTerminator);
}

/// <summary>
/// Retrieves the messages to be processed from the network manager
/// </summary>
/// <returns>Queue of messaged to be processed</returns>
std::queue<std::string> NetworkManager::ReadMessages()
{
	//Swaps the queue with an empty queue to empty the current queue and returns the queue containing messages
	std::queue<std::string> messages;
	std::lock_guard<std::mutex> lock(mx);
	mMessagesReceived.swap(messages);
	return messages;
}

/// <summary>
/// Gets the count of players currently connected
/// </summary>
/// <returns>Count of players currently connected</returns>
int NetworkManager::PeerCount()
{
	return mPeerCount;
}

/// <summary>
/// If an instance of the network manager does not already exists, creates one and then provides a pointer to it
/// </summary>
/// <returns>Pointer to the network manager instance</returns>
std::shared_ptr<NetworkManager> NetworkManager::Instance()
{
	static std::shared_ptr<NetworkManager> instance{ new NetworkManager() };
	return instance;
}
