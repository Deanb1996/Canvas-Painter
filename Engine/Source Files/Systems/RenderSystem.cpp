#include "RenderSystem.h"

/// <summary>
/// Constructor
/// Initialises entity vector with size of 200000
/// </summary>
/// <param name="pMask">Mask for the system</param>
RenderSystem::RenderSystem(const ComponentType& pMask) : ISystem(pMask)
{
	mEntities = std::vector<Entity>(200000, Entity{ -1, ComponentType::COMPONENT_NONE });
}