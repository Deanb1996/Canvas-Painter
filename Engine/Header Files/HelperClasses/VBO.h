#pragma once
#include <wrl.h>
#include <string>
#include "InstanceData.h"
#include <vector>
#include <algorithm>

class RenderSystem;

class VBO
{
protected:
	VBO();

	std::vector<std::pair<int, InstanceData>> mInstanceData;

	int mIndexCount;
public:
	virtual ~VBO() = default;

	virtual HRESULT Create(const RenderSystem* pRenderer, const std::wstring& pFilename) = 0;
	virtual void Load(const RenderSystem* pRenderer) = 0;
	virtual void Draw(const RenderSystem* pRenderer) const = 0;
	void AddInstance(const int pEntityID, const MathsHelper::Vector3& pPosition, const MathsHelper::Vector3& pColour);
	void RemoveInstance(const int pEntityID);
	int InstanceCount();
};
