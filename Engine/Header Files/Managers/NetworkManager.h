#pragma once
#include <WinSock2.h>
#include <memory>
#include <iostream>
#include "ThreadManager.h"

class NetworkManager
{
private:

	std::shared_ptr<ThreadManager> mThreadManager = ThreadManager::Instance();

	SOCKET mListenSocket;
	SOCKET mPeerSocket;
	sockaddr_in mPort;
	int mPeers;

	//Private constructor for singleton pattern
	NetworkManager(const int pPort);

	void InitWinSock(const int pPort);
	void Update();
	void ProcessPeer(void* pPeerSocket);
public:
	~NetworkManager();

	//Singleton pattern
	//Deleted copy constructor and assignment operator so no copies of the singleton instance can be made
	NetworkManager(const NetworkManager& NetworkManager) = delete;
	NetworkManager& operator=(NetworkManager const&) = delete;


	static std::shared_ptr< NetworkManager > Instance(const int pPort);
};