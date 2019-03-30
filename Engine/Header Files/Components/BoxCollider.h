#pragma once
#include <vector>

struct BoxCollider
{
	std::vector<int> mCollidedWith;
	float mWidth;
	float mHeight;
	float mDepth;
};