#include "RayAABBIntersectionSystem.h"

/// <summary>
/// Constructor
/// Sets component mask to contain both a transform component and box collider component
/// Resizes entity vector to 200000 entities
/// </summary>
RayAABBIntersectionSystem::RayAABBIntersectionSystem() : ISystem(ComponentType::COMPONENT_TRANSFORM | ComponentType::COMPONENT_BOXCOLLIDER)
{
	mEntities = std::vector<Entity>(200000, Entity{ -1, ComponentType::COMPONENT_NONE });
}

/// <summary>
/// Default destructor
/// </summary>
RayAABBIntersectionSystem::~RayAABBIntersectionSystem()
{
}

/// <summary>
/// Assigns entity to system if the entities mask matches the system mask
/// </summary>
/// <param name="pEntity">Entity to be assigned</param>
void RayAABBIntersectionSystem::AssignEntity(const Entity & pEntity)
{
	//Checks if entity mask matches the box collider mask
	if ((pEntity.componentMask & mMask) == mMask)
	{
		//If the entity matches box collider mask then update entry in systems entity list
		mEntities[pEntity.ID] = pEntity;
	}

	//Checks if entity mask matches the ray mask
	if ((pEntity.componentMask & ComponentType::COMPONENT_RAY) == ComponentType::COMPONENT_RAY)
	{
		//If the entity has a ray component then find it in the rays
		const auto entity = std::find_if(mRays.begin(), mRays.end(), [&](const Entity& entity) {return entity.ID == pEntity.ID; });
		if (entity == mRays.end())
		{
			//If not found then add it
			mRays.push_back(pEntity);
		}
		else
		{
			//If already in the list, then update mask
			entity->componentMask = pEntity.componentMask;
		}
	}
}

/// <summary>
/// Re-assigns entity to system when component is removed from entity
/// </summary>
/// <param name="pEntity">Entity to re-assign</param>
void RayAABBIntersectionSystem::ReAssignEntity(const Entity & pEntity)
{
	//Checks if entity mask matches the box collider mask
	if ((pEntity.componentMask & mMask) == mMask)
	{
		//If the entity matches box collider mask then update entry in systems entity list
		mEntities[pEntity.ID] = pEntity;
	}
	else
	{
		//If the mask doesn't match then set ID to -1
		mEntities[pEntity.ID].ID = -1;
	}

	//Checks if entity mask matches the ray mask
	if ((pEntity.componentMask & ComponentType::COMPONENT_RAY) == ComponentType::COMPONENT_RAY)
	{
		//If the entity has a ray component then find it in the rays
		const auto entity = std::find_if(mRays.begin(), mRays.end(), [&](const Entity& entity) {return entity.ID == pEntity.ID; });
		if (entity == mRays.end())
		{
			//If not found then add it
			mRays.push_back(pEntity);
		}
		else
		{
			//If already in the list, then update mask
			entity->componentMask = pEntity.componentMask;
		}
	}
	else
	{
		//If the mask doesn't match then remove it (if it wasn't in the list then the remove acts as a search to confirm it is not there)
		mRays.erase(std::remove_if(mRays.begin(), mRays.end(), [&](const Entity& entity) {return entity.ID == pEntity.ID; }), mRays.end());
	}
}

/// <summary>
/// Systems process function, core logic of system
/// Calculates the ray AABB intersection between every ray and AABB
/// </summary>
void RayAABBIntersectionSystem::Process()
{
	for (const auto& ray : mRays)
	{
		//Retrieve ray component and calculate inverse ray direction
		Ray rayComp = *mEcsManager->RayComp(ray.ID);
		KodeboldsMath::Vector3 inverseRayDir(1 / rayComp.direction.X, 1 / rayComp.direction.Y, 1 / rayComp.direction.Z);

		//Check to see if ray intersects with any AABBs
		for (const auto& box : mEntities)
		{
			if (box.ID != -1)
			{
				float minX = 0, maxX = 0, minY = 0, maxY = 0, minZ = 0, maxZ = 0, highestMin = 0, lowestMax = 0;
				BoxCollider boxComp = *mEcsManager->BoxColliderComp(box.ID);

				//Swap min and max of X around depending on if the ray is travelling positive or negative direction
				if (inverseRayDir.X >= 0)
				{
					minX = (boxComp.minBounds.X - rayComp.origin.X) * inverseRayDir.X;
					maxX = (boxComp.maxBounds.X - rayComp.origin.X) * inverseRayDir.X;
				}
				else
				{
					minX = (boxComp.maxBounds.X - rayComp.origin.X) * inverseRayDir.X;
					maxX = (boxComp.minBounds.X - rayComp.origin.X) * inverseRayDir.X;
				}

				//Swap min and max of Y around depending on if the ray is travelling positive or negative direction
				if (inverseRayDir.Y >= 0)
				{
					minY = (boxComp.minBounds.Y - rayComp.origin.Y) * inverseRayDir.Y;
					maxY = (boxComp.maxBounds.Y - rayComp.origin.Y) * inverseRayDir.Y;
				}
				else
				{
					minY = (boxComp.maxBounds.Y - rayComp.origin.Y) * inverseRayDir.Y;
					maxY = (boxComp.minBounds.Y - rayComp.origin.Y) * inverseRayDir.Y;
				}

				//If min is greater than max, ray did not intersect then continue
				if ((minX > maxY) || (minY > maxX))
				{
					continue;
				}

				//Find lowest max and highest min
				if (minY > minX)
				{
					highestMin = minY;
				}
				else
				{
					highestMin = minX;
				}
				if (maxY < maxX)
				{
					lowestMax = maxY;
				}
				else
				{
					lowestMax = maxX;
				}


				//Swap min and max of Z around depending on if the ray is travelling positive or negative direction
				if (inverseRayDir.Z >= 0)
				{
					minZ = (boxComp.minBounds.Z - rayComp.origin.Z) * inverseRayDir.Z;
					maxZ = (boxComp.maxBounds.Z - rayComp.origin.Z) * inverseRayDir.Z;
				}
				else
				{
					minZ = (boxComp.maxBounds.Z - rayComp.origin.Z) * inverseRayDir.Z;
					maxZ = (boxComp.minBounds.Z - rayComp.origin.Z) * inverseRayDir.Z;
				}

				//If min is greater than max, ray did not intersect then continue
				if ((highestMin > maxZ) || (minZ > lowestMax))
				{
					continue;
				}

				//Set rays intersected with property to the id of this box
				mEcsManager->RayComp(ray.ID)->intersectedWith = box.ID;

				//Find lowest max and highest min
				if (minZ > highestMin)
				{
					highestMin = minZ;
				}
				if (maxZ < lowestMax)
				{
					lowestMax = maxZ;
				}

				//Set rays intersection point property to the point of intersection
				KodeboldsMath::Vector3 intersection(rayComp.origin + rayComp.direction * highestMin);
				mEcsManager->RayComp(ray.ID)->intersectionPoint = intersection;
				break;
			}
		}
	}
}
