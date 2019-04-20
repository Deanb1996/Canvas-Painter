#pragma once
#include "ISystem.h"
#include "Managers.h"

class NetworkSystem : public ISystem
{
private:
	std::shared_ptr<ECSManager> mEcsManager = ECSManager::Instance();
	std::shared_ptr<NetworkManager> mNetworkManager = NetworkManager::Instance();

	std::vector<std::pair<std::string, KodeboldsMath::Vector4>> mAvailableColours;
	std::vector<int> mAvailablePlayerNumbers;
	std::string mPlayerColour;
	int mPlayerNumber;
	int mPeersResponded;

	//Colour constants
	const KodeboldsMath::Vector4 RED = KodeboldsMath::Vector4(0.5f, 0, 0, 0);
	const KodeboldsMath::Vector4 GREEN = KodeboldsMath::Vector4(0, 0.5f, 0, 0);
	const KodeboldsMath::Vector4 BLUE = KodeboldsMath::Vector4(0, 0, 0.5f, 0);
	const KodeboldsMath::Vector4 YELLOW = KodeboldsMath::Vector4(0.6f, 0.6f, 0, 0);

	//Network commands
	void ClickedCommand(std::vector<std::string>& pSplitString);
	void ClickedResponseCommand(std::vector<std::string>& pSplitString);
	void NewPlayerCommand(std::vector<std::string>& pSplitString);
	void NewPlayerConfirmedCommand(std::vector<std::string>& pSplitString);
	void ConnectCommmand();

public:
	explicit NetworkSystem();
	virtual ~NetworkSystem();

	void AssignEntity(const Entity& pEntity) override;
	void ReAssignEntity(const Entity& pEntity) override;
	void Process() override;
};