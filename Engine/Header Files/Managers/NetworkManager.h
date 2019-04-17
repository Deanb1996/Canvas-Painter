#pragma once
#include <WinSock2.h>
#include <memory>
#include <iostream>
#include "ThreadManager.h"
#include <queue>
#include <string>

class NetworkManager
{
private:
	std::shared_ptr<ThreadManager> mThreadManager = ThreadManager::Instance();

	std::queue<std::string> mMessagesToSend;
	std::queue<std::string> mMessagesReceived;
	std::vector<SOCKET> mPeers;

	const std::string mIdentifier = "$DB$";
	const std::string mTerminator = "*DB*";

	SOCKET mListenSocket;
	sockaddr_in mPort;

	std::mutex mx;

	//Private constructor for singleton pattern
	NetworkManager(const int pPort);

	void InitWinSock(const int pPort);
	void Listen();
	void ProcessPeer(void* pPeerSocket);
	void SendMessages();
public:
	~NetworkManager();

	//Singleton pattern
	//Deleted copy constructor and assignment operator so no copies of the singleton instance can be made
	NetworkManager(const NetworkManager& NetworkManager) = delete;
	NetworkManager& operator=(NetworkManager const&) = delete;

	void AddMessage(const std::string& pMessage);
	std::queue<std::string>& ReadMessages();

	static std::shared_ptr< NetworkManager > Instance(const int pPort);
};