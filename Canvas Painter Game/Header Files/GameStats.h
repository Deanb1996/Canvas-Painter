#pragma once
#include "KodeboldsMath.h"
#include <vector>

namespace GameStats
{
	extern bool gCanvasColoured;
	extern int gPlayerNumber;
	extern KodeboldsMath::Vector4 gPlayerColour;
	extern int gPlayerCount;
	extern int gCubeCount;
	extern int gStartingMass;
	extern int gCurrentMass;
	extern int gTotalMass;
	extern std::vector<int> gTakenPlayerNumbers;
	extern std::vector<KodeboldsMath::Vector4> gColoursBeforeIntegrity;
}