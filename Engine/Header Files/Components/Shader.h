#pragma once
#include "BlendState.h"
#include "CullState.h"
#include "DepthState.h"

struct Shader
{
	std::wstring filename;
	BlendState blendState;
	CullState cullState;
	DepthState  depthState;
};
