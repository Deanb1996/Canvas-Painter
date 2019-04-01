#include "RenderSystem.h"

RenderSystem::RenderSystem(const ComponentType& pMask) : ISystem(pMask)
{
	mEntities = std::vector<Entity>(200000, Entity{ -1, ComponentType::COMPONENT_NONE });
}