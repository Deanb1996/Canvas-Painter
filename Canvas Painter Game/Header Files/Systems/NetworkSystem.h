#pragma once
#include "ISystem.h"
#include "Managers.h"
#include "GameStats.h"

class NetworkSystem : public ISystem
{
private:
	std::shared_ptr<ECSManager> mEcsManager = ECSManager::Instance();
	std::shared_ptr<NetworkManager> mNetworkManager = NetworkManager::Instance();

	std::vector<std::pair<std::string, KodeboldsMath::Vector4>> mAvailableColours;
	std::vector<std::pair<std::string, KodeboldsMath::Vector4>> mTakenColours;
	std::vector<std::pair<std::string, KodeboldsMath::Vector4>> mColoursTemp;
	std::vector<int> mAvailablePlayerNumbers;
	std::vector<int> mPlayerNumbersTemp;
	std::vector<int> mWeights;
	std::string mPlayerColour;
	int mPeersResponded;
	int mPeersRespondedToDisconnect;
	int mPeersRespondedToIntegrity;

	//Colour constants
	const KodeboldsMath::Vector4 RED = KodeboldsMath::Vector4(0.5f, 0, 0, 1);
	const KodeboldsMath::Vector4 GREEN = KodeboldsMath::Vector4(0, 0.5f, 0, 1);
	const KodeboldsMath::Vector4 BLUE = KodeboldsMath::Vector4(0, 0, 0.5f, 1);
	const KodeboldsMath::Vector4 YELLOW = KodeboldsMath::Vector4(0.6f, 0.6f, 0, 1);

	//Network commands
	void ClickedCommand(std::vector<std::string>& pSplitString);
	void ClickedResponseCommand(std::vector<std::string>& pSplitString);
	void NewPlayerCommand(std::vector<std::string>& pSplitString);
	void NewPlayerConfirmedCommand(std::vector<std::string>& pSplitString);
	void ConnectCommmand();
	void ResetCanvasCommand();
	void IntegrityCommand(std::vector<std::string>& pSplitString);
	void IntegrityResponseCommand(std::vector<std::string>& pSplitString);
	void DisconnectDetectedCommand(std::vector<std::string>& pSplitString);

public:
	explicit NetworkSystem();
	virtual ~NetworkSystem();

	void AssignEntity(const Entity& pEntity) override;
	void ReAssignEntity(const Entity& pEntity) override;
	void Process() override;
};