#include "NetworkSystem.h"

/// <summary>
/// 
/// </summary>
NetworkSystem::NetworkSystem() : ISystem(ComponentType::COMPONENT_NONE), mPlayerNumber(1), mColourAssigner(false)
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
	if (mNetworkManager->PlayerCount() == 1)
	{
		mColourAssigner = true;
		mPlayerNumber = 1;
		mPlayerColour = "RED";
		mAvailableColours.push_back("GREEN");
		mAvailableColours.push_back("BLUE");
		mAvailableColours.push_back("YELLOW");
	}

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
			if (mColourAssigner == true)
			{
				mNetworkManager->AddMessage("NEWPLAYER:" + mAvailableColours.back() + ":" + std::to_string(mNetworkManager->PlayerCount()), mNetworkManager->PlayerCount() - 2);
				mAvailableColours.pop_back();
			}

		}
		else if (splitString[0] == "CLICKED")
		{
			int cubeID = std::stoi(splitString[1]);
			std::string colourToSteal;

			if (mEcsManager->WeightComp(cubeID)->weight > 0)
			{

				if (mEcsManager->ColourComp(cubeID)->colour.X == 0.5f)
				{
					mEcsManager->ColourComp(cubeID)->colour = mEcsManager->ColourComp(0)->colour - MathsHelper::Vector4(0.5f, 0, 0, 0);
					colourToSteal = "RED";
				}
				else if (mEcsManager->ColourComp(cubeID)->colour.Y == 0.5f)
				{
					mEcsManager->ColourComp(cubeID)->colour = mEcsManager->ColourComp(0)->colour - MathsHelper::Vector4(0, 0.5f, 0, 0);
					colourToSteal = "GREEN";
				}
				else if (mEcsManager->ColourComp(cubeID)->colour.Z == 0.5f)
				{
					mEcsManager->ColourComp(cubeID)->colour = mEcsManager->ColourComp(0)->colour - MathsHelper::Vector4(0, 0, 0.5f, 0);
					colourToSteal = "BLUE";
				}
				else if ((mEcsManager->ColourComp(cubeID)->colour.X == 0.6f || mEcsManager->ColourComp(cubeID)->colour.X == 1.1f)
					&& (mEcsManager->ColourComp(cubeID)->colour.Y == 0.6f || mEcsManager->ColourComp(cubeID)->colour.Y == 1.1f))
				{
					mEcsManager->ColourComp(cubeID)->colour = mEcsManager->ColourComp(0)->colour - MathsHelper::Vector4(0.6f, 0.6f, 0, 0);
					colourToSteal = "YELLOW";
				}

				mEcsManager->WeightComp(cubeID)->weight -= 1;
				if (mEcsManager->WeightComp(cubeID)->weight == 0)
				{
					mEcsManager->RemoveColourComp(cubeID);
				}

				mNetworkManager->AddMessage("CLICKEDRESPONSE:" + std::to_string(cubeID) + ":" + colourToSteal, std::stoi(splitString[2]) - 2);
			}

		}
		else if (splitString[0] == "NEWPLAYER")
		{
			if (splitString[1] == "BLUE")
			{
				mEcsManager->ColourComp(0)->colour = MathsHelper::Vector4(0, 0, 0.5f, 0);
				mPlayerColour = "BLUE";
			}
			else if (splitString[1] == "GREEN")
			{
				mEcsManager->ColourComp(0)->colour = MathsHelper::Vector4(0, 0.5f, 0, 0);
				mPlayerColour = "GREEN";
			}
			else if (splitString[1] == "YELLOW")
			{
				mEcsManager->ColourComp(0)->colour = MathsHelper::Vector4(0.6f, 0.6f, 0, 0);
				mPlayerColour = "YELLOW";
			}

			mPlayerNumber = std::stoi(splitString[2]);
		}
		else if (splitString[0] == "CLICKEDRESPONSE")
		{
			int cubeID = std::stoi(splitString[1]);

			if (splitString[2] == "RED")
			{
				if (mEcsManager->WeightComp(cubeID)->weight > 0)
				{
					mEcsManager->ColourComp(cubeID)->colour = mEcsManager->ColourComp(0)->colour + MathsHelper::Vector4(0.5f, 0, 0, 0);
				}
				else
				{
					Colour colour{ MathsHelper::Vector4(0.5f, 0, 0, 0) };
					mEcsManager->AddColourComp(colour, cubeID);
				}
			}
			else if (splitString[2] == "GREEN")
			{
				if (mEcsManager->WeightComp(cubeID)->weight > 0)
				{
					mEcsManager->ColourComp(cubeID)->colour = mEcsManager->ColourComp(0)->colour + MathsHelper::Vector4(0, 0.5f, 0, 0);
				}
				else
				{
					Colour colour{ MathsHelper::Vector4(0, 0.5f, 0, 0) };
					mEcsManager->AddColourComp(colour, cubeID);
				}
			}
			else if (splitString[2] == "BLUE")
			{
				if (mEcsManager->WeightComp(cubeID)->weight > 0)
				{
					mEcsManager->ColourComp(cubeID)->colour = mEcsManager->ColourComp(0)->colour + MathsHelper::Vector4(0, 0, 0.5f, 0);
				}
				else
				{
					Colour colour{ MathsHelper::Vector4(0, 0, 0.5f, 0) };
					mEcsManager->AddColourComp(colour, cubeID);
				}
			}
			else if (splitString[2] == "YELLOW")
			{
				if (mEcsManager->WeightComp(cubeID)->weight > 0)
				{
					mEcsManager->ColourComp(cubeID)->colour = mEcsManager->ColourComp(0)->colour + MathsHelper::Vector4(0.6f, 0.6f, 0, 0);
				}
				else
				{
					Colour colour{ MathsHelper::Vector4(0.6f, 0.6f, 0, 0) };
					mEcsManager->AddColourComp(colour, cubeID);
				}
			}
			mEcsManager->WeightComp(cubeID)->weight += 1;
		}

		messages.pop();
	}
}
