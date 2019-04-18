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

void NetworkSystem::AssignEntity(const Entity & pEntity)
{
}

void NetworkSystem::ReAssignEntity(const Entity & pEntity)
{
}

/// <summary>
/// 
/// </summary>
void NetworkSystem::Process()
{
	std::queue<std::string> messages = mNetworkManager->ReadMessages();
	std::string message;
	char delim = ':';
	int messageCount = static_cast<int>(messages.size());

	for (int i = 0; i < messageCount; i++)
	{
		std::vector<std::string> splitString;

		message = messages.front();

		size_t start;
		size_t end = 0;

		while ((start = message.find_first_not_of(delim, end)) != std::string::npos)
		{
			end = message.find(delim, start);
			splitString.push_back(message.substr(start, end - start));
		}

		if (splitString[0] == "CONNECT")
		{

		}
		else if (splitString[0] == "CLICKED")
		{
			int cubeID = std::stoi(splitString[1]);

			if (splitString[2] == "RED")
			{
				mEcsManager->ColourComp(cubeID)->colour += MathsHelper::Vector4(0.5f, 0, 0, 0);
			}
			else if (splitString[2] == "GREEN")
			{
				mEcsManager->ColourComp(cubeID)->colour += MathsHelper::Vector4(0, 0.5f, 0, 0);
			}
			else if (splitString[2] == "YELLOW")
			{

			}
			else if (splitString[2] == "BLUE")
			{
				mEcsManager->ColourComp(cubeID)->colour += MathsHelper::Vector4(0, 0, 0.5f, 0);
			}
		}

		messages.pop();
	}
}
