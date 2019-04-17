#include "NetworkSystem.h"

/// <summary>
/// 
/// </summary>
NetworkSystem::NetworkSystem() : ISystem(ComponentType::COMPONENT_NONE)
{
}

/// <summary>
/// 
/// </summary>
NetworkSystem::~NetworkSystem()
{
}

/// <summary>
/// 
/// </summary>
void NetworkSystem::Process()
{
	std::queue<std::string> messages = mNetworkManager->ReadMessages();
	std::string message;
	int messageCount = messages.size();

	for (int i = 0; i < messageCount; i++)
	{
		message = messages.front();
		messages.pop();
	}
}
