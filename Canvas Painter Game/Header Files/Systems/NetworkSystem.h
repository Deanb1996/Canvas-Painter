#pragma once
#include "ISystem.h"
#include "Managers.h"

class NetworkSystem : public ISystem
{
private:
	std::shared_ptr<ECSManager> mEcsManager = ECSManager::Instance();
	std::shared_ptr<NetworkManager> mNetworkManager = NetworkManager::Instance(0);

public:
	explicit NetworkSystem();
	virtual ~NetworkSystem();

	void Process() override;
};