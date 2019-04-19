#pragma once
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <memory>
#include <iostream>
#include "ThreadManager.h"
#include <queue>
#include <string>
#include <fstream>
#include <sstream>

class NetworkManager
{
private:
	std::shared_ptr<ThreadManager> mThreadManager = ThreadManager::Instance();

	std::queue<std::pair<std::string, int>> mMessagesToSend;
	std::queue<std::pair<std::string, int>> mMessagesToSend2;
	std::queue<std::pair<std::string, int>>* mActiveQueue;
	std::queue<std::pair<std::string, int>>* mFlushingQueue;

	std::queue<std::string> mMessagesReceived;
	std::vector<SOCKET> mPeers;
	int mPeerCount;

	const std::string mIdentifier = "$DB$";
	const std::string mTerminator = "*DB*";

	SOCKET mListenSocket;
	sockaddr_in mListenAddress;

	std::mutex mx;

	//Private constructor for singleton pattern
	NetworkManager();

	void Listen();
	void ListenToPeer(void* pPeerSocket);
	void SendMessages();
	void FindPeers();
public:
	~NetworkManager();

	//Singleton pattern
	//Deleted copy constructor and assignment operator so no copies of the singleton instance can be made
	NetworkManager(const NetworkManager& NetworkManager) = delete;
	NetworkManager& operator=(NetworkManager const&) = delete;

	void InitWinSock(const int pPort);
	void AddMessage(const std::string& pMessage, int pPeer);
	std::queue<std::string> ReadMessages();
	int PlayerCount();

	static std::shared_ptr< NetworkManager > Instance();
};