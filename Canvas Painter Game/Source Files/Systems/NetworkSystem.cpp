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
	//Adds it to taken colours list
	mTakenColours.push_back(*std::find_if(mAvailableColours.begin(), mAvailableColours.end(),
		[&](const std::pair<std::string, KodeboldsMath::Vector4> pair) {return pair.first == pSplitString[1]; }));

	//Removes the colour of the replying peer from the available colours list
	mAvailableColours.erase(std::remove_if(mAvailableColours.begin(), mAvailableColours.end(),
		[&](const std::pair<std::string, KodeboldsMath::Vector4> pair) {return pair.first == pSplitString[1]; }),
		mAvailableColours.end());

	//Adds it to the taken numbers list
	GameStats::gTakenPlayerNumbers.push_back(std::stoi(pSplitString[2]));

	//Removes the number of the replying peer from the available numbers list
	mAvailablePlayerNumbers.erase(remove(mAvailablePlayerNumbers.begin(), mAvailablePlayerNumbers.end(), std::stoi(pSplitString[2])), mAvailablePlayerNumbers.end());

	//Keep track of how many peers have responded to the connection request
	mPeersResponded++;
	GameStats::gPlayerCount++;
	GameStats::gStartingMass = GameStats::gCubeCount * GameStats::gPlayerCount;

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
	//Adds it to taken colours list
	mTakenColours.push_back(*std::find_if(mAvailableColours.begin(), mAvailableColours.end(),
		[&](const std::pair<std::string, KodeboldsMath::Vector4> pair) {return pair.first == pSplitString[1]; }));

	//Removes the colour of the replying peer from the available colours list
	mAvailableColours.erase(std::remove_if(mAvailableColours.begin(), mAvailableColours.end(),
		[&](const std::pair<std::string, KodeboldsMath::Vector4> pair) {return pair.first == pSplitString[1]; }),
		mAvailableColours.end());

	//Add it to the taken numbers list
	GameStats::gTakenPlayerNumbers.push_back(std::stoi(pSplitString[2]));

	//Removes the number of the replying peer from the available numbers list
	mAvailablePlayerNumbers.erase(remove(mAvailablePlayerNumbers.begin(), mAvailablePlayerNumbers.end(), std::stoi(pSplitString[2])), mAvailablePlayerNumbers.end());

	GameStats::gPlayerCount++;
	GameStats::gStartingMass = GameStats::gCubeCount * GameStats::gPlayerCount;
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
void NetworkSystem::ResetCanvasCommand()
{
	//Loops through every voxel of the canvas and resets the colour and weight to starting values
	for (int i = 2; i < GameStats::gCubeCount + 2; i++)
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
	GameStats::gCurrentMass = GameStats::gStartingMass;
}

/// <summary>
/// Executes the logic of the integrity command
/// </summary>
/// <param name="pSplitString">Command args</param>
void NetworkSystem::IntegrityCommand(std::vector<std::string>& pSplitString)
{
	//Create an integrity response message with the player number of the player that started the integrity 
	std::string response = "INTEGRITYRESPONSE:" + pSplitString[1] + ":";

	//Add all the weights of the current canvas to the message
	for (int i = 2; i < GameStats::gCubeCount + 2; i++)
	{
		response += std::to_string(mEcsManager->WeightComp(i)->weight) + ":";
	}

	//Send message
	mNetworkManager->AddMessage(response);
}

/// <summary>
/// Exectures the logic of the integrity response command
/// </summary>
/// <param name="pSplitString">Command args</param>
void NetworkSystem::IntegrityResponseCommand(std::vector<std::string>& pSplitString)
{
	//If weights has not yet been resized to cube count, resize it.
	if (mWeights.size() != GameStats::gCubeCount)
	{
		mWeights = std::vector<int>(GameStats::gCubeCount);
	}

	//Store the weights of the received canvas
	for (int i = 0; i < GameStats::gCubeCount; i++)
	{
		mWeights[i] += std::stoi(pSplitString[i + 2]);
		GameStats::gTotalMass++;
	}

	//Keep track of how many peers have responded
	mPeersRespondedToIntegrity++;

	//If all the peers of responded
	if (mPeersRespondedToIntegrity == mNetworkManager->PeerCount())
	{
		for (int i = 2; i < GameStats::gCubeCount + 2; i++)
		{
			//Tally up total weight
			mWeights[i - 2] += mEcsManager->WeightComp(i)->weight;
			GameStats::gTotalMass++;

			//Store current colour of cube before doing integrity colouring
			if (mEcsManager->ColourComp(i))
			{
				GameStats::gColoursBeforeIntegrity[i - 2] = mEcsManager->ColourComp(i)->colour;
			}
			else
			{
				GameStats::gColoursBeforeIntegrity[i - 2].W = 0;
			}

			//If missing weight, colour blue
			if (mWeights[i-2] < GameStats::gPlayerCount)
			{
				if (mEcsManager->ColourComp(i))
				{
					mEcsManager->ColourComp(i)->colour = BLUE;
				}
				else
				{
					mEcsManager->AddColourComp(Colour{ BLUE }, i);
				}
			}
			//If has too much weight, colour red
			else if (mWeights[i - 2] > GameStats::gPlayerCount)
			{
				if (mEcsManager->ColourComp(i))
				{
					mEcsManager->ColourComp(i)->colour = RED;
				}
				else
				{
					mEcsManager->AddColourComp(Colour{ RED }, i);
				}
			}
			//Else colour gray
			else
			{
				if (mEcsManager->ColourComp(i))
				{
					mEcsManager->ColourComp(i)->colour = KodeboldsMath::Vector4(0.5, 0.5, 0.5, 1);
				}
				else
				{
					mEcsManager->AddColourComp(Colour{ KodeboldsMath::Vector4(0.5, 0.5, 0.5, 1) }, i);
				}
			}
		}
		mPeersRespondedToIntegrity = 0;

		//Reset weights to zero
		for (int i = 0; i < mWeights.size(); i++)
		{
			mWeights[i] = 0;
		}
	}
}

/// <summary>
/// Executes the logic of the disconnect command
/// </summary>
void NetworkSystem::DisconnectDetectedCommand(std::vector<std::string>& pSplitString)
{
	//Find the corresponding colour pair
	auto it = std::find_if(mTakenColours.begin(), mTakenColours.end(),
		[&](const std::pair<std::string, KodeboldsMath::Vector4> pair) { return pair.first == pSplitString[1]; });

	//Add the colour pair to the temp colours list
	if (it != mTakenColours.end())
	{
		mColoursTemp.push_back(*it);
		mPlayerNumbersTemp.push_back(std::stoi(pSplitString[2]));
	}

	//Keep track of how many peers have responded
	mPeersRespondedToDisconnect++;

	//If all the peers of responded
	if (mPeersRespondedToDisconnect == mNetworkManager->PeerCount())
	{
		std::pair<std::string, KodeboldsMath::Vector4> colourToRemove;
		int numberToRemove;

		//Search for the colour that is missing from the temporary colours list
		for (const auto& colour : mTakenColours)
		{
			auto it = std::find_if(mColoursTemp.begin(), mColoursTemp.end(),
				[&](const std::pair<std::string, KodeboldsMath::Vector4> pair) { return pair.first == colour.first; });

			if (it == mColoursTemp.end())
			{
				colourToRemove = colour;
			}
		}

		//Remove from the taken list and add to the available list
		mAvailableColours.push_back(colourToRemove);
		mTakenColours.erase(std::remove_if(mTakenColours.begin(), mTakenColours.end(), 
			[&](const std::pair<std::string, KodeboldsMath::Vector4> pair) { return pair.first == colourToRemove.first; }
		), mTakenColours.end());

		//Search for the number that is missing from the temporary numbers list
		for (const auto& number : GameStats::gTakenPlayerNumbers)
		{
			auto it = std::find(mPlayerNumbersTemp.begin(), mPlayerNumbersTemp.end(), number);

			if (it == mPlayerNumbersTemp.end())
			{
				numberToRemove = number;
			}
		}

		//Remove from the taken list and add to the available list
		mAvailablePlayerNumbers.push_back(numberToRemove);
		GameStats::gTakenPlayerNumbers.erase(remove(GameStats::gTakenPlayerNumbers.begin(), GameStats::gTakenPlayerNumbers.end(), numberToRemove), GameStats::gTakenPlayerNumbers.end());

		//Clear temp lists
		mColoursTemp.clear();
		mPlayerNumbersTemp.clear();

		//Reset canvas
		ResetCanvasCommand();

		mPeersRespondedToDisconnect = 0;
	}
}

/// <summary>
/// Constructor
/// Adds the available colours and player numbers to the appropriate lists
/// </summary>
NetworkSystem::NetworkSystem() : ISystem(ComponentType::COMPONENT_NONE), mPeersResponded(0), mPeersRespondedToDisconnect(0), mPeersRespondedToIntegrity(0)
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
	//If there are no peers connected, this is the first player, so set number to 1 and colour to red and remove it from availability lists and add it to the taken lists
	if (mNetworkManager->PeerCount() == 0 && GameStats::gPlayerNumber == 0)
	{
		GameStats::gPlayerNumber = 1;
		GameStats::gPlayerColour = RED;
		mPlayerColour = "RED";
		GameStats::gCanvasColoured = false;

		mAvailableColours.erase(std::remove_if(mAvailableColours.begin(), mAvailableColours.end(),
			[&](const std::pair<std::string, KodeboldsMath::Vector4> pair) {return pair.first == mPlayerColour; }),
			mAvailableColours.end());

		mTakenColours.push_back(std::make_pair("RED", RED));

		mAvailablePlayerNumbers.erase(remove(mAvailablePlayerNumbers.begin(), mAvailablePlayerNumbers.end(), 1), mAvailablePlayerNumbers.end());

		GameStats::gTakenPlayerNumbers.push_back(1);
	}

	//If peer count is lower than player count, a player has disconnected, send disconnect message to peers to figure out which peer has disconnected then reset canvas
	if (mNetworkManager->PeerCount() < (GameStats::gPlayerCount - 1))
	{
		if (mNetworkManager->PeerCount() > 0)
		{
			mNetworkManager->AddMessage("DISCONNECTDETECTED:" + mPlayerColour + ":" + std::to_string(GameStats::gPlayerNumber));
		}
		else
		{
			ResetCanvasCommand();
		}

		GameStats::gPlayerCount -= 1;
		GameStats::gStartingMass = GameStats::gCubeCount * GameStats::gPlayerCount;
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
			ResetCanvasCommand();
		}
		//Integrity test command, sends all data to a single canvas for an integrity test
		else if (splitString[0] == "INTEGRITY")
		{
			IntegrityCommand(splitString);
		}
		//Integrity test response containing the weights for all the cubes from a single peer
		else if (splitString[0] == "INTEGRITYRESPONSE")
		{
			//If this player is the one that initiated the integrity test request
			if (std::stoi(splitString[1]) == GameStats::gPlayerNumber)
			{
				IntegrityResponseCommand(splitString);
			}
		}
		//Disconnect detected command, sends out the colour and number of this player to other peers 
		//so all peers can figure out which player has disconnected and response appropriately
		else if (splitString[0] == "DISCONNECTDETECTED")
		{
			DisconnectDetectedCommand(splitString);
		}

		//Remove the message from the queue
		messages.pop();
	}
}
