#pragma once
#include "ISystem.h"
#include <algorithm>
#include "Managers.h"

class RayAABBIntersectionSystem : public ISystem
{
private:
	std::shared_ptr<ECSManager> mEcsManager = ECSManager::Instance();

	std::vector<Entity> mRays;

public:
	explicit RayAABBIntersectionSystem();
	virtual ~RayAABBIntersectionSystem();

	void AssignEntity(const Entity& pEntity) override;
	void ReAssignEntity(const Entity& pEntity) override;
	void Process() override;
};