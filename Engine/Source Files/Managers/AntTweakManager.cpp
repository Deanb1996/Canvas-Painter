#include "AntTweakManager.h"

/// <summary>
/// Default destructor
/// </summary>
AntTweakManager::~AntTweakManager()
{
}

/// <summary>
/// Creates a singleton instance of AntTweak Manager if one hasn't been created before
/// Returns pointer to the instance of AntTweak Manager
/// </summary>
/// <returns>Shared pointer to the AntTweak Manager instance</returns>
std::shared_ptr<AntTweakManager> AntTweakManager::Instance()
{
	static std::shared_ptr<AntTweakManager> instance{ new AntTweakManager };
	return instance;
}

/// <summary>
/// 
/// </summary>
/// <param name="pGraphicsAPI"></param>
/// <param name="pDXDevice"></param>
/// <param name="pWidth"></param>
/// <param name="pHeight"></param>
void AntTweakManager::Init(const TwGraphAPI & pGraphicsAPI, ID3D11Device * const pDXDevice, const int & pWidth, const int & pHeight) const
{
	TwInit(pGraphicsAPI, pDXDevice);
	TwWindowSize(pWidth, pHeight);
}

/// <summary>
/// 
/// </summary>
/// <param name="pBarName"></param>
void AntTweakManager::AddBar(const std::string & pBarName)
{
	TwBar* newBar = TwNewBar(pBarName.c_str());
	mBars.emplace_back(std::make_pair(pBarName, newBar));
}

/// <summary>
/// 
/// </summary>
/// <param name="pBarName"></param>
/// <param name="variableName"></param>
/// <param name="variableType"></param>
/// <param name="variable"></param>
/// <param name="behaviourDefinition"></param>
void AntTweakManager::AddVariable(const std::string & pBarName, const std::string & pVariableName, const TwType & pVariableType, const void * const pVariable, const std::string & pBehaviourDefinition)
{
	auto it = std::find_if(mBars.begin(), mBars.end(), [&](const std::pair<std::string, TwBar*> bar) { return bar.first == pBarName; });
	TwAddVarRW(it->second, pVariableName.c_str(), pVariableType, const_cast<void*>(pVariable), pBehaviourDefinition.c_str());
}

/// <summary>
/// 
/// </summary>
/// <param name="pBarName"></param>
void AntTweakManager::DeleteBar(const std::string & pBarName)
{
	auto it = std::find_if(mBars.begin(), mBars.end(), [&](const std::pair<std::string, TwBar*> bar) { return bar.first == pBarName; });
	TwDeleteBar(it->second);
	mBars.erase(it);
}

/// <summary>
/// 
/// </summary>
void AntTweakManager::Draw()
{
	for (auto& bar : mBars)
	{
		TwRefreshBar(bar.second);
	}
	TwDraw();
}

/// <summary>
/// 
/// </summary>
void AntTweakManager::Cleanup() const
{
	TwDeleteAllBars();
	TwTerminate();
}
