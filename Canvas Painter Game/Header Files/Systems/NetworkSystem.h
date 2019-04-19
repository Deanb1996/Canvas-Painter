#pragma once
#include "ISystem.h"
#include "Managers.h"

class NetworkSystem : public ISystem
{
private:
	std::shared_ptr<ECSManager> mEcsManager = ECSManager::Instance();
	std::shared_ptr<NetworkManager> mNetworkManager = NetworkManager::Instance();

	int mPlayerNumber;
	std::string mPlayerColour;
	bool mColourAssigner;
	std::vector<std::string> mAvailableColours;

public:
	explicit NetworkSystem();
	virtual ~NetworkSystem();

	void AssignEntity(const Entity& pEntity) override;
	void ReAssignEntity(const Entity& pEntity) override;
	void Process() override;
};