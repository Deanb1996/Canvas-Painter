#include "NetworkSystem.h"

/// <summary>
/// Executes the logic of the clicked command
/// </summary>
/// <param name="pSplitString">Command args</param>
void NetworkSystem::ClickedCommand(std::vector<std::string>& pSplitString)
{
	int cubeID = std::stoi(pSplitString[1]);
	std::string colourToSteal = " ";

	//Checks if this cube has weight to steal
	if (mEcsManager->WeightComp(cubeID)->weight > 0)
	{
		//If has colour red, steal colour red
		if (mEcsManager->ColourComp(cubeID)->colour.X == 0.5f || mEcsManager->ColourComp(cubeID)->colour.X == 1.1f)
		{
			mEcsManager->ColourComp(cubeID)->colour = mEcsManager->ColourComp(cubeID)->colour - RED;
			colourToSteal = "RED";
		}
		//If has colour green, steal green
		else if (mEcsManager->ColourComp(cubeID)->colour.Y == 0.5f || mEcsManager->ColourComp(cubeID)->colour.Y == 1.1f)
		{
			mEcsManager->ColourComp(cubeID)->colour = mEcsManager->ColourComp(cubeID)->colour - GREEN;
			colourToSteal = "GREEN";
		}
		//If has colour blue, steal blue
		else if (mEcsManager->ColourComp(cubeID)->colour.Z == 0.5f)
		{
			mEcsManager->ColourComp(cubeID)->colour = mEcsManager->ColourComp(cubeID)->colour - BLUE;
			colourToSteal = "BLUE";
		}
		//If has colour yellow, steal yellow
		else if ((mEcsManager->ColourComp(cubeID)->colour.X == 0.6f || mEcsManager->ColourComp(cubeID)->colour.X == 1.1f)
			&& (mEcsManager->ColourComp(cubeID)->colour.Y == 0.6f || mEcsManager->ColourComp(cubeID)->colour.Y == 1.1f))
		{
			mEcsManager->ColourComp(cubeID)->colour = mEcsManager->ColourComp(cubeID)->colour - YELLOW;
			colourToSteal = "YELLOW";
		}

		//Decrease weight of cube by 1
		GameStats::gCurrentMass--;
		mEcsManager->WeightComp(cubeID)->weight -= 1;

		//If cube is left with no weight, remove the cubes colour component entirely to cause it not to render anymore
		if (mEcsManager->WeightComp(cubeID)->weight == 0)
		{
			mEcsManager->RemoveColourComp(cubeID);
		}

		//Send a response back to the stealing player with the stolen colour
		mNetworkManager->AddMessage("CLICKEDRESPONSE:" + std::to_string(cubeID) + ":" + colourToSteal + ":" + pSplitString[3]);
	}

	//If no weight available, steal nothing or find another player with weight to steal?
}

/// <summary>
/// Executes the logic of the clicked response command
/// </summary>
/// <param name="pSplitString">Command args</param>
void NetworkSystem::ClickedResponseCommand(std::vector<std::string>& pSplitString)
{
	int cubeID = std::stoi(pSplitString[1]);
	KodeboldsMath::Vector4 stolenColour;

	//If stolen colour is red
	if (pSplitString[2] == "RED")
	{
		stolenColour = RED;
	}
	//If stolen colour is green
	else if (pSplitString[2] == "GREEN")
	{
		stolenColour = GREEN;
	}
	//If stolen colour is blue
	else if (pSplitString[2] == "BLUE")
	{
		stolenColour = BLUE;
	}
	//If stolen colour is yellow
	else if (pSplitString[2] == "YELLOW")
	{
		stolenColour = YELLOW;
	}

	//If the cube already has weight, then add the colour on to its existing colour
	if (mEcsManager->WeightComp(cubeID)->weight > 0)
	{
		mEcsManager->ColourComp(cubeID)->colour = mEcsManager->ColourComp(cubeID)->colour + stolenColour;
	}
	//Else add a colour component with the stolen colour
	else
	{
		Colour colour{ stolenColour };
		mEcsManager->AddColourComp(colour, cubeID);
	}

	//Add one weight to cube
	GameStats::gCurrentMass++;
	mEcsManager->WeightComp(cubeID)->weight += 1;
}

/// <summary>
/// Executes the logic of the new player command
/// </summary>
/// <param name="pSplitString">Command args</param>
void NetworkSystem::NewPlayerCommand(std::vector<std::string>& pSplitString)
{
	//Removes the colour of the replying peer from the available colours list
	mAvailableColours.erase(std::remove_if(mAvailableColours.begin(), mAvailableColours.end(),
		[&](const std::pair<std::string, KodeboldsMath::Vector4> pair) {return pair.first == pSplitString[1]; }),
		mAvailableColours.end());

	//Removes the number of the replying peer from the available numbers list
	mAvailablePlayerNumbers.erase(remove(mAvailablePlayerNumbers.begin(), mAvailablePlayerNumbers.end(), std::stoi(pSplitString[2])), mAvailablePlayerNumbers.end());

	//Keep track of how many peers have responded to the connection request
	mPeersResponded++;

	//If all the peers of responded
	if (mPeersResponded == mNetworkManager->PeerCount())
	{
		//Set player colour to one of the available colours and then remove it from the list
		mPlayerColour = mAvailableColours.back().first;
		GameStats::gPlayerColour = mAvailableColours.back().second;
		GameStats::gCanvasColoured = false;
		mAvailableColours.pop_back();

		//Set player number to one of the available numbers and then remove it from the list
		GameStats::gPlayerNumber = mAvailablePlayerNumbers.back();
		mAvailablePlayerNumbers.pop_back();

		//Send a response to the peers informing them of the new players colour and number
		mNetworkManager->AddMessage("NEWPLAYERCONFIRMED:" + mPlayerColour + ":" + std::to_string(GameStats::gPlayerNumber));
	}
}

/// <summary>
/// Executes the logic of the new player confirmed command
/// </summary>
/// <param name="pSplitString">Command args</param>
void NetworkSystem::NewPlayerConfirmedCommand(std::vector<std::string>& pSplitString)
{
	//Removes the colour of the replying peer from the available colours list
	mAvailableColours.erase(std::remove_if(mAvailableColours.begin(), mAvailableColours.end(),
		[&](const std::pair<std::string, KodeboldsMath::Vector4> pair) {return pair.first == pSplitString[1]; }),
		mAvailableColours.end());

	//Removes the number of the replying peer from the available numbers list
	mAvailablePlayerNumbers.erase(remove(mAvailablePlayerNumbers.begin(), mAvailablePlayerNumbers.end(), std::stoi(pSplitString[2])), mAvailablePlayerNumbers.end());

	GameStats::gPlayerCount++;
}

/// <summary>
/// Executes the logic of the connect command
/// </summary>
void NetworkSystem::ConnectCommmand()
{
	//Sends a response to the connection request with this players colour and number
	mNetworkManager->AddMessage("NEWPLAYER:" + mPlayerColour + ":" + std::to_string(GameStats::gPlayerNumber));
}

/// <summary>
/// Executes the logic of the reset command
/// </summary>
void NetworkSystem::ResetCanvas()
{
	//Loops through every voxel of the canvas and resets the colour and weight to starting values
	for (int i = 1; i < GameStats::gCubeCount; i++)
	{
		//If colour exists, set colour to player colour
		if (mEcsManager->ColourComp(i))
		{
			mEcsManager->ColourComp(i)->colour = GameStats::gPlayerColour;
		}
		//Else create colour component with player colour
		else
		{
			Colour colour{ GameStats::gPlayerColour };
			mEcsManager->AddColourComp(colour, i);
		}

		//Set weight to 1
		mEcsManager->WeightComp(i)->weight = 1;
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="pSplitString"></param>
void NetworkSystem::Integrity(std::vector<std::string>& pSplitString)
{
}

/// <summary>
/// Constructor
/// Adds the available colours and player numbers to the appropriate lists
/// </summary>
NetworkSystem::NetworkSystem() : ISystem(ComponentType::COMPONENT_NONE), mPeersResponded(0)
{
	//Colours
	mAvailableColours.push_back(std::make_pair("GREEN", GREEN));
	mAvailableColours.push_back(std::make_pair("BLUE", BLUE));
	mAvailableColours.push_back(std::make_pair("YELLOW", YELLOW));
	mAvailableColours.push_back(std::make_pair("RED", RED));

	//Player numbers
	mAvailablePlayerNumbers.push_back(4);
	mAvailablePlayerNumbers.push_back(3);
	mAvailablePlayerNumbers.push_back(2);
	mAvailablePlayerNumbers.push_back(1);
}

/// <summary>
/// Default destructor
/// </summary>
NetworkSystem::~NetworkSystem()
{
}

/// <summary>
/// NOT IMPLEMENTED FOR THIS SYSTEM
/// </summary>
/// <param name="pEntity"></param>
void NetworkSystem::AssignEntity(const Entity & pEntity)
{
}

/// <summary>
/// NOT IMPLEMENTED FOR THIS SYSTEM
/// </summary>
/// <param name="pEntity"></param>
void NetworkSystem::ReAssignEntity(const Entity & pEntity)
{
}

/// <summary>
/// Systems process function, core logic of system
/// Handles all the network messages incoming from other peers
/// </summary>
void NetworkSystem::Process()
{
	//If there are no peers connected, this is the first player, so set number to 1 and colour to red and remove it from availability lists
	if (mNetworkManager->PeerCount() == 0)
	{
		GameStats::gPlayerNumber = 1;
		mPlayerColour = "RED";

		mAvailableColours.erase(std::remove_if(mAvailableColours.begin(), mAvailableColours.end(),
			[&](const std::pair<std::string, KodeboldsMath::Vector4> pair) {return pair.first == mPlayerColour; }),
			mAvailableColours.end());

		mAvailablePlayerNumbers.erase(remove(mAvailablePlayerNumbers.begin(), mAvailablePlayerNumbers.end(), 1), mAvailablePlayerNumbers.end());
	}

	//Retrieves messages from the network manager queue
	std::queue<std::string> messages = mNetworkManager->ReadMessages();
	std::string message;
	const char delim = ':';
	const int messageCount = static_cast<int>(messages.size());

	//Loop through every message in the queue
	for (int i = 0; i < messageCount; i++)
	{
		std::vector<std::string> splitString;
		size_t start;
		size_t end = 0;
		message = messages.front();

		//Split the message up into the command and its args
		while ((start = message.find_first_not_of(delim, end)) != std::string::npos)
		{
			end = message.find(delim, start);
			splitString.push_back(message.substr(start, end - start));
		}

		//Connection command when a new peer requests to connect to the game
		if (splitString[0] == "CONNECT")
		{
			ConnectCommmand();
		}
		//Clicked command when a peer has clicked a cube
		else if (splitString[0] == "CLICKED")
		{
			//If the peer decided to steal a cube from this player
			if (std::stoi(splitString[2]) == GameStats::gPlayerNumber)
			{
				ClickedCommand(splitString);
			}
		}
		//New player command sent as a response to the connection request with the peers current colour and number
		else if (splitString[0] == "NEWPLAYER")
		{
			//If player has not been assigned a number it is a new player and needs to be assigned one
			if (GameStats::gPlayerNumber == 0)
			{
				NewPlayerCommand(splitString);
			}
		}
		//Clicked response command sent back to the clicker with details of the stolen voxel
		else if (splitString[0] == "CLICKEDRESPONSE")
		{
			//If this player is the one that initiated the voxel stealing transaction
			if (std::stoi(splitString[3]) == GameStats::gPlayerNumber)
			{
				ClickedResponseCommand(splitString);
			}
		}
		//New player confirmed command, confirming the newly connected players colour and number
		else if (splitString[0] == "NEWPLAYERCONFIRMED")
		{
			NewPlayerConfirmedCommand(splitString);
		}
		//Reset canvas command, resets the canvas to the original colour and weight
		else if (splitString[0] == "RESET")
		{
			ResetCanvas();
		}
		//Integrity test command, sends all data to a single canvas for an integrity test
		else if (splitString[0] == "INTEGRITY")
		{
			Integrity(splitString);
		}

		//Remove the message from the queue
		messages.pop();
	}
}
