#pragma once
#include "AntTweakBar.h"
#include <d3d11.h>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

class AntTweakManager
{
private:
	std::vector<std::pair<std::string, TwBar*>> mBars;

	//Private constructor for singleton pattern
	AntTweakManager();

public:
	~AntTweakManager();

	//Singleton pattern
	//Deleted copy constructor and assignment operator so no copies of the singleton instance can be made
	AntTweakManager(const AntTweakManager& pAntTweakManager) = delete;
	AntTweakManager& operator=(AntTweakManager const&) = delete;

	static std::shared_ptr<AntTweakManager> Instance();

	void Init(const TwGraphAPI& pGraphicsAPI, void* const pDevice, const int pWidth, const int pHeight) const;
	void AddBar(const std::string& pBarName);
	void AddVariable(const std::string & pBarName, const std::string & pVariableName, const TwType & pVariableType, const void * const pVariable, const std::string & pBehaviourDefinition);
	void DeleteBar(const std::string& pBarName);
	void Draw();
	void Cleanup() const;
};