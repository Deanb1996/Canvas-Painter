#pragma once
#include "NetworkListener.h"
#include <memory>

class NetworkManager
{
private:

	//Private constructor for singleton pattern
	NetworkManager();

	void InitWinSock();
public:
	~NetworkManager();

	//Singleton pattern
	//Deleted copy constructor and assignment operator so no copies of the singleton instance can be made
	NetworkManager(const NetworkManager& NetworkManager) = delete;
	NetworkManager& operator=(NetworkManager const&) = delete;


	static std::shared_ptr< NetworkManager > Instance();
};