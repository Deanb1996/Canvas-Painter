#include "..\..\Header Files\Systems\RayAABBIntersectionSystem.h"

/// <summary>
/// 
/// </summary>
RayAABBIntersectionSystem::RayAABBIntersectionSystem() : ISystem(ComponentType::COMPONENT_TRANSFORM | ComponentType::COMPONENT_BOXCOLLIDER)
{
	mEntities = std::vector<Entity>(200000, Entity{ -1, ComponentType::COMPONENT_NONE });
}

/// <summary>
/// 
/// </summary>
RayAABBIntersectionSystem::~RayAABBIntersectionSystem()
{
}

/// <summary>
/// 
/// </summary>
/// <param name="pEntity"></param>
void RayAABBIntersectionSystem::AssignEntity(const Entity & pEntity)
{
	//Checks if entity mask matches the box collider mask
	if ((pEntity.mComponentMask & mMask) == mMask)
	{
		//If the entity matches box collider mask then update entry in systems entity list
		mEntities[pEntity.mID] = pEntity;
	}

	//Checks if entity mask matches the ray mask
	if ((pEntity.mComponentMask & ComponentType::COMPONENT_RAY) == ComponentType::COMPONENT_RAY)
	{
		//If the entity has a ray component then find it in the rays
		const auto entity = std::find_if(mRays.begin(), mRays.end(), [&](const Entity& entity) {return entity.mID == pEntity.mID; });
		if (entity == mRays.end())
		{
			//If not found then add it
			mRays.push_back(pEntity);
		}
		else
		{
			//If already in the list, then update mask
			entity->mComponentMask = pEntity.mComponentMask;
		}
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="pEntity"></param>
void RayAABBIntersectionSystem::ReAssignEntity(const Entity & pEntity)
{
	//Checks if entity mask matches the box collider mask
	if ((pEntity.mComponentMask & mMask) == mMask)
	{
		//If the entity matches box collider mask then update entry in systems entity list
		mEntities[pEntity.mID] = pEntity;
	}
	else
	{
		//If the mask doesn't match then set ID to -1
		mEntities[pEntity.mID].mID = -1;
	}

	//Checks if entity mask matches the ray mask
	if ((pEntity.mComponentMask & ComponentType::COMPONENT_RAY) == ComponentType::COMPONENT_RAY)
	{
		//If the entity has a ray component then find it in the rays
		const auto entity = std::find_if(mRays.begin(), mRays.end(), [&](const Entity& entity) {return entity.mID == pEntity.mID; });
		if (entity == mRays.end())
		{
			//If not found then add it
			mRays.push_back(pEntity);
		}
		else
		{
			//If already in the list, then update mask
			entity->mComponentMask = pEntity.mComponentMask;
		}
	}
	else
	{
		//If the mask doesn't match then remove it (if it wasn't in the list then the remove acts as a search to confirm it is not there)
		mRays.erase(std::remove_if(mRays.begin(), mRays.end(), [&](const Entity& entity) {return entity.mID == pEntity.mID; }), mRays.end());
	}
}

/// <summary>
/// 
/// </summary>
void RayAABBIntersectionSystem::Process()
{
	for (const auto& ray : mRays)
	{
		//Retrieve ray component and calculate inverse ray direction
		Ray rayComp = *mEcsManager->RayComp(ray.mID);
		MathsHelper::Vector3 inverseRayDir(1 / rayComp.mDirection.X, 1 / rayComp.mDirection.Y, 1 / rayComp.mDirection.Z);

		//Check to see if ray intersects with any AABBs
		for (const auto& box : mEntities)
		{
			if (box.mID != -1)
			{
				float minX = 0, maxX = 0, minY = 0, maxY = 0, minZ = 0, maxZ = 0;
				BoxCollider boxComp = *mEcsManager->BoxColliderComp(box.mID);

				//Swap min and max of Y around depending on if the ray is travelling positive or negative direction
				if (inverseRayDir.X >= 0)
				{
					minX = (boxComp.mMinBounds.X - rayComp.mOrigin.X) * inverseRayDir.X;
					maxX = (boxComp.mMaxBounds.X - rayComp.mOrigin.X) * inverseRayDir.X;
				}
				else
				{
					minX = (boxComp.mMaxBounds.X - rayComp.mOrigin.X) * inverseRayDir.X;
					maxX = (boxComp.mMinBounds.X - rayComp.mOrigin.X) * inverseRayDir.X;
				}

				//Swap min and max of Y around depending on if the ray is travelling positive or negative direction
				if (inverseRayDir.Y >= 0)
				{
					minY = (boxComp.mMinBounds.Y - rayComp.mOrigin.Y) * inverseRayDir.Y;
					maxY = (boxComp.mMaxBounds.Y - rayComp.mOrigin.Y) * inverseRayDir.Y;
				}
				else
				{
					minY = (boxComp.mMaxBounds.Y - rayComp.mOrigin.Y) * inverseRayDir.Y;
					maxY = (boxComp.mMinBounds.Y - rayComp.mOrigin.Y) * inverseRayDir.Y;
				}

				//If min is greater than max, ray did not intersect and break
				if ((minX > maxY) || (minY > maxX))
				{
					continue;
				}

				//Find lowest max and highest min
				if (minY > minX)
				{
					minX = minY;
				}
				if (maxY < maxX)
				{
					maxX = maxY;
				}


				//Swap min and max of Z around depending on if the ray is travelling positive or negative direction
				if (inverseRayDir.Z >= 0)
				{
					minZ = (boxComp.mMinBounds.Z - rayComp.mOrigin.Z) * inverseRayDir.Z;
					maxZ = (boxComp.mMaxBounds.Z - rayComp.mOrigin.Z) * inverseRayDir.Z;
				}
				else
				{
					minZ = (boxComp.mMaxBounds.Z - rayComp.mOrigin.Z) * inverseRayDir.Z;
					maxZ = (boxComp.mMinBounds.Z - rayComp.mOrigin.Z) * inverseRayDir.Z;
				}

				//If min is greater than max, ray did not intersect and break
				//Else set rays intersected with property to the id of this box
				if ((minX > maxZ) || (minZ > maxX))
				{
					continue;
				}
				else
				{
					mEcsManager->RayComp(ray.mID)->mIntersectedWith = box.mID;
					break;
				}
			}
		}
	}
}