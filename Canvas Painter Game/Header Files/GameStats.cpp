#include "GameStats.h"

namespace GameStats
{
	bool gCanvasColoured = false;
	int gPlayerNumber = 0;
	KodeboldsMath::Vector4 gPlayerColour = KodeboldsMath::Vector4(0, 0, 0, 0);
	int gPlayerCount = 1;
	int gCubeCount = 0;
	int gStartingMass = 0;
	int gCurrentMass = 0;
	int gTotalMass = 0;
	std::vector<int> gTakenPlayerNumbers;
	std::vector<KodeboldsMath::Vector4> gColoursBeforeIntegrity;
}