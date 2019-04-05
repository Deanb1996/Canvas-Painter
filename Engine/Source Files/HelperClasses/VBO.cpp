#include "VBO.h"

/// <summary>
/// Default constructor
/// </summary>
VBO::VBO()
	: mIndexCount(0)
{
}

/// <summary>
/// Adds new instance to instance data of VBO
/// </summary>
void VBO::AddInstance(const int pEntityID, const MathsHelper::Vector3& pPosition, const MathsHelper::Vector3& pColour)
{
	mInstanceData.emplace_back(std::make_pair(pEntityID, InstanceData{pPosition, pColour}));
}

/// <summary>
/// Removes instance of this entity from the instance data of VBO
/// </summary>
void VBO::RemoveInstance(const int pEntityID)
{
	std::remove_if(mInstanceData.begin(), mInstanceData.end(), [&](const std::pair<int, InstanceData> mInstance) {return mInstance.first == pEntityID; });
}

/// <summary>
/// Get method for VBOs instance count
/// </summary>
/// <returns>Count of instances for this VBO</returns>
int VBO::InstanceCount()
{
	return static_cast<int>(mInstanceData.size());
}
